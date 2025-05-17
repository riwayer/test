/*=========放头文件区=========*/
#include "Init.h"
#include "Key.h"
#include "Seg.h"
#include "Led.h"
#include "Ds18b20.h"
#include "ds1302.h"

/*=========变量声明区=========*/
uc 		 All_Index;
ui		 Seg_350ms;
uc 		 Key_10ms;
uc		 Led_100ms;
idata uc Seg_Buf[] = {10, 10, 10, 10, 10, 10, 10, 10};
idata uc Seg_Point[] = {0, 0, 0, 0, 0, 0, 0, 0};
idata uc Led_Buf[] = {0, 0, 0, 0, 0, 0, 0, 0};
uc		 Key_Val, Key_Down, Key_Up, Key_Old;

uc		 Seg_Mode;			//0-温度显示界面，1-时间显示，2-参数设置
pdata ui Temprature_10x = 0;
uc code	 Time[] = {0x23, 0x25, 0x20};
uc		 Time_Read[3];
uc		 Temperature_Set = 23;
bit 	 S17_Flag = 0;	// 0-参数界面减一， 1-时间界面显示秒

bit		 Realy_Flag = 0;	//0-温度控制， 1-时间控制
idata uc Time_AlamL1[3];		//闹钟
bit		 Time_AlamL1_Flag;	//0-不在时间内， 1-在时间内
uc		 L3_100ms;
bit 	 L3_Flish_Flag;


/*=========其他函数区=========*/
void Realy_Proc(void)
{
	//	if (Key_10ms < 10) return;			//测试时序，不再进行10ms的减速
//	Key_10ms = 0;
	
	if (Realy_Flag)
		N_Relay(Time_AlamL1_Flag);
	else
		N_Relay((Temprature_10x) > Temperature_Set*10);	
}
void Led_Proc(void)
{
	if (Led_100ms < 50) return;
	Led_100ms = 0;
	
	Ds1302_Read(Time_Read);
	if ((Time_Read[1] == 0) && (Time_Read[2] == 0))//整点闹钟
	{	
		Time_AlamL1[2] = Time_Read[2];
		Time_AlamL1_Flag = 1;
	}
	if ((Time_AlamL1[2] / 16 % 16 + 5) == Time_Read[2])
	{
		Time_AlamL1_Flag = 0;
	}
	
	Led_Buf[0] = Time_AlamL1_Flag ? 1 : 0;
	Led_Buf[1] = Realy_Flag ? 0 : 1;
	Led_Buf[2] = ((Temprature_10x) > Temperature_Set*10) && L3_Flish_Flag && (!Realy_Flag);
}
void Delay750ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 35;
	j = 51;
	k = 182;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/*=========按键函数区=========*/
void Key_Proc(void)
{
	if (Key_10ms < 10) return;
	Key_10ms = 0;
	
	Key_Val = Key_Read();
	Key_Down = Key_Val & (Key_Old ^ Key_Val);
	Key_Up = Key_Val & ~(Key_Old ^ Key_Val);
	Key_Old = Key_Val;
	
	if ((Key_Down == 17) &&(Seg_Mode == 2))		//S17//参数界面
	{			
		if (--Temperature_Set < 10)
			Temperature_Set = 10;				
	}
	if ((Seg_Mode == 1) && (Key_Old == 17))
		S17_Flag = 1;
	else
		S17_Flag = 0;
	
	switch(Key_Down)
	{
		case 12:	//界面切换
			if (++Seg_Mode == 3)
				Seg_Mode = 0;
		break;
		case 13:	//模式切换
			Realy_Flag ^= 1;
		break;
		case 16:	//参数加
			if (Seg_Mode != 2) break;
			if (++Temperature_Set > 99)
				Temperature_Set = 99;
		break;
	}
}
/*=========显示函数区=========*/
void Seg_Proc(void)
{
	if (Seg_350ms < 500) return;
	Seg_350ms = 0;
	
	switch(Seg_Mode)
	{
		case 0:	//温度显示界面
			Temprature_10x =  (ui)(10*Ds18b20_Read());
			Seg_Buf[0] = 11;
			Seg_Buf[1] = 1;
			Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
			Seg_Buf[5] = Temprature_10x / 100 % 10;
			Seg_Buf[6] = Temprature_10x / 10 % 10;
			Seg_Buf[7] = Temprature_10x % 10;
			Seg_Point[6] = 1;
		break;
		case 1:		//时间显示界面
			Ds1302_Read(Time_Read);
			Seg_Point[6] = 0;
			Seg_Buf[0] = 11;
			Seg_Buf[1] = 2;
			Seg_Buf[2] = 10;
			Seg_Buf[3] =  S17_Flag ? Time_Read[1] / 16 % 16 : Time_Read[0] / 16 % 16;
			Seg_Buf[4] =  S17_Flag ? Time_Read[1] % 16 		: Time_Read[0] % 16;
			Seg_Buf[5] = 12;//Key_Old % 
			Seg_Buf[6] =  S17_Flag ? Time_Read[2] / 16 % 16 : Time_Read[1] / 16 % 16;
			Seg_Buf[7] =  S17_Flag ? Time_Read[2] % 16 		: Time_Read[1] % 16;
		break;
		case 2:		//参数设置
			Seg_Buf[0] = 11;
			Seg_Buf[1] = 3;
			Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = Seg_Buf[5] = 10;
			Seg_Buf[6] = Temperature_Set / 10 % 10;
			Seg_Buf[7] = Temperature_Set % 10;
		break;
	}
}
/*=========中断函数区=========*/
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	EA = 1;
	ET0 = 1;
}

/*=========中断服务区=========*/
void Timer0Server(void)		interrupt 1
{
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	++Key_10ms;
	++Seg_350ms;
	++Led_100ms;
	if (++All_Index == 8) All_Index = 0;
	Seg_Disp(All_Index, Seg_Buf[All_Index], Seg_Point[All_Index]);
	Led_Disp(All_Index, Led_Buf[All_Index]);
	if (++L3_100ms == 100)
	{
		L3_100ms = 0;
		L3_Flish_Flag ^= 1;
	}
}
/*=========用户主函数=========*/
void main(void)
{
	System_Init();
	Timer0Init();
	Ds1302_Write(Time);
	Ds18b20_Read();
	Delay750ms();
	
	while(1)
	{
		Key_Proc();
		Led_Proc();
		Seg_Proc();
		Realy_Proc();
	}
}