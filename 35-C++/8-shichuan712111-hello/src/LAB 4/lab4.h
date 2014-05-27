//
//  Lab 4.h
//  EEN_218_Lab
//
//  Created by Chuan Shi on 10/22/13.
//  Copyright (c) 2013 Chuan Shi. All rights reserved.
//
#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;
#ifndef EEN_218_Lab_Lab_4_h
#define EEN_218_Lab_Lab_4_h

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
	void addSong( song &Add)
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
};

#endif
