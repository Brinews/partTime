//2_6.cpp ��ĳ�����������
#include <iostream>
using namespace std;
int main()
{
	int n, right_digit, newnum = 0; //�����Ķ��岿��
	
	cout << "Enter the number: ";
	cin >> n;
	cout << "The number in reverse order is  ";
	//�����������������

	do
	{
		right_digit = n % 10;  
		cout << right_digit;
		n /= 10;  //n=n/10;
	} 
	while (n != 0);  //���n=0ѭ������
	cout<<endl;  //����
	//�����Ǵ���ִ�в���
}
