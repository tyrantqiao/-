

#include "lcd1602.h"
#include "DS18b20.h"
#define uchar unsigned char
#define uint  unsigned int

uchar  cKey;				//�ж�����λ[������һ��]
uchar  cKeyCode;			//�жϾ�����˭[����]
uint nDelayKey;			//�ӳ�
uint  nDelay100uS;		
sbit red=P1^6;			//ָʾ��1
sbit blue=P1^7;			//ָʾ��2
char temp_str[5] = {0};			//�¶ȵ�����
uchar code table[] ="25.35";	//�����ܷ���ʾ������'.'
uchar code table1[]="WWW.XXXXX.COM";	//����Ӣ��
void KeyScan(void);   //ɨ�谴��
void DisposeKey(void);	//�����¼�
char* convert(int Temp);	//ת��uint���¶�,��get_temp()��õ�����
uchar num;							//i,j,k֮���	
uint result;					//����get_temp()���¶�ֵ
void main(void) 
{  
	/*lcd �����Լ���ȡ�¶ȵĲ���
	
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
	
	
	TMOD= 0x20;		//��ʱ��������ʽ2
	TL1 = -92;			//�ж�һ��Ϊ100us
	TH1 = -92;		
	TR1 = 1;			//��ʱ��1
	ET1 = 1;			//�����ж�1
	IP=0x04;			//��ʱ��1�ж�Ϊ��߼�
	EA=1;					//���ж�	
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

void DisposeKey(void) //�������¼�
{
	switch(cKeyCode)
	{
		case 0x60:			  		//0110 0000  ��������������ľ�����
			blue=1;red=0;				//�����ָʾ�Ƶ�,��ʱ����������û��ʵ�ֳɹ�
			Step = Step+30;			//����ǲ���
			nDelayKey = 2000;		//��ʱ(�����ʱ)	
			break;
		case 0xA0:						//0001 0000  ��������ÿ�����ô��
			blue=0;red=1;				//ͬ��
			SetTemp = SetTemp+Step;  //SetTemp���õ��¶�++
			nDelayKey = 2000;		
			convert(SetTemp);
													//��ʾ��ǰ�����¶���ʲô
			for(num=0;num<5;num++){   
				WriteOneChar(temp_str[num]);
			}
			break;
		case 0xC0:						//1100 0000
			blue=0;red=0;
			SetTemp = SetTemp-Step;  //SetTemp�����¶�--
			nDelayKey = 2000;
			convert(SetTemp);
													//��ʾ��ǰ�����¶���ʲô
			for(num=0;num<5;num++){
				WriteOneChar(temp_str[num]);
			}
			break;
		default:
			break;
	}
	cKeyCode = 0;		
}


char* convert(int Temp) //ת��uint���¶�ֵ--get_temp()
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
			cKeyCode = P2 &0xE0;		  //������Ҫ���ÿ���İ��������Ƕ���,Ȼ��ȥ�ж��Ƿ�����
			if(cKeyCode != cKey)	
			{
				cKeyCode = 0;				
			}
		}
	}
}
