#ifndef _DOC7_H

#include <string>
#include <iostream>

using namespace std;

#define SIZ	64

typedef struct WordNode {
	string strWord;
	int count;

	/***
	 * Reused As *next when being a List Node
	 */
	struct WordNode *left;
	struct WordNode *right;

}WordNode_t;

class WordArray {

private:
	string *strWordArray;
	int nTotal;
	int nCapacity;

public:
	WordArray()
	{
		strWordArray = new string[SIZ];
		nTotal = 0;
		nCapacity = SIZ;
	}

	~WordArray()
	{
		delete []strWordArray;
		nCapacity = nTotal = 0;
	}

	void Push(string str);
	void Trim();
	string IndexOf(int n);
	int Size();
	void Sort();

	void PrintOut(ostream &out)
	{
		for (int i = 0; i < nTotal; i++)
		{
			out << strWordArray[i] << endl;
		}
	}

	string* GetWordArray()
	{
		return strWordArray;
	}
};

class WordTree
{
private:
	WordNode_t *root;
	int nTotal;

public:
	WordTree()
	{
		root = NULL;
		nTotal = 0;
	}

	~WordTree()
	{
		DestroyTree(root);
	}

	void DestroyTree(WordNode_t *r)
	{
		if (NULL != r)
		{
			DestroyTree(r->left);
			DestroyTree(r->right);

			delete r;
			r = NULL;
		}
	}

	bool FindTree(WordNode_t *root, string& str)
	{
		if (NULL != root && root->strWord.compare(str) == 0)
		{
			return true;
		}

		FindTree(root->left, str);
		FindTree(root->right, str);

		return false;
	}

	WordNode_t* InsertNode(WordNode_t *r, string str)
	{
		if (NULL == r)
		{
			r = new WordNode_t;
			r->strWord = str;
			r->count = 1;

			r->left = r->right = NULL;

			return r;
		}

		/***
		 * Exist Already
		 */
		if (r->strWord.compare(str) == 0)
		{
			r->count++;
			return r;
		}

		/***
		 * Right tree
		 */
		if (r->strWord.compare(str) < 0)
		{
			r ->right = InsertNode(r->right, str);
		}

		/***
		 * Left tree
		 */
		if (r->strWord.compare(str) > 0)
		{
			r->left = InsertNode(r->left, str);
		}

		return r;
	}

	void PrintNode(WordNode_t *r, ostream &out)
	{
		if (NULL != r) 
		{
			PrintNode(r->left, out);

			out << r->strWord << " " << r->count << endl;

			PrintNode(r->right, out);
		}
	}

	WordNode_t *GetTreeRoot()
	{
		return root;
	}

	void InsertTree(string str);
	void PrintTree(ostream &out);
};

class WordList
{
private:
	WordNode_t *wordList;
	WordNode_t *tail;
	int nTotal;

public:

	void InsertList(WordNode_t *wn);
	void SortList();
	void WriteOutList(ostream &out);

	WordList()
	{
		wordList = NULL;
		nTotal = 0;
	}

	void DestroyList(WordNode_t *r)
	{
		if (NULL != r)
		{
			DestroyList(r->left);

			delete r;
			r = NULL;
		}
	}

	~WordList()
	{
		DestroyList(wordList);
	}
};

#endif
