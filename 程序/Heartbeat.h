
#define PTC_PWM_MAX 98 //PTC输出的最大pwm占空比
#define PTC_PWM_MIN 0 //PTC输出的最小pwm占空比


#define PTC_CONTROL_PID_I_MAX 90 //温度控制的PID中积分项的最大值
#define PTC_CONTROL_PID_I_MIN -50 //温度控制的PID中积分项的最小值


void HeartBeat_Init(void);//初始化系统心跳

void HeartBeat_IRQ(void);//系统心跳中断

void TemperatureSetMode_SetUP(void);//设置温度上升一度

void TemperatureSetMode_SetDOWN(void);//设置温度下降一度

void Temperature_Control(void);//温度控制
extern  float Set_Temperature;//设定温度
extern  bit IsTemperatureSetMode;//处于设定温度模式
extern  unsigned int TemperatureSetModeTimeOutCnt;//设定温度模式超时计数