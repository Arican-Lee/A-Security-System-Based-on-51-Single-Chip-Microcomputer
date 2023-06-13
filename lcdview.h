#ifndef _NOD_H_
#define _NOD_H_

sbit DSPORT=P2^2;

#define uchar unsigned char
#define uint unsigned int
sbit LM1602_EN = P3^4;				   		//1602引脚位变量定义
sbit LM1602_RS = P3^5;
sbit LM1602_RW = P3^6;

void delay1(uint x);
void delays(uint n,uint i);
void write_1602com(uchar com);
void write_1602dat(uchar dat);

void T0init(void);
void INT1init(void);	
void delay(uint y);
void Ds18b20Init();
bit readBit();
uchar Ds18b20ReadByte();
void Ds18b20WriteByte(uchar dat);
void Ds18b20ReadTempCom();
void Ds18b20ChangTemp();
int Ds18b20ReadTemp();
void init();
void writeString(uchar * str, uchar length);
void display(int v);

#endif