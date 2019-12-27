#include "lcd1602.h"
#include "DS18b20.h"
#define uchar unsigned char
#define uint  unsigned int

uchar  cKey;				//判断整个位[按键那一块]
uchar  cKeyCode;			//判断具体是谁[按键]
uint nDelayKey;			//延迟键盘
uint  nDelay100uS;		//ds18b20复位延时

int Step;
int setTemp;				/*set temp*/
int currentTemp;		/*current*/
int roomT;				/*room*/
int zkb;					/*占空比*/
int timer;					/*中断时间*/

uchar num;							//i,j,k之类的	
uint result;					//储存get_temp()的温度值

char* currentTemp_str;
char* setTemp_str;

uchar code table[] ="25.3";	//测试能否显示数字与'.'
uchar code table1[]="WWW.XXXXX.COM";	//测试英文
char temp_str[4] = {0};			//温度的数组

sbit red=P1^0;			//指示灯1
sbit blue=P1^1;			//指示灯2
//sbit PWM=Px^x     //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!需要设置

void KeyScan(void);   //扫描按键
void DisposeKey(void);	//处理事件
char* convert(int Temp);	//转换uint的温度,用get_temp()获得的数据
void displayTemp(void);  //displayTemp
void main(void) 
{  
	//lcd 测试以及读取温度的测试
	/*
		init();
		Init_DS18B20();
		while(1)
		{
			tempchange();
			currentTemp=get_temp();
			displayTemp();
		}	
	*/
	
	
	
	int k = 0;				//室温标志
	
	TMOD= 0x02;		//定时器2
	TL1 = -92;			//周期为100us
	TH1 = -92;			
	TR1 = 1;				//定时器1
	ET1 = 1;				//定时器1中断
	IP=0x04;				//定时器1中断最高级
	EA=1;				//总中断
	
	init();       //液晶的
	Init_DS18B20();		//温度传感器的
	while(1)
	{	
		//tempchange();
		//currentTemp = get_temp();		//获取温度,并设置为当前温度
		//if(k == 0)									//判断室温标志,若为0,则设置室温.
		//{
		//	roomT = currentTemp;
		//	k++;											//加1避免循环再次更改
		//	displayTemp();
		//}
		//if(cKeyCode)		//扫描键
		//{
		//	DisposeKey();  //处理键函数
		//}
		//init();
		/*
		tempchange();
		currentTemp=get_temp();
		write_com(0x80);
		convert(currentTemp);
		for(num=0;num<4;num++)
		{
			write_data(temp_str[num]);
			delay(5);
		}
		delay(1000);
		*/
		tempchange();
		currentTemp = get_temp();		//获取温度,并设置为当前温度
		
		if(k == 0)									//判断室温标志,若为0,则设置室温.
		{
			roomT = currentTemp;
			k++;											//加1避免循环再次更改
			//displayTemp();
			//delay(5);
		}
		
		
		if(cKeyCode)		//扫描键
		{
			DisposeKey();  //处理键函数
			//displayTemp();	//展示
			//delay(5);
		}
	
	
	  zkb = (int)100 * (setTemp-currentTemp)/(setTemp-roomT);  //判断电机速度
		if(setTemp-currentTemp < 0)	//判断是否关电机.
		{
			  zkb = 0;
		}
	
		displayTemp();
		delay(1000);
		
	}	
}

void displayTemp(void)
{
			init();
			write_com(0x80);
			currentTemp = get_temp();
			currentTemp_str=convert(currentTemp);
			write_data('C');
			delay(5);
			write_data('.');
			delay(5);
			for(num=0;num<4;num++){
				write_data(currentTemp_str[num]);
				delay(5);
			}
			write_com(0x80+0x40);
			setTemp_str=convert(setTemp);
			write_data('S');
			delay(5);
			write_data('.');
			delay(5);
			for(num=0;num<4;num++){
				write_data(setTemp_str[num]);
				delay(5);
			}
			//delay(50);
}

void KeyScan(void)
{
	cKey = P3 & 0x70;					// 0111 0000 
	if(cKey != 0x70)
	{
		nDelayKey = 100;				
	}
}

void DisposeKey(void) //处理按键事件
{
	switch(cKeyCode)
	{
		case 0x60:			  		//P3^4
			blue=1;red=0;				//这个是指示灯的,到时用来看看有没有实现成功
			Step = Step+30;			//这个是步进
			nDelayKey = 2000;		//延时(软件延时)	
			break;
		case 0x50:						//P3^5
			blue=0;red=1;				//同上
			setTemp = setTemp+Step;  //SetTemp设置的温度++
			nDelayKey = 2000;		
			displayTemp();
			delay(10);
			break;
		case 0x30:						//P3^6
			blue=0;red=0;
			setTemp = setTemp-Step;  //SetTemp设置温度--
			nDelayKey = 2000;
			displayTemp();
			delay(10);
			break;
		default:
			break;
	}
	cKeyCode = 0;		
}


char* convert(int Temp) //转换uint的温度值--get_temp()
{	 
	//char temp_str[4] = {0};			//温度的数组
	shi = Temp/100;	  
	ge = Temp%100/10;
	shifen = Temp%100%10;
//	baifen = Temp%10;
	temp_str[0] = shi+48;
	temp_str[1] = ge+48;
	temp_str[2] = '.';
	temp_str[3] = shifen+48;
	//temp_str[4] = baifen+48;
	return temp_str;	
}

void IntT1(void) interrupt 3
{
	timer++;
	if(timer >= zkb)
	{
		//PWM = 1;
	}
	else
	{
	  // PWM = 0;
	}
	if(timer == 100)
	{
		timer = 0;
	}
	
	if(nDelayKey == 0)
	{
		KeyScan();
	}	
	else
	{
		nDelayKey--;
		if(nDelayKey == 0)
		{
			cKeyCode = P3 &0x70;		  // 0111 0000  P3^456  这里你要设置看你的按键具体是多少,然后去判断是否按下了
			if(cKeyCode != cKey)	
			{
				cKeyCode = 0;				
			}
		}
	}

	
	if(nDelay100uS)
	{
		nDelay100uS--;	
	}
	
}
