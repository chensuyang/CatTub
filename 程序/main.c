#include "stc15.h"
#include "tm1620.h"
#include "delay.h"
#include "intrins.h"
#include "ds18b20.h"
#include "delay.h"
#include "PTC.h"
#include "buzzer.h"
#include "button.h"
#include "Heartbeat.h"
void main(void)
{
	unsigned char i=0;
	TM1620_Init();//��ʼ�����������оƬ
	Button_Init();//��ʼ����ť
	Buzzer_Init();//��ʼ��������
	PTC_Init();//��ʼ��PTC����Ԫ��
	DS18B20_Convert();//�����¶�ת��
	Buzzer_Short_Tweet(1);//��������  
	HeartBeat_Init();//ϵͳ������ʼ��
	 
	while(1)
	{

	}
}