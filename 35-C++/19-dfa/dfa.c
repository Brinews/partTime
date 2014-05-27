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

	/*初始化*/
	state = 0;

	while (idx < len)
	{
		now = buf[idx];

		if (state == 0 && now == '1')
		{
			printf("Into Trap State, END陷井状态.\n");
			return 0;
		}

		if (now != '0' && now != '1')
		{
			printf("非01串,");
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
		printf ("Valid String, Accept接受.\n");
	}
	else 
	{
		printf ("InValid String, Reject错误.\n");
	}

	return 0;
}

int main(int argc, char **argv)
{
	printf("请输入01串, 每条一行(q退出): \n");

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
