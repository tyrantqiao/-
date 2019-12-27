#ifndef _DS18B20_H_  
#define _DS18B20_H_  
 
#define uchar unsigned char
#define uint  unsigned int
int t;
 
sbit DQ=P2^2;   //数据位ds18b20的

int shi;				
int ge;					
int shifen;				
int baifen;		

uint temp; 			 //温度uint

uchar dat;			 //数据(data)
uchar a=0;	
uchar b=0; 

float f_temp;		 //温度float
float tep=0; 			//读一个温度时的中间温度(temp缩写)

void WriteOneChar(uchar dat);   //写一个字符[数据/命令]
 
void delay_time(uint num)   //delay函数,因为与主函数中的delay不同,所以二者名字不同 
{
 
while(num--);
 
}
 
void Init_DS18B20(void) //初始化
{
	uint i;   //定义变量,用于延迟
	DQ=0;     //复位 产生脉冲(下面属于这个过程)
	i=103;
	while(i>0)i--;
	DQ=1;
	i=4;
	while(i>0)i--;
}
bit tempreadBit(void)  //读一位数据
{
	uint i;
	bit dat;
	DQ=0;i++;     //i++ 延时
	DQ=1;i++;i++;
	dat=DQ;
	i=8;while(i>0)i--;
	return(dat);
	
}
uchar ReadOneChar(void)  //读一位字符
{
uchar i,j;
uchar dat=0;
for(i=0;i<8;i++)
{
	j=tempreadBit();
	dat=(j<<7)|(dat>>1);   //数据最低位在最前面
}
return(dat);
}
void tempchange(void)  //获取温度并转换
{
	Init_DS18B20();      //初始化
	delay(1);	
	WriteOneChar(0xcc);		//写跳过读ROM指令
	WriteOneChar(0x44);		//写温度转换指令
}

uint get_temp()    //获得温度(uint)
{
	uchar a,b;
	Init_DS18B20();
	delay(1);
	WriteOneChar(0xcc);   //读ROM指令
	WriteOneChar(0xbe);		//读暂存器9字节的温度数据
	a=ReadOneChar();			//低8位
	b=ReadOneChar();			//高8位
	temp=b;								
	temp<<=8;							//拼在一起
	temp=temp|a;					//拼在一起
	f_temp=temp*0.0625;		
	temp=f_temp*10+0.5;		//+0.5四舍五入    *10是只取小数点后面一位,要看我们要不要用到百分位
	f_temp=f_temp+0.05;
	return temp;
	
}

 
void WriteOneChar(uchar dat)  //写一个字符[命令/数据]
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