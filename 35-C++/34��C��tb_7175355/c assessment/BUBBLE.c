#include<stdio.h>
#define CLSS_SIZ 4

void bsort (int *ary, int siz)
 {
 /*PURPOSE to sort into order array of integers.
	CREATER 24/12/02 by PLJ.
	INPUTS Pointer to the int array to be sorted &
			 Size of the array as an int number.
	RETURN function doesn't return anything.
	REVISIONS none.
 */
	int j, k, temp;

	for(k=0;k < siz - 1; ++k)
	  for(j=siz-1;j>k;--j)
		 if(ary[j-1] < ary[j])
		  {
			temp = ary[j-1]; /*swap routine*/
			ary[j-1]=ary[j];
			ary[j]=temp;
		  }
 } /* end function */

int main (void)
 { int mark [CLSS_SIZ], i;
	printf("\n\nEnter %d student exam marks as decimal integers:",CLSS_SIZ);
	for(i=0;i < CLSS_SIZ;i++)
	 {
	  printf("\nEnter mrk %d ",i);
	  scanf("%d",&mark[i]); /*loads array*/
	 } /* End for loop */

  bsort(mark, CLSS_SIZ); /*Here we call our sort function*/
  /*Note how, via passing the name of our local array mark,
	 we are giving the bsort function direct access to to
	 this array to allow us to manipulate it even though
	 the nark[] array is out of scope (invisible) to bsort()*/

  for(i=0;i < CLSS_SIZ;i++)
	printf("\nmark[%d]=%d",i,mark[i]);
  
  printf("\n\nEnter a number and press enter to end.");
  scanf("%d",&i);
  return(0);
 }
