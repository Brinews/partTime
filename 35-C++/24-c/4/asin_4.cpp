/***
 * Program Name : wcalender
 * Date : 2014-03-09
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define DAYOFWEEK	7
#define LINES		6

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0])

/**
 * Global Vars 
 */
int DayOfMonth[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

char *WeekTitle[DAYOFWEEK] 
	   = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
int Months[LINES][DAYOFWEEK] = {0};

class Calendar
{
private:
	int year, month, day;
public:
	Calendar(int y, int m, int d) : year(y), month(m), day(d) {}
	int wcal_getMonthDays(int y, int m);
	int wcal_getNthYearDay(int y, int m, int d);
	int wcal_getNthCenturyDay(int y, int m, int d);
	int wcal_getNthForeverDay(int y, int m, int d);
	int wcal_getWeekDay(int y, int m, int d);
	void wcal_getCalenderMonth(int y, int m);
	void wcal_printCalender(int y, int m);
	int wcal_getSundaysCount(int y);
};


/***
 * Length of Month
 * @para(year, month)
 * ret: N days
 */
int Calendar::wcal_getMonthDays(int year, int month)
{
	int isLeap = 0;

	if ((year % 4 == 0 && year % 100 != 0) 
		|| year % 400 == 0)
		isLeap = 1;

	return DayOfMonth[isLeap][month-1];
}

/***
 * Day of the Year
 * @para(year, month, day)
 * ret : Nth day of year
 */
int Calendar::wcal_getNthYearDay(int year, int month, int day)
{
	int i, Nth = day;

	for (i = 1; i <= month - 1; i++)
	{
		Nth += wcal_getMonthDays(year, i);
	}

	return Nth;
}

/***
 * Day of the Century
 * @para(year, month, day)
 * ret: Nth day of century
 */
int Calendar::wcal_getNthCenturyDay(int year, int month, int day)
{
	int i, Nth = wcal_getNthYearDay(year, month, day);

	/* year start from 2000 */
	for (i = 2000; i <= year - 1; i++)
		Nth += wcal_getNthYearDay(i, 12, 31);

	return Nth;
}

/***
 * Day of forever
 * @para(year, month, day)
 * ret : Nth day of forever
 */
int Calendar::wcal_getNthForeverDay(int year, int month, int day)
{
	int i, Nth = wcal_getNthYearDay(year, month, day);

	for (i = 0; i <= year - 1; i++)
		Nth += wcal_getNthYearDay(i, 12, 31);

	return Nth;
}

/***
 * Day of Week
 * @para(year, month, day)
 * ret: week number
 */
int Calendar::wcal_getWeekDay(int year, int month, int day)
{
	int days = wcal_getNthForeverDay(year, month, day);

	return (days-1) % DAYOFWEEK;
}

int day_of_the_week (int year, int month, int day)
{
	int day_of_week = 0;

	if (year < 0)
		return 0;
	else
	{
		if (month < 3)
		{
			month += 12;
			year  -= 1;
		}
	}
	day_of_week = (day + 2*month + 3*(month+1)/5 + 
			year + year/4 - year/100 + year/400 + 1) % 7;

	cout << "day_of_week=" << day_of_week << endl;

	day_of_week = (day_of_week + 6) % 7;

	return day_of_week;
}


/***
 * Calender for a month
 * @para(year, month)
 * ret : Months
 */
void Calendar::wcal_getCalenderMonth(int year, int month)
{
	int i, j, start = 1;
	int days = wcal_getMonthDays(year, month);
	//int weekday = wcal_getWeekDay(year, month, 1);

	//if (weekday == 0) weekday = 6;
	//else weekday -= 1;

	int weekday = day_of_the_week(year, month, 1);
	//cout << "weekday=" << weekday << endl;

	for (i = 0; i < 6; i ++)
	{
		for (j = 0; j < 7; j++)
		{
			if (0 == i && j < weekday) 
				Months[i][j] = 0;
			else if (start <= days)
			{
				Months[i][j] = start++;
			}
			else 
				Months[i][j] = 0;
		}
	}
}

/***
 * Print calender
 * @para(year, month)
 */
void Calendar::wcal_printCalender(int year, int month)
{
	int i, j;
	int end = 0;
	wcal_getCalenderMonth(year, month);

	for (i = 0; i < 7; i++)
		printf("%s  ", WeekTitle[i]);
	printf("\n");

	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 7; j++)
		{
			if (i == 0 && Months[i][j] == 0) printf("  ");
			if (Months[i][j] != 0) printf("%2d", Months[i][j]);

			if (Months[i][j] == 0 && i != 0)
			{
				end = 1;
				break;
			}
			
			printf("  ");
		}

		if (end == 1) break;
		printf("\n");
	}

	printf("\n");
}

/***
 * Extra Function: calculate sundays in any given year
 * @para(year)
 * ret : sundays count
 */
int Calendar::wcal_getSundaysCount(int year)
{
	int days = wcal_getNthYearDay(year, 12, 31);
	int suCnt = (int) (days / DAYOFWEEK);
	int left = days % DAYOFWEEK;
	int s = wcal_getWeekDay(year, 1, 1);

	/*
	printf("days: %d, left:%d, s:%d\n", 
			days, left, s);
	 */

	while (left--)
	{
		s = (s+1) % DAYOFWEEK;
		if (s == DAYOFWEEK - 1) suCnt++;
	}

	return suCnt;
}

int main(int argc, char **argv)
{
	int year, month, day;


	cout << "Input (Month, Day, Year)  ([-1, -1, -1] for end):\n";
	while (cin >> month >> day >> year) 
	{
		if (year == -1) break;

		Calendar cal(year, month, day);

		cout << "Century Day Number: " 
			 << cal.wcal_getNthCenturyDay(year, month, day)
			 << endl;
		cout << "Forever Day Number: "
			 << cal.wcal_getNthForeverDay(year, month, day)
			 << endl;

		cout << "Sunday Count in Year: "
			 << cal.wcal_getSundaysCount(year)
			 << endl;

		cout << endl;

		cal.wcal_printCalender(year, month);
	}

	return 0;
}
