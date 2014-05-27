//2_6.cpp 将某个数逆序输出
#include <iostream>
using namespace std;
int main()
{
	int n, right_digit, newnum = 0; //变量的定义部分
	
	cout << "Enter the number: ";
	cin >> n;
	cout << "The number in reverse order is  ";
	//上面是输入输出部分

	do
	{
		right_digit = n % 10;  
		cout << right_digit;
		n /= 10;  //n=n/10;
	} 
	while (n != 0);  //如果n=0循环结束
	cout<<endl;  //换行
	//上面是代码执行部分
}
