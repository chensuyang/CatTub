#include "Heartbeat.h"
#include "stc15.h"
#include "timer0.h"
#include "tm1620.h"
#include "ds18b20.h"
#include "EEPROM.h"
#include "buzzer.h"
#include "math.h"
#include "PTC.h"
float Set_Temperature=0;//�趨�¶�
bit IsTemperatureSetMode=0;//�����趨�¶�ģʽ
unsigned int TemperatureSetModeTimeOutCnt=0;//�趨�¶�ģʽ��ʱ����

float PTC_Power=0;//PTC����


int current_error=0; //��ǰ��ֵ
int last_error=0; //�ϴβ�ֵ
int prev_error=0; //���ϴβ�ֵ

float PID_kp =40;//PID�ı���ϵ��
float PID_ki =80;//PID�Ļ���ϵ��
float PID_kd =50;//PID��΢��ϵ��
double PID_P =0;//PID�ı���������
double PID_I =0;//PID�Ļ��ּ�����
double PID_D =0;//PID��΢�ּ�����
double PID_PWM =0;//PID��PWM�����ֵ

void HeartBeat_Init(void)//��ʼ��ϵͳ����
{
	Timer0_Init();//��ʼ����ʱ��0
	Set_Temperature=EEPROM_Read_SetTemperature();//��ȡ�趨�¶�ֵ
}



void HeartBeat_IRQ(void)//ϵͳ�����ж�
{
	/*Buzzer_Tweet_HeartBeat_IRQ();//����������ϵͳ����������
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
	}*/
	
}

void TemperatureSetMode_SetUP(void)//�����¶�����һ��
{
	IsTemperatureSetMode=1;//���õ�ǰΪ�����¶�ģʽ
	TemperatureSetModeTimeOutCnt=0;//��������¶�ģʽ�ĳ�ʱ����(���¿�ʼ�㳬ʱʱ��)
	if(Set_Temperature<55)Set_Temperature++;
}

void TemperatureSetMode_SetDOWN(void)//�����¶��½�һ��
{
	IsTemperatureSetMode=1;//���õ�ǰΪ�����¶�ģʽ
	TemperatureSetModeTimeOutCnt=0;//��������¶�ģʽ�ĳ�ʱ����(���¿�ʼ�㳬ʱʱ��)
	if(Set_Temperature>10)Set_Temperature--;
}



void Temperature_Control(void)//�¶ȿ���
{
	if(!IsTemperatureSetMode)//������ڲ������¶ȿ���ģʽ
	{
		prev_error = last_error;//����ÿ�εĲ�ֵ
		last_error = current_error;//����ÿ�εĲ�ֵ
		current_error = fabs(Set_Temperature) - fabs(DS18B20_Get_Temperature());//����ÿ�εĲ�ֵ
		PID_P = PID_kp * (current_error - last_error); //���� P �����ʽ
		PID_I = PID_ki * current_error; //���� I �����ʽ
		if (PID_I > PTC_CONTROL_PID_I_MAX)PID_I = PTC_CONTROL_PID_I_MAX;//�Ի������޽�������
		if (PID_I < PTC_CONTROL_PID_I_MIN)PID_I = PTC_CONTROL_PID_I_MIN;//�Ի������޽�������
		PID_D = PID_kd * (current_error - 2 * last_error + prev_error); //΢�� D �����ʽ
		PID_PWM=PID_P+PID_I+PID_D;
		if (PID_PWM<0 || current_error<0)
		{
			PID_PWM = fabs(PID_PWM);//��֤PWM��ԶΪ��ֵ
			PTC_Off();//�ر�PTC
		}
		else
		{
			if (PID_PWM > PTC_PWM_MAX)PID_PWM = PTC_PWM_MAX; //����PTC�������
			if (PID_PWM < PTC_PWM_MIN)PID_PWM = PTC_PWM_MIN; //����PTC����С����
			PTC_On(PID_PWM);//�ύ���ݵ�PTC
		}
	}
}


