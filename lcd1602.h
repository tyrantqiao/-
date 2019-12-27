#include <reg52.h> 
sbit rs= P3^5; 
//sbit rw = P3^1; 
sbit en = P3^4;   

sbit dula=P2^6;  
sbit wela=P2^7;  
unsigned char code dis1[] = {"0571-85956028"};
void delay(unsigned int ms) 
{  
	 unsigned int x,y;
	 for(x=ms;x>0;x--)
	 	for(y=110;y>0;y--);
}  

void write_com(unsigned char cmd) 
{  
	rs=0;
	P0=cmd;
	delay(5);
	en=1;
	delay(5);
	en=0;
}
void write_data(unsigned char dat) 
{  
	rs=1;
	P0=dat;
	delay(5);
	en=1;
	delay(5);
	en=0;
}  
void init() 
{  
	dula=0;
	wela=0;
	en=0;
	write_com(0x38); //16*2
	write_com(0x0c); //open display
	write_com(0x06); //write char then address add one
	write_com(0x01); //clear
}  


