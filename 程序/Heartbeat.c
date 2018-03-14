#include "Heartbeat.h"
#include "stc15.h"
#include "timer0.h"
#include "tm1620.h"
#include "ds18b20.h"
#include "EEPROM.h"
#include "buzzer.h"
float Set_Temperature=0;//�趨�¶�
bit IsTemperatureSetMode=0;//�����趨�¶�ģʽ
unsigned int TemperatureSetModeTimeOutCnt=0;//�趨�¶�ģʽ��ʱ����

void HeartBeat_Init(void)//��ʼ��ϵͳ����
{
	Timer0_Init();//��ʼ����ʱ��0
	Set_Temperature=EEPROM_Read_SetTemperature();//��ȡ�趨�¶�ֵ
}



void HeartBeat_IRQ(void)//ϵͳ�����ж�
{
	Buzzer_Tweet_HeartBeat_IRQ();//����������ϵͳ����������
	DS18B20_HeartBeat_IRQ();//DS18B20ϵͳ����������
	if(!IsTemperatureSetMode)//����������趨�¶�ģʽ(�ճ��¶���ʾ״̬)
	{
		TM1620_Show_Temperature(DS18B20_Get_New_Temperature());//��ʾ����ʵʱ�¶�
		TM1620_Write_Digitron_Data(3,0x00);//�������¶�����ģʽLED
		TM1620_display_on(3);//������ʾ
	}
	else
	{
		TM1620_Show_Temperature(Set_Temperature);//��ʾ�����¶�
		//TM1620_Write_Digitron_Data(3,0x80);//�����¶�����ģʽLED
		TM1620_display_on(3);//������ʾ
		TemperatureSetModeTimeOutCnt++;//�趨�¶�ģʽ��ʱ����++
		if(TemperatureSetModeTimeOutCnt>150)//����趨�¶�ģʽ��ʱ��������1.5��
		{
			TemperatureSetModeTimeOutCnt=0;//��ռ���
			IsTemperatureSetMode=0;//�˳��趨�¶�ģʽ
		}
	}
	
}

void TemperatureSetMode_SetUP(void)//�����¶�����һ��
{
	IsTemperatureSetMode=1;//���õ�ǰΪ�����¶�ģʽ
	TemperatureSetModeTimeOutCnt=0;//��������¶�ģʽ�ĳ�ʱ����(���¿�ʼ�㳬ʱʱ��)
	if(Set_Temperature<55)Set_Temperature++;
	EEPROM_Write_SetTemperature(Set_Temperature);
}

void TemperatureSetMode_SetDOWN(void)//�����¶��½�һ��
{
	IsTemperatureSetMode=1;//���õ�ǰΪ�����¶�ģʽ
	TemperatureSetModeTimeOutCnt=0;//��������¶�ģʽ�ĳ�ʱ����(���¿�ʼ�㳬ʱʱ��)
	if(Set_Temperature>10)Set_Temperature--;
	EEPROM_Write_SetTemperature(Set_Temperature);
}

