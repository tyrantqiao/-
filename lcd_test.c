

#include "LM016L.h"
#define uchar unsigned char
#define uint  unsigned int

uchar  cKey;				/*????*/
uchar  cKeyCode;			/* ??	*/
uint nDelayKey;			/*??????,?????????????*/
uint  nDelay100uS;		/*18B20??????*/
sbit red=P0^6;
sbit blue=P0^7;
void KeyScan(void);
void DisposeKey(void);
void main(void) 
{  
	
		unsigned char i;  
		lcd_init();
	  delay(10);
		lcd_pos(0x01);
		i = 0;  
		while(dis1[i] != '\0') 
		{  
			lcd_wdat(dis1[i]);
			i++;  
		}
		/*
	TMOD= 0x20;		//?????????
	TL1 = -92;			//????????100us
	TH1 = -92;
	TR1 = 1;				//?????1
	ET1 = 1;				//?????1??
	IP=0x04;				//????1?????????
	EA=1;				//????
	while(1){
		
		if(cKeyCode)
			{
				DisposeKey();
			}
	}*/
}


void KeyScan(void)
{
	cKey = P2 & 0xE0;					//P27,P26,P25
	if(cKey != 0xE0)
	{
		nDelayKey = 100;				//
	}
}

void DisposeKey(void)
{
	switch(cKeyCode)
	{
		case 0x60:			  		
			blue=1;red=0;
			nDelayKey = 2000;			
			break;
		case 0xA0:			
			blue=0;red=1;
			nDelayKey = 2000;
			break;
		case 0xC0:				
			blue=0;red=0;
			nDelayKey = 2000;
			break;
		default:
			break;
	}
	cKeyCode = 0;		
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
			cKeyCode = P2 &0xE0;		
			if(cKeyCode != cKey)	
			{
				cKeyCode = 0;				
			}
		}
	}
}
