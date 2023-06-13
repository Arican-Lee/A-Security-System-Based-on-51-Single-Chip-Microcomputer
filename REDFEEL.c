#include <reg51.h>    

sbit IR=P3^3;  			//����ӿڱ�־
sbit dula=P2^6;		//��������ʹ�ܶ˿� ������
sbit wela=P2^7;		//                 λ����

unsigned char code Table_c[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char  irtime;	//������ȫ�ֱ���

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
	
	EX0init(); //��ʼ���ⲿ�ж�
	TIM0init();//��ʼ����ʱ��
	dula=0;      //λ����
	P0=0xfe; //ȡλ�� ��һλ�����ѡͨ����������1111 1110
	wela=1;      //λ����
	wela=0;
	P0=0x3f; //ȡλ�� ��һλ�����ѡͨ����������1111 1110
	dula=1;      //λ����
	
	IRcord[2]=0;
	while(1)//��ѭ��
	{

		if(irok)                 //������պ��˽��к��⴦��
		{   
			Ircordpro();
			irok=0;
		}
		if(irpro_ok) //�������ú���й��������簴��Ӧ�İ�������ʾ��Ӧ�����ֵ�
		{
			Ir_work();
		}
	}
}
//********************************REDFEEL**********************************//

void Ir_work(void)//�����ֵɢת����
{
       switch(IRcord[2])//�жϵ���������ֵ
	         {
			 case 0x0c:HCSR04();break;//1 ��ʾ��Ӧ�İ���ֵ
			 case 0x18:LCD();break;//2
			 case 0x5e:;break;//3���Լ��Զ��幦��
			 case 0x08:MotorOPEN();break;//4
			 case 0x1c:MotorCLOSE();break;//5
			 case 0x5a:;break;//6���Լ��Զ��幦��
			 case 0x42:DS18B20();break;//7
			 case 0x52:;break;//8���Լ��Զ��幦��
			 case 0x4a:(*((void (*)(void))(rst)))();break;//9
             default:break;
			 }

		  irpro_ok=0;//������ɱ�־

}

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;                      //���ڼ���2���½���֮���ʱ��
}

void EX0_ISR (void) interrupt 2 //�ⲿ�ж�0������
{
	static unsigned char  i;             //���պ����źŴ���
	static bit startflag=0;                //�Ƿ�ʼ�����־λ

	if(startflag)                         
	{
		if(irtime<63&&irtime>=33)//������ TC9012��ͷ�룬9ms+4.5ms
            i=0;
    		irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
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


void Ircordpro(void)//    ������ֵ������
{ 
	unsigned char i, j, k,cord,value;
	k=1;
	for(i=0;i<4;i++)      //����4���ֽ�
	{
		for(j=1;j<=8;j++) //����1���ֽ�8λ
		{
			cord=irdata[k];
			if(cord>7)
				//����ĳֵΪ1������;����о��Թ�ϵ������ʹ��12M���㣬��ֵ������һ�����
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
	irpro_ok=1;//������ϱ�־λ��1
}


void TIM0init(void)//��ʱ��0��ʼ��
{
	TMOD=0x02;//��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
	TH0=0x00; //����ֵ
	TL0=0x00; //��ʼ��ֵ
	ET0=1;    //���ж�
	TR0=1;    
}
void EX0init(void)
{
 IT1 = 1;   //ָ���ⲿ�ж�0�½��ش�����INT0 (P3.2)
 EX1 = 1;   //ʹ���ⲿ�ж�
 EA = 1;    //�����ж�
}
