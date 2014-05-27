#include <stdio.h>

int inc_int (int a)
{
  a++;
  /* notice how incrementing this
     local a has no effect on the
     different a in the main function
  */
  return (a);
}

int alt_inc (int a)
{ int b;
  /* note who b is created and used but
     its value never set in the program!
     Very bad!
   */
  b = b + a;
  return(b);
}

int main (void)
{
  int a = 1, b, c, d;

  b = inc_int(a);
  c = alt_inc(b);
  d = alt_inc(b);
  /* notice how we call this function
     twice and the un-predictable nature of the result in c
     shows the problem of failing to initialise the
     b local variable inside the function and also
     that the content of b is NOT remembered between
     successive calls of the function (local
     variables in a function live and die with it)
  */
  printf("\na=%d b=%d c=%d d=%d",a,b,c,d);
  /* below is just something to stop the screen closing too soon */
  printf("\n\nPress any letter or number key followed by the ENTER key to close down....");
  scanf(" %c",&a);

  return (0);
}
