#include<fstream>
#include<iostream>
#include<iomanip>
#include<string>
#include<cstdlib>

using namespace std;

#define M 16
#define N 14
#define K 3

class Student                                           //类的定义，包括学生信息
{  
   char num[N];
   char name[M];
   int score[K];
   double gpa[K];
   double agpa;
public:
	Student(){}
	char *getnum()                                      //获取学号
	{ 
		return num;
	}
	char *getname()                                     //获取姓名
	{
		return name;
	}
	void setnum(char nu[])                              //设置学号
	{
		strcpy(num,nu);
	}

	void setname(char na[])                             //设置姓名
	{
        strcpy(name,na);
	}

	void setscore(int sc[])                            //设置学生成绩
	{
		for(int i=0;i<K;i++)
			score[i]=sc[i];
	}

	void setgpa(double g[])                           //设置学生成绩相应绩点
	{
		int i;
		for(i=0;i<K;i++)
			gpa[i]=g[i];
	}

	void setagpa(double ag)                            //设置学生的平均绩点
	{
		agpa=ag;
	}

	void computegpa()                                 //求学生成绩相应绩点
	{
		double x[K];
		for(int i=0;i<K;i++)
		{
			//cin>>score[i];
			x[i]=(score[i]%10)*(0.1);
			gpa[i]=(score[i]-50)/10+x[i];
		}
	}

	void computeagpa()                                //求学生的平均绩点
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

	void getscore(int sc[])                            //取出学生成绩
	{
		int i;
		for(i=0;i<K;i++)
			sc[i]=score[i];
	}

	void getgpa(double g[])                            //取出学生绩点
	{
		for(int i=0;i<K;i++)
			g[i]=gpa[i];
	}

	double getagpa()                          //取得学生的平均绩点
	{
		return agpa;
	}

	void disp(int header = 0)                                         //输出学生成绩
	{
		int i;
		if (header != 0)
		{
			cout<<setw(N)<<"学号"<<setw(M)<<"姓名"<<"\t高数"<<"\t物理"<<"\t程序设计"<<"\t绩点1"<<"\t绩点2"<<"\t绩点3"<<"\t平均绩点"<<endl;
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
	int nElem;                                           //学生信息表中的元素个数
	int Maxu;                                            //最多的学生
	Student *user;                                       //指向学生信息的指针
public:
	UserDatabase(int n=30);                             //构造函数，初始化学生信息表，将user.txt读到User[]中
	~UserDatabase();                                     //析构函数，将user[]写入user.txt文件中
	void clear();                                       //删除所有学生信息
	void add_record(char *nu,char *na,int sc[]);        //添加学生信息，插入学生后仍按升序排列
	void delete_record(char *k);              //删除学生信息
	void modify_record(char *na);            //修改学生信息
	Student * query(char *nu);                   //按学号查找 顺序查找，这里也可以用折半查找
	void sorta_num();                                   //按学号升序排序 选择排序法
	void sorta_name();                                  //按姓名升序排序 插入排序法
	void sorta_agpa();                                  //按平均绩点升序排序 冒泡排序法
	void sort_num();                                    //按学号降序排序 选择排序法
	void sort_name();                                   //按姓名降序排序，将排成升序的对象数组逆向放置
	void sort_agpa();                                   //按平均绩点降序排序 冒泡排序法
	void disp();                                        //输出所有学生信息
};

UserDatabase::UserDatabase(int n)                       //构造函数，初始化学生信息表，将user.txt读到User[]中
{
	int i,sc[K];
	double g[K],ag;
	char num[N],na[M];

	ifstream infile;                                    //定义文件对象
	infile.open("user.txt",ios::in);                    //打开该数据文件

	if(!infile)                                         //打开文件发生错误，程序退出
	{
		cout<<"打开文件错误!\n";
		exit(0);
	}

	nElem=0;
	Maxu=n;
	if(n)
		user=new Student[n];
	else
		user=NULL;

	//读取文件头
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

		if(nElem==Maxu)                                  //如果实际空间不够
		{
			Student *newptr;
			newptr=new Student[Maxu+10];                 //申请一新表空间
			for(i=0;i<nElem;i++)
				newptr[i]=user[i];                       //将原表中的元素拷贝到新表中
			delete []user;	                             //释放原表空间
			Maxu+=10;

			user=newptr;
		}

		user[nElem].setnum(num);                         //增加元素
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
		cout<<"打开文件错误!"<<endl;
		exit(1);
	}

	outfile<<setw(N)<<"学号"<<setw(M)<<"姓名"<<"\t高数"<<"\t物理"<<"\t程序设计"<<"\t绩点1"<<"\t绩点2"<<"\t绩点3"<<"\t平均绩点"<<endl;

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
	cout<<"您确认要全部删除么?(y-是，n-否):";
	cin>>a;
	if(a=='y')
	{
		delete []user;

		user=NULL;
		nElem=0;
		Maxu=0;
	}
}

void UserDatabase::add_record (char *nu,char *na,int sc[])  //添加用户
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


void UserDatabase::delete_record(char *na)      //删除学生
{
	if(nElem==0)
		cout<<"没有成绩!"<<endl;
	else
	{
		Student *p;char a;
		p=query(na);
		if(p==NULL)
			cout<<"该学生不存在!"<<endl;
		else
		{
			cout<<"学号:"<<p->getnum()<<endl;
			cout<<"您确定要删除么?(y-是,n-否):";
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

void UserDatabase::modify_record(char *na)     //修改学生信息
{
	Student *p;
	char c;
	int score[K];
	p=query(na);
	if(p==0)
		cout<<"该学生不存在!"<<endl;
	else
	{
		p->getscore(score);
		cout<<"原分数(高数 物理 程序设计):"
			<<score[0] <<"\t" 
			<< score[1] <<"\t" << score[2]<<endl;

		cout<<"请输入新分数(高数 物理 程序设计):";
		cin >> score[0] >> score[1] >> score[2];

		cout<<"您确认要修改么?(y-是，n-否):";
		cin>>c;
		if(c=='y')
		{
			p->setscore(score);
			p->computegpa();
			p->computeagpa();
		}
	}
}

Student* UserDatabase::query(char *nu)                  //查找
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

void UserDatabase::sorta_num()                         //按学号升序排序 选择排序法
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

void UserDatabase::sorta_name()                        //按姓名升序排序 插入排序法
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

void UserDatabase::sorta_agpa()                          //平均绩点升序 冒泡
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

void UserDatabase::sort_num()                            //学号降序选择
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

void UserDatabase::sort_name()                          //姓名降序
{
	sorta_name();

	Student *newuser;

	newuser=new Student[nElem];
	for(int i=0;i<nElem;i++)
		newuser[i]=user[nElem-i-1];

	delete []user;

	user=newuser;
}

void UserDatabase::sort_agpa()                          //平均绩点降序 选择
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

void UserDatabase::disp()                               //显示
{
	cout<<setw(N)<<"学号"<<setw(M)<<"姓名"<<"\t高数"<<"\t物理"<<"\t程序设计"<<"\t绩点1"<<"\t绩点2"<<"\t绩点3"<<"\t平均绩点"<<endl;

	for (int i = 0; i < nElem; i++)
	{
		user[i].disp();
	}
}

int menu_select()
{  
	cout<<endl;

	cout<<'\t'<<"请选择以下功能"<<endl<<endl;
	cout<<'\t'<<"1 - 增加记录"<<endl;
	cout<<'\t'<<"2 - 修改记录"<<endl;
	cout<<'\t'<<"3 - 删除记录"<<endl;        
	cout<<'\t'<<"4 - 查找"<<endl;        
	cout<<'\t'<<"5 - 排序"<<endl;        
	cout<<'\t'<<"6 - 显示记录"<<endl;       
	cout<<'\t'<<"7 - 全删"<<endl;        
	cout<<'\t'<<"0 - 退出"<<endl;
	cout<<endl;
	char *m[8]={" 退出"," 增加记录"," 修改记录"," 删除记录"," 查找"," 排序"," 显示记录"," 全删"};
	char c;

	do 
	{
		cout<<'\t'<<"输入选择:"<<'\t';
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
	cout<<setw(45)<<"欢迎使用学生成绩管理系统"<<endl;
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
			case 1:                                                 //增加记录

			system("cls");
			cout<<"\n******************************************************\n";
			cout<<"\n                    增加记录                          \n";
			cout<<"\n******************************************************\n";
			cout<<endl;
			cout<<"\n    请输入学生学号:";
			cin>>usernum;
			u=apply.query(usernum);

			if(u!=NULL)
			{
				cout<<"\n   增加失败"<<endl;
				system("pause");
			}
			else
			{ 
				cout<<"\n   请输入学生姓名:";
				cin>>username;
				cout<<"\n   请输入学生成绩:";
				for(int i=0;i<K;i++)
					cin>>sc[i];

				apply.add_record(usernum, username, sc);
			}
			break;

			/* update */
			case 2:

			cout<<"\n******************************************************\n";
			cout<<"\n                    修改记录                          \n";
			cout<<"\n******************************************************\n";
			cout<<endl;
			{
				char i='y';
				for(;;)
				{
					if(i=='y')
					{
						cout<<"请输入用户学号:";
						cin.get();
						cin.getline(na,M);        
						apply.modify_record(na); 
						cout<<"继续修改吗?(y-是，n-否):";
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
			cout<<"\n                    删除记录                          \n";
			cout<<"\n******************************************************\n";
			cout<<endl;
			{
				char i='y';
				for(;;)
				{
					if(i=='y')
					{
						cout<<"请输入学生学号:";
						cin.get();
						cin.getline(na,M);
						apply.delete_record(na);
						cout<<"继续删除么?(y-是,n-否):";
						cin>>i;
					}
					else break;
				}
			}
			system("cls");
			break;

			/* query */
			case 4:

			cout<<"请输入学生学号:";
			cin.get();
			cin.getline(na,M);

			t=apply.query(na);

			if(t!=NULL)
				t->disp(1);
			else
				cout<<"该学生不存在!"<<endl;

			system("pause");
			system("cls");
			break;

			/* sort */
			case 5:
			cout<<"请选择以下排序方式:"<<endl;
			cout<<endl;
			cout<<"1-按学号(升序)"<<endl;
			cout<<"2-按姓名(升序)"<<endl;
			cout<<"3-按平均绩点(升序)"<<endl;
			cout<<"4-按学号(降序)"<<endl;
			cout<<"5-按姓名(降序)"<<endl;
			cout<<"6-按平均绩点(降序)"<<endl;
			cout<<"0-返回"<<endl;

			char d;
			do
			{
				cout<<"输入选择:";
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
			case 6://每屏显示10

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
			cout<<setw(45)<<"退出系统!"<<endl;
			apply.~UserDatabase();
			exit(0);
		}
	}
}
