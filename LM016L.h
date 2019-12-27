#include <reg51.h> 
#include <intrins.h> 
sbit rs= P1^5; 
sbit rw = P1^6; 
sbit ep = P1^7;  
unsigned char code dis1[] = {"0571-85956028"};
void delay(unsigned char ms) 
{  
	unsigned char i; 
	while(ms--)  
	{  
		for(i = 0; i< 250; i++) 
		{ 
			_nop_(); 
			_nop_(); 
			_nop_(); 
			_nop_(); 
		} 
	} 
}  
bit lcd_bz() 
{  
	bit result;
	rs = 0; 
	rw = 1; 
	ep = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();  
	result = (bit)(P3 & 0x80); 
	ep = 0;  
	return result; 
}  
void lcd_wcmd(unsigned char cmd) 
{  
	while(lcd_bz());//ÅÐ¶ÏLCDÊÇ·ñÃ¦Âµ 
		rs = 0; 
	rw = 0; 
	ep = 0; 
	_nop_(); 
	_nop_(); 
	P3 = cmd; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	ep = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	ep = 0; 
}
void lcd_pos(unsigned char pos) 
{  
	lcd_wcmd(pos | 0x80); 
 }  
void lcd_wdat(unsigned char dat) 
{  
	while(lcd_bz());//ÅÐ¶ÏLCDÊÇ·ñÃ¦Âµ 
	rs = 1; 
	rw = 0; 
	ep = 0; 
	P3 = dat; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	ep = 1; 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	ep = 0; 
}  
void lcd_init() 
{  
	lcd_wcmd(0x38); 
	delay(1);  
	lcd_wcmd(0x0c); 
	delay(1); 
	lcd_wcmd(0x06); 
	delay(1); 
	lcd_wcmd(0x01); 
	delay(1); 
}  
