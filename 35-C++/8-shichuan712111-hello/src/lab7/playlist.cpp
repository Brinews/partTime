#include "playlist.h"

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

void playList::addTailSong(song *s)
{
	if (findSameSong(s) == false) 
	{
		songNode *temp;
		temp = new songNode;
		temp->s = new song(*s);
		temp->next = NULL;

		if (head == NULL) {
			head = temp;
			tail = temp;
		}
		else {
			tail->next = temp;
			tail = temp;
		}

		numSong++;
	}
}

int playList::addSong(song *s){

    addTailSong(s);

    return numSong;
}

bool playList::findSameSong(song *s)
{
	bool ret = false;

	int i = 1;
	songNode *temp = head;
	song *s1;

	while (i <= numSong)
	{
		s1 = temp->s;

		if (s1->getTitle().compare(s->getTitle()) == 0
			&& s1->getArtist().compare(s->getArtist()) == 0
			&& s1->getRunTime() == s->getRunTime())
			return true;

		i++;
		if (i > numSong) break;

		temp = temp->next;
		s1 = temp->s;
	}

	return ret;
}

song* playList::findSong(string t)
{
	song *s1,*s2 = NULL;
	songNode *temp;
	temp=head;
	s1=temp->s;
	int i=1;

	while (s1->getTitle() != t && i < numSong)
	{
		temp=temp->next;
		s1=temp->s;
		i++;
	}
	
	if(s1->getTitle() == t)
	{
		temp->s->print(cout);
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
	if (getNumSong() == 0) return 0;

	song *s1 = head->s;
	songNode *temp1;
	songNode *temp2;
	temp1 = head;
	int i=1;

	while(s1->getTitle() != t && i < numSong)
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
		if(s1->getTitle()==t)
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
	song *s1,*s2,*s3;

	for(temp=head;temp!=NULL;temp=temp->next)
	{
        minLoc = temp;
		for(temp1=temp->next;temp1!=NULL;temp1=temp1->next)
		{
			s2=temp1->s;
			s1=minLoc->s;
			if (s1->getTitle().compare(s2->getTitle())>0)
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
    out<<"Number of songs in the list: "<<numSong<<endl;
    songNode *s = head;
    while (s != NULL) {
        s->s->print(out);
        s = s->next;
    }
}

/***
 * SongBst Class
 */
void SongBst::addSong(song *s)
{
	insertTree(root, s);
}

song *SongBst::findSong(string title)
{
	snodebst *b = findTree(root, title);

	if (NULL != b)
		return b->s;

	return NULL;
}

int SongBst::getDepth()
{
	return getTreeDepth(root);
}

int SongBst::getNumSongs()
{
	return songCount;
}

void SongBst::printTree(bool asc)
{
	printBNode(root, 0);
}

/***
 * PlayListBst Class
 */
void PlayListBst::addPlayList(playList *p)
{
	insertTree(root, p);
}

playList* PlayListBst::findPlayList(string listTitle)
{
	pnodebst *pbst = findTree(root, listTitle);

	if (NULL != pbst)
		return pbst->p;

	return NULL;
}

void PlayListBst::printListTree(bool asc)
{
	printPNode(root, 0);
}

/***
 * Build The PlayList From data file
 */
void BuildPlayList(const string filename, playList &l) 
{
	ifstream fin;
	fin.open(filename.c_str());
	if (fin.fail()) cout << "ERROR opening file in BuildPlayList\n";

	song *s = new song();
    
	while (!fin.eof()) {
		string title,artist,runtime;
		getline(fin,title);
		getline(fin,artist);
		getline(fin,runtime);
        
		s->addSong(title,artist,(atoi(runtime.c_str()))/3600,((atoi(runtime.c_str()))%3600)/60, (atoi(runtime.c_str()))%60);

		if (title.compare("") != 0)
			l.addSong(s);
	}

	delete s;
    
	fin.close();
}

/***
 * Build BST from song file
 */
void BuildSongTree(const string filename, SongBst &st)
{
	ifstream fin;
	fin.open(filename.c_str());
	if (fin.fail()) cout << "ERROR opening file in BuildSongTree\n";

	song *s = new song();
    
	while (!fin.eof()) {

		string title,artist,runtime;
		getline(fin,title);
		getline(fin,artist);
		getline(fin,runtime);
        
		s->addSong(title,artist,(atoi(runtime.c_str()))/3600,((atoi(runtime.c_str()))%3600)/60, (atoi(runtime.c_str()))%60);

		if (title.compare("") != 0)
			st.addSong(s);

	}

	delete s;
    
	fin.close();
}

/***
 * Build BST PlayList from song file
 */
void BuildPlaylistTree(const string filename, PlayListBst &plt)
{
	ifstream fin;
	fin.open(filename.c_str());
	if (fin.fail()) cout << "ERROR opening file in BuildPlaylistTree\n";

	song *s = new song();
	playList *pl;
    
	while (!fin.eof()) {

		string playlist_title;
		string title,artist,runtime;

		getline(fin, playlist_title);
		getline(fin, title);
		getline(fin, artist);
		getline(fin, runtime);
        
		s->addSong(title,artist,(atoi(runtime.c_str()))/3600,((atoi(runtime.c_str()))%3600)/60, (atoi(runtime.c_str()))%60);

		if (playlist_title.compare("") != 0)
		{
			pl = plt.findPlayList(playlist_title);
			if (NULL == pl)
			{
				/* create new playlist */
				pl = new playList(playlist_title);
				plt.addPlayList(pl);
			}

			pl->addSong(s);
		}
	}

	delete s;
    
	fin.close();
}

/***
 * Menu Command
 */
void MenuCommand()
{
	int opt = 0;

	SongBst st;
	PlayListBst plt;

	string itle;
	string ist, isa, ism;
	string ltle, stle;
	playList *pl;
	song *s;

	while (true)
	{
		cout << "1. Load Songs Into SongBST from file." << endl;
		cout << "2. Load Playlists Into PlaylistBST from file." << endl;
		cout << "3. List All Songs in SongBST." << endl;
		cout << "4. Search for Song by Title in SongBST." << endl;
		cout << "5. List All PlayLists in PlayListBST." << endl;
		cout << "6. Search for PlayList by Title in PlayListBST." << endl;
		cout << "7. List Songs for PlayList in PlayListBST." << endl;
		cout << "8. Add Song to PlayList in PlayListBST." << endl;
		cout << "9. Remove Song from PlayList in PlayListBST." << endl;
		cout << "0. Exit." << endl;

		cin >> opt;

		if (0 == opt) return;
		switch (opt)
		{
			case 1:
				BuildSongTree("songs.txt", st);
				cout << ">> load songs complete." << endl;
				break;
			case 2:
				BuildPlaylistTree("playlists.txt", plt);
				cout << ">> load playlists complete." << endl;
				break;
			case 3:
				cout << ">> list songs complete." << endl;
				st.printTree();
				break;
			case 4:
				//cin.clear();
				cout << ">> input song title :" << endl;
				getline(cin, ist);
				getline(cin, ist);

				s = st.findSong(ist);
				if (NULL != s) 
				{
					cout << ">> find songs complete:" << endl;
					s->print(cout);
				}
				else
					cout << ">> not find songs ." << endl;

				break;
			case 5:
				cout << ">> list playlists complete." << endl;
				plt.printListTree();
				break;
			case 6:
				cout << ">> input playlist title :\n";
				getline(cin, itle);
				getline(cin, itle);

				pl = plt.findPlayList(itle);
				if (NULL != pl)
				{
					cout << ">> find playlist complete:" << endl;
					pl->print(cout);
				}
				else
					cout << ">> not find playlist by title: " << itle << endl;
				break;
			case 7:
				cout << ">> list songs for playlists complete." << endl;
				plt.printPlayListSongs(cout);
				break;
			case 8:
				cout << ">> begin to add songs into playlists." << endl;
				s = new song();
				cout << ">> input playlist's title, songs's title, artist and time, each one line:" << endl;
				getline(cin,itle);
				getline(cin,itle);
				getline(cin,ist);
				getline(cin,isa);
				getline(cin,ism);

				s->addSong(ist,isa,(atoi(ism.c_str()))/3600,((atoi(ism.c_str()))%3600)/60, (atoi(ism.c_str()))%60);

				if (itle.compare("") != 0) 
				{
					pl = plt.findPlayList(itle);
					if (NULL == pl)
					{
						pl = new playList(itle);
						plt.addPlayList(pl);
					}
					pl->addSong(s);

				}
				delete s;

				break;
			case 9:
				cout << ">> remove songs from playlists ." << endl;
				cout << ">> input playlist's title and song's title, each one line:" << endl;
				getline(cin,itle);
				getline(cin,itle);
				getline(cin,ist);

				if (itle.compare("") != 0) 
				{
					pl = plt.findPlayList(itle);
					if (NULL != pl)
						pl->removeSong(ist);
				}

				break;
		}
	}
}

/***
 * SongBst and playList Test
 */
void TestCase()
{
	SongBst *myUnsortTree = new SongBst();

	playList l;
	BuildPlayList("Lab4.txt", l);
	l.print(cout);

	cout << l.getNumSong() << endl;

	cout << "****Disorder Insert****" << endl;
	songNode *myHead = l.getListHead();
	while (NULL != myHead)
	{
		//cout << myHead->s.getTitle() << endl;

		myUnsortTree->addSong(myHead->s);

		myHead = myHead->next;
	}

	myUnsortTree->printTree(true);

	cout << "When Insert No Order, Tree Depth = " << myUnsortTree->getDepth() << endl;
	cout << "********************" << endl << endl;

	SongBst *mySortTree = new SongBst();
	l.sortingSong();
	//l.print(cout);

	cout << "===Order Insert====" << endl;

	myHead = l.getListHead();
	while (NULL != myHead)
	{
		//cout << myHead->s.getTitle() << endl;

		mySortTree->addSong(myHead->s);

		myHead = myHead->next;
	}

	mySortTree->printTree(true);

	cout << "==================" << endl;

	cout << "When Insert By Order, Tree Depth = " << mySortTree->getDepth() << endl;


	delete myUnsortTree;
	delete mySortTree;
}

//---------------------------------------
int main()
{

	MenuCommand();
	return 0;
}

