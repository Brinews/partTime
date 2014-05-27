#include<stdio.h>
/* Note: this program expects to find its data file on the "U:" drive in 
   folder "\"  and to be called "my_data.dat".  If your 
   data file is called anything else or is in any other location you will 
   need to edit the string in function "fopen" accordingly. 
*/

int main (void)
{

  FILE *my_in, *my_out;
  int count = 0;
  float  number, sum = 0,average;


  printf("\n\nProgram to calculate average temperature from a set of readings");



  if ((my_in = fopen ("U:\\my_dat.dat","rt")) == NULL)
	 {
	  printf("\nError opening input file, program exiting\n");
      printf("\nType a number & press the enter key to finish");
      scanf("%d",&count);
      return(1);
    }

  if ((my_out = fopen ("U:\\rslts.dat","wt")) == NULL)
    {
      printf("\nError opening output file, program exiting\n");
      printf("\nType a number & press the enter key to finish");
      scanf("%d",&count);
      return(1);
    }
  printf("\nProgram reading data from file\n");
  while (fscanf(my_in,"%f",&number)!=EOF)
    {
       printf(".");
       sum += number;
       count ++;
    }

  average = sum/count;
  fprintf(my_out, "The avaerage is %4.3f\n", average);
  fclose(my_in);
  fclose(my_out);
  printf("\nProgram complete.");
  printf("\nType a number & press the enter key to finish");
  scanf("%d",&count);

  return(0);
}