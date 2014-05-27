/***
 * Author : root
 * Date: 2014-05-02
 * Compile: g++ -pedantic-errors -Wall -Werror -ansi
 */

#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>
#include <list>

using namespace std;

/***
 * Error State Define
 */
typedef enum 
{
	E_TOOMANYSF,
	E_NOSF,
	E_INVALIDCHAR,
	E_SFOUTSIDE,
	E_OK
} STATE;

char* emsg[] = 
{
	"Mulitple s or f in the maze",
	"s or f declared outside of the maze",
	"s or f not declared at all",
	"Invalid characters other than spaces, #, s, f or \\n",
};

char* getErrorMsg(STATE s, char **emsg)
{
	return emsg[s];
}

/* helper function */
bool validChar(char c);
STATE validFS(const list <list <char> >& maze, int linemax, 
		int f, int s);

STATE loadMaze(list <list <char> >&maze, const string& line,
		int& f, int &s, int &linemax);
void completeMaze(list <list <char> >&maze, int linemax);
void calculateMazeMetrics(list <list <char> >&maze, int &dn, int &bn);
void printMaze(const list <list <char> > &maze);

/* functions */
bool validChar(char c)
{
	if (c != '#' && c != 's' 
			&& c != 'f' && c != ' ' 
			&& c != '\n')
		return false;

	return true;
}

STATE validFS(const list <list <char> >&maze, int linemax,
		int f, int s)
{
	if (f > 1 || s > 1) return E_TOOMANYSF;
	if (f == 0 || s == 0) return E_NOSF;

	int H_s = 0, H_f;

	list< list<char> >::const_iterator litr;
	for (litr = maze.begin(); litr != maze.end(); litr++)
	{
		list<char>::const_iterator itr;
		
		int Sbound = 0, Fbound = 0;
		for (itr = (*litr).begin(); itr != (*litr).end(); itr++)
		{
			if (*itr == '#' && Sbound == 0 && Fbound == 0) 
			{
				Sbound = 1;
				Fbound = 1;
			}

			if (*itr == 's')
			{
				if (Sbound == 1) Sbound = 2;
			}

			if (*itr == 'f')
			{
				if (Fbound = 1) Fbound = 2;
			}

			if (Sbound == 2 && *itr == '#')
				Sbound = 3;

			if (Fbound == 2 && *itr == '#')
				Fbound = 3;
		}

		if (Sbound == 3) H_s = 1;
		if (Fbound == 3) H_f = 1;
	}

	if (H_s != 1 || H_f != 1) return E_SFOUTSIDE;

	return E_OK;
}

/* load the maze line by line */
STATE loadMaze(list <list <char> >&maze, const string& line,
		int &f, int &s, int &linemax)
{
	list<char> lstr;

	for (unsigned int i = 0; i < line.length(); i++)
	{
		char c = line.at(i);
		if (validChar(c))
		{
			if (c == 's') s++;
			if (c == 'f') f++;

			lstr.push_back(c);
		}
		else
			return E_INVALIDCHAR;
	}

	/* update line max length */
	if (line.length() > (unsigned int)linemax)
		linemax = line.length();

	maze.push_back(lstr);

	return E_OK;
}

void completeMaze(list <list <char> >&maze, int linemax)
{
	list <list <char> >::iterator Litr;
	for (Litr = maze.begin(); Litr != maze.end(); Litr++)
	{
		int added = linemax - (*Litr).size();
		while (added > 0)
		{
			(*Litr).push_back('-');
			added --;
		}
	}
}

void calculateMazeMetrics(list <list <char> >&maze, int &deadNum, int &branchNum)
{
	if (maze.size() < 3) return;

	list <list <char> >::iterator pLitr, cLitr, nLitr;
	pLitr = maze.begin();
	cLitr = ++pLitr;
	nLitr = ++pLitr;
	pLitr = maze.begin();

	for (;nLitr != maze.end(); nLitr++)
	{
		list<char>::iterator now, left, right, up, down, temp;

		left = (*cLitr).begin();
		now = ++left;
		right = ++left;
		left = (*cLitr).begin();

		temp = (*pLitr).begin();
		up = ++temp;
		temp = (*nLitr).begin();
		down = ++temp;

		int inmaze = 0;
		for (;right != (*cLitr).end(); right++)
		{
			if (*now == '-') break;

			if (*left == '#' && inmaze == 0) inmaze = 1;

			if (*now == ' ' && inmaze == 1)
			{
				/* two branch */
				if (*left != '#' && *right != '#'
					&& *up != '#' && *down != '#')
				{
					*now = '2';
					branchNum += 2;
				}

				/* one branch */
				if (*left == '#' && *right != '#'
					&& *up != '#' && *down != '#')
				{
					*now = '1';
					branchNum += 1;
				}

				if (*left != '#' && *right == '#'
					&& *up != '#' && *down != '#')
				{
					*now = '1';
					branchNum += 1;
				}

				if (*left != '#' && *right != '#'
					&& *up == '#' && *down != '#')
				{
					*now = '1';
					branchNum += 1;
				}

				if (*left != '#' && *right != '#'
					&& *up != '#' && *down == '#')
				{
					*now = '1';
					branchNum += 1;
				}

				/* dead-end */
				if (*left == '#' && *right == '#'
					&& *up == '#' && *down == ' ')
				{
					*now = 'd';
					deadNum += 1;
				}

				if (*left == '#' && *right == '#'
					&& *up == ' ' && *down == '#')
				{
					*now = 'd';
					deadNum += 1;
				}

				if (*left == '#' && *right == ' '
					&& *up == '#' && *down == '#')
				{
					*now = 'd';
					deadNum += 1;
				}

				if (*left == ' ' && *right == '#'
					&& *up == '#' && *down == '#')
				{
					*now = 'd';
					deadNum += 1;
				}
			}

			if (*now == 's' || *now == 'f')
			{
				int br = 0;
				if (*left == ' ')
					br++;
				if (*right == ' ')
					br++;
				if (*up ==  ' ')
					br++;
				if (*down ==  ' ')
					br++;

				if (br > 1)
				{
					//*now = '0' + (br-1);
					branchNum += (br-1);
				}
			}

			left++;
			now++;
			up++;
			down++;
		}

		pLitr++;
		cLitr++;
	}
}

/***
 * print out the Maze
 */
void printMaze(const list <list <char> >&maze)
{
	list <list <char> >::const_iterator Litr;

	for (Litr = maze.begin(); Litr != maze.end(); Litr++)
	{
		list<char>::const_iterator Citr;
		for (Citr = (*Litr).begin(); Citr != (*Litr).end(); Citr++)
		{
			if (*Citr == '-') break;
			cout << *Citr;
		}

		cout << endl;
	}
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "Usage:" << argv[0] << " mazefile " <<endl;
		return 0;
	}

	ifstream infile(argv[1]);
	if (!infile)
	{
		cout << "Open file " << argv[1] << "failed."<<endl;
		return 0;
	}

	string line;
	list <list <char> > myMaze;
	STATE ret;
	int F_num = 0, S_num = 0;
	int linemax = 0;
	int dn = 0, bn = 0;

	while (getline(infile, line))
	{
		ret = loadMaze(myMaze, line, F_num, S_num, linemax);
		if (ret != E_OK)
		{
			cout << getErrorMsg(ret, emsg) << endl;
			return 0;
		}
	}

	ret = validFS(myMaze, linemax, F_num, S_num);
	if (ret != E_OK)
	{
		cout << getErrorMsg(ret, emsg) << endl;
		return 0;
	}

	completeMaze(myMaze, linemax);
	calculateMazeMetrics(myMaze, dn, bn);
	printMaze(myMaze);

	//cout << "Branches Num = " << bn << ", Dead-end Num = " << dn << endl;

	//system("pause");
	return 0;
}

