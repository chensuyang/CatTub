#include "ds18b20.h"
#include "stc15.h"
#include "delay.h"
float New_Temperature=0;//��ǰ�¶�
sbit ds18b20=P1^2;
unsigned int HeartBeat_Cnt=0;//ϵͳ��������(���ڷ�Ƶ)
bit IsDS18B20Converting=0;//DS18B20�Ƿ�æµ(�Ƿ�����ת��״̬)

float temp=0;
bit no;


void DS18B20_Reset()//��λDS18B20
{
    ds18b20=0;     //����DQ���߿�ʼ��λ	
    Delay720us();     //����DQ�ʹ�Լ720uS	    
    ds18b20=1;     //�ͷ�DQλ    
}

bit DS18B20_WaitPresence()//�ȴ�оƬ��λӦ���ź�
{
	bit k;
	Delay50us();
	if(ds18b20==0)k=1;
	else k=0;
  Delay350us();
	return k;
    
}

bit DS18B20_ReadBit()//��һλ����
{
    bit dat;    
    ds18b20=0; 
		Delay1us();//��ʱ1΢��
    ds18b20=1; 
    Delay12us();//��ʱ12΢��
    dat=ds18b20; 	  
    Delay50us();//��ʱ50΢��
    return dat;
}


unsigned char DS18B20_ReadByte()//��һ�ֽ�
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


void DS18B20_WriteByte(unsigned char dat)//дһ�ֽ�
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
            Delay5us();//��ʱ5΢��
            ds18b20=1; 	        
            Delay70us();//��ʱ70΢��
        }
        else
        {            
            ds18b20=0; 	        
            Delay64us();//��ʱ70΢��
            ds18b20=1; 	        
           Delay1us();//��ʱ5΢��
        }
    }
}

bit DS18B20_Convert()//�¶�ת��
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


bit DS18B20_ReadFlash()//���洢
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
			if(nMsb&0x80)//����Ǹ���
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



float DS18B20_Get_Temperature(void)//��ȡת����ɵ��¶�
{
	return temp;
}



void DS18B20_HeartBeat_IRQ(void)//DS18B20ϵͳ����������
{
	EA=0;
	if(HeartBeat_Cnt<80)//���ϵͳ��������С��80,˵������δ���
	{
		HeartBeat_Cnt++;
		return;
	}
	else//��������ѵ�,˵����Ƶ���,���Կ�ʼDS18B20������
	{
		HeartBeat_Cnt=0;//��ռ���
		if(!IsDS18B20Converting)//�����ǰ������ת����(��ʼ״̬)
		{
			DS18B20_Convert();//�����¶�ת��,�ȴ���һ�ζ�ȡ
		}
		else
		{
			DS18B20_ReadFlash();//��ȡ�¶�
			New_Temperature=DS18B20_Get_Temperature();//���¶ȷŵ�ȫ�ֱ����ȴ�ȡ��
			IsDS18B20Converting=0;//ȥ��ת����״̬
			
		}
	}
	EA=1;
}


float DS18B20_Get_New_Temperature(void)//��ȡ�����¶�
{
	return New_Temperature;
}