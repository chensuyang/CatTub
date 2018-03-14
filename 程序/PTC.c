#include "PTC.h"
#include "stc15.h"
sbit ptc_pin=P1^1;

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

void PTC_Init(void)
{
	ACC = P_SW1;
	ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
	P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
	
	  CCON = 0;                       //初始化PCA控制寄存器
                                    //PCA定时器停止
                                    //清除CF标志
                                    //清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CMOD = 0x02;                    //设置PCA时钟源
                                    //禁止PCA定时器溢出中断
    PCA_PWM1 = 0x00;                //PCA模块0工作于8位PWM
    CCAP1H = CCAP0L = 0x20;         //PWM0的占空比为87.5% ((100H-20H)/100H)
    CCAPM1 = 0x42;                  //PCA模块0为8位PWM模式
    CR = 1;                         //PCA定时器开始工作

}

void PTC_On(float duty)
{
	P1M0 |= 0x02;  //设置P1.1为推挽输出模式
  P1M1 |= 0x00;
	PCA_PWM1 = 0x00;                //PCA模块1工作于8位PWM
  CCAP1H = CCAP0L = 0x100-(duty/100*0x100);         //PWM1的占空比为87.5% ((100H-20H)/100H)
  CCAPM1 = 0x42;                  //PCA模块0为8位PWM模式
}

void PTC_Off(void)
{
	PCA_PWM1 = 0x00;                //PCA模块1工作于8位PWM
  CCAP1H = CCAP0L = 0x100-(0/100*0x100);         //PWM1的占空比为87.5% ((100H-20H)/100H)
  CCAPM1 = 0x42;                  //PCA模块1为8位PWM模式
}