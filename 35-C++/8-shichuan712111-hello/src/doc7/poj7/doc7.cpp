#include <fstream>

#include "doc7.h"

using namespace std;

/***
 * WordArray Class
 */

void WordArray::Push(string str)
{
	/***
	 * Need Resize
	 */
	if (nTotal >= nCapacity)
	{
		string *newWordArray = new string[nCapacity<<1];

		for (int i = 0; i < nCapacity; i++)
		{
			newWordArray[i] = strWordArray[i];
		}

		delete []strWordArray;

		nCapacity = nCapacity<<1;
		strWordArray = newWordArray;
	}

	strWordArray[nTotal++] = str;
}

void WordArray::Trim()
{
	if (nTotal < nCapacity)
	{
		string *newWordArray = new string[nTotal];

		for (int i = 0; i < nTotal; i++)
		{
			newWordArray[i] = strWordArray[i];
		}

		delete []strWordArray;

		nCapacity = nTotal;
		strWordArray = newWordArray;
	}
}

string WordArray::IndexOf(int n)
{
	if (n < nTotal && n >= 0)
	{
		return strWordArray[n];
	}

	return NULL;
}

int WordArray::Size()
{
	return nTotal;
}

void WordArray::Sort()
{
	/***
	 * Bubble Sort
	 */
	for (int i = 0; i < nTotal; i++)
	{
		for (int j = i+1; j < nTotal; j++) 
		{
			if (strWordArray[i].compare(strWordArray[j]) > 0)
			{
				/***
				 * Switch
				 */
				string temp = strWordArray[i];
				strWordArray[i] = strWordArray[j];
				strWordArray[j] = temp;
			}
		}
	}
}

/***
 * WordTree Class
 */
void WordTree::InsertTree(string str)
{
	root = InsertNode(root, str);
}

void WordTree::PrintTree(ostream &out)
{
	out << "Word  Occurrence\n";
	out << "================\n";
	PrintNode(root, out);
}

/***
 * WordList Class
 */
void WordList::InsertList(WordNode_t *wn)
{
	WordNode_t *node = new WordNode_t;
	node->strWord = wn->strWord;
	node->count = wn->count;
	node->left = node->right = NULL;

	if(NULL == wordList)
	{
		wordList = tail = node;
	}
	else 
	{
		tail->left = node;
		tail = tail->left;
	}

	nTotal++;
}

void WordList::SortList()
{
	/***
	 * Select Sort
	 */
	WordNode_t *maxNode, *iNode, *jNode;
	string temp;
	int c;

	for (iNode = wordList; NULL != iNode; iNode = iNode->left)
	{
		maxNode = iNode;

		for (jNode = iNode->left; NULL != jNode; jNode = jNode->left)
		{
			if (jNode->count > maxNode->count)
			{
				maxNode = jNode;
			}
			else if (jNode->count == maxNode->count)
			{
				if (jNode->strWord.compare(maxNode->strWord) < 0)
				{
					maxNode = jNode;
				}
			}
		}

		/***
		 * Switch
		 */
		if (maxNode != iNode)
		{
			temp = iNode->strWord;
			iNode->strWord = maxNode->strWord;
			maxNode->strWord = temp;

			c = iNode->count;
			iNode->count = maxNode->count;
			maxNode->count = c;
		}
	}
}

void WordList::WriteOutList(ostream &out)
{
	out << "Words   Occurence\n";
	out << "-----------------\n";
	for (WordNode_t *ptr = wordList; NULL != ptr; ptr = ptr->left)
	{
		out << ptr->strWord << " " << ptr->count << endl;
	}
}

string ExtractLetter(string word)
{
	string w = "";

	for (int i = 0; i < word.length(); i++)
	{
		if (word[i] >= 'A' && word[i] <= 'Z')
			w += word[i];

		if (word[i] >= 'a' && word[i] <= 'z')
			w += word[i];
	}

	return w;
}

/***
 * Build List From Tree
 */
void PopulateTreeIntoList(WordNode_t *r, WordList *list)
{
	if (NULL != r) 
	{
		list->InsertList(r);

		PopulateTreeIntoList(r->left, list);
		PopulateTreeIntoList(r->right, list);
	}
}

void ReadWords(const string fileName)
{
	string word;
	WordArray *myArray = new WordArray;

	ifstream fin;
	fin.open(fileName.c_str());

	if (fin.fail()) 
	{
		cout << "Error Opening file: " << fileName.c_str() <<endl;
		return;
	}

	ofstream fout;
	fout.open("wordarray.txt", ios::out|ios::trunc);

	if (fout.fail())
	{
		cout << "Error Opening File: wordarray.txt\n";
		return;
	}

	while (!fin.eof())
	{
		fin >> word;

		word = ExtractLetter(word);

		if (word.compare("") != 0)
			myArray->Push(word);
	}

	cout << "Total Words Count = " << myArray->Size() << endl;

	myArray->Trim();

	myArray->PrintOut(fout);

	myArray->Sort();

	myArray->PrintOut(fout);

	/***
	 * Get Mid Value
	 */
	int midIndex = myArray->Size()/2;
	string midWord = myArray->IndexOf(midIndex);
	cout << midWord << endl;

	/***
	 * Build Tree
	 */
	WordTree *myTree = new WordTree;

	myTree->InsertTree(midWord);

	for (int i = 0; i < myArray->Size(); i++)
	{
		if (i == midIndex) continue;

		myTree->InsertTree(myArray->IndexOf(i));
	}
	
	fout.close();
	fout.open("words.txt", ios::out|ios::trunc);

	if (fout.fail())
	{
		cout << "Error Opening File: words.txt\n";
		return;
	}
	myTree->PrintTree(fout);


	/***
	 * Build List
	 */
	WordList *myList = new WordList;

	WordNode_t *r = myTree->GetTreeRoot();

	/***
	 * pre-seq populate
	 */
	PopulateTreeIntoList(r, myList);

	fout.close();
	fout.open("frequency.txt", ios::out|ios::trunc);

	if (fout.fail())
	{
		cout << "Error Opening File: frequency.txt\n";
		return;
	}
	//myList->WriteOutList(fout);

	myList->SortList();
	myList->WriteOutList(fout);

	fout.close();

	delete myList;
	delete myTree;
	delete myArray;
}

int main(int argc, char** argv)
{
	ReadWords("inferno3.txt");

	return 0;
}
