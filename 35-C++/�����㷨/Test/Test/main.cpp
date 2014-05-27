#include <conio.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <ctime>
#include "colorConsole.h"   //��ʦ���ļ�

void begin();               //��ʼ��Ϸ
void frame();               //�߿��趨
int * getblocks();          //�������
void move(int line);        //�ƶ�
void drawblocks(int line);  //������ʾ
void clearsquare(int line); //�������
void turn(int line);        //������ת
bool isavailable(int line); //�ж��Ƿ�������
void remember(int line);    //���䷽��λ��
void deleteline(int line);  //������һ������
bool ifgameover();          //�ж��Ƿ���Ϸ����
void end();                 //��Ϸ���� 

#define up      72
#define down    80
#define left    75
#define right   77
#define esc     27

HANDLE handle;

int a1[4][4]={{1},{1,1,1}};            //���ַ���Ķ�ά����
int a2[4][4]={{0,1},{1,1,1}};
int a3[4][4]={{1,1},{0,1,1}};
int a4[4][4]={{0,0,1},{1,1,1}};
int a5[4][4]={{0,1,1},{1,1}};
int a6[4][4]={{1,1,1,1}};
int a7[4][4]={{1,1},{1,1}};
int row=0;                              //����
int score=0;
int level=0;
int * block1=NULL;  
int * block2=NULL;
int * block3=NULL;
int coordinate[12][18]={0};             //�������飬�߿�12*18(���һ�У����߱߿��������)
int judge=0;
int scorex=0;
int temp[4][4]={0};

void main()                             //������
{ 
	int t=1;
	handle = initiate();
	while(t)
	{
		t=0;
	    begin(); 
	    //sndPlaySound("music.wav",SND_LOOP|SND_ASYNC);
	    frame();
	    WORD wColors[1];
	    wColors[0]=FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY;
	    for(int k=1;k<=999999;k++)
		{
			if(ifgameover())       //�ж��Ƿ����
			{
			    textout(handle,34,10,wColors,1,"Game Over");
		    	Sleep(800);
		    	end();
			}
	    	else
			{
		    	if(k==1)
				block2=getblocks();
				block3=block2;         //block2ָ�򽫳��ֵķ����ַ
				block2=getblocks();    //��ȡ��һ���µķ���
				block1=block3;         
				row=52; 
				clearsquare(16);       //����next�ķ���
				block1=block2;
				drawblocks(15);        //��next��ʾ��һ�鷽��ͼ��
				row=34;
				block1=block3;
				for(int i=4;i<=7;i++)  //�������ķ���ͼ�����ֻ��ռ�����ţ�����ֻ��4-7���ɶ�Ӧ
				{   
					if(*(block1+i))
					textout(handle,26+i*2,4,wColors,1,"��");     //������¶�����沿��
				}
				Sleep(500-50*level);
				for(int line=4;line<=22;line++)                      //�����������䣬����ӵ����ſ�ʼ����
				{
					if(isavailable(line))                            //����ղ����ķ����Ƿ����ڣ��������䷽��
					{
						clearsquare(line);                           //����������¶���������
						drawblocks(line);                            //�������������䷽��
						move(line);
					}
					else
					{
						remember(line);                              //�䶨����Щλ�ö�Ӧ��all������Ԫ����1
						deleteline(line);                            //�����Լ��ӷ�
						if(line==4)
						judge=1;
						break;
					}
				}
			}
		}	
	}
}


void begin()
{
	int i=1;
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_INTENSITY;
	WORD wColors1[2];
	wColors1[0]=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
    wColors1[1]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	textout(handle,18,4,wColors,1,"           ����     ����");
	textout(handle,18,5,wColors,1,"         �� �� �� �� �� �� ");
	textout(handle,18,6,wColors,1,"����������������������������");
	textout(handle,18,7,wColors,1,"�����������ޡ����������ި�����");
	textout(handle,18,8,wColors,1,"����������������������  ������ ");
	textout(handle,18,9,wColors,1,"��������������������������������");
	textout(handle,18,10,wColors,1,"�����������񡡡��������񡡨�����");
	textout(handle,18,11,wColors,1,"���������򡡡������������򡡨���");
	textout(handle,18,12,wColors,1,"�����������������ԡ�������������");
	textout(handle,18,13,wColors,1,"����������������������������");
	textout(handle,18,14,wColors,1,"��������������������������");
	textout(handle,18,15,wColors,1,"        ��������		    ");
	textout(handle,18,16,wColors,1,"�� �� ������������������ �е�");
	textout(handle,18,17,wColors,1,"���밴1����     ��         �밴2");
	textout(handle,18,18,wColors,1,"��������        ��");
	textout(handle,18,19,wColors,1,"��������    ����  ����");
	textout(handle,18,20,wColors,1,"           ����    �밴 3");
    textout(handle,54,22,wColors,1,"MADE BY ");
	while(i)
	{
		textout(handle,30,8,wColors1,2,"����˹����");
		Sleep(800);
		textout(handle,30,8,wColors1,2,"          ");
        Sleep(800);
	    if (_kbhit())         //����ȼ�
		{
			switch(_getch())
			{
			case '1':            
				{
					level=1;
					i=0;      //����ѭ��
					break;
				}
			case '2':           
				{
					level=4;
					i=0;
					break;
				}
			case '3':           
				{
					level=7;
					i=0;
					break;
				}
			}
		}
	}
	system("cls");      //����
}

void frame()            //�߿���趨
{
	
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
	WORD wColors1[1];
	wColors1[0]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	for(int i=0;i<=11;i++)
		coordinate[i][17]=1;   //���ű߿���Ϊ1
	for(int j=0;j<=17;j++)
	{
		coordinate[0][j]=1;    //���߱߿���Ϊ1
		coordinate[11][j]=1;
	}
	char string[5];
	textout(handle,59,5,wColors,1,itoa(level,string,10));
    textout(handle,52,5,wColors,1,"level: ");
	textout(handle,52,9,wColors,1,"score: 0");
	textout(handle,52,13,wColors,1,"next:");
	textout(handle,10,6,wColors1,1,"��ͣ    SPACE");
	textout(handle,10,7,wColors1,1,"�˳�    ESC");
	textout(handle,10,8,wColors1,1,"��ת    ��");
	textout(handle,10,9,wColors1,1,"����    ��");
	textout(handle,10,10,wColors1,1,"����    ��");
	textout(handle,10,11,wColors1,1,"����    ��");
	textout(handle,33,2,wColors,1,"��~ս��ʹ");
	for(int m=13;m<=24;m++)
	{
		textout(handle,2*m,3,wColors,1,"�T");   //�ϱ߿�
	}
	for(int n=4;n<=21;n++)
	{
		textout(handle,26,n,wColors,1,"��");    //��߿�
	}
	for(int k=4;k<=21;k++)
	{
		textout(handle,48,k,wColors,1,"��");    //�ұ߿�
	}
	for(int l=13;l<=23;l++)
	{
		textout(handle,2*l,21,wColors,1,"�T");  //�±߿�
	}
	textout(handle,26,3,wColors,1,"��");
	textout(handle,48,3,wColors,1,"��");
	textout(handle,26,21,wColors,1,"��");
	textout(handle,48,21,wColors,1,"��");
}

int * getblocks()           //�����������
{
	int * m=NULL;
	srand(time(NULL));    
	int n=rand()%7;        
	switch(n)
	{
	case 0:
		m=&a1[0][0];break;
	case 1:
		m=&a2[0][0];break;
	case 2:
		m=&a3[0][0];break;
	case 3:
		m=&a4[0][0];break;
	case 4:
		m=&a5[0][0];break;
	case 5:
		m=&a6[0][0];break;
	case 6:
		m=&a7[0][0];break;
	}
	return m;
}


void drawblocks(int line)     //���ַ���
{
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY;
	for(int j=0;j<=15;j++)
	{ 	
		int temp;
		temp=j/4;
		if(*(block1+j))
		textout(handle,row+j*2-temp*8,line+temp,wColors,1,"��");
	}
}


void clearsquare(int line)      //������ʧ
{
	WORD wColors[1];
	wColors[0]=FOREGROUND_BLUE|FOREGROUND_INTENSITY;
	if(line==4)                                  //��������ղ���������
	{
		textout(handle,34,4,wColors,1,"  ");
		textout(handle,36,4,wColors,1,"  ");
		textout(handle,38,4,wColors,1,"  ");
		textout(handle,40,4,wColors,1,"  ");
	}
	else
	{
		for(int m=0;m<=15;m++)
		{ 
			int temp;
			temp=m/4;       //��0-3��Ӧ��������1-4��
			if(*(block1+m))
			textout(handle,row+m*2-temp*8,line-1+temp,wColors,1,"  ");
		}
	}	
}

void move(int line)        //����������ƶ����������䣬��ת��
{
	int mid=0,speed=100-10*level;
	while(mid<speed)
	{
		if (_kbhit())
		{
			switch(_getch())
			{
			case 72:                        //��ת
				{
					turn(line);
					break;
				}			
			case 75:                         //����
				{
					row=row-2;               //�������2
					if(isavailable(line))    //�ж��Ƿ����ƶ�
					{
						row=row+2;
						clearsquare(line+1); //����ԭ��ͼ����line+1�Ǳ���line=4�������
						row=row-2;
						drawblocks(line);    //������ͼ��
					}
					else
						row=row+2;           //�������ƶ��������겻��
					break;
				}
			case 77:                         //����
				{
					row=row+2;
					if(isavailable(line))
					{
						row=row-2;
						clearsquare(line+1);
						row=row+2;
						drawblocks(line);
					}
					else
						row=row-2;
					break;
				}
			case 80:          //�������䣬��ֱ������ѭ��
				{
					mid=speed;
					break;
				}
			case 27:          //��ֹ��Ϸ
				{
					end();
					break;
				}	
          	case 32:          //��ͣ
				{
					int flag=1;
					while(flag)
					{
						if (_kbhit())
						{
							if(_getch()==32)
								flag=0;
							break;
						}
						else
							Sleep(10);
					}
				}
			default:
				break;
			}
			
		}
		Sleep(8);             //ʹ�����ӳ�
		mid++;
	}
}

void turn(int line)
{
	clearsquare(line+1);   	  //����ԭ����ͼ��
	int b[4][4]={0};          //������תǰ�ķ���
	int num=0,l=0;
	for(int m=0;m<=3;m++)
	{
		for(int n=0;n<=3;n++)
		{
			b[m][n]=*(block1+m*4+n);    //��b[4][4]ȫ��ֵΪ��ǰͼ������
            temp[m][n]=0;
		}
	}
	for(int i=3;i>=0;i--)               //���д�������ɨ��
	{
		for(int j=0;j<4;j++)            //���д�������ɨ��
		{
			if(b[i][j])                 //���Ϊ��Ч�㣬�����90����ת
			{
		      temp[j][l]=b[i][j];
		      num=1;
			}
		}
		if(num)
		{
			l++;
			num=0;
		}
	}
	block1=&temp[0][0];
	if(isavailable(line))
		drawblocks(line);	
	else
	{
		for(int p=0;p<=3;p++)
		{
			for(int q=0;q<=3;q++)
				temp[p][q]=b[p][q];
		}
		block1=&temp[0][0];
		drawblocks(line);
	}
}

bool isavailable(int line)                //���飬�������鼴������λ���Ƿ��Ѿ���1
{
	int x,y;
	for(int m=0;m<=15;m++)
	{ 
		int temp;
		temp=m/4;
		x=row/2-13-4*temp+m;              //�߿��������13��λ��
		y=line-4+temp;                    //��������4��λ��
		if(*(block1+m)&&coordinate[x][y]) //����Ϊ1�򷵻�0������������ѭ������
			return 0;
	}
}

void remember(int line)                   //����
{
	int x,y;
	for(int m=0;m<=15;m++)
	{ 
		int temp;
		temp=m/4;
		x=row/2-13-temp*4+m;
		y=line-4+temp;
		if(*(block1+m))                   //�����ǰλ��Ϊ1���򷵻�ԭλ�ã�������Ϊ1
			coordinate[x][y-1]=1;
	}
}

void deleteline(int l)                    //����
{
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY;
	int snum=0,b=0;
	for(int m=0;m<=16;m++)                //����������ȥ����
	{
      if(coordinate[1][m]==1&&coordinate[2][m]==1&&coordinate[3][m]==1&&coordinate[4][m]==1&&coordinate[5][m]==1&&coordinate[6][m]==1&&coordinate[7][m]==1&&coordinate[8][m]==1&&coordinate[9][m]==1&&coordinate[10][m]==1)
	  {
		textout(handle,28,m+4,wColors,1,"����������");
		for(int n=1;n<=m;n++)
		{
		  for(int j=1;j<=10;j++)
		  coordinate[j][m-n+1]=coordinate[j][m-n];
		}
		snum++;
	  }
	}
	for(int n=1;n<=10;n++)
	{
	   for(int d=0;d<=16;d++)
	   {
			int x,y;
			x=n*2+26;
			y=d+4;
			textout(handle,x,y,wColors,1,"  ");	
			if(coordinate[n][d])
			{
				textout(handle,x,y,wColors,1,"��");	
			}
		}
	}
	score=score+(level*snum*(snum+1)/2);
	if((score-scorex)>=30*level)         //ÿ�õ�30���Զ�����
	{
		level++;
		scorex=score;
	}
	char string[5];
	textout(handle,59,9,wColors,1,itoa(score,string,10));
	textout(handle,59,5,wColors,1,itoa(level,string,10));
}


bool ifgameover()                 //��ֹ��Ϸ
{
	if(judge==1)
	{
		return 1;
	}
	else
		return 0;
}

void end()                        //�˳�
{
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_INTENSITY;
	textout(handle,28,22,wColors,1,"Press any key to exit");
	while(1)
	{
	  if (_kbhit())
	  {	
		exit(EXIT_SUCCESS);	
	  }		
	}
}

