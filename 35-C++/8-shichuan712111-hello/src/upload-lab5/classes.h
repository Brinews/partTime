//
//  classes.h
//  lab5
//
//  Created by Ciel Gao on 13-11-7.
//  Copyright (c) 2013年 Ciel Gao. All rights reserved.
//

#ifndef __lab5__classes__
#define __lab5__classes__

#include <iostream>

using namespace std;

int randomg();

class customer{
public:
    int customerID,arrivaltime,dequeuetime,waittime;
    customer();
    customer(int,int);
    void setarv(int);
    void setde(int);
};


template <class T>
class queue{
public:
    struct node {
        T data;
        node (T d) {data = d;}
        node *next;
    };
	node *top;
    node *end;


    int timenow;
    int max;
    int qnum;
	queue(int);
	~queue();
	void enqueue(T,int);
	bool dequeue(T&);
    void maxnum();
    
};

template <class T>
queue<T>::queue(int t){
    top=NULL;
    end=NULL;
    timenow=t;
    qnum=0;
}


template <class T>
queue<T>::~queue(){
    node *n;
    while(top!=NULL){
        n=top;
        top=top->next;
        n->next=NULL;
        delete n;
    }
}


template <class T>
void queue<T>::enqueue(T d,int t){
    node *k=new node(d);
    timenow=t;
    cout<<"Customer "<<d.customerID<<" arrived at "<<timenow<<" mins"<<endl;
    if(top==NULL){
        
        k->next=NULL;
        top=k;
        end=k;
        
    }
    else{
        //k->data=d;
        k->next=NULL;
        end->next=k;
        end=k;
        
    }
    
    qnum++;
    maxnum();
}


template <class T>
bool queue<T>::dequeue(T &d){
   	if (top==NULL)
        return false;
	node *n = top;
	top = top->next;
	n->next = NULL;
	d = n->data;
	delete n;
    qnum--;
	return true;
    
}



template <class T>
void queue<T>::maxnum(){
    if (qnum>max)
        max=qnum;
    
}



class cashier{
public:
    customer* current;
    int servt;
    int start,end;
    bool busy;
    
    cashier(customer&,int);
    void setcus(int);
};



#endif /* defined(__lab5__classes__) */
