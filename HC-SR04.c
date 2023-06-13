#include"reg51.h"
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
/**//*****************************LCD*******************************/
sbit LM1602_EN = P3^4;
sbit LM1602_RS = P3^5;
sbit LM1602_RW = P3^6;
//*****************************HC-SR04*******************************/
sbit trig=P1^0;  
sbit echo=P1^1; //
sbit SPK=P2^3;   //*****************************speak******
bit flag1;
uchar count;       
long int distance;      
unsigned char code tables[]={"0123456789"};

//*****************************HCSR04*******************************/
//*************************相当于main函数*************************/
void HCSR04()
{
	void delays(uint n,uint i);
	void delay(uint y) ;
	void comwrite(uchar com);
	void datwrite(uchar dat);
	void lcd_init1();
	void dis();
	void trigger();
	void measure_init();
	void measure();
	void echodisplay(uint x);
	void jingbao();
	void T1_init();

	lcd_init1();
	T1_init();
	measure_init();
	while(1)
	{ 
		dis();
		trigger();
		while(echo==0)
		{
			;
		}
		measure();
		echodisplay(distance);
		measure_init();
		delay(500);
		if(distance != 0)
			jingbao();
		
	}
}
//*****************************HCSR04*******************************/

void delays(uint n,uint i)  
	
{
	uint x,y;
		for(x=n;x>0;x--)
			for(y=110+i;y>0;y--);
}

void comwrite(uchar com)
{
	LM1602_RS=0;
	LM1602_RW=0;
	P0=com;
	delays(5,0);
	LM1602_EN=1;
	LM1602_EN=0;
}

void datwrite(uchar dat)
{
	LM1602_RS=1;
	LM1602_RW=0;
	P0=dat;
	delays(5,0);
	LM1602_EN=1;
	LM1602_EN=0;
}

void lcd_init1()   
{
	comwrite(0x38);
	comwrite(0x0c);
	comwrite(0x06);
	comwrite(0x01);
}

void dis()   //??????
{
	comwrite(0x80+0x40);
	datwrite('d');
	datwrite('i');
	datwrite('s');
	datwrite('t');
	datwrite('a');
	datwrite('n');
	datwrite('c');
	datwrite('e');
	datwrite(':');
	comwrite(0x80+0x4c);
	datwrite('.');
	comwrite(0x80+0x4e);
	datwrite('c');
	datwrite('m');
}

void trigger()

{
	trig=1;
	delays(1,0);
	trig=0;
}

void measure_init()
{
	trig=0;
	echo=0;
	count=0;  
}

void measure()
{
	uchar a;
	uint b,c;
	TR1=1;
	while(echo)
	{
		;
	}
	TR1=0;
	a=TL1;
	b=TH1;
	c=(b<<8)+a;
	c=c-0xfc66;
	distance=c+1000*count;
	TL1=0x66;
	TH1=0xfc;
	delay(30);
	distance=3453*distance/20000;
}

void echodisplay(uint x)
{
	uchar q,b,s,g;
	q=x/1000;
	b=(x/100)%10;
	s=(x/10)%10;
	g=x%10;
	comwrite(0x80+0x49);
	datwrite(tables[q]);
	datwrite(tables[b]);
	datwrite(tables[s]);
	comwrite(0x80+0x4d);
	datwrite(tables[g]);
}
/**//*****************************speak*******************************/
uint j;
uchar k;

unsigned char time1=998;  //报警20秒，time1控制报警时间长度，值越小时间越长，time1>0 
void jingbao()
{
	uchar i=999;
	for(i=999;i>0;i--)
	{
		SPK=0;
		delays(800,15);
		SPK=1;
		delays(800,15);
		SPK=0;
		delays(1200,15);
		SPK=1;
		delays(1300,15);
		if(i<time1)
		{
		lcd_init1();
		REDFEEL();
		}
	}
		
}

void T1_init()    
{
	TMOD=0x10;
	TL1=0x66;
	TH1=0xfc;
	ET1=1;
	EA=1;
}

void T_1() interrupt 3
	
{
	TF1=0;
	TL1=0x66;
	TH1=0xfc;
	count++;
    if(count==18)
	{
		TR1=0;
		TL1=0x66;
		TH1=0xfc;
		count=0;
	}
}
