#include "stc15.h"
#include "buzzer.h"

unsigned int buzzer_tweet_residue_time=0;//鸣叫剩余次数
unsigned char buzzer_tweet_time_cnt=0;//时间计数,用于产生短鸣和长鸣
unsigned char buzzer_tweet_type=0;//鸣叫类型
bit buzzer_IsBusyFlag=0;//蜂鸣器工作标志位
#define BUZZER_SHORT_TWEET 0 //短鸣
#define BUZZER_LONG_TWEET 1 //长鸣

sbit buzzer_pin=P5^5;



bit Buzzer_IsBusy(void)//获取蜂鸣器工作状态
{
	return buzzer_IsBusyFlag;
}

void Buzzer_Init(void)//初始化蜂鸣器
{
	P5M0 |= 0x10;  //设置P5.5为推挽输出模式
  P5M1 |= 0x00;
	buzzer_pin=1;
}


void Buzzer_On(void)//开启蜂鸣器
{
	buzzer_pin=0;
}


void Buzzer_Off(void)//关闭蜂鸣器
{
	buzzer_pin=1;
}


void Buzzer_Short_Tweet(unsigned char time)//蜂鸣器短鸣(time=鸣叫次数)
{
	buzzer_IsBusyFlag=1;//设置蜂鸣器正在工作中
	buzzer_tweet_residue_time=time*2;//设置剩余次数
	buzzer_tweet_type=BUZZER_SHORT_TWEET;//设置鸣叫类型
}


void Buzzer_Long_Tweet(unsigned char time)//蜂鸣器长鸣(time=鸣叫次数)
{
	buzzer_IsBusyFlag=1;//设置蜂鸣器正在工作中
	buzzer_tweet_residue_time=time*2;//设置剩余次数
	buzzer_tweet_type=BUZZER_LONG_TWEET;//设置鸣叫类型
}


void Buzzer_Tweet_HeartBeat_IRQ(void)//蜂鸣器鸣叫系统心跳函数
{
	if(buzzer_tweet_residue_time>0)//如果还有剩余的鸣叫次数
	{
		if(buzzer_tweet_type==BUZZER_SHORT_TWEET)//如果当前是短鸣
		{
			if(buzzer_tweet_time_cnt<1)
			{
				buzzer_tweet_time_cnt++;//增加时间计数
			}
			else
			{
				buzzer_pin=!buzzer_pin;//取反
				buzzer_tweet_time_cnt=0;//清空时间计数
				buzzer_tweet_residue_time--;//减少剩余鸣叫次数
			}
		}
		else//如果当前是长鸣
		{
			if(buzzer_tweet_time_cnt<20)
			{
				buzzer_tweet_time_cnt++;//增加时间计数
			}
			else
			{
				buzzer_pin=!buzzer_pin;//取反
				buzzer_tweet_time_cnt=0;//清空时间计数
				buzzer_tweet_residue_time--;//减少剩余鸣叫次数
			}
		}
	}
	else//如果已经没有鸣叫次数
	{
		buzzer_pin=1;//关闭蜂鸣器
		buzzer_IsBusyFlag=0;//设置蜂鸣器停止工作
	}
	
}