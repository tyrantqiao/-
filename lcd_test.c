

#include "lcd1602.h"
#include "DS18b20.h"
#define uchar unsigned char
#define uint  unsigned int

uchar  cKey;				//判断整个位[按键那一块]
uchar  cKeyCode;			//判断具体是谁[按键]
uint nDelayKey;			//延迟
uint  nDelay100uS;		
sbit red=P1^6;			//指示灯1
sbit blue=P1^7;			//指示灯2
char temp_str[5] = {0};			//温度的数组
uchar code table[] ="25.35";	//测试能否显示数字与'.'
uchar code table1[]="WWW.XXXXX.COM";	//测试英文
void KeyScan(void);   //扫描按键
void DisposeKey(void);	//处理事件
char* convert(int Temp);	//转换uint的温度,用get_temp()获得的数据
uchar num;							//i,j,k之类的	
uint result;					//储存get_temp()的温度值
void main(void) 
{  
	/*lcd 测试以及读取温度的测试
	
		init();
		Init_DS18B20();
		while(1)
		{
			tempchange();
			result=get_temp();
			write_com(0x80);
			convert(result);
			for(num=0;num<5;num++){
				write_data(temp_str[num]);
				delay(5);
			}
		}	
	*/
	
	
	TMOD= 0x20;		//定时器工作方式2
	TL1 = -92;			//中断一次为100us
	TH1 = -92;		
	TR1 = 1;			//定时器1
	ET1 = 1;			//启动中断1
	IP=0x04;			//定时器1中断为最高级
	EA=1;					//总中断	
	while(1){
		
		if(cKeyCode)
		{
				DisposeKey();
		}
		
		
	}
}


void KeyScan(void)
{
	cKey = P2 & 0x38;					//P23,P24,P25
	if(cKey != 0xE0)
	{
		nDelayKey = 100;				
	}
}

void DisposeKey(void) //处理按键事件
{
	switch(cKeyCode)
	{
		case 0x60:			  		//0110 0000  这个是三个按键的具体编号
			blue=1;red=0;				//这个是指示灯的,到时用来看看有没有实现成功
			Step = Step+30;			//这个是步进
			nDelayKey = 2000;		//延时(软件延时)	
			break;
		case 0xA0:						//0001 0000  这个编号你得看看怎么改
			blue=0;red=1;				//同上
			SetTemp = SetTemp+Step;  //SetTemp设置的温度++
			nDelayKey = 2000;		
			convert(SetTemp);
													//显示当前设置温度是什么
			for(num=0;num<5;num++){   
				WriteOneChar(temp_str[num]);
			}
			break;
		case 0xC0:						//1100 0000
			blue=0;red=0;
			SetTemp = SetTemp-Step;  //SetTemp设置温度--
			nDelayKey = 2000;
			convert(SetTemp);
													//显示当前设置温度是什么
			for(num=0;num<5;num++){
				WriteOneChar(temp_str[num]);
			}
			break;
		default:
			break;
	}
	cKeyCode = 0;		
}


char* convert(int Temp) //转换uint的温度值--get_temp()
{	 
	shi = Temp/100;	  
	ge = Temp%100/10;
	shifen = Temp%100%10;
	baifen = Temp%10;
	temp_str[0] = shi+48;
	temp_str[1] = ge+48;
	temp_str[2] = '.';
	temp_str[3] = shifen+48;
	temp_str[4] = baifen+48;
	return temp_str;	
}

void IntT1(void) interrupt 3
{
	if(nDelayKey == 0)
	{
		KeyScan();
	}	
	else
	{
		nDelayKey--;
		if(nDelayKey == 0)
		{
			cKeyCode = P2 &0xE0;		  //这里你要设置看你的按键具体是多少,然后去判断是否按下了
			if(cKeyCode != cKey)	
			{
				cKeyCode = 0;				
			}
		}
	}
}
