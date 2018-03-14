#include "button.h"
#include "stc15.h"
#include "buzzer.h"
#include "Heartbeat.h"
void Button_Init(void)//��ʼ������
{
	INT_CLKO |= 0x20;               //(EX3 = 1)ʹ��INT3�ж�
	INT_CLKO |= 0x10;               //(EX2 = 1)ʹ��INT2�ж�
  EA = 1;
}






//-----------------------------------------------
//�жϷ������
void exint2() interrupt 10          //INT2�ж����
{
	EA=0;
	if(!Buzzer_IsBusy())Buzzer_Short_Tweet(1);//��������  
	TemperatureSetMode_SetUP();//�����¶�����һ��
//  INT_CLKO &= 0xEF;               //����Ҫ�ֶ�����жϱ�־,���ȹر��ж�,��ʱϵͳ���Զ�����ڲ����жϱ�־
//  INT_CLKO |= 0x10;               //Ȼ���ٿ��жϼ���
	EA=1;
}




//-----------------------------------------------
//�жϷ������
void exint3() interrupt 11          //INT3�ж����
{
	EA=0;
	if(!Buzzer_IsBusy())Buzzer_Short_Tweet(1);//��������  
	TemperatureSetMode_SetDOWN();//�����¶��½�һ��
//  INT_CLKO &= 0xDF;               //����Ҫ�ֶ�����жϱ�־,���ȹر��ж�,��ʱϵͳ���Զ�����ڲ����жϱ�־
//  INT_CLKO |= 0x20;               //Ȼ���ٿ��жϼ���
	EA=1;
}
