/* my reg number is 130160429 */


/*1 The input marks are examined in a very simple way. Ideally, all possible inputs should be checked, e.g. char types */
/*2 This programme can be potentially simplied using array */

#include <stdio.h>
#include <string.h>

void main()
{   /* welcome statement*/
    printf("**************************************************************** \n");
	printf("***** welcome using Year_End Overall Result dicision system **** \n");
	printf("**************************************************************** \n");
	
	/* illustrate the aim of this programme*/
	printf("This programme is using marks entered by user to calculate the \nReported Marks, Overall Average, Credits Awarded and Year-End \nOverall Result decision of a student. Maximum and minimum marks \nwill be figured out after the calculation. \n\n");
	
	
	/* declare related variables*/
	int math1, math2, tech1, tech2, course, totalCredit;
	int credit1, credit2;
	float overallAverage;
	int mark[5];
	int credit[5] = {0};
	char strYesNo[3];
	int count = 1;
    int max, min;
	int passORfail;
	char tmp;
	
	do{
	
	/*initialise related variables*/
	math1=0, math2=0, tech1=0, tech2=0, course=0, totalCredit=0;
	credit1=10, credit2=20;
	overallAverage=0;
	passORfail = 1;
	
	/*Part I: input validate marks */
	printf("Do the calculation for the No. %d student\n\n", count);

    /* ask the user to enter module marks within 0-100*/
	while(1==1)
	{
		printf("Please input mark of Math1 module  (0 - 100)\n");
		scanf("%d", &math1);
		if (math1<0 || math1>100)
		{
			printf("This Math1 mark is not valid , re-enter \n");
		}
		else
			break;
	}

	while(1==1)
	{
		printf("Please input mark of Math2 module  (0 - 100)\n");
		scanf("%d", &math2);
		if (math2<0 || math2>100)
		{
			printf("This Math2 mark is not valid , re-enter \n");
		}
		else
			break;
	}

	while(1==1)
	{
		printf("Please input mark of Tech.1 module  (0 - 100)\n");
		scanf("%d", &tech1);
		if (tech1<0 || tech1>100)
		{
			printf("This Tech.1 mark is not valid , re-enter \n");
		}
		else
			break;
	}

	while(1==1)
	{
		printf("Please input mark of Tech.2 module  (0 - 100)\n");
		scanf("%d", &tech2);
		if (tech2<0 || tech2>100)
		{
			printf("This Tech.2 mark is not valid , re-enter \n");
		}
		else
			break;
	}

    while(1==1)
	{
		printf("Please input mark of Course work module  (0 - 100)\n");
		scanf("%d", &course);
		if (course<0 || course>100)
		{
			printf("This Course work mark is not valid , re-enter \n");
		}
		else
			break;
	}
     
	
	//Part II: compute the results 
    overallAverage = (float)(credit1*(math1+math2+tech1+tech2)+credit2*course)/(credit1*4+credit2);
   
	mark[0] = math1; mark[1] = math2; mark[2] = tech1; mark[3] = tech2; mark[4] = course; 
    int i ;
	for ( i = 0; i<4;i++)
		if (mark[i]>40 || mark[i]==40)
		{
			totalCredit = totalCredit+10;
			credit[i] = 10;
		}


    if (mark[4]>40 || mark[4]==40)
	{
		totalCredit = totalCredit + 20;
		credit[4] = 20;
	}

	if (overallAverage<39.5 || totalCredit<40 || mark[4]<40)
    {
		passORfail = 0;
	}
	
    //Part III: display the results 
    printf("\n***Display all the MARKS and the obtained credits for the No. %d student *** \n", count);
	printf("Math 1 = %d       Credit = %d\n", math1, credit[0]);
	printf("Math 2 = %d       Credit = %d\n", math2, credit[1]);
	printf("Tech.1 = %d       Credit = %d\n", tech1, credit[2]);
	printf("Tech.2 = %d       Credit = %d\n", tech2, credit[3]);
	printf("Course work = %d  Credit = %d\n", course, credit[4]);

	printf("\n***Display all the results the No. %d student *** \n", count);
	printf("Overall Average mark = %.1f\n", overallAverage);
    printf("Total credit = %d\n", totalCredit);
	if (passORfail==1)
		printf("This student receives a PASS\n");
	else
        printf("This student receives a FAIL\n");


    printf("Do you want to know the highest mark and lowest mark of this student? (Yes/No)\n");
    scanf("%s", &strYesNo);
	if(strcmp(strYesNo,"Y")==0 || strcmp(strYesNo,"Yes")==0)
	{
		max = mark[0];
		min = mark[0];
		int i;
		for (i = 1; i<5;i++)
		{
			if (mark[i]>max)
				max = mark[i];
			if (mark[i]<min)
				min = mark[i];
		}
     printf("The highest module mark is %d\n", max);
	 printf("The lowest  module mark is %d\n", min);
	}


	printf("Do you want to calculate results for another student (Yes/No)\n");
    scanf("%s", &strYesNo);
    if(strcmp(strYesNo,"Y")==0 || strcmp(strYesNo,"Yes")==0)
		count++;
	}while( strcmp(strYesNo,"Y")==0 || strcmp(strYesNo,"Yes")==0) ;
   

	printf("****************************************** \n");
	printf("***** Thanks for using this programme **** \n");
	printf("****************************************** \n");
	printf("** Press any key to close the programme ** \n");
	scanf("%s",&tmp); 
}
