#include "ds18b20.h"
#include "stc15.h"
#include "delay.h"
float New_Temperature=0;//当前温度
sbit ds18b20=P1^2;
unsigned int HeartBeat_Cnt=0;//系统心跳计数(用于分频)
bit IsDS18B20Converting=0;//DS18B20是否忙碌(是否正在转换状态)

float temp=0;
bit no;


void DS18B20_Reset()//复位DS18B20
{
    ds18b20=0;     //拉低DQ总线开始复位	
    Delay720us();     //保持DQ低大约720uS	    
    ds18b20=1;     //释放DQ位    
}

bit DS18B20_WaitPresence()//等待芯片复位应答信号
{
	bit k;
	Delay50us();
	if(ds18b20==0)k=1;
	else k=0;
  Delay350us();
	return k;
    
}

bit DS18B20_ReadBit()//读一位数据
{
    bit dat;    
    ds18b20=0; 
		Delay1us();//延时1微秒
    ds18b20=1; 
    Delay12us();//延时12微秒
    dat=ds18b20; 	  
    Delay50us();//延时50微秒
    return dat;
}


unsigned char DS18B20_ReadByte()//读一字节
{
    unsigned char i,j,dat;

    dat=0;

    for(i=1;i<=8;i++)
    {
        j=DS18B20_ReadBit();
        dat=(j<<7) | (dat>>1);
    }

    return dat;
}


void DS18B20_WriteByte(unsigned char dat)//写一字节
{
    unsigned char j;
    bit testb;

    for(j=1;j<=8;j++)
    {
        testb=dat & 0x01;
        dat=dat>>1;

        if(testb)
        {
            ds18b20=0; 	       
            Delay5us();//延时5微秒
            ds18b20=1; 	        
            Delay70us();//延时70微秒
        }
        else
        {            
            ds18b20=0; 	        
            Delay64us();//延时70微秒
            ds18b20=1; 	        
           Delay1us();//延时5微秒
        }
    }
}

bit DS18B20_Convert()//温度转换
{    
    DS18B20_Reset();
    if(DS18B20_WaitPresence()==1)
	{
      Delay(1);
      DS18B20_WriteByte(0xcc);//skip rom
      DS18B20_WriteByte(0x44);//convert	  
	  return 1;
	}
	else
	{	  
	  return 0;
	}
}


bit DS18B20_ReadFlash()//读存储
{    
    unsigned int nLsb,nMsb;
		unsigned int temp1;
    DS18B20_Reset();	
    if(DS18B20_WaitPresence()==1)
	{
      Delay(1);
      DS18B20_WriteByte(0xcc);//skip rom
      DS18B20_WriteByte(0xbe);//convert
      nLsb=DS18B20_ReadByte();//LSB
      nMsb=DS18B20_ReadByte();//MSB	  
			if(no==1) return 1;	  
			temp1=((nMsb&0x0f)<<8)+nLsb;
			if(nMsb&0x80)//如果是负数
			{
				temp=((((~temp1)+1)&0xfff)+1)*0.0625;
				temp=-1*temp;
			}
			else
			{
					temp=temp1*0.0625f;
			}
			
	  return 1;
	}
	else
	{
	  return 0;
	}
}



float DS18B20_Get_Temperature(void)//读取转换完成的温度
{
	return temp;
}



void DS18B20_HeartBeat_IRQ(void)//DS18B20系统心跳处理函数
{
	EA=0;
	if(HeartBeat_Cnt<80)//如果系统心跳计数小于80,说明计数未完成
	{
		HeartBeat_Cnt++;
		return;
	}
	else//如果计数已到,说明分频完成,可以开始DS18B20的心跳
	{
		HeartBeat_Cnt=0;//清空计数
		if(!IsDS18B20Converting)//如果当前不处于转换中(初始状态)
		{
			DS18B20_Convert();//开启温度转换,等待下一次读取
		}
		else
		{
			DS18B20_ReadFlash();//读取温度
			New_Temperature=DS18B20_Get_Temperature();//将温度放到全局变量等待取用
			IsDS18B20Converting=0;//去除转换的状态
			
		}
	}
	EA=1;
}


float DS18B20_Get_New_Temperature(void)//获取最新温度
{
	return New_Temperature;
}