#include "Init.h"

void System_Init(void)
{
	uc gate;
	
	P0 = 0XFF;		//关LED
	gate = P2 & 0X1F;
	gate |= 0X80;
	P2 = gate;
	gate = P2 & 0X1F;
	P2 = gate;
	
	P0 = 0X00;		//关外设
	gate = P2 & 0X1F;
	gate |= 0XA0;
	P2 = gate;
	gate = P2 & 0X1F;
	P2 = gate;
	
	P0 = 0XFF;		//关SEG
	gate = P2 & 0X1F;
	gate |= 0XE0;
	P2 = gate;
	gate = P2 & 0X1F;
	P2 = gate;
}