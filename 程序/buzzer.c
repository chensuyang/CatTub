#include "stc15.h"
#include "buzzer.h"

unsigned int buzzer_tweet_residue_time=0;//����ʣ�����
unsigned char buzzer_tweet_time_cnt=0;//ʱ�����,���ڲ��������ͳ���
unsigned char buzzer_tweet_type=0;//��������
bit buzzer_IsBusyFlag=0;//������������־λ
#define BUZZER_SHORT_TWEET 0 //����
#define BUZZER_LONG_TWEET 1 //����

sbit buzzer_pin=P5^5;



bit Buzzer_IsBusy(void)//��ȡ����������״̬
{
	return buzzer_IsBusyFlag;
}

void Buzzer_Init(void)//��ʼ��������
{
	P5M0 |= 0x10;  //����P5.5Ϊ�������ģʽ
  P5M1 |= 0x00;
	buzzer_pin=1;
}


void Buzzer_On(void)//����������
{
	buzzer_pin=0;
}


void Buzzer_Off(void)//�رշ�����
{
	buzzer_pin=1;
}


void Buzzer_Short_Tweet(unsigned char time)//����������(time=���д���)
{
	buzzer_IsBusyFlag=1;//���÷��������ڹ�����
	buzzer_tweet_residue_time=time*2;//����ʣ�����
	buzzer_tweet_type=BUZZER_SHORT_TWEET;//������������
}


void Buzzer_Long_Tweet(unsigned char time)//����������(time=���д���)
{
	buzzer_IsBusyFlag=1;//���÷��������ڹ�����
	buzzer_tweet_residue_time=time*2;//����ʣ�����
	buzzer_tweet_type=BUZZER_LONG_TWEET;//������������
}


void Buzzer_Tweet_HeartBeat_IRQ(void)//����������ϵͳ��������
{
	if(buzzer_tweet_residue_time>0)//�������ʣ������д���
	{
		if(buzzer_tweet_type==BUZZER_SHORT_TWEET)//�����ǰ�Ƕ���
		{
			if(buzzer_tweet_time_cnt<1)
			{
				buzzer_tweet_time_cnt++;//����ʱ�����
			}
			else
			{
				buzzer_pin=!buzzer_pin;//ȡ��
				buzzer_tweet_time_cnt=0;//���ʱ�����
				buzzer_tweet_residue_time--;//����ʣ�����д���
			}
		}
		else//�����ǰ�ǳ���
		{
			if(buzzer_tweet_time_cnt<20)
			{
				buzzer_tweet_time_cnt++;//����ʱ�����
			}
			else
			{
				buzzer_pin=!buzzer_pin;//ȡ��
				buzzer_tweet_time_cnt=0;//���ʱ�����
				buzzer_tweet_residue_time--;//����ʣ�����д���
			}
		}
	}
	else//����Ѿ�û�����д���
	{
		buzzer_pin=1;//�رշ�����
		buzzer_IsBusyFlag=0;//���÷�����ֹͣ����
	}
	
}