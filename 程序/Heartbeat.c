#include "Heartbeat.h"
#include "stc15.h"
#include "timer0.h"
#include "tm1620.h"
#include "ds18b20.h"
#include "EEPROM.h"
#include "buzzer.h"
float Set_Temperature=0;//设定温度
bit IsTemperatureSetMode=0;//处于设定温度模式
unsigned int TemperatureSetModeTimeOutCnt=0;//设定温度模式超时计数

void HeartBeat_Init(void)//初始化系统心跳
{
	Timer0_Init();//初始化定时器0
	Set_Temperature=EEPROM_Read_SetTemperature();//读取设定温度值
}



void HeartBeat_IRQ(void)//系统心跳中断
{
	Buzzer_Tweet_HeartBeat_IRQ();//蜂鸣器鸣叫系统心跳处理函数
	DS18B20_HeartBeat_IRQ();//DS18B20系统心跳处理函数
	if(!IsTemperatureSetMode)//如果不是在设定温度模式(日常温度显示状态)
	{
		TM1620_Show_Temperature(DS18B20_Get_New_Temperature());//显示最新实时温度
		TM1620_Write_Digitron_Data(3,0x00);//不点亮温度设置模式LED
		TM1620_display_on(3);//发出显示
	}
	else
	{
		TM1620_Show_Temperature(Set_Temperature);//显示设置温度
		//TM1620_Write_Digitron_Data(3,0x80);//点亮温度设置模式LED
		TM1620_display_on(3);//发出显示
		TemperatureSetModeTimeOutCnt++;//设定温度模式超时计数++
		if(TemperatureSetModeTimeOutCnt>150)//如果设定温度模式超时计数大于1.5秒
		{
			TemperatureSetModeTimeOutCnt=0;//清空计数
			IsTemperatureSetMode=0;//退出设定温度模式
		}
	}
	
}

void TemperatureSetMode_SetUP(void)//设置温度上升一度
{
	IsTemperatureSetMode=1;//设置当前为设置温度模式
	TemperatureSetModeTimeOutCnt=0;//清空设置温度模式的超时计数(重新开始算超时时间)
	if(Set_Temperature<55)Set_Temperature++;
	EEPROM_Write_SetTemperature(Set_Temperature);
}

void TemperatureSetMode_SetDOWN(void)//设置温度下降一度
{
	IsTemperatureSetMode=1;//设置当前为设置温度模式
	TemperatureSetModeTimeOutCnt=0;//清空设置温度模式的超时计数(重新开始算超时时间)
	if(Set_Temperature>10)Set_Temperature--;
	EEPROM_Write_SetTemperature(Set_Temperature);
}

