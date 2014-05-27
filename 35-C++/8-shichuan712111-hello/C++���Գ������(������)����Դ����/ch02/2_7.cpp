//2_7.cpp
#include<iostream>
using namespace std;
int main()
{
	int i(1), sum(0);
	do  {
		sum+=i; //sum=sum|+i;
		i++;  //i=i+1;
	} while(i<=10);
		cout<<"sum="<<sum<<endl;
}
