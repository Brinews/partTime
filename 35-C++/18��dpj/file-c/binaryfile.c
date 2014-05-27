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
	printf("�����ļ���:");
	memset(fileName, 0, sizeof(fileName));
	scanf("%s", fileName);

	printf(" �����(width), ��(height):");
	scanf("%d%d", &width, &height);

	printf("������ʼ��x1, y1:");
	scanf("%d%d", &startX, &startY);

	printf("�����ֹ��x2, y2:");
	scanf("%d%d", &endX, &endY);
}

/***
 *����λ��ת��
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
		printf("���ļ�ʧ��.\n");
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

	/**�����ļ����в����滻**/
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
		printf("������Ҫ�滻���ַ�����01(-1�˳���:");
		scanf("%s", srcChar);
		srcChar[2] = '\0';
		if (strcmp(srcChar, "-1") == 0) break;

		printf("���������滻���ַ�����67:");
		scanf("%s", destChar);
		destChar[2] = '\0';

		num = doReplace();

		if (num > 0)
			printf("�滻����%d\n", num);

	} while(1);

	return 0;
}
