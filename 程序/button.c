#include "button.h"
#include "stc15.h"
#include "buzzer.h"
#include "Heartbeat.h"
void Button_Init(void)//初始化按键
{
	INT_CLKO |= 0x20;               //(EX3 = 1)使能INT3中断
	INT_CLKO |= 0x10;               //(EX2 = 1)使能INT2中断
  EA = 1;
}






//-----------------------------------------------
//中断服务程序
void exint2() interrupt 10          //INT2中断入口
{
	EA=0;
	if(!Buzzer_IsBusy())Buzzer_Short_Tweet(1);//开机鸣叫  
	TemperatureSetMode_SetUP();//设置温度上升一度
//  INT_CLKO &= 0xEF;               //若需要手动清除中断标志,可先关闭中断,此时系统会自动清除内部的中断标志
//  INT_CLKO |= 0x10;               //然后再开中断即可
	EA=1;
}




//-----------------------------------------------
//中断服务程序
void exint3() interrupt 11          //INT3中断入口
{
	EA=0;
	if(!Buzzer_IsBusy())Buzzer_Short_Tweet(1);//开机鸣叫  
	TemperatureSetMode_SetDOWN();//设置温度下降一度
//  INT_CLKO &= 0xDF;               //若需要手动清除中断标志,可先关闭中断,此时系统会自动清除内部的中断标志
//  INT_CLKO |= 0x20;               //然后再开中断即可
	EA=1;
}
