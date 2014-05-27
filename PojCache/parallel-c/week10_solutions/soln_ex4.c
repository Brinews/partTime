#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>
#include <assert.h>

int range = 12;

enum color {R = 0, G = 1, B = 2, A = 3};

typedef uint32 pixel;

struct img {
  int w, h;
  pixel * data;
};

unsigned char 
getValue(pixel * p, enum color c) {
  return ((unsigned char *) p)[c];
}

void 
setValue(pixel * p, enum color c, unsigned char v) {
  ((unsigned char *) p)[c] = v;
}

pixel * 
getPixel(const struct img * image, int x, int y) {
  assert(y * image->w + x < image->w * image->h);
  return &(image->data[y * image->w + x]); 
}

void 
my_sub_filter(struct img * output_img, const struct img * input_img, 
              int x_start, int x_end, int y_start, int y_end) {
  int x, y;
  for (x = x_start; x <= x_end; x++) {
    for (y = y_start; y <= y_end; y++) {
      // Compute the color for the pixel (x,y) of the output

      unsigned int red, green, blue, alpha, count;
      red = 0; green = 0; blue = 0; alpha = 0; count = 0;
      
      // Compute the avarage color of the surrounding pixels
      // including the pixel itself
      int dx, dy;
      for (dx = x-range; dx <= x+range; dx++) {
      	for (dy = y-range; dy <= y+range; dy++) {
          if (dx >= 0 && dx < input_img->w && dy >= 0 && dy < input_img->h) {
            count++;
            red += getValue(getPixel(input_img,dx,dy),R);
            green += getValue(getPixel(input_img,dx,dy),G);
            blue += getValue(getPixel(input_img,dx,dy),B);
            alpha += getValue(getPixel(input_img,dx,dy),A);
          }
        }
      }
      
      // Set the computed color for the pixel in the output
      setValue(getPixel(output_img,x,y),R,(unsigned char) (red/count));
      setValue(getPixel(output_img,x,y),G,(unsigned char) (green/count));
      setValue(getPixel(output_img,x,y),B,(unsigned char) (blue/count));
      setValue(getPixel(output_img,x,y),A,(unsigned char) (alpha/count));
    }
  }
}

struct args {
  struct img * output_img;
  const struct img * input_img;
  int x_start, x_end, y_start, y_end;
};

void *
thread_fn(void * _args) {
  struct args * args = (struct args *) _args;
  my_sub_filter(args->output_img, 
                args->input_img, 
                args->x_start, 
                args->x_end, 
                args->y_start, 
                args->y_end);
  return NULL;
}

void 
my_filter(struct img * output_img, const struct img * input_img) {
  pthread_t threads[16];
  int i;
  for (i = 0; i < 16; i++) {
    int _i = i % 4;
    int _j = i / 4;

    struct args * args = (struct args *) malloc(sizeof(struct args));
    args->output_img = output_img;
    args->input_img = input_img;
    int w_tile = (output_img->w / 4);
    int h_tile = (output_img->h / 4);
    args->x_start = _j * w_tile;
    args->y_start = _i * h_tile;
    if (_j < 3)
      args->x_end = args->x_start + w_tile - 1;
    else
      args->x_end = output_img->w - 1;
    if (_i < 3)
      args->y_end = args->y_start + h_tile - 1;
    else
      args->y_end = output_img->h - 1;

    pthread_create(&(threads[i]), NULL, thread_fn, args);
  }
  for (i = 0; i < 16; i++) {
    pthread_join(threads[i], NULL);
  }
}

int 
main(int argc, char** argv) {
  if (argc == 2) {
    range = atoi(argv[1]);
  }

  int sampleperpixel = 4; 
  
  TIFF* tif = TIFFOpen("input.tiff", "r");
  if (tif == NULL) 
    return 1;

  struct img input_img;
  
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &input_img.w);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &input_img.h);
  size_t npixels = input_img.w * input_img.h;
  input_img.data = (pixel *) _TIFFmalloc(npixels * sizeof (pixel));
  if (input_img.data == NULL) 
    return 1;
  
  if (TIFFReadRGBAImage(tif, input_img.w, input_img.h, input_img.data, 0) == 0) 
    return 1;

  TIFF * out= TIFFOpen("output.tiff", "w");
  if (out == NULL)
    return 1;
  TIFFSetField (out, TIFFTAG_IMAGEWIDTH, input_img.w);  // set the width of the image
  TIFFSetField(out, TIFFTAG_IMAGELENGTH, input_img.h);    // set the height of the image
  TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);   // set number of channels per pixel
  TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);    // set the size of the channels
  TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
  TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

  struct img output_img;
  output_img.w = input_img.w;
  output_img.h = input_img.h;
  output_img.data = (pixel *) _TIFFmalloc(npixels * sizeof (pixel));

  my_filter(&output_img, &input_img);

  if (output_img.data == NULL)
    return 1;

  tsize_t linebytes = sampleperpixel * output_img.w; 
  unsigned char * line_buf = NULL; 

  // Allocating memory to store the pixels of current row
  line_buf = (unsigned char *)_TIFFmalloc(linebytes);
  if (line_buf == NULL)
    return 1;
  
  // We set the strip size of the file to be size of one row of pixels
  TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, linebytes));
  
  // Now writing image to the file one strip at a time
  int row;
  for (row = 0; row < output_img.h; row++) {
    // check the index here, and figure out why not using h*linebytes
    memcpy(line_buf, &(((char *) output_img.data)[(output_img.h - row - 1) * linebytes]), linebytes);    
    if (TIFFWriteScanline(out, line_buf, row, 0) < 0)
      return 1;
  }
  
  TIFFClose(out);
  _TIFFfree(line_buf);
  _TIFFfree(input_img.data);
  _TIFFfree(output_img.data);
  TIFFClose(tif);

  printf("done.\n");
  return 0;
}
