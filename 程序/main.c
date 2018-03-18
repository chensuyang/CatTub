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
#include "EEPROM.h"
sbit ptc_pin=P1^1;
unsigned int while_cnt=0;
bit wenduzuanhuan_flag=0;
void main(void)
{
	unsigned char i=0;
	PTC_Init();//��ʼ��PTC����Ԫ��
	TM1620_Init();//��ʼ�����������оƬ
	Button_Init();//��ʼ����ť
	Buzzer_Init();//��ʼ��������
	PTC_Init();//��ʼ��PTC����Ԫ��
	DS18B20_Convert();//�����¶�ת��
	Buzzer_Short_Tweet(1);//��������  
	//HeartBeat_Init();//ϵͳ������ʼ��
	Set_Temperature=EEPROM_Read_SetTemperature();//��ȡ�趨�¶�ֵ
	while(1)
	{
		Buzzer_Tweet_HeartBeat_IRQ();//����������ϵͳ����������
		
		
		if(while_cnt<10)
		{
			while_cnt++;
		}
		else
		{
			while_cnt=0;
			
			if(!wenduzuanhuan_flag)
			{
				DS18B20_Convert();//�����¶�ת��,�ȴ���һ�ζ�ȡ
				wenduzuanhuan_flag=!wenduzuanhuan_flag;
			}	
			else
			{
				DS18B20_ReadFlash();//��ȡ�¶�
				wenduzuanhuan_flag=!wenduzuanhuan_flag;
			}
			
			
			if(!IsTemperatureSetMode)//����������趨�¶�ģʽ(�ճ��¶���ʾ״̬)
			{
				TM1620_Show_Temperature(DS18B20_Get_Temperature());//��ʾ����ʵʱ�¶�
				TM1620_Write_Digitron_Data(3,0x00);//�������¶�����ģʽLED
				TM1620_display_on(1);//������ʾ
				
				
				Temperature_Control();//�¶ȿ���
			}
			else
			{
				TM1620_Show_Temperature(Set_Temperature);//��ʾ�����¶�
				TM1620_Write_Digitron_Data(3,0x80);//�����¶�����ģʽLED
				TM1620_display_on(1);//������ʾ
				if(IsTemperatureSetMode)TemperatureSetModeTimeOutCnt++;//�趨�¶�ģʽ��ʱ����++
				if(IsTemperatureSetMode && TemperatureSetModeTimeOutCnt>7)//����趨�¶�ģʽ��ʱ��������1.5��
				{
					TemperatureSetModeTimeOutCnt=0;//��ռ���
					IsTemperatureSetMode=0;//�˳��趨�¶�ģʽ
					EEPROM_Write_SetTemperature(Set_Temperature);
				}
			}
		}
		Delay_ms(10);
			
	}
}