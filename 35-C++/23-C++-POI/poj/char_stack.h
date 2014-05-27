/* File: char_stack.h       

  Header file for a very basic array-based implementation of a stack.

*/
#ifndef _CHAR_STACK_H
#define _CHAR_STACK_H

class char_stack
{
  public:
    char_stack();
    void push( char item );
    char pop(); 
    char top();
    bool empty();
    int size();
	~char_stack()
	{
		delete []CharArray;
	}

  private:
    // fill this in.
	char *CharArray;
	int count;
	int siz;

	int Top;
};

#endif

