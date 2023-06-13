#include <reg51.h>
#include <intrins.H>
#include <math.H>
#include "lcdview.h"

//关中断
char num=0;
void INT1init(void)
{
 EA = 0;    //中断
}
	
//*****************************DS18B20***********************************/
void DS18B20()
{

	uchar table[] = {" TEMPERATURE:    "};
	
	INT1init(); //关中断

  Ds18b20ChangTemp();
  init();
  write_1602com(0x80);
  writeString(table, 16);		//table where
  while(1)
  {
    delay(1000); //温度转换时间需要750ms以上
    write_1602com(0xc0);							
    display(Ds18b20ReadTemp());
    Ds18b20ChangTemp();
	
		REDFEEL();
  }
}
 //*****************************DS18B20***********************************/

void delay(uint y)             			//延时函数
{
	uint x;
	for(;y>0;y--)
	{
		for(x=110;x>0;x--);
	}
}

void Ds18b20Init()        //初始化,成功返1失败返0
{
		uint i;  
    DSPORT = 0;
    i = 100;  
     while(i>0) i--;
    DSPORT = 1;   
    i = 4;
     while(i>0) i--;
}

bit readBit()				//√
{
    uint i;
    bit b;
    DSPORT = 0;
    i++;   
    DSPORT = 1; 
   i++; i++;  
    b = DSPORT;
    i = 8; 
    while(i>0) i--;
    return b;
}

uchar Ds18b20ReadByte()        //读取一个字节
{
	uint i;
  uchar j, dat;
   dat = 0;
    for(i=0; i<8; i++)
    {
        j = readBit();
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}

void Ds18b20WriteByte(uchar dat)        //向DS18B20写一个字节
{
	uint i;
  uchar j;
    bit b;
    for(j = 0; j < 8; j++)
    {
        b = dat & 0x01;
        dat >>= 1;
    
        if(b)   
        {
           DSPORT = 0;          i++; i++;  
            DSPORT = 1;    
            i = 8; while(i>0) i--;  
        }
        else  
        {
            DSPORT = 0;
          i = 8; while(i>0) i--;  
            DSPORT = 1;
           i++; i++;
        }
   }
}

void  Ds18b20ReadTempCom()        //√发送读取温度指令
{	

	Ds18b20Init();
	delay(1);
	Ds18b20WriteByte(0xcc);	 //跳过ROM操作命令
	Ds18b20WriteByte(0xbe);	 //发送读取温度命令
}

void  Ds18b20ChangTemp()        //√让DS18B20开始转换温度
{
	Ds18b20Init();
	delay(1);
	Ds18b20WriteByte(0xcc);		//跳过ROM操作命令		 
	Ds18b20WriteByte(0x44);	    //温度转换命令
	//delay(100);	//等待转换成功，而如果你是一直刷着的话，就不用这个延时了
}

int Ds18b20ReadTemp()        //读取温度指令,并返回温度值
{
		uint tmpvalue;
    int value; 
    float t;
    uchar low, high;
    Ds18b20ReadTempCom();
    
    low = Ds18b20ReadByte(); 
    high = Ds18b20ReadByte();
   
    tmpvalue = high;
    tmpvalue <<= 8;
    tmpvalue |= low;
    value = tmpvalue;
    
  
    t = value * 0.0625;
    
    value = t * 100 + (value > 0 ? 0.5 : -0.5); //大于0加0.5, 小于0减0.5
    return value;
}

//*****************************LCD1602*******************************//

void init(){   			                              //写指令
	write_1602com(0x01); 		
	write_1602com(0x38); 		
	write_1602com(0x0f); 		
	write_1602com(0x06); 		
}
void writeString(uchar * str, uchar length)
{
     uchar i;
    for(i = 0; i < length; i++)
    {
         write_1602dat(str[i]);
     }
 }
//*****************************display*******************************/
 void display(int v) 
{
    uchar count;
    uchar datas[] = {0, 0, 0, 0, 0};
    uint tmp = abs(v);
    datas[0] = tmp / 10000;
    datas[1] = tmp % 10000 / 1000;
    datas[2] = tmp % 1000 / 100;
    datas[3] = tmp % 100 / 10;
    datas[4] = tmp % 10;
    write_1602com(0xc0+3);
    if(v < 0)
    {
        writeString("- ", 2);
   }
    else
    {
       writeString("+ ", 2);
    }
    if(datas[0] != 0)
    {
        write_1602dat('0'+datas[0]);
    }
    for(count = 1; count != 5; count++)
    {
       write_1602dat('0'+datas[count]);
        if(count == 2)
        {
            write_1602dat('.');
        }
    }
}

void delay1(uint x)
{             			//延时函数
	uint i;
	for(i=x;i>0;i--);
}

void write_1602com(uchar com)
{ 	 	  												//LM1602写指令函数
	P0 = com;	   							//送出指令
	LM1602_RS=0; LM1602_RW=0; LM1602_EN=1;	//写指令时序
	delay1(150);
	LM1602_EN=0; 
}
 
void write_1602dat(uchar dat)
{ 														//LM1602读数据函数
	P0 = dat;								//送出数据
	LM1602_RS=1; LM1602_RW=0; LM1602_EN=1;	//写数据时序
	delay1(150);
	LM1602_EN=0;
}
