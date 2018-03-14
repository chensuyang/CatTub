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
	
	  CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CMOD = 0x02;                    //����PCAʱ��Դ
                                    //��ֹPCA��ʱ������ж�
    PCA_PWM1 = 0x00;                //PCAģ��0������8λPWM
    CCAP1H = CCAP0L = 0x20;         //PWM0��ռ�ձ�Ϊ87.5% ((100H-20H)/100H)
    CCAPM1 = 0x42;                  //PCAģ��0Ϊ8λPWMģʽ
    CR = 1;                         //PCA��ʱ����ʼ����

}

void PTC_On(float duty)
{
	P1M0 |= 0x02;  //����P1.1Ϊ�������ģʽ
  P1M1 |= 0x00;
	PCA_PWM1 = 0x00;                //PCAģ��1������8λPWM
  CCAP1H = CCAP0L = 0x100-(duty/100*0x100);         //PWM1��ռ�ձ�Ϊ87.5% ((100H-20H)/100H)
  CCAPM1 = 0x42;                  //PCAģ��0Ϊ8λPWMģʽ
}

void PTC_Off(void)
{
	PCA_PWM1 = 0x00;                //PCAģ��1������8λPWM
  CCAP1H = CCAP0L = 0x100-(0/100*0x100);         //PWM1��ռ�ձ�Ϊ87.5% ((100H-20H)/100H)
  CCAPM1 = 0x42;                  //PCAģ��1Ϊ8λPWMģʽ
}