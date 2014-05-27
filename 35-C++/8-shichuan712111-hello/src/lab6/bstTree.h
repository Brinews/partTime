#ifndef _BSDTREE_H
#define _BSDTREE_H

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
		virtual~song();

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
    
    song s;
    songNode *next;
    
};

class playList {
    
private:
    
    string title;
    int numSong;
    songTime totalPlayTime;
    songNode *head;
    songNode *tail;
    void addTailSong(song);

public:
    
    playList();
    playList(string);
    virtual ~playList();
   
    int addSong(song);
    song findSong(string);
    int removeSong(string);
    void print(ostream&);
    
    void sortingSong();
    void getHead(song &s)
	{
		s.addSong(head->s);
	}

	songNode* getListHead()
	{
		return head;
	}

	int getNumSong()
	{
		return numSong;
	}
};

class BstTree
{
private:
	struct bnode
	{
		song *s;
		struct bnode *left, *right;

		bnode()
		{
			s = NULL;
			left = right = NULL;
		}

		~bnode()
		{
			delete s;
		}
	};

	bnode *root;
	int songCount;

public:
	BstTree()
	{
		root = NULL;
		songCount = 0;
	}

	~BstTree()
	{
		destroyTree(root);
	}

	void destroyTree(bnode *r)
	{
		if (NULL != r)
		{
			destroyTree(r->left);
			destroyTree(r->right);

			delete r;
			r = NULL;
		}
	}

	bool insertTree(bnode *&r, song *s)
	{
		if (NULL == r)
		{
			r = new bnode();
			r->s = new song;
			r->s->addSong(*s);

			r->left = r->right = NULL;

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

	bnode* findTree(bnode *r, string t)
	{
		if (NULL != r)
		{
			if (r->s->getTitle().compare(t) == 0)
				return r;

			findTree(r->left, t);
			findTree(r->right, t);
		}

		return NULL;
	}

	int getTreeDepth(bnode *r)
	{
		if (NULL == r) return 0;

		int ld = getTreeDepth(r->left);
		int rd = getTreeDepth(r->right);

		if (ld < rd) ld = rd;

		return ld+1;
	}

	void printBNode(bnode *r, int dep)
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

#endif
