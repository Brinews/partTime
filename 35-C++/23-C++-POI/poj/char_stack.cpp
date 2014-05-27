/* File: char_stack.cpp

  Implementation of functions for basic simple array-based implementation of a stack.

*/
#include "char_stack.h"// contains the declarations of the variables and functions.

#include <iostream>

using namespace std;

#define SLEN	1024

// Fill this in.
char_stack::char_stack()
{
	Top = 0;
	count = 0;
	CharArray = new char[SLEN];
	siz = SLEN;

	if (NULL == CharArray)
	{
		cerr << "Malloc Failed.\n";
	}
}

void char_stack::push(char item)
{
	if (count == siz)
	{
		char *newArray = new char[siz + SLEN];
		memset(newArray, 0, siz+SLEN);

		memcpy(newArray, CharArray, siz);
		siz = siz + SLEN;

		delete CharArray;
		CharArray = newArray;
	}

	//begin to push
	CharArray[Top++] = item;
	count++;
}

char char_stack::pop()
{
	char c = CharArray[Top - 1];
	Top--;
	count--;

	return c;
}

char char_stack::top()
{
	return CharArray[Top-1];
}

bool char_stack::empty()
{
	return count == 0;
}

int char_stack::size()
{
	return count;
}
