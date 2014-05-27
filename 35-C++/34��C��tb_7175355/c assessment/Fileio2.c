#include<stdio.h>

/* This example file read/write program is designed to be compiled with 
   Bloodshed's DEV-C compiler 
   
   NOTE: this program will only work correctly if the data file "my_data.dat" 
   is already placed in the root folder of the U:\ drive.
*/

int main (void)
{
/* declarations */
  FILE *my_in, *my_out;  /* File handles */
  int count = 0; /* an integer to keep a count of numbers read out of the file 
                 */
  float number, sum = 0,average; /* floats to hold numbers read from file, a 
                                    sum total and an average value */

/* Announcing to user what is happening */
  printf("\n\nProgram to calculate average temperature from a set of readings");

/* Try to open file with data in.  Note: by putting "U:\\my_data.dat" I am 
   clearly specifying that I am expecting the data file "my_data.dat" to be 
   located in the root folder of the U: drive. Note: "rt" means open file 
   for reading in text mode */
   
  if ((my_in = fopen ("U:\\my_dat.dat","rt")) == NULL)
	 {  /* If fopen fails to find or open the file then an error message will
           appear and the program will end */
		printf("\nError opening input file, program exiting\n");
		system("pause");
      return(1);
    }
/* Try to create a new file, to be called "rslts.dat" in the root folder of the 
   U: drive. Note: "wt" means open file for writing in text mode */
   
  if ((my_out = fopen ("U:\\rslts.dat","wt")) == NULL)
    { /* If fopen fails to be able to open the file then an error message will
         appear and the program will end */	
      printf("\nError opening output file, program exiting\n");
      system("pause");
      return(1);
    }
    
/* If we reach here then both files must have been opened successfully */
    
  printf("\nProgram reading data from file\n");

/* This loop will repeat until the end of "my_data.dat" is reached, reading 
  each float value found there one by one and placing that value in variable 
  "number" */
  
  while (fscanf(my_in,"%f",&number)!=EOF)
    {  
       printf(".");  /* to let user know something is happening put a '*' on 
                         the screen for each successful read from the file */
       sum += number; /* Each value read is added to running total "sum" */ 
       count ++; /* keeping a count of number of values read from file */
    }

  average = sum/count; /* calculate average of values read */
  
  /* Now write to "rslts.dat" file the following message */
  
  fprintf(my_out, "The avaerage is %4.3f\n", average);
  
  /* Close both files */
  
  fclose(my_in);
  fclose(my_out);
  
  /* Let user know all is now done */
  
  printf("\nProgram complete.");
  system("pause");
  return(0);
}