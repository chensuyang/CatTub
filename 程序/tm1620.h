void TM1620_Write_Byte(unsigned char byte_data);//��TM1620��д1�ֽ�,���漰STB���ſ���

void TM1620_Write_Command_Only(unsigned char command_data);//��TM1620��д����,��������,�漰STB���ſ���

void TM1620_Write_Command_And_Data(unsigned char command_data,unsigned char _data);//��TM1620��д�����Լ�һ�ֽ�����,�漰STB���ſ���

void TM1620_Init(void);//��ʼ������TM1620

void TM1620_Write_Digitron_Data(unsigned char pos,unsigned char digitron_data);//��TM1620�Դ���ָ�������λд�����������

void TM1620_display_on(unsigned char light_intensity);//TM1620����ʾ

void TM1620_display_off(void);//TM1620����ʾ

void TM1620_Write_Number(unsigned char pos,unsigned number,bit point);//��TM1620�Դ���ָ�������λд����

void TM1620_Show_Temperature(float temperature);//TM1620��ʾ�¶�