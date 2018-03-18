#include "Heartbeat.h"
#include "stc15.h"
#include "timer0.h"
#include "tm1620.h"
#include "ds18b20.h"
#include "EEPROM.h"
#include "buzzer.h"
#include "math.h"
#include "PTC.h"
float Set_Temperature=0;//设定温度
bit IsTemperatureSetMode=0;//处于设定温度模式
unsigned int TemperatureSetModeTimeOutCnt=0;//设定温度模式超时计数

float PTC_Power=0;//PTC功率


int current_error=0; //当前差值
int last_error=0; //上次差值
int prev_error=0; //上上次差值

float PID_kp =40;//PID的比例系数
float PID_ki =80;//PID的积分系数
float PID_kd =50;//PID的微分系数
double PID_P =0;//PID的比例计算结果
double PID_I =0;//PID的积分计算结果
double PID_D =0;//PID的微分计算结果
double PID_PWM =0;//PID的PWM输出数值

void HeartBeat_Init(void)//初始化系统心跳
{
	Timer0_Init();//初始化定时器0
	Set_Temperature=EEPROM_Read_SetTemperature();//读取设定温度值
}



void HeartBeat_IRQ(void)//系统心跳中断
{
	/*Buzzer_Tweet_HeartBeat_IRQ();//蜂鸣器鸣叫系统心跳处理函数
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
	}*/
	
}

void TemperatureSetMode_SetUP(void)//设置温度上升一度
{
	IsTemperatureSetMode=1;//设置当前为设置温度模式
	TemperatureSetModeTimeOutCnt=0;//清空设置温度模式的超时计数(重新开始算超时时间)
	if(Set_Temperature<55)Set_Temperature++;
}

void TemperatureSetMode_SetDOWN(void)//设置温度下降一度
{
	IsTemperatureSetMode=1;//设置当前为设置温度模式
	TemperatureSetModeTimeOutCnt=0;//清空设置温度模式的超时计数(重新开始算超时时间)
	if(Set_Temperature>10)Set_Temperature--;
}



void Temperature_Control(void)//温度控制
{
	if(!IsTemperatureSetMode)//如果现在不处于温度控制模式
	{
		prev_error = last_error;//更新每次的差值
		last_error = current_error;//更新每次的差值
		current_error = fabs(Set_Temperature) - fabs(DS18B20_Get_Temperature());//更新每次的差值
		PID_P = PID_kp * (current_error - last_error); //比例 P 输出公式
		PID_I = PID_ki * current_error; //积分 I 输出公式
		if (PID_I > PTC_CONTROL_PID_I_MAX)PID_I = PTC_CONTROL_PID_I_MAX;//对积分上限进行限制
		if (PID_I < PTC_CONTROL_PID_I_MIN)PID_I = PTC_CONTROL_PID_I_MIN;//对积分下限进行限制
		PID_D = PID_kd * (current_error - 2 * last_error + prev_error); //微分 D 输出公式
		PID_PWM=PID_P+PID_I+PID_D;
		if (PID_PWM<0 || current_error<0)
		{
			PID_PWM = fabs(PID_PWM);//保证PWM永远为正值
			PTC_Off();//关闭PTC
		}
		else
		{
			if (PID_PWM > PTC_PWM_MAX)PID_PWM = PTC_PWM_MAX; //限制PTC的最大功率
			if (PID_PWM < PTC_PWM_MIN)PID_PWM = PTC_PWM_MIN; //限制PTC的最小功率
			PTC_On(PID_PWM);//提交数据到PTC
		}
	}
}


