#include <stdio.h>
#include <stdlib.h>

#define MAXLEN	1024

int state = 0;
char buf[MAXLEN] = {0};

int dfa(char *buf)
{
	//printf("GOT:<%s>\n", buf);

	int len = strlen(buf);
	int idx = 0;

	char now, next;

	/*��ʼ��*/
	state = 0;

	while (idx < len)
	{
		now = buf[idx];

		if (state == 0 && now == '1')
		{
			printf("Into Trap State, END�ݾ�״̬.\n");
			return 0;
		}

		if (now != '0' && now != '1')
		{
			printf("��01��,");
			state = 3;
		}

		if (state == 0 && now == '0')
		{
			state = 1;
		}

		if (state == 1 && now == '0')
		{
			state = 1;
		}

		if (state == 1 && now == '1')
		{
			state = 2;
		}

		if (state == 2 && now == '1')
		{
			state = 2;
		}

		if (state == 2 && now == '0')
		{
			state = 1;
		}

		if (state != 2 && state != 1)
			break;

		//printf("[%c, %d, %d]\n", now, idx, state);
		idx++;
	}

	if (state == 2) 
	{
		printf ("Valid String, Accept����.\n");
	}
	else 
	{
		printf ("InValid String, Reject����.\n");
	}

	return 0;
}

int main(int argc, char **argv)
{
	printf("������01��, ÿ��һ��(q�˳�): \n");

	while (fgets(buf, MAXLEN, stdin))
	{
		buf[strlen(buf) - 1] = '\0';
		if (strncmp(buf, "q", 1) == 0) break;

		if (strlen(buf) != 0)
			dfa(buf);
		memset(buf, 0, sizeof(buf));
	}

	return 0;
}
