//2_5.cpp
#include<iostream>
using namespace std;
int main()
{
	int i(1), sum(0);  //��������i����ʼ��Ϊ1����������sum����ʼ��Ϊ0
	while(i<=10)
	{
		sum+=i;// sum=sum+i;
		i++; //����ѭ������
	}
	cout<<"sum="<<sum<<endl;
}
