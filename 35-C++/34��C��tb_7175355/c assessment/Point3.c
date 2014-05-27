#include <stdio.h>

void swap_int (int *x, int *y)
/* Here we have a function defined with two
   formal parameters of type 'pointer to int'
   This means it can manipulate variables
   created SOMEWHERE ELSE provided it
   is supplied with their addresses in via these
   parameters */
{ int temp;
  temp = *x;
  /* here the indirection operator '*' has been used to
     get access to the 'thing pointed to by' the pointer x rather than
     the contents of x itself hence it is the value of the
     thing pointed to by x which is assigned to temp */
  *x = *y;
  /* here the contents of the location pointed to by y is
     assigned to the location pointed to by y */
  *y = temp;
  /* and finally the value kept in local variable temp is
     assigned to the location pointed to by y */
}

int main (void)
{ int a = 1, b = 20;
  printf("\nThe starting values are: a= %d, b= %d",a,b);
  swap_int ( &a, &b);
  /* when the function swap_int is called it is passed two
     'actual parameters' which in this case (because of the
     use of the address operator '&') are the addresses of
     the two variables a and b  rather than their values */
  printf("\nThe end values are: a= %d, b= %d\n\n",a,b);
  /* you should see that the values of a and b have swapped */
  system("PAUSE");
  return (0);
}


