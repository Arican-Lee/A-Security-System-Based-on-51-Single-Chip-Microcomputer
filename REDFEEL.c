#include <reg51.h>    

sbit IR=P3^3;  			//红外接口标志
sbit dula=P2^6;		//定义锁存使能端口 段锁存
sbit wela=P2^7;		//                 位锁存

unsigned char code Table_c[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char  irtime;	//红外用全局变量

bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];

unsigned char code rst[ ]={0xe4,0xc0,0xe0,0xc0,0xe0,0x32};
void Reset(void)
{
}

//*************************************REDFEEL*****************************************//
void REDFEEL()
{
	//*************************
	void LCD();
	void DS18B20();
	void HCSR04();
	void MotorOPEN();
	void MotorCLOSE();
	//*************************
	void Ir_work(void);
	void Ircordpro(void);
	void TIM0init(void);
	void EX0init(void);
	
	EX0init(); //初始化外部中断
	TIM0init();//初始化定时器
	dula=0;      //位锁存
	P0=0xfe; //取位码 第一位数码管选通，即二进制1111 1110
	wela=1;      //位锁存
	wela=0;
	P0=0x3f; //取位码 第一位数码管选通，即二进制1111 1110
	dula=1;      //位锁存
	
	IRcord[2]=0;
	while(1)//主循环
	{

		if(irok)                 //如果接收好了进行红外处理
		{   
			Ircordpro();
			irok=0;
		}
		if(irpro_ok) //如果处理好后进行工作处理，如按对应的按键后显示对应的数字等
		{
			Ir_work();
		}
	}
}
//********************************REDFEEL**********************************//

void Ir_work(void)//红外键值散转程序
{
       switch(IRcord[2])//判断第三个数码值
	         {
			 case 0x0c:HCSR04();break;//1 显示相应的按键值
			 case 0x18:LCD();break;//2
			 case 0x5e:;break;//3可自己自定义功能
			 case 0x08:MotorOPEN();break;//4
			 case 0x1c:MotorCLOSE();break;//5
			 case 0x5a:;break;//6可自己自定义功能
			 case 0x42:DS18B20();break;//7
			 case 0x52:;break;//8可自己自定义功能
			 case 0x4a:(*((void (*)(void))(rst)))();break;//9
             default:break;
			 }

		  irpro_ok=0;//处理完成标志

}

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;                      //用于计数2个下降沿之间的时间
}

void EX0_ISR (void) interrupt 2 //外部中断0服务函数
{
	static unsigned char  i;             //接收红外信号处理
	static bit startflag=0;                //是否开始处理标志位

	if(startflag)                         
	{
		if(irtime<63&&irtime>=33)//引导码 TC9012的头码，9ms+4.5ms
            i=0;
    		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
    		irtime=0;
    		i++;
   			if(i==33)
      		{
	  			irok=1;
				i=0;
	  		}
	}
	else
	{
		irtime=0;
		startflag=1;
	}
}


void Ircordpro(void)//    红外码值处理函数
{ 
	unsigned char i, j, k,cord,value;
	k=1;
	for(i=0;i<4;i++)      //处理4个字节
	{
		for(j=1;j<=8;j++) //处理1个字节8位
		{
			cord=irdata[k];
			if(cord>7)
				//大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
			value=value|0x80;
			if(j<8)
			{
				value>>=1;
			}
			k++;
		}
		IRcord[i]=value;
		value=0;     
	} 
	irpro_ok=1;//处理完毕标志位置1
}


void TIM0init(void)//定时器0初始化
{
	TMOD=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
	TH0=0x00; //重载值
	TL0=0x00; //初始化值
	ET0=1;    //开中断
	TR0=1;    
}
void EX0init(void)
{
 IT1 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX1 = 1;   //使能外部中断
 EA = 1;    //开总中断
}
