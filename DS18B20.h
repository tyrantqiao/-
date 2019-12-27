#ifndef _DS18B20_H_  
#define _DS18B20_H_  
 
#define uchar unsigned char
#define uint  unsigned int
int t;
 
sbit DQ=P2^2;   //����λds18b20��

int shi;				
int ge;					
int shifen;				
int baifen;		

uint temp; 			 //�¶�uint

uchar dat;			 //����(data)
uchar a=0;	
uchar b=0; 

float f_temp;		 //�¶�float
float tep=0; 			//��һ���¶�ʱ���м��¶�(temp��д)

void WriteOneChar(uchar dat);   //дһ���ַ�[����/����]
 
void delay_time(uint num)   //delay����,��Ϊ���������е�delay��ͬ,���Զ������ֲ�ͬ 
{
 
while(num--);
 
}
 
void Init_DS18B20(void) //��ʼ��
{
	uint i;   //�������,�����ӳ�
	DQ=0;     //��λ ��������(���������������)
	i=103;
	while(i>0)i--;
	DQ=1;
	i=4;
	while(i>0)i--;
}
bit tempreadBit(void)  //��һλ����
{
	uint i;
	bit dat;
	DQ=0;i++;     //i++ ��ʱ
	DQ=1;i++;i++;
	dat=DQ;
	i=8;while(i>0)i--;
	return(dat);
	
}
uchar ReadOneChar(void)  //��һλ�ַ�
{
uchar i,j;
uchar dat=0;
for(i=0;i<8;i++)
{
	j=tempreadBit();
	dat=(j<<7)|(dat>>1);   //�������λ����ǰ��
}
return(dat);
}
void tempchange(void)  //��ȡ�¶Ȳ�ת��
{
	Init_DS18B20();      //��ʼ��
	delay(1);	
	WriteOneChar(0xcc);		//д������ROMָ��
	WriteOneChar(0x44);		//д�¶�ת��ָ��
}

uint get_temp()    //����¶�(uint)
{
	uchar a,b;
	Init_DS18B20();
	delay(1);
	WriteOneChar(0xcc);   //��ROMָ��
	WriteOneChar(0xbe);		//���ݴ���9�ֽڵ��¶�����
	a=ReadOneChar();			//��8λ
	b=ReadOneChar();			//��8λ
	temp=b;								
	temp<<=8;							//ƴ��һ��
	temp=temp|a;					//ƴ��һ��
	f_temp=temp*0.0625;		
	temp=f_temp*10+0.5;		//+0.5��������    *10��ֻȡС�������һλ,Ҫ������Ҫ��Ҫ�õ��ٷ�λ
	f_temp=f_temp+0.05;
	return temp;
	
}

 
void WriteOneChar(uchar dat)  //дһ���ַ�[����/����]
{
	uint i;
	uchar j;
	bit testb;
	for(j=1;j<=8;j++)
	{
		testb=dat&0x01;
		dat=dat>>1;
		if(testb)
		{
			DQ=0;
			i++;i++;
			DQ=1;
			i=8;while(i>0)i--;
		}
		else
		{
			DQ=0;
			i=8;while(i>0)i--;
			DQ=1;
			i++;i++;
		}
	}
}


  
#endif  