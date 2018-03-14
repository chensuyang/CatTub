void TM1620_Write_Byte(unsigned char byte_data);//向TM1620中写1字节,不涉及STB引脚控制

void TM1620_Write_Command_Only(unsigned char command_data);//向TM1620中写命令,不带数据,涉及STB引脚控制

void TM1620_Write_Command_And_Data(unsigned char command_data,unsigned char _data);//向TM1620中写命令以及一字节数据,涉及STB引脚控制

void TM1620_Init(void);//初始化设置TM1620

void TM1620_Write_Digitron_Data(unsigned char pos,unsigned char digitron_data);//向TM1620显存中指定数码管位写入数码管数据

void TM1620_display_on(unsigned char light_intensity);//TM1620开显示

void TM1620_display_off(void);//TM1620关显示

void TM1620_Write_Number(unsigned char pos,unsigned number,bit point);//向TM1620显存中指定数码管位写数字

void TM1620_Show_Temperature(float temperature);//TM1620显示温度