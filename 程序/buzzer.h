bit Buzzer_IsBusy(void);//获取蜂鸣器工作状态

void Buzzer_Init(void);//初始化蜂鸣器

void Buzzer_On(void);//开启蜂鸣器

void Buzzer_Off(void);//关闭蜂鸣器

void Buzzer_Short_Tweet(unsigned char time);//蜂鸣器短鸣(time=鸣叫次数)

void Buzzer_Long_Tweet(unsigned char time);//蜂鸣器长鸣(time=鸣叫次数)

void Buzzer_Tweet_HeartBeat_IRQ(void);//蜂鸣器鸣叫系统心跳函数