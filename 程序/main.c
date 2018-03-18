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
	PTC_Init();//初始化PTC加热元件
	TM1620_Init();//初始化数码管驱动芯片
	Button_Init();//初始化按钮
	Buzzer_Init();//初始化蜂鸣器
	PTC_Init();//初始化PTC加热元件
	DS18B20_Convert();//开启温度转换
	Buzzer_Short_Tweet(1);//开机鸣叫  
	//HeartBeat_Init();//系统心跳初始化
	Set_Temperature=EEPROM_Read_SetTemperature();//读取设定温度值
	while(1)
	{
		Buzzer_Tweet_HeartBeat_IRQ();//蜂鸣器鸣叫系统心跳处理函数
		
		
		if(while_cnt<10)
		{
			while_cnt++;
		}
		else
		{
			while_cnt=0;
			
			if(!wenduzuanhuan_flag)
			{
				DS18B20_Convert();//开启温度转换,等待下一次读取
				wenduzuanhuan_flag=!wenduzuanhuan_flag;
			}	
			else
			{
				DS18B20_ReadFlash();//读取温度
				wenduzuanhuan_flag=!wenduzuanhuan_flag;
			}
			
			
			if(!IsTemperatureSetMode)//如果不是在设定温度模式(日常温度显示状态)
			{
				TM1620_Show_Temperature(DS18B20_Get_Temperature());//显示最新实时温度
				TM1620_Write_Digitron_Data(3,0x00);//不点亮温度设置模式LED
				TM1620_display_on(1);//发出显示
				
				
				Temperature_Control();//温度控制
			}
			else
			{
				TM1620_Show_Temperature(Set_Temperature);//显示设置温度
				TM1620_Write_Digitron_Data(3,0x80);//点亮温度设置模式LED
				TM1620_display_on(1);//发出显示
				if(IsTemperatureSetMode)TemperatureSetModeTimeOutCnt++;//设定温度模式超时计数++
				if(IsTemperatureSetMode && TemperatureSetModeTimeOutCnt>7)//如果设定温度模式超时计数大于1.5秒
				{
					TemperatureSetModeTimeOutCnt=0;//清空计数
					IsTemperatureSetMode=0;//退出设定温度模式
					EEPROM_Write_SetTemperature(Set_Temperature);
				}
			}
		}
		Delay_ms(10);
			
	}
}