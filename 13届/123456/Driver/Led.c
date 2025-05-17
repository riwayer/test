#include "Led.h"

//Led
void Led_Disp(uc addr, enable)
{
	uc gate;
	static temp = 0x00;
	static temp_old = 0xff;
	
	if (enable)
		temp |= 0x01 << addr;
	else
		temp &= ~(0x01 << addr);
	
	if (temp != temp_old)
	{
		P0 = ~temp;
		gate = P2 & 0X1F;
		gate |= 0X80;
		P2 = gate;
		gate = P2 & 0X1F;
		P2 = gate;
		temp_old = temp;
	}
}

//继电器，1吸合，2断开
void N_Relay(uc enable)
{
	uc gate;
	
	if (enable)
	{
		P0 = 0X10;		//开继电器
		gate = P2 & 0X1F;
		gate |= 0XA0;
		P2 = gate;
		gate = P2 & 0X1F;
		P2 = gate;
	}
	else
	{
		P0 = 0X00;		//关外设
		gate = P2 & 0X1F;
		gate |= 0XA0;
		P2 = gate;
		gate = P2 & 0X1F;
		P2 = gate;
	}
}