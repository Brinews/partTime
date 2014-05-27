#include "char_stack.h"

#include <iostream>
using namespace std;

#define LINEMAX	256

bool match(char l, char r)
{
	if ((l == '(' && r == ')')
	   || (l == '[' && r == ']')
	   || (l == '{' && r == '}'))
		return true;

	return false;
}

bool SymbolCheck(char *buf, int lineno, char_stack *csk)
{
	if (NULL == buf) return true;

	int len = strlen(buf);
	int i = 0;

	while (i < len)
	{
		char c = buf[i];
		
		if (c == '(' || c == '[' || c == '{')
		{
			csk->push(c);
		}
		else if (c == ')' || c == ']' || c == '}')
		{
			if (csk->empty()) {
				cout << "Error on line " << lineno << ": too many " << c << endl;
				return false;
			}

			char l = csk->pop();

			if (!match(l, c))
			{
				cout << "Error on line " << lineno << " : Read " << c << ", Expected " << l << endl;
				return false;
			}
		}
		i++;
	}

	return true;
}

int main()
{
	char buf[LINEMAX];
	int lineno = 1;

	char_stack *csk = new char_stack();
	bool ret;

	while (cin.getline(buf, LINEMAX))
	{
		ret = SymbolCheck(buf, lineno, csk);

		if (!ret) break;

		lineno++;
	}

	if (ret && !csk->empty()) {

		char c = csk->top();
		cout << "Error, Too Many  " << c << endl;

	} else {
		cout << "No Error Found\n";
	}

	delete csk;

	return 0;
}
