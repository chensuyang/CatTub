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
	TM1620_Init();//初始化数码管驱动芯片
	Button_Init();//初始化按钮
	Buzzer_Init();//初始化蜂鸣器
	PTC_Init();//初始化PTC加热元件
	DS18B20_Convert();//开启温度转换
	Buzzer_Short_Tweet(1);//开机鸣叫  
	HeartBeat_Init();//系统心跳初始化
	 
	while(1)
	{

	}
}