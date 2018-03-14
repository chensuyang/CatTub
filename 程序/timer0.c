#include "timer0.h"
#include "stc15.h"
#include "Heartbeat.h"
#define FOSC 20000000L

//#define T10MS (65536-(FOSC/100))      //1T模式
#define T10MS (65536-(FOSC/12/100)) //12T模式

void Timer0_Init(void)//初始化定时器0(10ms一次中断)
{
	//AUXR |= 0x80;                   //定时器0为1T模式
	AUXR &= 0x7f;                   //定时器0为12T模式

	TMOD = 0x00;                    //设置定时器为模式0(16位自动重装载)
  TL0 = T10MS;                     //初始化计时值
  TH0 = T10MS >> 8;
  TR0 = 1;                        //定时器0开始计时
  ET0 = 1;                        //使能定时器0中断
  EA = 1;
}


//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
    HeartBeat_IRQ();//系统心跳中断服务函数
}

//-----------------------------------------------
