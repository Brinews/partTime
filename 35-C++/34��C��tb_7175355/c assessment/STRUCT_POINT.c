#include <stdio.h>

/* Here I make my own structure type, I've chosen to call my new type "pple". 
   I can now make variables of this type where ever I need to in the program.
*/
typedef struct {
    int age;
    float height;
  } pple;

  /* ------------------------------------------------------- */

  void test_struct (pple *p_s)
  /* the formal parameter for this function is a pointer to something of type 
      "pple", in other words the function expects to be given the address of a 
      variable of type "pple" when it's called.
  */
   { printf("age = %d\n", p_s->age); /* here I show that you can read a value out
                                        of the structure that the function has 
                                        been given the address of. I'm using the
                                        structure pointer operator "->" which 
                                        means in this case go to the structure 
                                        pointed to by pointer "p_s" and get the 
                                        value of that structure's member "age".
                                     */
     printf("\n please enter a height for this person: ");
     scanf("%f", &p_s->height);  /* here I show that you can put a value into 
                                    the structure that the function has been 
                                    given the address of too.
                                 */   
   }

  /* ------------------------------------------------------- */

  int main (void)
   { pple my_struct;  /* here I make a variable using my own structure type */
  
     my_struct.age = 20;  /* I put a value into one of the members of my 
                             structure variable.
                          */
     test_struct(&my_struct); /* I now call my function whilst passing to it
                                 the address of the structure variable here in 
                                 main that I want to the function to work with.
                              */
     printf("\nHeight found = %f \n",my_struct.height);  /* here I show that 
                                                            my function really
                                                            did load something
                                                            into my structure 
                                                            here in main.
                                                         */
     system("pause");
     
     return (0);
   }
