#include <stdio.h>
#include <stdlib.h>

char fileName[128] = {0};
char buf[1024] = {0};

void readConf()
{
	printf("输入文件名(exit退出):");
	memset(fileName, 0, sizeof(fileName));
	scanf("%s", fileName);
}

int hex2dec(char *s)
{
	int i = 0;
	int len = strlen(s);
	int dec = 0;
	char c;

	if (NULL == s) return 0;

	while (i < len)
	{
		c = s[i];
		dec *= 16;
		dec += (int)(c - '0');
		i++;
	}

	return dec;
}

int doReplace()
{
	int i, j = 0;
	long long p = 0;
	int ret, count = 0;
	FILE *ofile = NULL;
	char newFileName[128] = {0};

	FILE *fp = fopen(fileName, "r");
	if (NULL == fp) {
		printf("打开%s文件失败.\n", fileName);
		return -1;
	}

	for (i = 0; i < strlen(fileName); i++)
	{
		if (fileName[i] != '.') newFileName[j++] = fileName[i];
		else {
			strcat(newFileName, "001.");
			j += 4;
		}
	}

	newFileName[j] = '\0';

	ofile = fopen(newFileName, "w+");
	if (NULL == ofile) {
		printf("打开%s文件失败.\n", newFileName);
		return -1;
	}

	i = 1;
	/**遍历文件进行处理**/
	while (fscanf(fp, "%lld", &p) != EOF)
	{
		printf("%lld\n", p);
		fprintf(ofile, "%012lld %012lld %03d\n", p-30000, p+30000, i++);
		count++;
	}

	fclose(fp);
	fclose(ofile);

	return count;
}

int main()
{
	int num = 0;

	do {
		readConf();

		if (strcmp(fileName, "exit") == 0) break;

		num = doReplace();

		if (num > 0)
			printf("处理数字个数%d\n", num);

	} while(1);

	return 0;
}
