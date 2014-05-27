//2_2.cpp
#include <iostream>
using namespace std;
int main()
{
	int year;
	bool IsLeapYear;
	
	cout << "Enter the year(输入年份): ";
	cin >> year;
	IsLeapYear = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
	//"="这是赋值号  “==”这是等于号
	
	if (IsLeapYear)
	    cout << year << " is a leap year（是闰年）" << endl;
	else
	    cout << year << " is not a leap year（不是闰年）" << endl;
}

