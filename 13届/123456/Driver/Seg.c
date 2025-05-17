#include "Seg.h"

uc Dula[] = {~0x3F, ~0x06, ~0x5B, ~0x4F, ~0x66, ~0x6D, ~0x7D, ~0x07, ~0x7F, ~0x6F, 0xff,
			 ~0x3E, ~0x40};
			//11=U, 12--
uc Wela[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void Seg_Disp(uc We, Du, Point)
{
	uc gate;
	
	P0 = 0XFF;		//消影
	gate = P2 & 0X1F;
	gate |= 0XE0;
	P2 = gate;
	gate = P2 & 0X1F;
	P2 = gate;
	
	P0 = Wela[We];		//位选
	gate = P2 & 0X1F;
	gate |= 0XC0;
	P2 = gate;
	gate = P2 & 0X1F;
	P2 = gate;
	
	P0 = Dula[Du];		//段选
	if(Point) P0 &= 0X7F;
	gate = P2 & 0X1F;
	gate |= 0XE0;
	P2 = gate;
	gate = P2 & 0X1F;
	P2 = gate;
}