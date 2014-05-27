/*-------------------------------

温度控制器V1.5
显示为三个共阳极LED
温度传感器用单总线DS18B20
CPU为2051，四个按键，分别为UP，DOWN，SET
温度调节上限为125度，下限为-55度
只能用于单只18B20



-------------------------------*/

#include <AT89X051.H>
#include <intrins.h>
#define Key_UP            P3_0    //上调温度
#define Key_DOWN          P3_1    //下调温度
#define Key_SET           P1_7    //设定键（温度设定，长按开电源）
#define RelayOutPort      P3_5    //继电器输出
#define LEDPort           P1      //LED控制口
#define LEDOneC           P3_2    //LED DS1控制（百位）
#define LEDTwoC           P3_3    //LED DS2控制（十位）
#define LEDThreeC         P3_4    //LED DS3控制（个位）
#define TMPort						P3_7		//DS1820 DataPort

unsigned char code LEDDis[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF}; //0-9的LED笔划,0xFF为空,0xF7为负号

static unsigned char bdata StateREG;	//可位寻址的状态寄存器
sbit DS1820ON = StateREG^0;	          //DS1820是否存在
sbit SetTF = StateREG^1;              //是否是在温度设置状态
sbit KeySETDown = StateREG^2;         //是否已按过SET键标识
sbit PowTF = StateREG^3;              //电源电源标识
sbit KeyTF = StateREG^4;							//键盘是否允许

//sbit KeySETDowning = StateREG^5;			//SET是否正在按下
static unsigned char bdata TLV _at_ 0x0029;         //温度变量高低位
static unsigned char bdata THV _at_ 0x0028;
static signed char TMV;       //转换后的温度值
static unsigned char KeyV,TempKeyV;      //键值
static signed char TMRomV _at_ 0x0027;    //高温限制
static signed char TMSetV _at_ 0x0026;    //温度设定值
static unsigned char KSDNum;    //SET键连按时的采集次数
static unsigned char IntNum,IntNum2,IntNum3;    //中断发生次数，IntNum用于SET长按检测，IntNum2用于设定状态时LED闪烁
static signed char LED_One,LED_Two,LED_Three;  //LED的显示位 LED_One为十位，LED_Two为个位
static unsigned char Sign; //负号标识

void main(void)
{
	void InitDS1820(void);   //定义函数
	void ROMDS1820(void);
	void TMVDS1820(void);
	void TMRDS1820(void);
	void TMWDS1820(void);
	void TMREDS1820(void);
	void TMERDS1820(void);
	void ReadDS1820(void);
	void WriteDS1820(void);
	void Delay_510(void);
	void Delay_110(void);
	void Delay_10ms(void);
	void Delay_4s(void);
	void V2ToV(void);

	StateREG = 0;   //初始化变量
	SetTF = 1;	
	PowTF = 1;			//关电源
	THV = 0;
	TLV = 0;
	TMV = 0;
	KeyV = 0;
	TempKeyV = 0;
	KSDNum = 0;
	IntNum = 0;
	IntNum2 = 0;
	IntNum3 = 0;
	LED_One = 0;
	LED_Two	= 0;

	InitDS1820(); //初始化
	ROMDS1820(); 	//跳过ROM
	TMERDS1820();  //E2PRAM中温度上限值调入RAM
	InitDS1820(); //初始化
	ROMDS1820(); 	//跳过ROM
	TMRDS1820(); 	//读出温度指令
	ReadDS1820(); //读出温度值和上限值
	TMSetV = TMRomV;  //拷贝保存在DS18B20ROM里的上限值到TMSetV

	EA = 1;					//允许CPU中断
	ET0 = 1; 				//定时器0中断打开
	TMOD = 0x1;			//设定时器0为模式1，16位模式
	TH0=0xB1;
	TL0=0xDF;				//设定时值为20000us（20ms）
	TR0 = 1;				//开始定时
	while(1);
}

//定时器0中断外理中键扫描和显示
void KeyAndDis_Time0(void) interrupt 1 using 2
{
  TH0=0xB1;
	TL0=0xDF;				//设定时值为20000us（20ms)

	LEDPort = 0xFF;
  if (!Key_UP)
    KeyV = 1;
  if (!Key_DOWN)
    KeyV = 2;
  if (!Key_SET)
    KeyV = 3;
	//KeySETDowning = 0;  //清除
  if (KeyV != 0)      //有键按下
    {
      Delay_10ms();   //延时防抖  按下10ms再测
      if (!Key_UP)
        TempKeyV = 1;
      if (!Key_DOWN)
        TempKeyV = 2;
      if (!Key_SET)
        TempKeyV = 3;
      if (KeyV == TempKeyV)   //两次值相等为确定接下了键
        {
          if (KeyV == 3)      //按下SET键，如在SET状态就退出，否则进入
            {
							//KeySETDowning = 1; //表明SET正在按下
              PowTF = 0;      //电源标识开
							if (!KeyTF)
              	if (SetTF)
									{
                		SetTF = 0;    //标识位标识退出设定
										InitDS1820(); //初始化
  									ROMDS1820(); 	//跳过ROM
  									TMWDS1820(); 	//写温度上限指令
										WriteDS1820(); 	//写温度上限到DS18B20ROM
										WriteDS1820(); 	//写温度上限到DS18B20ROM
										WriteDS1820(); 	//写温度上限到DS18B20ROM
										InitDS1820(); //初始化
  									ROMDS1820(); 	//跳过ROM
  									TMREDS1820(); 	//温度上限值COPY回E2PRAM
									}
              	else
                	SetTF = 1;
              if (!KeySETDown) //没有第一次按下SET时，KeySETDown标识置1
                KeySETDown = 1;
              else
                KSDNum = KSDNum + 1;  //前一秒内有按过SET则开始计数
            }
          if (SetTF)  //在SET状态下
            {
              if ((KeyV == 1) && (!KeyTF))
                TMSetV = TMSetV + 1;    //上调温度
              if ((KeyV == 2) && (!KeyTF))
                TMSetV = TMSetV - 1;    //下调温度
              if (TMSetV <= -55)          //限制温度上下限
                TMSetV = -55;
              if (TMSetV >= 125)
                TMSetV = 125;
            }
		if ((!KeyTF) && (IntNum3 == 0)) KeyTF = 1; //当键盘处于可用时，锁定
        }

      if (KeySETDown)         //在2秒内按下了SET则计中断发生次数用于长按SET时计时用
        IntNum = IntNum + 1;  
      if (IntNum > 55)     //中断发生了55次时（大约1.2秒）75为1.5秒左右
        {
          IntNum = 0;
					KeySETDown = 0;
          if (KSDNum == 55)      //如一直长按了SET1.2秒左右
            {
              RelayOutPort = 1;  //关闭继电器输出
              PowTF = 1;         //电源标识关
              LEDOneC = 0;
              LEDTwoC = 0;
              LEDThreeC = 0;
              LEDPort = 0xBF;      //显示"--"
              Delay_4s();        //延时
              LEDOneC = 1;
              LEDTwoC = 1;      //关显示
              LEDThreeC = 1;
              Delay_4s();
						  IntNum = 0;
						  IntNum2 = 0;
						  IntNum3 = 0;
            }
          KSDNum = 0;
         }
    }
  KeyV = 0;
  TempKeyV = 0;   //清空变量准备下次键扫描

  if (!PowTF)
    {
      InitDS1820();	//初始化
      ROMDS1820(); 	//跳过ROM
      TMVDS1820();  //温度转换指令

      Delay_510();
      Delay_510();  //延时等待转换完成

      InitDS1820(); //初始化
      ROMDS1820(); 	//跳过ROM
      TMRDS1820(); 	//读出温度指令
      ReadDS1820(); //读出温度值

      V2ToV();        //转换显示值
      if (TMV > TMSetV)   //根据采集到的温度值控制继电器
        {
          RelayOutPort = 0;
		    }
      else
		    {
          RelayOutPort = 1;
  		  }

      if (SetTF) IntNum2 = IntNum2 + 1; //用于闪烁计数
      if (IntNum2 > 50 ) IntNum2 = 0;
			if (KeyTF) IntNum3 = IntNum3 + 1; //用于防止按键连按
			if (IntNum3 > 25) 
				{
					IntNum3 = 0;
					KeyTF = 0;
				}

      if ((SetTF) && (IntNum2 < 25)) goto InitEnd;  //计数在后半段时显示
      LEDPort = LED_One;
      LEDOneC = 0;
      Delay_510();
      LEDOneC = 1;    //显示百位数
      LEDPort = LED_Two;
      LEDTwoC = 0;
      Delay_510();
      LEDTwoC = 1;    //显示十位数
      LEDPort = LED_Three;
      LEDThreeC = 0;
      Delay_510();
      LEDThreeC = 1;    //显示个位数
    }
InitEnd:;
}

void V2ToV(void)	//数值转换
{
  TLV = TLV >> 4;			
  THV = THV << 4;			//读出的高低位数值移位
  TMV = TLV | THV;		//合并高低位放入TM为实际温度值
	Sign = 0;
  if (SetTF || !Key_SET)
    Sign = TMSetV >> 7;          //取符号
  else
     Sign = TMV >> 7;

	if (Sign)
		{
  		if (SetTF || !Key_SET)
				{
    			LED_One = (~(TMSetV-1)) / 100;          //SET状态下显示设定值
    			LED_Two = ((~(TMSetV-1)) - LED_One * 100)/10;
			    LED_Three = (~(TMSetV-1)) - LED_One * 100 - LED_Two * 10;
				}
  		else
				{
    			LED_One = (~TMV) / 100;							//转换百位值		
    			LED_Two = ((~TMV) - LED_One * 100)/10;
			    LED_Three = (~TMV) - LED_One * 100 - LED_Two * 10;
				}
		}
	else
		{ 
  		if (SetTF || !Key_SET)
				{
    			LED_One = (TMSetV) / 100;          //SET状态下显示设定值
    			LED_Two = (TMSetV - LED_One * 100)/10;
			    LED_Three = TMSetV - LED_One * 100 - LED_Two * 10;
				}
  		else
				{
    			LED_One = (TMV) / 100;							//转换百位值		
    			LED_Two = (TMV - LED_One * 100)/10;
			    LED_Three = TMV - LED_One * 100 - LED_Two * 10;
				}
		}

 //转LED字段	
	if (LED_One) //超过百时十位的处理
		LED_Two = LEDDis[LED_Two];		
	else
		{
			if (LED_Two == 0)
				LED_Two = LEDDis[10];
			else
				LED_Two = LEDDis[LED_Two];
		}
	if (Sign)
		LED_One = LEDDis[11];
	else
		{
			if (LED_One == 0)
				LED_One = LEDDis[10];
			else
				LED_One = LEDDis[LED_One];
		}
	LED_Three = LEDDis[LED_Three];
}

void InitDS1820(void)		//初始化DS1820
{
	TMPort = 1;		//拉高TMPort
	_nop_();      //保持一个周期
	TMPort = 0;		//拉低TMPort
  Delay_510();  //延时  DS1820复位时间要500us的低电平
  TMPort = 1;   //拉高TMPort
	_nop_();      //保持  
	_nop_();
	_nop_();

  Delay_110();  //延时110us 等待DS1820回应
  if (!TMPort)  //回应信号为低电平
    DS1820ON = 1;
  else
    DS1820ON = 0;
  Delay_110();  //延时
  Delay_110();
  TMPort = 1;   //拉高TMPort
}

void ROMDS1820(void)  //跳过ROM匹配
{
#pragma asm
  MOV A,#0CCH
  MOV R2,#8
  CLR C
  WR1:
  CLR P3_7
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_7,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_7
  NOP
  DJNZ R2,WR1
  SETB P3_7
#pragma endasm
}

void TMVDS1820(void)  //温度转换指令
{
#pragma asm
  MOV A,#44H
  MOV R2,#8
  CLR C
  WR2:
  CLR P3_7
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_7,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_7
  NOP
  DJNZ R2,WR2
  SETB P3_7
#pragma endasm
}

void TMRDS1820(void)  //读出温度指令
{
#pragma asm
  MOV A,#0BEH
  MOV R2,#8
  CLR C
  WR3:
  CLR P3_7
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_7,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_7
  NOP
  DJNZ R2,WR3
  SETB P3_7
#pragma endasm
}

void TMWDS1820(void)  //写入温度限制指令
{
#pragma asm
  MOV A,#04EH
  MOV R2,#8
  CLR C
  WR13:
  CLR P3_7
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_7,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_7
  NOP
  DJNZ R2,WR13
  SETB P3_7
#pragma endasm
}

void TMREDS1820(void)		//COPY RAM to E2PRAM
{
#pragma asm
  MOV A,#48H
  MOV R2,#8
  CLR C
  WR33:
  CLR P3_7
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_7,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_7
  NOP
  DJNZ R2,WR33
  SETB P3_7
#pragma endasm
}

void TMERDS1820(void)		//COPY E2PRAM to RAM
{
#pragma asm
  MOV A,#0B8H
  MOV R2,#8
  CLR C
  WR43:
  CLR P3_7
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_7,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_7
  NOP
  DJNZ R2,WR43
  SETB P3_7
#pragma endasm
}

void WriteDS1820(void)	//写入温度限制值
{
#pragma asm
	MOV A,26H					//发出4EH写ROM指令后连发两个字节分别为上下限
  MOV R2,#8
  CLR C
  WR23:
  CLR P3_7
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_7,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_7
  NOP
  DJNZ R2,WR23
  SETB P3_7       
#pragma endasm
}

void ReadDS1820(void) //读出温度值
{
#pragma asm
  MOV R4,#3 ; 将温度高位和低位，高温限制位从DS18B20中读出
  MOV R1,#29H ; 低位存入29H(TEMPER_L),高位存入28H(TEMPER_H)，高温限制位存入27H(TMRomV)
  RE00:
  MOV R2,#8
  RE01:
  CLR C
  SETB P3_7
  NOP
  NOP
  CLR P3_7
  NOP
  NOP
  NOP
  SETB P3_7
  MOV R3,#09
  RE10: 
  DJNZ R3,RE10
  MOV C,P3_7
  MOV R3,#23
  RE20: 
  DJNZ R3,RE20
  RRC A
  DJNZ R2,RE01
  MOV @R1,A
  DEC R1
  DJNZ R4,RE00
#pragma endasm
}

void Delay_510(void)	//延时510微秒
{
#pragma asm
	MOV R0,#7DH
	MOV R1,#02H
	TSR1:
	DJNZ R0,TSR1
	MOV R0,#7DH
	DJNZ R1,TSR1
#pragma endasm
}

void Delay_110(void)	//延时110微秒
{
#pragma asm
	MOV R0,#19H
	MOV R1,#02H
	TSR2:
	DJNZ R0,TSR2
	MOV R0,#19H
	DJNZ R1,TSR2
#pragma endasm
}

void Delay_10ms(void)	//延时10ms
{
#pragma asm
	MOV R0,#19H
	MOV R1,#0C8H
	TSR3:
	DJNZ R0,TSR3
	MOV R0,#19H
	DJNZ R1,TSR3
#pragma endasm
}

void Delay_4s(void)	//延时4s
{
#pragma asm
  MOV R2,#28H
  TSR5:
	MOV R0,#0FAH
	MOV R1,#0C8H
	TSR4:
	DJNZ R0,TSR4
	MOV R0,#0FAH
	DJNZ R1,TSR4
  DJNZ R2,TSR5
#pragma endasm
}
