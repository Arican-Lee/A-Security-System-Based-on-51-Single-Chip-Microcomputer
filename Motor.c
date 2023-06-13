#include <reg51.h>
#define uchar unsigned char 
#define uint unsigned int
sbit motor=P2^0;
void delay(uint y);
//***************************MotorOPEN*********************************//
void MotorOPEN()
{
	motor=0;
}
//**************************MotorCLOSE*******************************//
void MotorCLOSE()
{
	motor=1;
}