/***********************************************************	    
名称:16*64点阵单红翻页时钟 
单片机: STC89C52 
晶振：22.1184M
显示屏: 16*64点阵,标准的08接口,OE高电平有效   
作者；振甬电子
QQ：1766135164
淘宝套件网址：zy-mcu.taobao.com  
视频演示：http://v.youku.com/v_show/id_XNDkwOTgyNzQ4.html     
***********************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <REGX52.H>
#include "DS1302.h"
#include "DS18B20.h"
#include "font.h"
#include "lunar.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
#define ORANGE   0
#define GREEN    1
#define RED      2

///////////////////////////////////////////////////////////////////////////////////////////////////
#define H_PORT P0       //ABCD P0
sbit RData 	= 	P0^4; 
sbit OE=P0^5;	
sbit L_STB 	= 	P0^6;
sbit L_CLK	= 	P0^7;	
sbit GData  =   P3^4;

sbit key_adj=P3^0;								   
sbit key_add=P3^1;
sbit key_dec=P3^6;
sbit key_esc=P3^7;

#include "port.h"

#define FESTIVAL_MAX 9			//定义农历节日数
#define KEY_VAL 5               //定义按键手感

#define set_row(row) {H_PORT=(row&0x0f);}

///////////////////////////////////////////////////////////////////////////////////////////////////

SYSTEMTIME CurrentTime;

unsigned char status=0;			//按键状态
unsigned char disp_mode=0;		//显示状态  
unsigned char offset=0;	   		//移动显示用的变量
unsigned char disrow=0;			//当前扫描行
unsigned char k=0;				//往buff中填的字
unsigned int  t_counter=0,stop_counter=0,stop_time=0;
unsigned char f1=18,f2=18,f3=18,f4=18,f5=18,f6=18;  //18就不会出现刚开始全部翻页的情况了.
bit switch_flag=0,stop_bit=0;
bit one_sec_flag=0,half_sec_flag;
bit nongli_flag=0,nongli_change_flag=0,festival_flag=0;
unsigned char temp_time[6];     //翻页钟用到

unsigned char color = 0;

bit add_flag=0;
bit dec_flag=0;
bit adj_flag=0;
bit esc_flag=0;

unsigned char ka=0,kb=0,kc=0,kd=0;  //按键用

///////////////////////////////////////////////////////////////////////////////////////////////////


unsigned char date[]=
{
	10,10,10,10,10,10,10,10, 		//开头8个空格
	2,0, 							//21世纪
	1,2, 							//年数据	No.10,11
	49,50,							//“年”
	0,1,							//月数据 	No.14,15
	51,52,							//“月”
	0,1,							//日数据	No.18,19
	53,54,							//“日”
	
	10,         					//空格
	
	55,56,57,58,					//“星期”
	53,54,							//星期几	No.27,28
	
	10,								//空格
	
	0,0,  							//小时数据 	No.30,31
	11,    						    // :  		No.32
	0,0,  							//分钟数据 	No.33,34
	11,    							// :		No.35
	0,0,   							//秒数据 	No.36,37
	
	10,								//空格
	
	10,10,10,12,10,13,14,		    //温度 		No.39,40,41,43
					
    10,10,10,10,10,10,10,10,        //MAX       No.53									  
};

///////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char code mode[4]={0,1,7,8}; //几种显示模式的排列，依次为
                                     //0:流动显示年月日+固定一段时间翻页钟
                                     //1:翻页钟
                                     //7:全部流动显示
                                     //8:温度

unsigned char code moon_date[48]=
{
    10,10,10,10,10,10,10,10, 		//开头8个空格
    10,10,                          //No.8,9
    10,10,                          //No.10,11
    49,50,                          //“年”
    10,10,                          //No.14,15
    10,10,                          //No.16,17
    51,52,                          //“月”
    10,10,                          //No.20,21
    10,10,                          //No.22,23
    10,10,10,10,10,10,10,10, 		//开头8个空格
    10,10,10,10,10,10,10,10, 		//开头8个空格
    10,10,10,10,10,10,10,10,       //MAX No.47
};


unsigned char code init_date[]=
{
	10,10,10,10,10,10,10,10, 		//开头8个空格
	2,0, 							//21世纪
	1,2, 							//年数据	No.10,11
	49,50,							//“年”
	0,1,							//月数据 	No.14,15
	51,52,							//“月”
	0,1,							//日数据	No.18,19
	53,54,							//“日”
	
	10,         					//空格
	
	55,56,57,58,					//“星期”
	53,54,							//星期几	No.27,28
	
	10,								//空格
	
	0,0,  							//小时数据 	No.30,31
	11,    						    // : 		No.32
	0,0,  							//分钟数据 	No.33,34
	11,    							// :		No.35
	0,0,   							//秒数据 	No.36,37
	
	10,								//空格
	
	10,10,10,12,10,13,14,			    //温度 		No.39,40,41,43
					
    10,10,10,10,10,10,10,10,       //MAX       No.53	 
};

unsigned char code week_data[]={0,25,27,29,31,33,35,53};  //一二三四五六日
unsigned char code moon_data1[]={45,43,47,29};	//初十廿卅

///////////////////////////////////////////////////////////////////////////////////////////////////

void delay_nms(unsigned int count);
void Timer_init(void);
void time_to_buff_all(void);
void update_buff(void);
void key_scan(void);
void display(unsigned char mode);
void shutter_clock(void);


///////////////////////////////////////////////////////////////////////////////////////////////////

void main(void)
{
	Initial_DS1302();
	time_to_buff_all();
    ReadTemperature(&date,39);	
	Timer_init();
 
	while(1)
	{
        update_buff();
        
	 	for(disrow=0;disrow<16;disrow++)		  //disrow,当前扫描的行
		{		
//////////////////////////////////////////		
			display(disp_mode);
			OE=0;		     //关显示屏
			L_STB=1;		
			L_STB=0;
			set_row(disrow);//行138的输出.依次点亮1~16行.	 			
		   	OE=1;			 //开显示屏
			delay_nms(1);   //此处延时,可以省略，		
//////////////////////////////////////////////////////////
		}	
		if(stop_counter>stop_time) 
		{
			disp_mode=0;
			offset++;
			stop_counter=0;
			stop_bit=0;
		}
        if(stop_bit==0) offset++;
		if(disp_mode==1)
		{
			if(f1<200) f1++;
			if(f2<200) f2++;
			if(f3<200) f3++;
			if(f4<200) f4++;
			if(f5<200) f5++;
			if(f6<200) f6++;
		}
		if(nongli_flag==0) key_scan();  //显示农历时调整会出现错误，图省力屏蔽了
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void Timer_init(void)				
{
	EA=0;
    ET0=1;      //开定时器中断    
    TMOD=0x01;	//定时器0模式1
    // TH0=0x3c;      	//24M晶振 25ms中断
    // TL0=0xb0;
	TH0=0x4c;   //22.1148M晶振 25ms中断
    TL0=0x07;
    TR0=1;		//开定时器运行
    EA=1;	
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void T0_ISR(void) interrupt 1 using 3	//定时器中断
{	
   // TH0=0x3c;      	//24M晶振 25ms中断
   // TL0=0xb0;
	TH0=0x4c;   //22.1148M晶振 25ms中断
    TL0=0x07;
	t_counter++;
    if(t_counter==20) half_sec_flag=1;
    if(t_counter==40) {t_counter=0;one_sec_flag=1;}
	if(stop_bit==1) stop_counter++;
    
    if(ka!=0)
	{    
        ka--;
        if(ka==0 && key_add==0) add_flag=1;
    }	 
	else if(key_add==0)ka=KEY_VAL;
    
    if(kb!=0)
	{    
        kb--;
        if(kb==0 && key_dec==0) dec_flag=1;
    }	 
	else if(key_dec==0)kb=KEY_VAL;
    
    if(kc!=0)
	{    
        kc--;
        if(kc==0 && key_adj==0) adj_flag=1;
    }	 
	else if(key_adj==0)kc=KEY_VAL;
    
    if(kd!=0)
	{    
        kd--;
        if(kd==0 && key_esc==0) esc_flag=1;
    }	 
	else if(key_esc==0)kd=KEY_VAL;   
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void update_buff(void)
{
    unsigned char i;
    if(nongli_change_flag==1)
    {
        if(nongli_flag==1)
        {
            for(i=0;i<46;i++)
                date[i]=moon_date[i];
                
            DS1302_GetTime_ALL(&CurrentTime);
			festival_flag = GetSolar(CurrentTime.Year,CurrentTime.Month,CurrentTime.Day,&date,26);
            DS1302_GetTime_ALL(&CurrentTime);
			Color(&color,CurrentTime.Hour);
            getLunar(&CurrentTime);
 
            date[8]= (CurrentTime.Year+100-84)%10*2+61;   //从"甲"开始
            date[9]= (CurrentTime.Year+100-84)%10*2+62; 
            date[10]=(CurrentTime.Year+100-84)%12*2+81;   //从"子"开始
            date[11]=(CurrentTime.Year+100-84)%12*2+82;  
			
            if(CurrentTime.Month<11)    //不显示为“零一月”
            {   
				if(CurrentTime.Month==1) 
				{
					date[14]=59;		//"正"
					date[15]=60;
				}
				else if(CurrentTime.Month==10)
                {
                    date[14]=43;		//"十"
					date[15]=44;
                }
                else
				{
					date[14]=CurrentTime.Month%10*2+23;     //月个位
					date[15]=CurrentTime.Month%10*2+24;     // 
				}
                date[16]=51;
                date[17]=52;                            //"月"
             
                switch(CurrentTime.Day)
                {
                    case 10:
                    date[18]=45;                        //"初"
                    date[19]=46;
                    date[20]=43;                        //"十"
                    date[21]=44;
                    break;
                    
                    case 20:
                    date[18]=27;                        //"廿"
                    date[19]=28;
                    date[20]=43;                        //"十"
                    date[21]=44;
                    break;
                    
                    case 30:
                    date[18]=29;                        //"卅"
                    date[19]=30;
                    date[20]=43;                        //"十"
                    date[21]=44;
                    break;                    
                    
                    default:                                  
                    date[18]=moon_data1[CurrentTime.Day/10];     //日十位
                    date[19]=moon_data1[CurrentTime.Day/10]+1;   //
                    date[20]=CurrentTime.Day%10*2+23;     //日个位
                    date[21]=CurrentTime.Day%10*2+24;     // 
                    break;
                }
                date[22]=10;
                date[23]=10;
            }
            else
            {
                date[14]=43;                          //"十"
                date[15]=44;                          //           
                date[16]=CurrentTime.Month%10*2+23;   //月个位
                date[17]=CurrentTime.Month%10*2+24;   //  
                switch(CurrentTime.Day)
                {
                    case 10:
                    date[20]=45;                        //"初"
                    date[21]=46;
                    date[22]=43;                        //"十"
                    date[23]=44;
                    break;
                    
                    case 20:
                    date[20]=27;                        //"廿"
                    date[21]=28;
                    date[22]=43;                        //"十"
                    date[23]=44;
                    break;
                    
                    case 30:
                    date[20]=29;                        //"卅"
                    date[21]=30;
                    date[22]=43;                        //"十"
                    date[23]=44;
                    break;                    
                    
                    default:                                  
                    date[20]=moon_data1[CurrentTime.Day/10];     //日十位
                    date[21]=moon_data1[CurrentTime.Day/10]+1;   //
                    date[22]=CurrentTime.Day%10*2+23;     //日个位
                    date[23]=CurrentTime.Day%10*2+24;     // 
                    break;
                }
            }
            nongli_change_flag=0; 
        }
        else
        {
            for(i=0;i<48;i++)
                date[i]=init_date[i];   
                
            time_to_buff_all();
            nongli_change_flag=0;   
        }
    }   
    
	if(half_sec_flag==1&&nongli_flag==0)     //0.5S
	{   
        half_sec_flag=0;

        time_to_buff_all();
		switch(status)
		{
			case 0:
			break;
			case 1:
			date[10]=15,date[11]=15;
			break;
			case 2:
			date[14]=15,date[15]=15;
			break;
			case 3:
			date[18]=15,date[19]=15;
			break;
			case 4:
			date[27]=15,date[28]=15;
			break;
			case 5:
			date[30]=15,date[31]=15;
			break;
			case 6:
			date[33]=15,date[34]=15;
			break;
			case 7:
			date[36]=15,date[37]=15;
			break;		
            default:
            break;
		}	
	} 		
	if(one_sec_flag==1&&nongli_flag==0)      //1.0S
	{   
		one_sec_flag=0;
        
        if((disp_mode==8||k==0)&&nongli_flag==0) ReadTemperature(&date,39);

		date[32]=11;
        date[35]=11;
		time_to_buff_all();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void display(unsigned char mode)		//送显示数据
{
	unsigned char i;
	switch(mode)
	{
		case 0:	            //流动暂停显示
		if(switch_flag==1)	{k=0;switch_flag=0;}
		for(i=0;i<8;i++)	  	//送往8个4094的数据.
			SendByte(color,a[date[k+i]][disrow]);	
		    Send2Byte(color,a[date[k+8]][disrow],offset);
		if(offset==8) 
		{
			offset=0;k++;
			if(k==30&&nongli_flag==0) {stop_bit=1,stop_time=900,disp_mode=1;}	//到了时间的时候停一下。
			if(k==39&&nongli_flag==0) {stop_bit=1,stop_time=100;}	//温度也停一下下。
			if(k>45&&nongli_flag==0) 
            {
                k=0;
                nongli_flag=~nongli_flag;
                nongli_change_flag=1;
            }
            if(k>24&&festival_flag==0&&nongli_flag==1)
            {
                k=0;
                nongli_flag=~nongli_flag;
                nongli_change_flag=1;
            }
			else if(k>30&&festival_flag==1&&nongli_flag==1)
			{
                k=0;
                nongli_flag=~nongli_flag;
                nongli_change_flag=1;
            }
		}
		break;
		      
        case 7:             //全部流动显示
        if(switch_flag==1)	{k=0;switch_flag=0;}
		for(i=0;i<8;i++)	  	//送往8个4094的数据.
			SendByte(color,a[date[k+i]][disrow]);	
		    Send2Byte(color,a[date[k+8]][disrow],offset);
		if(offset==8) 
		{
			offset=0;k++;
			if(nongli_flag==0&&k>45) 
            {
                k=0;
                nongli_flag=~nongli_flag;
                nongli_change_flag=1;
            }
            if(k>24&&festival_flag==0&&nongli_flag==1)
            {
                k=0;
                nongli_flag=~nongli_flag;
                nongli_change_flag=1;
            }
			else if(k>30&&festival_flag==1&&nongli_flag==1)
			{
                k=0;
                nongli_flag=~nongli_flag;
                nongli_change_flag=1;
            }
		}
        break;
      
        case 8:         //固定显示温度
        for(i=0;i<8;i++)	  	//送往8个4094的数据.
			SendByte(color,a[date[39+i]][disrow]);	
        break;	
        
		case 1:			//翻页钟显示
		shutter_clock();
		break;
		
		case 2:			   		//显示时间
		if(switch_flag==1)	{/*k=23;*/switch_flag=0;}
		if(k<30)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[k+i]][disrow]);	
			    Send2Byte(color,a[date[k+8]][disrow],offset);
			if(offset==8) 
			{
				offset=0;k++;
			}
		}
		if(k==30)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[30+i]][disrow]);	
		}
		break;
			
		case 3:			   		//显示年		
		if(switch_flag==1)	{k=0;switch_flag=0;}
		if(k<10)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[k+i]][disrow]);	
			    Send2Byte(color,a[date[k+8]][disrow],offset);
			if(offset==8) 
			{
				offset=0;k++;
			}
		}
		if(k==10)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[10+i]][disrow]);	
		}
		break;
		
		case 4:			   		//显示月
		if(switch_flag==1)	{k=10;switch_flag=0;}
		if(k<14)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[k+i]][disrow]);	
			    Send2Byte(color,a[date[k+8]][disrow],offset);
			if(offset==8) 
			{
				offset=0;k++;
			}
		}
		if(k==14)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[14+i]][disrow]);	
		}
		break;
		
		case 5:			   		//显示日
		if(switch_flag==1)	{k=14;switch_flag=0;}		
		if(k<18)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[k+i]][disrow]);	
			    Send2Byte(color,a[date[k+8]][disrow],offset);
			if(offset==8) 
			{
				offset=0;k++;
			}
		}
		if(k==18)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[18+i]][disrow]);	
		}
		break;

		case 6:			   		//显示星期
		if(switch_flag==1)	{k=18;switch_flag=0;}
		if(k<23)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[k+i]][disrow]);	
			    Send2Byte(color,a[date[k+8]][disrow],offset);
			if(offset==8) 
			{
				offset=0;k++;
			}
		}
		if(k==23)
		{
			for(i=0;i<8;i++)	  	//送往8个4094的数据.
				SendByte(color,a[date[23+i]][disrow]);	
		}
		break;

		default:
		break;		
	}		
}

/////////////////////////////////////////////////////////////////////////////
void shutter_clock(void)				//翻页钟
{
		if(date[30]!=temp_time[0]) f1=0;								//时十位
		if(f1<17)
		{
			if(date[30]==0) SendByte(color,a[17][disrow+f1]);
			else SendByte(color,a[date[30]-1][disrow+f1]);
		}
		else SendByte(color,a[date[30]][disrow]);		
		
		if(date[31]!=temp_time[1]) f2=0;								//时个位
		if(f2<17)
		{
			if(date[31]==0) SendByte(color,a[19][disrow+f2]);
			else SendByte(color,a[date[31]-1][disrow+f2]);
		}
		else SendByte(color,a[date[31]][disrow]);
        
		SendByte(color,a[date[32]][disrow]);							    //冒号
		
		if(date[33]!=temp_time[2]) f3=0;								//分十位
		if(f3<17)
		{
			if(date[33]==0) SendByte(color,a[21][disrow+f3]);
			else SendByte(color,a[date[33]-1][disrow+f3]);
		}
		else SendByte(color,a[date[33]][disrow]);		
		
		if(date[34]!=temp_time[3]) f4=0;								//分个位
		if(f4<17)
		{
			if(date[34]==0) SendByte(color,a[23][disrow+f4]);
			else SendByte(color,a[date[34]-1][disrow+f4]);
		}
		else SendByte(color,a[date[34]][disrow]);
		
		SendByte(color,a[date[35]][disrow]);							    //冒号
		
		if(date[36]!=temp_time[4]) f5=0;								//秒十位
		if(f5<17)
		{
			if(date[36]==0) SendByte(color,a[21][disrow+f5]);
			else SendByte(color,a[date[36]-1][disrow+f5]);
		}
		else SendByte(color,a[date[36]][disrow]);
		
		if(date[37]!=temp_time[5]) f6=0;								//秒个位
		if(f6<17)
		{
			if(date[37]==0) SendByte(color,a[23][disrow+f6]);
			else SendByte(color,a[date[37]-1][disrow+f6]);
		}
		else SendByte(color,a[date[37]][disrow]);		
}	
///////////////////////////////////////////////////////////////////////////////////////////////////
void delay_nms(unsigned int count)		//软件延时
{
	unsigned int i,j;
	for(i=0;i<count;i++)
		for(j=0;j<45;j++);	  			//120 改成45看看
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void time_to_buff_all(void)				//读取全部时间
{
	DS1302_GetTime_ALL(&CurrentTime);
    
   
    temp_time[0]=date[30];
	temp_time[1]=date[31];
	temp_time[2]=date[33];
	temp_time[3]=date[34];
	temp_time[4]=date[36];
	temp_time[5]=date[37];

    Color(&color,CurrentTime.Hour);
    
   	date[30]=CurrentTime.Hour/10;		//小时
   	date[31]=CurrentTime.Hour%10;	  	 //小时
   	date[33]=CurrentTime.Minute/10;	 	//分钟
   	date[34]=CurrentTime.Minute%10;	  	//分钟
   	date[36]=CurrentTime.Second/10;	  	//秒
   	date[37]=CurrentTime.Second%10;	  	//秒 	
	
   	date[10]=CurrentTime.Year/10;	        //年
   	date[11]=CurrentTime.Year%10;	        //年
   	date[14]=CurrentTime.Month/10;	        //月
   	date[15]=CurrentTime.Month%10;	        //月
   	date[18]=CurrentTime.Day/10;	  	    //日
   	date[19]=CurrentTime.Day%10;	  	    //日
    date[27]=week_data[CurrentTime.Week];  //星期
    date[28]=week_data[CurrentTime.Week]+1;//星期
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void key_scan(void)						//扫描按键
{
    static unsigned char mode_counter=0;
	if(adj_flag==1)
	{
            adj_flag=0;

            stop_bit=0;
			offset=0;
			switch_flag=1;
			status++;
			if(status>7) status=1;
			time_to_buff_all();

	}		
	if(esc_flag==1)
	{
            esc_flag=0;

            stop_bit=0;
			disp_mode=0;			//退出时方式为流动显示
			k=0;
			offset=0;
			status=0;
			time_to_buff_all();
	}

	
	switch(status)
	{
		case 0:	  		//显示模式调整
		if(add_flag==1)
		{
            add_flag=0;
                stop_bit=0;
				offset=0;
				switch_flag=1;
                mode_counter++;
				if(mode_counter>3) mode_counter=0;
                disp_mode=mode[mode_counter];

		}
		if(dec_flag==1)
		{
            dec_flag=0;
				offset=0;
                stop_bit=0;
				switch_flag=1;
				if(mode_counter==0) mode_counter=3;
                else mode_counter--;
                disp_mode=mode[mode_counter];
		}		
		break;		
				
		case 1:			//年调整
		disp_mode=3;
		if(add_flag==1)
		{
            add_flag=0;
				CurrentTime.Year++;
				if(CurrentTime.Year>99) CurrentTime.Year=0;
				DS1302_SetTime(DS1302_YEAR,(CurrentTime.Year));
		}
		if(dec_flag==1)
		{
            dec_flag=0;
				if(CurrentTime.Year>0) CurrentTime.Year--;
				if(CurrentTime.Year==0) CurrentTime.Year=99;
				DS1302_SetTime(DS1302_YEAR,(CurrentTime.Year));
		}
		break;

	  	case 2:			//月调整
		disp_mode=4;
		if(add_flag==1)
		{
            add_flag=0;
				CurrentTime.Month++;
				if(CurrentTime.Month>12) CurrentTime.Month=1;
				DS1302_SetTime(DS1302_MONTH,(CurrentTime.Month));
		}
		if(dec_flag==1)
		{
            dec_flag=0;
				if(CurrentTime.Month>0) CurrentTime.Month--;
				if(CurrentTime.Month==0) CurrentTime.Month=12;
				DS1302_SetTime(DS1302_MONTH,(CurrentTime.Month));
		}
		break;

		case 3:			//日调整
		disp_mode=5;
		if(add_flag==1)
		{
            add_flag=0;
				CurrentTime.Day++;
				if(CurrentTime.Day>31) CurrentTime.Day=1;
				DS1302_SetTime(DS1302_DAY,(CurrentTime.Day));
		}
		if(dec_flag==1)
		{
            dec_flag=0;
				if(CurrentTime.Day>0) CurrentTime.Day--;
				if(CurrentTime.Day==0) CurrentTime.Day=31;
				DS1302_SetTime(DS1302_DAY,(CurrentTime.Day));
		}
		break;

		case 4:			//星期调整
		disp_mode=6;
		if(add_flag==1)
		{
            add_flag=0;
				CurrentTime.Week++;
				if(CurrentTime.Week>7) CurrentTime.Week=1;
				DS1302_SetTime(DS1302_WEEK,(CurrentTime.Week));
		}
		if(dec_flag==1)
		{
            dec_flag=0;
				if(CurrentTime.Week>1) CurrentTime.Week--;
				if(CurrentTime.Week==1) CurrentTime.Week=7;
				DS1302_SetTime(DS1302_WEEK,(CurrentTime.Week));
		}
		break;

		case 5:			//小时调整
		disp_mode=2;
		if(add_flag==1)
		{
            add_flag=0;
				CurrentTime.Hour++;
				if(CurrentTime.Hour>23) CurrentTime.Hour=0;
				DS1302_SetTime(DS1302_HOUR,(CurrentTime.Hour));
		}
		if(dec_flag==1)
		{
            dec_flag=0;
				if(CurrentTime.Hour>0) CurrentTime.Hour--;
				if(CurrentTime.Hour==0) CurrentTime.Hour=23;
				DS1302_SetTime(DS1302_HOUR,(CurrentTime.Hour));
		}
		break;

		case 6:			//分钟调整
		disp_mode=2;
		if(add_flag==1)
		{
            add_flag=0;
				CurrentTime.Minute++;
				if(CurrentTime.Minute>59) CurrentTime.Minute=0;
				DS1302_SetTime(DS1302_MINUTE,(CurrentTime.Minute));
		}
		if(dec_flag==1)
		{
            dec_flag=0;
				if(CurrentTime.Minute>0) CurrentTime.Minute--;
				if(CurrentTime.Minute==0) CurrentTime.Minute=59;
				DS1302_SetTime(DS1302_MINUTE,(CurrentTime.Minute));
		}
		break;

		case 7:			//秒调整
		disp_mode=2;
		if(add_flag==1)
		{
            add_flag=0;				
				CurrentTime.Second++;
				if(CurrentTime.Second>59) CurrentTime.Second=0;
				DS1302_SetTime(DS1302_SECOND,(CurrentTime.Second));
		}
		if(dec_flag==1)
		{
            dec_flag=0;				
				if(CurrentTime.Second>0) CurrentTime.Second--;
				if(CurrentTime.Second==0) CurrentTime.Second=59;
				DS1302_SetTime(DS1302_SECOND,(CurrentTime.Second));
		}
		break;

		default: 
		break;
	}
}