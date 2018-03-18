#include "delay.h"
#include "intrins.h"

void Delay1us()		//@20.000MHz
{

	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();

}

void Delay5us()		//@20.000MHz
{
	unsigned int i;

	for(i=0;i<1;i++)Delay1us();
}

void Delay12us()		//@20.000MHz
{
	unsigned int i;

	for(i=0;i<4;i++)Delay1us();
}

void Delay50us()		//@20.000MHz
{
	unsigned int i;

	for(i=0;i<20;i++)Delay1us();
}

void Delay64us()		//@20.000MHz
{
	unsigned int i;

	for(i=0;i<25;i++)Delay1us();
}


void Delay70us()		//@20.000MHz
{
	unsigned int i;

	for(i=0;i<29;i++)Delay1us();
}

void Delay350us()		//@20.000MHz
{
	unsigned int i;

	for(i=0;i<141;i++)Delay1us();
}


void Delay720us()		//@20.000MHz
{
	unsigned int i;

	for(i=0;i<300;i++)Delay1us();
}


void Delay(unsigned int time)//ÑÓÊ±nÎ¢Ãë
{
	unsigned int i;
	for(i=0;i<time;i++)Delay1us();
}

void Delay1ms()		//@20.000MHz
{
	unsigned char i, j;

	i = 20;
	j = 113;
	do
	{
		while (--j);
	} while (--i);
}


void Delay_ms(unsigned int time)//ÑÓÊ±nºÁÃë
{
	unsigned int i;
	for(i=0;i<time;i++)Delay1ms();
}


