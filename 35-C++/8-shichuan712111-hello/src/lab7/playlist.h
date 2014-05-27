#ifndef _PLAYLIST_H
#define _PLAYLIST_H

#include <iostream>
#include <string>

using namespace std;

struct songTime {
	//variables
	int hours, minutes, seconds;
	//constructor
	songTime()
	{
		hours = 0;
		minutes = 0;
		seconds = 0;
	}

	bool operator==(const songTime &st) const
	{
		return hours == st.hours && minutes == st.minutes 
			   && seconds == st.seconds;
	}

	songTime(int h, int m, int s)
	{
		hours = h;
		minutes = m;
		seconds = s;
	}
	//methods
	songTime addTime(songTime & time0)
	{
		songTime t;
		t.hours = time0.hours + hours;
		t.minutes = time0.minutes + minutes;
		t.seconds = time0.seconds + seconds;
		if (t.seconds >60 ){
			t.seconds=t.seconds % 60;
			t.minutes=t.minutes+1;
		}
		if (t.minutes>60) {
			t.minutes=t.minutes % 60;
			t.hours=t.hours+1;
		}

		return t;

	}

	void print()
	{
		std::cout<<"time is "<<hours<<" hours, "<<minutes<<" minutes "<<seconds<<" seconds."<<endl;
	}

	void setTime(songTime &time)
	{
		hours		= time.hours;
		minutes		= time.minutes;
		seconds		= time.seconds;
	}
};

class song {
private:
	string title;
	string artist;
	songTime runtime;

public:
	song();
	song(string, string, int, int, int);
	song(song &a)
	{
		title = a.title;
		artist = a.artist;
		runtime.setTime(a.runtime);
	}

	virtual ~song();

	void addSong(string, string, int, int, int);
	void addSong(song &Add)
	{
		title = Add.title;
		artist = Add.artist;
		runtime.setTime(Add.runtime);
	}

	string getArtist()
	{
		return artist;
	}

	string getTitle()
	{
		return title;
	}

	songTime getRunTime()
	{
		return runtime;
	}

	void print(ostream& out)
	{
		songTime t=runtime;
		int r=t.hours*60*60+t.minutes*60+t.seconds;
		out<<title<<endl<<artist<<endl<<r<<endl;
	}
};

struct songNode {
    song *s;
    songNode *next;

	songNode()
	{
		s = NULL;
		next = NULL;
	}

	~songNode()
	{
		delete s;
	}
};

class playList {
private:
    string title;
    int numSong;
    songTime totalPlayTime;
    songNode *head;
    songNode *tail;
    void addTailSong(song *);
	bool findSameSong(song *);

public:
    playList();

    playList(string t) : title(t), numSong(0), head(NULL), tail(NULL)
	{
		totalPlayTime.hours = 0;
		totalPlayTime.minutes = 0;
		totalPlayTime.seconds = 0;
	}

    virtual ~playList();
   
    int addSong(song*);
    song* findSong(string);
    int removeSong(string);
    void print(ostream&);
    
    void sortingSong();
    void getHead(song &s)
	{
		s.addSong(*(head->s));
	}

	songNode* getListHead()
	{
		return head;
	}

	int getNumSong()
	{
		return numSong;
	}

	string getListTitle()
	{
		return title;
	}
};

class SongBst
{
private:
	struct snodebst
	{
		song *s;
		struct snodebst *left, *right;

		snodebst()
		{
			s = NULL;
			left = right = NULL;
		}

		~snodebst()
		{
			delete s;
		}
	};

	snodebst *root;
	int songCount;

public:
	SongBst()
	{
		root = NULL;
		songCount = 0;
	}

	virtual ~SongBst()
	{
		destroyTree(root);
	}

	void destroyTree(snodebst *r)
	{
		if (NULL != r)
		{
			destroyTree(r->left);
			destroyTree(r->right);

			delete r;
			r = NULL;
		}
	}

	bool insertTree(snodebst *&r, song *s)
	{
		if (NULL == r)
		{
			r = new snodebst();
			r->s = new song;
			r->s->addSong(*s);

			songCount++;

			return true;
		}

		/***
		 * Already exist
		 */
		int ts = r->s->getTitle().compare(s->getTitle());
		int as = r->s->getArtist().compare(s->getArtist());
		if (ts == 0 && as == 0)
			return false;

		bool ret = false;
		int ls = 0;
		if (ts < 0) 
		{
			ls = 0;
		}
		else if (ts > 0)
		{
			ls = 1;
		}
		else if (ts == 0)
		{
			if (as < 0) ls = 0;
			else ls = 1;
		}

		if (0 == ls)
		{
			ret = insertTree(r->left, s);
		}
		else if (1 == ls)
		{
			ret = insertTree(r->right, s);
		}

		return ret;
	}

	snodebst* findTree(snodebst *r, string t)
	{
		if (NULL != r)
		{
			int ret = r->s->getTitle().compare(t);
			if (ret == 0) 
			{
				return r;
			}

			if (ret < 0)
				return findTree(r->left, t);
			else
				return findTree(r->right, t);
		}

		return NULL;
	}

	int getTreeDepth(snodebst *r)
	{
		if (NULL == r) return 0;

		int ld = getTreeDepth(r->left);
		int rd = getTreeDepth(r->right);

		if (ld < rd) ld = rd;

		return ld+1;
	}

	void printBNode(snodebst *r, int dep)
	{
		if (NULL != r)
		{
			printBNode(r->left, dep + 1);
			for (int i = 0; i < dep; i++)
				cout << "- ";
			cout << ":" << r->s->getTitle() << " [" << r->s->getArtist() << "]" << endl;
			printBNode(r->right, dep + 1);
		}
	}

	void addSong(song *s);
	song *findSong(string title);
	int getDepth();
	int getNumSongs();
	void printTree(bool asc = true);

};

struct pnodebst
{
	playList *p;
	pnodebst *left, *right;

	pnodebst()
	{
		p = NULL;
		left = right = NULL;
	}

	~pnodebst()
	{
		delete p;
	}
};

class PlayListBst
{
private:
	pnodebst *root;
	int listTotal;

public:
	PlayListBst()
	{
		root = NULL;
		listTotal = 0;
	}

	virtual ~PlayListBst()
	{
		destroyTree(root);
	}

	void destroyTree(pnodebst *r)
	{
		if (NULL != r)
		{
			destroyTree(r->left);
			destroyTree(r->right);

			/* will cause ~playList() */
			delete r;
			r = NULL;
		}
	}

	bool insertTree(pnodebst *&r, playList *plist)
	{
		if (NULL == r)
		{
			r = new pnodebst();
			r->p = plist;

			listTotal++;
			
			return true;
		}

		/***
		 * Find Node
		 */
		int ret = r->p->getListTitle().compare(plist->getListTitle());

		if (0 == ret) return false;

		if (ret < 0)
		{
			return insertTree(r->left, plist);
		}
		else
		{
			return insertTree(r->right, plist);
		}
	}

	pnodebst* findTree(pnodebst *r, string title)
	{
		if (NULL != r)
		{
			int ret = r->p->getListTitle().compare(title);
			if (ret == 0)
				return r;

			if (ret < 0)
				return findTree(r->left, title);
			else
				return findTree(r->right, title);
		}

		return NULL;
	}

	void printPNode(pnodebst *r, int dep)
	{
		if (NULL != r)
		{
			printPNode(r->left, dep+1);

			for (int i = 0; i < dep; i++)
				cout << "- ";
			cout << ":" << r->p->getListTitle() << endl;

			printPNode(r->right, dep+1);
		}
	}

	void printPNodeSongs(pnodebst *r, int dep, ostream &out)
	{
		if (NULL != r)
		{
			printPNodeSongs(r->left, dep+1, out);

			for (int i = 0; i < dep; i++)
				out << "- ";
			out << ":" << r->p->getListTitle() << endl;
			r->p->print(out);

			printPNodeSongs(r->right, dep+1, out);
		}
	}

	void addPlayList(playList *);
	playList* findPlayList(string title);

	int getPlayListTotal()
	{
		return listTotal;
	}

	void printListTree(bool asc = true);
	void printPlayListSongs(ostream &out)
	{
		printPNodeSongs(root, 0, out);
	}
};


#endif
