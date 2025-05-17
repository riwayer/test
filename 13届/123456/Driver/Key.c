#include "Key.h"

uc Key_Read(void)
{
	uc temp = 0;
	P35 = 0; P34 = 1;
	if (P30 == 0) temp = 15;
	if (P31 == 0) temp = 14;
	if (P32 == 0) temp = 13;
	if (P33 == 0) temp = 12;
	
	P35 = 1; P34 = 0;
	if (P30 == 0) temp = 19;
	if (P31 == 0) temp = 18;
	if (P32 == 0) temp = 17;
	if (P33 == 0) temp = 16;
	return temp;
}