#include "bstTree.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

song::song(){
    // constructor
    title=" ";
    artist=" ";
    runtime. hours=0;
    runtime. minutes=0;
    runtime. seconds=0;
}

song::~song(){
}

song::song(string songTitle, string  artistName, int songHour, int songMinute, int songSecond){
    
    addSong(songTitle, artistName, songHour, songMinute, songSecond);
}

void song::addSong(string songTitle, string  artistName, int songHour, int songMinute, int songSecond){
    
    title=songTitle;
    artist=artistName;
    runtime.hours=songHour;
    runtime.minutes=songMinute;
    runtime.seconds=songSecond;
}

//--------------------------------------
playList::playList(){
    
    title=" ";
    numSong = 0;
    totalPlayTime.hours = 0;
    totalPlayTime.minutes = 0;
    totalPlayTime.seconds = 0;
    head = NULL;
    tail = NULL;
}

playList::~playList(){
    
    while (head!=NULL) {
        songNode *temp = head;
        head = temp->next;
        temp->next = NULL;
        
        //std::cout << "Deleting:"<<endl;
        //temp->s.print(std::cout);
        delete temp;      
    }   
}

void playList::addTailSong(song s){
    
    songNode *temp;
    temp = new songNode;
    temp->s=s;
    temp->next = NULL;
    
    if (head==NULL) {
        head = temp;
        tail = temp;
    }
    else {
        tail->next = temp;
        tail = temp;
    }
    
    numSong++;
}

int playList::addSong(song s){
    addTailSong(s);
    return numSong;
}

song playList::findSong(string t)
{
	song s1,s2;
	songNode *temp;
	temp=head;
	s1=temp->s;
	int i=1;
	while(s1.getTitle()!=t&&i<numSong)
	{
		temp=temp->next;
		s1=temp->s;
		i++;
	}
	
	if(s1.getTitle()==t)
	{
		temp->s.print(cout);
		return s1;
	}
	else
	{
		cout<<"Cannot find "<< t << "from the list!"<<endl;
		return s2;
	}
	
}

int playList::removeSong(string t)
{
	song s1;
	songNode *temp1;
	songNode *temp2;
	temp1=head;
	int i=1;
	while(s1.getTitle()!=t&&i<numSong)
	{
		temp2=temp1;
		temp1=temp1->next;
		s1=temp1->s;
		i++;
	}
	
	if(temp1==head)
	{
		head=temp1->next;
		temp1->next=NULL;
		delete temp1;
		numSong--;
	}
	else if(temp1==tail)
	{
		if(s1.getTitle()==t)
		{
			tail=temp2;
			temp2->next=NULL;
			delete temp1;
			numSong--;
		}
		else
			cout<<"Cannot find" << t << "from the list!"<<endl;
	}
	else
	{
		temp2->next=temp1->next;
		temp1->next=NULL;
		delete temp1;
		numSong--;
	}
	return numSong;
}

void playList::sortingSong()
{
	songNode *temp, *temp1, *minLoc;
	song s1,s2,s3;
	for(temp=head;temp!=NULL;temp=temp->next)
	{
        minLoc = temp;
		for(temp1=temp->next;temp1!=NULL;temp1=temp1->next)
		{
			s2=temp1->s;
			s1=minLoc->s;
			if (s1.getTitle().compare(s2.getTitle())>0)
			{
				minLoc = temp1;
			}
		}
		
		if(minLoc!=temp)
		{
			s3=minLoc->s;
			minLoc->s=temp->s;
			temp->s=s3;
		}
	}
}

void playList::print(ostream& out){
    
    if (head == NULL) {
        cout<< "Empty List"<<endl;
        return;
    }
    out<<"number of songs in the list: "<<numSong<<endl;
    songNode * s = head;
    while (s != NULL) {
        s->s.print(out);
        s = s->next;
    }
}


void readData(const string filename, playList &l) {
	ifstream fin;
	fin.open(filename.c_str());
	if (fin.fail()) cout << "ERROR opening file\n";
    
	while (!fin.eof()) {
		string title,artist,runtime;
		getline(fin,title);
		getline(fin,artist);
		getline(fin,runtime);
        
		song s(title,artist,(atoi(runtime.c_str()))/3600,((atoi(runtime.c_str()))%3600)/60, (atoi(runtime.c_str()))%60);

		if (title.compare("") != 0)
			l.addSong(s);
	}
    
	fin.close();
}

/***
 * BstTree Class
 */
void BstTree::addSong(song *s)
{
	insertTree(root, s);
}

song *BstTree::findSong(string title)
{
	bnode *b = findTree(root, title);

	if (NULL != b)
		return b->s;

	return NULL;
}

int BstTree::getDepth()
{
	return getTreeDepth(root);
}

int BstTree::getNumSongs()
{
	return songCount;
}

void BstTree::printTree(bool asc)
{
	printBNode(root, 0);
}

//---------------------------------------
int main()
{
	BstTree *myUnsortTree = new BstTree();

	playList l;
	readData("Lab4.txt", l);
	//l.print(cout);

	//cout << l.getNumSong() << endl;

	cout << "****Disorder Insert****" << endl;
	songNode *myHead = l.getListHead();
	while (NULL != myHead)
	{
		//cout << myHead->s.getTitle() << endl;

		myUnsortTree->addSong(&myHead->s);

		myHead = myHead->next;
	}

	myUnsortTree->printTree(true);

	cout << "When Insert No Order, Tree Depth = " << myUnsortTree->getDepth() << endl;
	cout << "********************" << endl << endl;

	BstTree *mySortTree = new BstTree();
	l.sortingSong();
	//l.print(cout);

	cout << "===Order Insert====" << endl;

	myHead = l.getListHead();
	while (NULL != myHead)
	{
		//cout << myHead->s.getTitle() << endl;

		mySortTree->addSong(&myHead->s);

		myHead = myHead->next;
	}

	mySortTree->printTree(true);

	cout << "==================" << endl;

	cout << "When Insert By Order, Tree Depth = " << mySortTree->getDepth() << endl;


	delete myUnsortTree;
	delete mySortTree;

    return 0;
}

