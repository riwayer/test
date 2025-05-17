/*	# 	DS1302代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/								

#include "Ds1302.h"

sbit SCK = P1^7;
sbit RST = P1^3;
sbit SDA = P2^3;

//
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

//写时间
void Ds1302_Write(uc *dat)
{
	Write_Ds1302_Byte(0x8e, 0x00);	//关写保护
	Write_Ds1302_Byte(0x80, 0x00);	//写入时停止计时
	Write_Ds1302_Byte(0x84, *dat++);
	Write_Ds1302_Byte(0x82, *dat++);
	Write_Ds1302_Byte(0x80, *dat);	//写入秒，顺便开始计时
	Write_Ds1302_Byte(0x8e, 0x80);	//开写保护
}

void Ds1302_Read(uc *dat)
{
	EA = 0;
	*dat++ = Read_Ds1302_Byte(0x85);	//时
	*dat++ = Read_Ds1302_Byte(0x83);	//分
	*dat   = Read_Ds1302_Byte(0x81);	//秒
	EA = 1;
}