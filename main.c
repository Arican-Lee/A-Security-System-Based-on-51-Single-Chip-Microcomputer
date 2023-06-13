#include <reg51.h> 
#include "lcdview.h"

void main()
{
	uchar welcome[] = {" WELCOME TO USE "};
	uchar name[] = {"MADE BY: zzh lcs "};
	init();
	write_1602com(0x80);
	writeString(welcome, 15);
	write_1602com(0xC0);
	writeString(name, 18);
	
	while(1)
	{
		
		REDFEEL();
	}
}