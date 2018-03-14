#include "timer0.h"
#include "stc15.h"
#include "Heartbeat.h"
#define FOSC 20000000L

//#define T10MS (65536-(FOSC/100))      //1Tģʽ
#define T10MS (65536-(FOSC/12/100)) //12Tģʽ

void Timer0_Init(void)//��ʼ����ʱ��0(10msһ���ж�)
{
	//AUXR |= 0x80;                   //��ʱ��0Ϊ1Tģʽ
	AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ

	TMOD = 0x00;                    //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
  TL0 = T10MS;                     //��ʼ����ʱֵ
  TH0 = T10MS >> 8;
  TR0 = 1;                        //��ʱ��0��ʼ��ʱ
  ET0 = 1;                        //ʹ�ܶ�ʱ��0�ж�
  EA = 1;
}


//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
    HeartBeat_IRQ();//ϵͳ�����жϷ�����
}

//-----------------------------------------------
