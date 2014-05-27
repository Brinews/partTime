#include <stdio.h>
#include <stdlib.h>

char destChar[3];
char srcChar[3];

int width;
int height;

int startX, startY;
int endX, endY;

char fileName[128] = {0};
char buf[1024] = {0};

void readConf()
{
	printf("输入文件名:");
	memset(fileName, 0, sizeof(fileName));
	scanf("%s", fileName);

	printf(" 输入宽(width), 高(height):");
	scanf("%d%d", &width, &height);

	printf("输入起始点x1, y1:");
	scanf("%d%d", &startX, &startY);

	printf("输入截止点x2, y2:");
	scanf("%d%d", &endX, &endY);
}

/***
 *进行位置转换
 */
int postion(int x, int y, int fileSize)
{
	int pos = y * width + x;

	if (pos < 0) return 0;
	if (pos >= fileSize) pos = fileSize - 1;

	return pos;
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
	char p;
	int fileSize, start, end;
	int i;
	int ret, count = 0;
	char hex[3] = {0};
	char r = hex2dec(destChar);
	printf("r = %d\n", r);

	FILE *fp = fopen(fileName, "rb+");
	if (NULL == fp) {
		printf("打开文件失败.\n");
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	fileSize = ftell(fp);
	printf("file size = %d\n", fileSize);

	start = postion(startX, startY, fileSize);
	end = postion(endX, endY, fileSize);
	printf("start=%d, end=%d\n", start, end);

	rewind(fp);
	fseek(fp, (long)start, SEEK_SET);

	/**遍历文件进行查找替换**/
	i = start;
	while (i <= end)
	{
		if (1 != fread(&p, 1, 1, fp))
		{
			fclose(fp);
			return -1;
		}

		sprintf(hex, "%02x", p);
		hex[2] = '\0';
		//printf("hex=%s\n", hex);

		if (0 == strcmp(hex, srcChar)) {
			printf("Found %s at %d, Begin to Replace.\n", hex, i);

			fseek(fp, i, SEEK_SET);
			fwrite(&r, 1, 1, fp);
			fseek(fp, i+1, SEEK_SET);

			count++;
		}
		i++;
	}

	fclose(fp);

	return count;
}

int main()
{
	int num = 0;
	readConf();

	do {
		printf("输入需要替换的字符，如01(-1退出）:");
		scanf("%s", srcChar);
		srcChar[2] = '\0';
		if (strcmp(srcChar, "-1") == 0) break;

		printf("输入用来替换的字符，如67:");
		scanf("%s", destChar);
		destChar[2] = '\0';

		num = doReplace();

		if (num > 0)
			printf("替换个数%d\n", num);

	} while(1);

	return 0;
}
