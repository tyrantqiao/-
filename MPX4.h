#include "reg52.h"

unsigned int percentile;
unsigned int tenths;
unsigned int decade;
unsigned int single;

char code number[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void delay20ms();
void displayNum(int num);

unsigned int tag;

void displayNum(int num)
{
	P3=0xf0;
	percentile=num%10;
	tenths=num/10%10;
	decade=num/1000; 
	single=num/100%10;
	tag=1;
	
		P1=number[percentile];
		P3=0x80;
		delay20ms();
		P3=0x00;
		
		P1=number[tenths];
		P3=0x40;
		delay20ms();
		P3=0x00;

		P1=number[single];
		P3=0x20;
		delay20ms();
		P3=0x00;

		P1=number[decade];
		P3=0x10;
		delay20ms();
		P3=0x00;
	

}




void delay20ms()     
{     
    int i,j;     
    
    for(i=0;i<5;i++)     
    {     
        for(j=0;j<200;j++)     
        {     
    
        }     
    }     
}   

