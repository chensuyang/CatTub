
#define PTC_PWM_MAX 98 //PTC��������pwmռ�ձ�
#define PTC_PWM_MIN 0 //PTC�������Сpwmռ�ձ�


#define PTC_CONTROL_PID_I_MAX 90 //�¶ȿ��Ƶ�PID�л���������ֵ
#define PTC_CONTROL_PID_I_MIN -50 //�¶ȿ��Ƶ�PID�л��������Сֵ


void HeartBeat_Init(void);//��ʼ��ϵͳ����

void HeartBeat_IRQ(void);//ϵͳ�����ж�

void TemperatureSetMode_SetUP(void);//�����¶�����һ��

void TemperatureSetMode_SetDOWN(void);//�����¶��½�һ��

void Temperature_Control(void);//�¶ȿ���
extern  float Set_Temperature;//�趨�¶�
extern  bit IsTemperatureSetMode;//�����趨�¶�ģʽ
extern  unsigned int TemperatureSetModeTimeOutCnt;//�趨�¶�ģʽ��ʱ����