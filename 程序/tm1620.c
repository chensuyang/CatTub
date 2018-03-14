#include "stc15.h"
#include "intrins.h"
#include "tm1620.h"
#include "math.h"

sbit TM1620_CLK=P1^5;
sbit TM1620_STB=P1^4;
sbit TM1620_DIN=P1^3;

const unsigned char digitron_table[18]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00,0x40};//�����0-10 A-F

void Delay2us()		//@20.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 7;
	while (--i);
}

void TM1620_Write_Byte(unsigned char byte_data)//��TM1620��д1�ֽ�,���漰STB���ſ���
{
	unsigned char i;
	TM1620_CLK=0;
	Delay2us();
	for(i=0;i<8;i++)
	{
		TM1620_DIN=byte_data&(0x01<<i);
		_nop_();
		_nop_();
		TM1620_CLK=1;
		_nop_();
		_nop_();
		TM1620_CLK=0;
	} 
}

void TM1620_Write_Command_Only(unsigned char command_data)//��TM1620��д����,��������,�漰STB���ſ���
{
	TM1620_STB=0;
	TM1620_Write_Byte(command_data);
	Delay2us();
	TM1620_STB=1;
	Delay2us();
}

void TM1620_Write_Command_And_Data(unsigned char command_data,unsigned char _data)//��TM1620��д�����Լ�һ�ֽ�����,�漰STB���ſ���
{
	TM1620_STB=0;
	TM1620_Write_Byte(command_data);
	Delay2us();
	TM1620_Write_Byte(_data);
	Delay2us();
	TM1620_STB=1;
	Delay2us();
}

void TM1620_Init(void)//��ʼ������TM1620
{
	TM1620_Write_Command_Only(0x02);//������ʾģʽΪ6λ8��
	TM1620_Write_Command_Only(0x44);//������������Ϊд���ݵ���ʾ�Ĵ���,���Ҵ�����ù̶���ַģʽ
	TM1620_Write_Number(0,16,0);
	TM1620_Write_Number(1,16,0);
	TM1620_Write_Number(2,16,0);
	TM1620_Write_Number(3,16,0);
	
}

void TM1620_Write_Digitron_Data(unsigned char pos,unsigned char digitron_data)//��TM1620�Դ���ָ�������λд�����������
{
	TM1620_Write_Command_And_Data((0xC0+(pos*2)),digitron_data);
}

void TM1620_display_on(unsigned char light_intensity)//TM1620����ʾ
{
	TM1620_Write_Command_Only(0x80+0x08+light_intensity);
}
void TM1620_display_off(void)//TM1620����ʾ
{
	TM1620_Write_Command_Only(0x80);
}

void TM1620_Write_Number(unsigned char pos,unsigned number,bit point)//��TM1620�Դ���ָ�������λд����
{
	if(point)TM1620_Write_Digitron_Data(pos,digitron_table[number]|0x80);
	else TM1620_Write_Digitron_Data(pos,digitron_table[number]);
}
	
void TM1620_Show_Temperature(float temperature)//TM1620��ʾ�¶�
{
	unsigned char tmp[3]={0};
	if(temperature<-0.009)//����¶��Ǹ���
	{
		tmp[0]=digitron_table[17];
		if(fabs(temperature)<10)
		{
			tmp[1]=digitron_table[(unsigned int)fabs(temperature)%10]|0x80;
			tmp[2]=digitron_table[(unsigned int)fabs(temperature*10)%10];
		}
		else
		{
			tmp[1]=digitron_table[(unsigned int)fabs(temperature)/10];
			tmp[2]=digitron_table[(unsigned int)fabs(temperature)%10];
		}
	}
	else
	{
		if(fabs(temperature)<10)
		{
			tmp[0]=digitron_table[(unsigned int)fabs(temperature)%10]|0x80;
			tmp[1]=digitron_table[(unsigned int)fabs(temperature*10)%10];
			tmp[2]=digitron_table[(unsigned int)fabs(temperature*100)%10];
		}
		else
		{
			tmp[0]=digitron_table[(unsigned int)fabs(temperature)/10];
			tmp[1]=digitron_table[(unsigned int)fabs(temperature)%10]|0x80;
			tmp[2]=digitron_table[(unsigned int)fabs(temperature*10)%10];
		}
	}
	TM1620_Write_Digitron_Data(0,tmp[0]);
	TM1620_Write_Digitron_Data(1,tmp[1]);
	TM1620_Write_Digitron_Data(2,tmp[2]);

}

