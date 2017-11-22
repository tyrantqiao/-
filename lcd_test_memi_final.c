#include "lcd1602.h"
#include "DS18b20.h"
#define uchar unsigned char
#define uint  unsigned int

uchar  cKey;				//�ж�����λ[������һ��]
uchar  cKeyCode;			//�жϾ�����˭[����]
uint nDelayKey;			//�ӳټ���
uint  nDelay100uS;		//ds18b20��λ��ʱ

int Step;
int setTemp;				/*set temp*/
int currentTemp;		/*current*/
int roomT;				/*room*/
int zkb;					/*ռ�ձ�*/
int timer;					/*�ж�ʱ��*/

uchar num;							//i,j,k֮���	
uint result;					//����get_temp()���¶�ֵ

char* currentTemp_str;
char* setTemp_str;

uchar code table[] ="25.3";	//�����ܷ���ʾ������'.'
uchar code table1[]="WWW.XXXXX.COM";	//����Ӣ��
char temp_str[4] = {0};			//�¶ȵ�����

sbit red=P1^0;			//ָʾ��1
sbit blue=P1^1;			//ָʾ��2
//sbit PWM=Px^x     //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!��Ҫ����

void KeyScan(void);   //ɨ�谴��
void DisposeKey(void);	//�����¼�
char* convert(int Temp);	//ת��uint���¶�,��get_temp()��õ�����
void displayTemp(void);  //displayTemp
void main(void) 
{  
	//lcd �����Լ���ȡ�¶ȵĲ���
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
	
	
	
	int k = 0;				//���±�־
	
	TMOD= 0x02;		//��ʱ��2
	TL1 = -92;			//����Ϊ100us
	TH1 = -92;			
	TR1 = 1;				//��ʱ��1
	ET1 = 1;				//��ʱ��1�ж�
	IP=0x04;				//��ʱ��1�ж���߼�
	EA=1;				//���ж�
	
	init();       //Һ����
	Init_DS18B20();		//�¶ȴ�������
	while(1)
	{	
		//tempchange();
		//currentTemp = get_temp();		//��ȡ�¶�,������Ϊ��ǰ�¶�
		//if(k == 0)									//�ж����±�־,��Ϊ0,����������.
		//{
		//	roomT = currentTemp;
		//	k++;											//��1����ѭ���ٴθ���
		//	displayTemp();
		//}
		//if(cKeyCode)		//ɨ���
		//{
		//	DisposeKey();  //���������
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
		currentTemp = get_temp();		//��ȡ�¶�,������Ϊ��ǰ�¶�
		
		if(k == 0)									//�ж����±�־,��Ϊ0,����������.
		{
			roomT = currentTemp;
			k++;											//��1����ѭ���ٴθ���
			//displayTemp();
			//delay(5);
		}
		
		
		if(cKeyCode)		//ɨ���
		{
			DisposeKey();  //���������
			//displayTemp();	//չʾ
			//delay(5);
		}
	
	
	  zkb = (int)100 * (setTemp-currentTemp)/(setTemp-roomT);  //�жϵ���ٶ�
		if(setTemp-currentTemp < 0)	//�ж��Ƿ�ص��.
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

void DisposeKey(void) //�������¼�
{
	switch(cKeyCode)
	{
		case 0x60:			  		//P3^4
			blue=1;red=0;				//�����ָʾ�Ƶ�,��ʱ����������û��ʵ�ֳɹ�
			Step = Step+30;			//����ǲ���
			nDelayKey = 2000;		//��ʱ(�����ʱ)	
			break;
		case 0x50:						//P3^5
			blue=0;red=1;				//ͬ��
			setTemp = setTemp+Step;  //SetTemp���õ��¶�++
			nDelayKey = 2000;		
			displayTemp();
			delay(10);
			break;
		case 0x30:						//P3^6
			blue=0;red=0;
			setTemp = setTemp-Step;  //SetTemp�����¶�--
			nDelayKey = 2000;
			displayTemp();
			delay(10);
			break;
		default:
			break;
	}
	cKeyCode = 0;		
}


char* convert(int Temp) //ת��uint���¶�ֵ--get_temp()
{	 
	//char temp_str[4] = {0};			//�¶ȵ�����
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
			cKeyCode = P3 &0x70;		  // 0111 0000  P3^456  ������Ҫ���ÿ���İ��������Ƕ���,Ȼ��ȥ�ж��Ƿ�����
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
