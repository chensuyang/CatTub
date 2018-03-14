void IapIdle();
unsigned char IapReadByte(unsigned int addr);
void IapProgramByte(unsigned int addr, unsigned char dat);
void IapEraseSector(unsigned int addr);
void EEPROM_Write_SetTemperature(float temperature);//EEPROM写设定温度值
float EEPROM_Read_SetTemperature(void);//EEPROM读设定温度值