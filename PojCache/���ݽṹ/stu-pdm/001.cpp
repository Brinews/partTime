#include<fstream>
#include<iostream>
#include<iomanip>
#include<string>
#include<cstdlib>

using namespace std;

#define M 16
#define N 14
#define K 3

class Student                                           //��Ķ��壬����ѧ����Ϣ
{  
   char num[N];
   char name[M];
   int score[K];
   double gpa[K];
   double agpa;
public:
	Student(){}
	char *getnum()                                      //��ȡѧ��
	{ 
		return num;
	}
	char *getname()                                     //��ȡ����
	{
		return name;
	}
	void setnum(char nu[])                              //����ѧ��
	{
		strcpy(num,nu);
	}

	void setname(char na[])                             //��������
	{
        strcpy(name,na);
	}

	void setscore(int sc[])                            //����ѧ���ɼ�
	{
		for(int i=0;i<K;i++)
			score[i]=sc[i];
	}

	void setgpa(double g[])                           //����ѧ���ɼ���Ӧ����
	{
		int i;
		for(i=0;i<K;i++)
			gpa[i]=g[i];
	}

	void setagpa(double ag)                            //����ѧ����ƽ������
	{
		agpa=ag;
	}

	void computegpa()                                 //��ѧ���ɼ���Ӧ����
	{
		double x[K];
		for(int i=0;i<K;i++)
		{
			//cin>>score[i];
			x[i]=(score[i]%10)*(0.1);
			gpa[i]=(score[i]-50)/10+x[i];
		}
	}

	void computeagpa()                                //��ѧ����ƽ������
	{
		double s=0;
		for(int i=0;i<K;i++)
		{
			s+=gpa[i];
		}
		agpa=s/K;

		int mid = (int)(agpa*10);

		agpa = mid*1.0/10;
	}

	void getscore(int sc[])                            //ȡ��ѧ���ɼ�
	{
		int i;
		for(i=0;i<K;i++)
			sc[i]=score[i];
	}

	void getgpa(double g[])                            //ȡ��ѧ������
	{
		for(int i=0;i<K;i++)
			g[i]=gpa[i];
	}

	double getagpa()                          //ȡ��ѧ����ƽ������
	{
		return agpa;
	}

	void disp(int header = 0)                                         //���ѧ���ɼ�
	{
		int i;
		if (header != 0)
		{
			cout<<setw(N)<<"ѧ��"<<setw(M)<<"����"<<"\t����"<<"\t����"<<"\t�������"<<"\t����1"<<"\t����2"<<"\t����3"<<"\tƽ������"<<endl;
		}
		cout << setw(N) << num;
		cout << setw(M) << name;
		for (i = 0; i < K; i++)
			cout << setw(8) << score[i];

		cout <<"\t    ";
		for (i = 0; i < K; i++)
			cout << setw(8) << gpa[i];
		cout << setw(8) << agpa << endl;
	}
};

class UserDatabase
{ 
	int nElem;                                           //ѧ����Ϣ���е�Ԫ�ظ���
	int Maxu;                                            //����ѧ��
	Student *user;                                       //ָ��ѧ����Ϣ��ָ��
public:
	UserDatabase(int n=30);                             //���캯������ʼ��ѧ����Ϣ����user.txt����User[]��
	~UserDatabase();                                     //������������user[]д��user.txt�ļ���
	void clear();                                       //ɾ������ѧ����Ϣ
	void add_record(char *nu,char *na,int sc[]);        //���ѧ����Ϣ������ѧ�����԰���������
	void delete_record(char *k);              //ɾ��ѧ����Ϣ
	void modify_record(char *na);            //�޸�ѧ����Ϣ
	Student * query(char *nu);                   //��ѧ�Ų��� ˳����ң�����Ҳ�������۰����
	void sorta_num();                                   //��ѧ���������� ѡ������
	void sorta_name();                                  //�������������� ��������
	void sorta_agpa();                                  //��ƽ�������������� ð������
	void sort_num();                                    //��ѧ�Ž������� ѡ������
	void sort_name();                                   //�������������򣬽��ų�����Ķ��������������
	void sort_agpa();                                   //��ƽ�����㽵������ ð������
	void disp();                                        //�������ѧ����Ϣ
};

UserDatabase::UserDatabase(int n)                       //���캯������ʼ��ѧ����Ϣ����user.txt����User[]��
{
	int i,sc[K];
	double g[K],ag;
	char num[N],na[M];

	ifstream infile;                                    //�����ļ�����
	infile.open("user.txt",ios::in);                    //�򿪸������ļ�

	if(!infile)                                         //���ļ��������󣬳����˳�
	{
		cout<<"���ļ�����!\n";
		exit(0);
	}

	nElem=0;
	Maxu=n;
	if(n)
		user=new Student[n];
	else
		user=NULL;

	//��ȡ�ļ�ͷ
	char buf[1024];
	infile.getline(buf, 1024);

	while(infile>>num)
	{
		infile>>na;
		for(i=0;i<K;i++)
			infile>>sc[i];

		for(i=0;i<K;i++)
			infile>>g[i];
		infile>>ag;

		if(nElem==Maxu)                                  //���ʵ�ʿռ䲻��
		{
			Student *newptr;
			newptr=new Student[Maxu+10];                 //����һ�±�ռ�
			for(i=0;i<nElem;i++)
				newptr[i]=user[i];                       //��ԭ���е�Ԫ�ؿ������±���
			delete []user;	                             //�ͷ�ԭ��ռ�
			Maxu+=10;

			user=newptr;
		}

		user[nElem].setnum(num);                         //����Ԫ��
        user[nElem].setname(na);
		user[nElem].setscore(sc);
		user[nElem].setgpa(g);
		user[nElem].setagpa(ag);
		nElem++;
	}

	infile.close();
}

UserDatabase::~UserDatabase ()
{
	int m;
	ofstream outfile;
	outfile.open("user.txt",ios::in);

	if(!outfile)
	{
		cout<<"���ļ�����!"<<endl;
		exit(1);
	}

	outfile<<setw(N)<<"ѧ��"<<setw(M)<<"����"<<"\t����"<<"\t����"<<"\t�������"<<"\t����1"<<"\t����2"<<"\t����3"<<"\tƽ������"<<endl;

	sorta_name();

	for(int i=0;i<nElem;i++)
	{
		outfile<<setw(N)<<user[i].getnum()<<'\t'<<setw(M)<<user[i].getname();

		int score[K];
		user[i].getscore(score);
		for (m = 0; m < K; m++)
		{
			outfile << setw(8) << score[m];
		}

		double gpa[K];
		user[i].getgpa(gpa);
		outfile << "\t";
		for (m = 0; m < K; m++)
		{
			outfile << setw(8) << gpa[m];
		}

		outfile << setw(14) << user[i].getagpa();
		outfile << endl;
	}

	outfile.close();
	delete [] user;
}

void UserDatabase::clear()                              
{
	char a;
	cout<<"��ȷ��Ҫȫ��ɾ��ô?(y-�ǣ�n-��):";
	cin>>a;
	if(a=='y')
	{
		delete []user;

		user=NULL;
		nElem=0;
		Maxu=0;
	}
}

void UserDatabase::add_record (char *nu,char *na,int sc[])  //����û�
{
	if(nElem==Maxu)
	{
		Student *newuser;
			newuser=new Student[Maxu+10];
		for(int i=0;i<nElem;i++)
			newuser[i]=user[i];

		delete []user;
		Maxu+=10;
		user=newuser;
	}

	user[nElem].setnum(nu);
	user[nElem].setname(na);
	user[nElem].setscore(sc);

	user[nElem].computegpa();
	user[nElem].computeagpa();

	nElem++;
}


void UserDatabase::delete_record(char *na)      //ɾ��ѧ��
{
	if(nElem==0)
		cout<<"û�гɼ�!"<<endl;
	else
	{
		Student *p;char a;
		p=query(na);
		if(p==NULL)
			cout<<"��ѧ��������!"<<endl;
		else
		{
			cout<<"ѧ��:"<<p->getnum()<<endl;
			cout<<"��ȷ��Ҫɾ��ô?(y-��,n-��):";
			cin>>a;
			if(a=='y')
			{
				for(;p<user+nElem;p++)
					*p=*(p+1);
				nElem--;
			}
		}
	}
}

void UserDatabase::modify_record(char *na)     //�޸�ѧ����Ϣ
{
	Student *p;
	char c;
	int score[K];
	p=query(na);
	if(p==0)
		cout<<"��ѧ��������!"<<endl;
	else
	{
		p->getscore(score);
		cout<<"ԭ����(���� ���� �������):"
			<<score[0] <<"\t" 
			<< score[1] <<"\t" << score[2]<<endl;

		cout<<"�������·���(���� ���� �������):";
		cin >> score[0] >> score[1] >> score[2];

		cout<<"��ȷ��Ҫ�޸�ô?(y-�ǣ�n-��):";
		cin>>c;
		if(c=='y')
		{
			p->setscore(score);
			p->computegpa();
			p->computeagpa();
		}
	}
}

Student* UserDatabase::query(char *nu)                  //����
{
	if(nElem==0)
		return NULL;
	else
	{
		for(int i=0;i<nElem;i++)
		{
			if(strcmp(user[i].getnum(), nu)==0)
				return user+i;
		}

		return NULL;
	}
}

void UserDatabase::sorta_num()                         //��ѧ���������� ѡ������
{
	int i,j,k;
	Student p;
	for(i=0;i<nElem-1;i++)
	{
		k=i;
		for(j=i+1;j<nElem;j++)
		{
			if(strcmp(user[j].getnum(),user[k].getnum()) < 0)
				k=j;
		}

		if(k!=i)
		{
			p = user[k];
			user[k]=user[i];
			user[i]=p;
		}
	}
}

void UserDatabase::sorta_name()                        //�������������� ��������
{
	int i,j;
	Student p;
	for(i=1;i<nElem;i++)
	{
		p=user[i];

		for(j=i-1;j>=0&&(strcmp(p.getname(),user[j].getname())<0);j--)
			user[j+1]=user[j];

		user[j+1]=p;
	}
}

void UserDatabase::sorta_agpa()                          //ƽ���������� ð��
{
	int i,j,k;
	Student p;
	for(i=nElem-1;i>=1;i--)
		for(j=0;j<i;j++)
			if(user[j].getagpa()>user[j+1].getagpa())
			{
				p=user[j];
				user[j]=user[j+1];
				user[j+1]=p;
			}
}

void UserDatabase::sort_num()                            //ѧ�Ž���ѡ��
{
	int i,j, k;
	Student p;

	for(i=nElem-1;i>=0;i--)
	{
		k = i;
		for(j=i-1;j>=0;j--)
			if(strcmp(user[k].getnum (),user[j].getnum()) > 0)
				k = j;

		if(i != k)
		{
			p = user[k];
			user[k] = user[i];
			user[i] = p;
		}
	}
}

void UserDatabase::sort_name()                          //��������
{
	sorta_name();

	Student *newuser;

	newuser=new Student[nElem];
	for(int i=0;i<nElem;i++)
		newuser[i]=user[nElem-i-1];

	delete []user;

	user=newuser;
}

void UserDatabase::sort_agpa()                          //ƽ�����㽵�� ѡ��
{
	int i,j,k;
	Student p;
	for(i=0;i<nElem-1;i++)
	{
		k=i;
		for(j=i+1;j<nElem;j++)
			if(user[k].getagpa()<user[j].getagpa())
				k=j;

		if(k!=i)
		{
			p=user[k];
			user[k]=user[i];
			user[i]=p;
		}
	}
}

void UserDatabase::disp()                               //��ʾ
{
	cout<<setw(N)<<"ѧ��"<<setw(M)<<"����"<<"\t����"<<"\t����"<<"\t�������"<<"\t����1"<<"\t����2"<<"\t����3"<<"\tƽ������"<<endl;

	for (int i = 0; i < nElem; i++)
	{
		user[i].disp();
	}
}

int menu_select()
{  
	cout<<endl;

	cout<<'\t'<<"��ѡ�����¹���"<<endl<<endl;
	cout<<'\t'<<"1 - ���Ӽ�¼"<<endl;
	cout<<'\t'<<"2 - �޸ļ�¼"<<endl;
	cout<<'\t'<<"3 - ɾ����¼"<<endl;        
	cout<<'\t'<<"4 - ����"<<endl;        
	cout<<'\t'<<"5 - ����"<<endl;        
	cout<<'\t'<<"6 - ��ʾ��¼"<<endl;       
	cout<<'\t'<<"7 - ȫɾ"<<endl;        
	cout<<'\t'<<"0 - �˳�"<<endl;
	cout<<endl;
	char *m[8]={" �˳�"," ���Ӽ�¼"," �޸ļ�¼"," ɾ����¼"," ����"," ����"," ��ʾ��¼"," ȫɾ"};
	char c;

	do 
	{
		cout<<'\t'<<"����ѡ��:"<<'\t';
		cin>>c;
	} while(c<'0'||c>'7');

	system("cls");
	cout<<"------------------------------------------------------------------------------"<<endl;
	cout<<endl;        
	cout<<setw(42)<<m[c-'0']<<endl;
	cout<<"------------------------------------------------------------------------------"<<endl;
	cout<<endl;

	return (c-'0');
}

/**** functions *****/
int main()
{
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<"------------------------------------------------------"<<endl;
    cout<<"------------------------------------------------------"<<endl;
	cout<<endl;
	cout<<setw(45)<<"��ӭʹ��ѧ���ɼ�����ϵͳ"<<endl;
	cout<<endl;
	cout<<"------------------------------------------------------"<<endl;
	cout<<"------------------------------------------------------"<<endl;

	UserDatabase apply;

	Student *u = NULL, *t = NULL;
	char usernum[M], username[N];
	char na[M];
	int sc[K];

	while (1) 
	{
		int OPT = menu_select();
		
		switch(OPT)
		{
			case 1:                                                 //���Ӽ�¼

			system("cls");
			cout<<"\n******************************************************\n";
			cout<<"\n                    ���Ӽ�¼                          \n";
			cout<<"\n******************************************************\n";
			cout<<endl;
			cout<<"\n    ������ѧ��ѧ��:";
			cin>>usernum;
			u=apply.query(usernum);

			if(u!=NULL)
			{
				cout<<"\n   ����ʧ��"<<endl;
				system("pause");
			}
			else
			{ 
				cout<<"\n   ������ѧ������:";
				cin>>username;
				cout<<"\n   ������ѧ���ɼ�:";
				for(int i=0;i<K;i++)
					cin>>sc[i];

				apply.add_record(usernum, username, sc);
			}
			break;

			/* update */
			case 2:

			cout<<"\n******************************************************\n";
			cout<<"\n                    �޸ļ�¼                          \n";
			cout<<"\n******************************************************\n";
			cout<<endl;
			{
				char i='y';
				for(;;)
				{
					if(i=='y')
					{
						cout<<"�������û�ѧ��:";
						cin.get();
						cin.getline(na,M);        
						apply.modify_record(na); 
						cout<<"�����޸���?(y-�ǣ�n-��):";
						cin>>i;
					}
					else break;
				}
			}
			system("cls");       
			break;
	
			/* delete */
			case 3:

			cout<<"\n******************************************************\n";
			cout<<"\n                    ɾ����¼                          \n";
			cout<<"\n******************************************************\n";
			cout<<endl;
			{
				char i='y';
				for(;;)
				{
					if(i=='y')
					{
						cout<<"������ѧ��ѧ��:";
						cin.get();
						cin.getline(na,M);
						apply.delete_record(na);
						cout<<"����ɾ��ô?(y-��,n-��):";
						cin>>i;
					}
					else break;
				}
			}
			system("cls");
			break;

			/* query */
			case 4:

			cout<<"������ѧ��ѧ��:";
			cin.get();
			cin.getline(na,M);

			t=apply.query(na);

			if(t!=NULL)
				t->disp(1);
			else
				cout<<"��ѧ��������!"<<endl;

			system("pause");
			system("cls");
			break;

			/* sort */
			case 5:
			cout<<"��ѡ����������ʽ:"<<endl;
			cout<<endl;
			cout<<"1-��ѧ��(����)"<<endl;
			cout<<"2-������(����)"<<endl;
			cout<<"3-��ƽ������(����)"<<endl;
			cout<<"4-��ѧ��(����)"<<endl;
			cout<<"5-������(����)"<<endl;
			cout<<"6-��ƽ������(����)"<<endl;
			cout<<"0-����"<<endl;

			char d;
			do
			{
				cout<<"����ѡ��:";
				cin>>d;
			} while (d<'0'|| d>'7');

			switch(d-'0')
			{
				case 1:
					apply.sorta_num(); 
					system("cls");         
					apply.disp();         
					system("pause");system("cls");  
					break;

				case 2:
					apply.sorta_name();        
					system("cls");
					apply.disp(); 
					system("pause");system("cls");
					break;

				case 3:
					apply.sorta_agpa();         
					system("cls");          
					apply.disp();           
					system("pause");
					system("cls");
					break;

				case 4:
					apply.sort_num(); 
					system("cls"); 
					apply.disp();
					system("pause");
					system("cls");  
					break; 

				case 5:
					apply.sort_name(); 
					system("cls"); 
					apply.disp();
					system("pause");
					system("cls");  
					break; 

				case 6:
					apply.sort_agpa(); 
					system("cls"); 
					apply.disp();
					system("pause");
					system("cls");  
					break; 

				case 0:
					system("cls"); 
					break;
			}
			break;

			/* display */
			case 6://ÿ����ʾ10

			apply.disp();
			system("pause");
			system("cls");
			break;

			/* remove all */
			case 7:

			apply.clear();
			system("pause");
			system("cls");
			break;

			/* quit */
			case 0:
			cout<<setw(45)<<"�˳�ϵͳ!"<<endl;
			apply.~UserDatabase();
			exit(0);
		}
	}
}
