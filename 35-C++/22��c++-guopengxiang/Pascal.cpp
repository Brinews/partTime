 #include<iostream>
 #include<fstream>
 
 using namespace std;
 
 void Output(int isize,int jsize){
 	ofstream fout("D:\Pascal.xls");
 	for(int i=0;i<isize;++i)
     {
    	for(int j=0;j<jsize;++j)
        {
             fout<<i<<"\t";
        }
	 	fout<<endl;
	 }
	 fout.close();
 }
 
 int main()
 {
    ofstream fout("D:\Pascal.xls");
    int i,j,n;  
    long int k;  
    long int fact(int);  
    cout<<"Please input a integer number n:";  
    cin>>n;  
    cout<<"Pascal:"<<endl;  
    for(i=0;i<=n;i++)  
    {
       for(j=0;j<=i;j++)  
       {  
           k=fact(i)/fact(j)/fact(i-j);  //����Է���ÿ��Ԫ�ض��� ��ϣ�������ϵ��Ǹ���ϣ��� 
           fout<<k<<"\t";  
       }  
       fout<<endl;  
       if(i!=n) 
           fout<<endl;  
    } 
    fout.close();
    cout<<"Finished!"<<endl;  
    system("pause"); 
    return 0;
 }  
  
 long int fact(int c) //��׳ˣ� 
 {  
    long int fa;  
    int i;  
    fa=1;  
    for(i=1;i<=c;i++)  
    {  
        fa*=i;  
    }  
    return fa;  
 } 
