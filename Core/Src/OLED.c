//#include "OLED.h"
//#include "OLED_Data.h"
//#include "MyI2C.h"
//#include "math.h"
//#include <stdio.h>
//
//#define OLED_ADDR    0x78
//#define OLED_CMD     0x00
//#define OLED_DATA    0x40
//
//void LOVE()
//{
//	OLED_ShowString(16, 2, "WLM LOVE LXF");
//}
//
//// ==================== 硬件I2C → 模拟I2C（只改了这个函数！）====================
//static void OLED_WriteByte(uint8_t data, uint8_t mode)
//{
//    MyI2C_Start();
//    MyI2C_SendByte(0x78);    // OLED固定地址
//    MyI2C_ReceiveAck();
//    MyI2C_SendByte(mode);    // 0x00命令 0x40数据
//    MyI2C_ReceiveAck();
//    MyI2C_SendByte(data);
//    MyI2C_ReceiveAck();
//    MyI2C_Stop();
//}
//
//void OLED_SetCursor(uint8_t x, uint8_t y)
//{
//	OLED_WriteByte(0xB0 + y, OLED_CMD);
//	OLED_WriteByte(((x & 0xF0) >> 4) | 0x10, OLED_CMD);
//	OLED_WriteByte(x & 0x0F, OLED_CMD);
//}
//
//void OLED_Clear(void)
//{
//	uint8_t y, x;
//	for(y=0; y<8; y++)
//	{
//		OLED_SetCursor(0, y);
//		for(x=0; x<128; x++)
//		{
//			OLED_WriteByte(0x00, OLED_DATA);
//		}
//	}
//}
//
//void OLED_Init(void)
//{
//    MyI2C_Init();  // 模拟I2C初始化
//	HAL_Delay(100);
//
//	OLED_WriteByte(0xAE, OLED_CMD);
//	OLED_WriteByte(0xD5, OLED_CMD);
//	OLED_WriteByte(0x80, OLED_CMD);
//	OLED_WriteByte(0xA8, OLED_CMD);
//	OLED_WriteByte(0x3F, OLED_CMD);
//	OLED_WriteByte(0x40, OLED_CMD);
//	OLED_WriteByte(0xB0, OLED_CMD);
//	OLED_WriteByte(0xC8, OLED_CMD);
//	OLED_WriteByte(0x81, OLED_CMD);
//	OLED_WriteByte(0xCF, OLED_CMD);
//	OLED_WriteByte(0xA1, OLED_CMD);
//	OLED_WriteByte(0xA6, OLED_CMD);
//	OLED_WriteByte(0x8D, OLED_CMD);
//	OLED_WriteByte(0x14, OLED_CMD);
//	OLED_WriteByte(0xAF, OLED_CMD);
//
//	OLED_Clear();
//}
//
//void OLED_ShowChar(uint8_t x, uint8_t y, char chr)
//{
//	uint8_t c = chr - ' ';
//	uint8_t i;
//
//	OLED_SetCursor(x, y);
//	for(i=0; i<8; i++)
//	{
//		OLED_WriteByte(OLED_F8x16[c][i], OLED_DATA);
//	}
//
//	OLED_SetCursor(x, y+1);
//	for(i=0; i<8; i++)
//	{
//		OLED_WriteByte(OLED_F8x16[c][i+8], OLED_DATA);
//	}
//}
//
//void OLED_ShowString(uint8_t x, uint8_t y, char *str)
//{
//	while(*str)
//	{
//		OLED_ShowChar(x, y, *str);
//		x += 8;
//		str++;
//	}
//}
//
//void OLED_ShowChinese(uint8_t x, uint8_t y, char *str)
//{
//	uint16_t i = 0;
//
//	while (OLED_CF16x16[i].Index[0] != '\0')
//	{
//		if (strcmp(OLED_CF16x16[i].Index, str) == 0)
//		{
//			break;
//		}
//		i++;
//	}
//
//	uint8_t j;
//	OLED_SetCursor(x, y);
//	for(j=0; j<16; j++)
//	{
//		OLED_WriteByte(OLED_CF16x16[i].Msk[j], OLED_DATA);
//	}
//
//	OLED_SetCursor(x, y+1);
//	for(j=0; j<16; j++)
//	{
//		OLED_WriteByte(OLED_CF16x16[i].Msk[j+16], OLED_DATA);
//	}
//}
//
//void OLED_ShowNum(uint8_t x, uint8_t y, int num)
//{
//	char buf[10];
//	sprintf(buf, "%d", num);
//	OLED_ShowString(x, y, buf);
//}
//
//void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
//{
//	uint32_t PowNum, IntNum, FraNum;
//
//	if (Number >= 0)
//	{
//		OLED_ShowChar(X, Y, '+');
//	}
//	else
//	{
//		OLED_ShowChar(X, Y, '-');
//		Number = -Number;
//	}
//
//	IntNum = Number;
//	Number -= IntNum;
//	PowNum = OLED_Pow(10, FraLength);
//	FraNum = round(Number * PowNum);
//	IntNum += FraNum / PowNum;
//
//	OLED_ShowNum(X + FontSize, Y, IntNum);
//	OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.');
//	OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, FraNum);
//}
//
//uint32_t OLED_Pow(uint32_t X, uint32_t Y)
//{
//    uint32_t Result = 1;
//    while (Y--)
//    {
//        Result *= X;
//    }
//    return Result;
//}
