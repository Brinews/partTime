#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "conio.h"
#include "malloc.h"

typedef struct node
{
	char name[10]; 
	int prio;  
	int round; 
	int cputime;
	int needtime;
	int count; 
	char state;
	struct node *next;
}PCB;
PCB *finish,*ready,*tail,*run;
int N;

void firstin()
{
	run=ready;  
	run->state='R';  
	ready=ready->next; 
}

void prt1(char a)
{
	if(toupper(a)=='P')
		printf("  name     cputime  needtime  priority  state\n");
	else
		printf("  name     cputime  needtime   count   round     state\n");
}

void prt2(char a,PCB *q)
{
	if(toupper(a)=='P') 
		printf("  %-10s%-10d%-10d%-10d %c\n",q->name,
				q->cputime,q->needtime,q->prio,q->state);
	else
		printf("  %-10s%-10d%-10d%-10d%-10d %-c\n",q->name,
				q->cputime,q->needtime,q->count,q->round,q->state);
}

void prt(char algo)
{
	PCB *p;
	prt1(algo); 
	if(run!=NULL)
		prt2(algo,run);
	p=ready; 
	while(p!=NULL)
	{
		prt2(algo,p);
		p=p->next;
	}
	p=finish; 
	while(p!=NULL)
	{
		prt2(algo,p);
		p=p->next;
	}
	getch(); 
}

void insert1(PCB *q)
{
	PCB *p1,*s,*r;
	int b;
	s=q; 
	p1=ready;
	r=p1;
	b=1;
	while((p1!=NULL)&&b) 
		if(p1->prio>=s->prio)
		{
			r=p1;
			p1=p1->next;
		}
		else
			b=0;
	if(r!=p1) 
	{
		r->next=s;
		s->next=p1;
	}
	else
	{
		s->next=p1; 
		ready=s;
	}
}

void insert2(PCB *p2)
{
	tail->next=p2; 
	tail=p2;
	p2->next=NULL;
}

void create1(char alg)
{
	PCB *p;
	int i,time;
	char na[10];
	ready=NULL;
	finish=NULL; 
	run=NULL;
	printf("Enter name and time of process\n");

	for(i=1;i<=N;i++)
	{
		p=(PCB *)malloc(sizeof(PCB));
		scanf("%s",na);
		scanf("%d",&time);
		strcpy(p->name,na);
		p->cputime=0;
		p->needtime=time;
		p->state='w';
		p->prio=50-time;
		if(ready!=NULL)
			insert1(p);
		else
		{
			p->next=ready;
			ready=p;
		}
	}
	//clr11();
	printf("          output of priority:\n");
	printf("************************************************\n");
	prt(alg); 
	run=ready;
	ready=ready->next;
	run->state='R';
}


void create2(char alg)
{
	PCB *p;
	int i,time;
	char na[10];
	ready=NULL;
	finish=NULL;
	run=NULL;
	printf("Enter name and time of round process\n");
	for(i=1;i<=N;i++)
	{
		p=(PCB *)malloc(sizeof(PCB));
		scanf("%s",na);
		scanf("%d",&time);
		strcpy(p->name,na);
		p->cputime=0;
		p->needtime=time;
		p->count=0;
		p->state='w';
		p->round=2; 
		if(ready!=NULL)
			insert2(p);
		else
		{
			p->next=ready;
			ready=p;
			tail=p;
		}
	}
	//clrscr();
	printf("              output of round\n");
	printf("************************************************\n");
	prt(alg);  
	run=ready; 
	ready=ready->next;
	run->state='R';
}






void priority(char alg)
{
	while(run!=NULL)
	{
		run->cputime=run->cputime+1;
		run->needtime=run->needtime-1;
		if(run->needtime==0)
		{
			run->next=finish;
			finish=run;
			run->state='F';
			run=NULL;
			if(ready!=NULL)
				firstin();

		}
		prt(alg);
	}
}







void roundrun(char alg)
{
	while(run!=NULL)
	{
		run->cputime=run->cputime+1;
		run->needtime=run->needtime-1;
		run->count=run->count+1;
		if(run->needtime==0)
		{
			run->next=finish;
			finish=run;
			run->state='F';
			run=NULL;
			if(ready!=NULL)
				firstin();
		}
		else
			if(run->count==run->round) 
			{
				run->count=0; 
				if(ready!=NULL)
				{
					run->state='W';
					insert2(run);
					firstin();
				}
			}
		prt(alg);
	}
}

void main(int argc, char* argv[])
{
	char algo; 
	//clrscr();
	printf("type the algorithm:P/R(priority/roundrobin)\n");
	scanf("%c",&algo);
	printf("Enter process number\n");
	scanf("%d",&N);
	if(algo=='P'||algo=='p')
	{
		create1(algo);
		priority(algo);
	}
	else
		if(algo=='R'||algo=='r')
		{
			create2(algo);
			roundrun(algo);
		}
	//return 0;
}

