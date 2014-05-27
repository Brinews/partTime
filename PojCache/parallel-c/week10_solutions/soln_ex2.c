#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Matrices here are WIDTH x WIDTH
#define WIDTH (512)

// Make sure NTHREADS divides WIDTH.
#define NTHREADS (4)

// We'll make each worker responsible for a range
// of rows [rowi, rowj) in the output matrix.
typedef struct {
  double *inA, *inB, *outC;
  int rowi, rowj;
} MatWkr;

double trace(double *A);
void *multiply(void *arg);

int main(void) {
  // Create three matrices.
  double *A = calloc(WIDTH * WIDTH, sizeof(double));
  double *B = calloc(WIDTH * WIDTH, sizeof(double));
  double *C = calloc(WIDTH * WIDTH, sizeof(double));

  // Create initial values for A and B
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < WIDTH; j++) {
      A[WIDTH * i + j] = i + j;
      B[WIDTH * i + j] = i - j;
    }
  }

  // Set C = A x B
  pthread_t tids[NTHREADS];
  MatWkr matwkrs[NTHREADS];
  for (int i = 0; i < NTHREADS; i++) {
    matwkrs[i] = (MatWkr) {
      .inA = A,
      .inB = B,
      .outC = C,
      .rowi = i * (WIDTH/NTHREADS),
      .rowj = (i+1) * (WIDTH/NTHREADS)
    };
    pthread_create(&tids[i], NULL, multiply, &matwkrs[i]);
  }

  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(tids[i], NULL);
  }

  // Print out the trace of the result.
  printf("%f\n", trace(C));
  
  free(A);
  free(B);
  free(C);
  return 0;
}

void *multiply(void *arg) {
  MatWkr *matwkr = (MatWkr *) arg;
  for (int i = matwkr->rowi; i < matwkr->rowj; i++) {
    for (int j = 0; j < WIDTH; j++) {
      matwkr->outC[WIDTH * i + j] = 0;
      for (int k = 0; k < WIDTH; k++) {
        matwkr->outC[WIDTH * i + j] += matwkr->inA[WIDTH * i + k] * matwkr->inB[WIDTH * k + j];
      }
    }
  }
  return NULL;
}

double trace(double *A) {
  double sum = 0;
  for (int i = 0; i < WIDTH; i++) {
    sum += A[WIDTH * i + i];
  }
  return sum;
}
