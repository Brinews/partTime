#include <windows.h>
#include <conio.h>

void main()
{
	HANDLE handle = initiate();

	WORD wColors[1];
	wColors[0] =FOREGROUND_RED | FOREGROUND_GREEN |FOREGROUND_INTENSITY;

	bool bSwitch = true;
	for (int i=4;i<76;)
	{
		textout(handle,i,5,wColors,1,"¡ö");
		textout(handle,i-2,6,wColors,1,"¡ö¡ö¡ö");
		Sleep(300);
		
		if (_kbhit())
		{
			_getch();
			bSwitch = !bSwitch;
			continue;
		}
		textout(handle,i,5,wColors,1,"  ");
		textout(handle,i-2,6,wColors,1,"      ");
		
		if (bSwitch) i+=2;
		
	}
}


