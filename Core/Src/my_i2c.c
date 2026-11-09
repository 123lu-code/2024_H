#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "my_i2c.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "stdio.h"



// -------------------------- 官方标准：写寄存器 --------------------------
uint8_t LSM6DSV_WriteReg(uint8_t reg, uint8_t data)
{
  HAL_StatusTypeDef sta = HAL_I2C_Mem_Write(&hi2c2, LSM6DSV_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
  return (sta == HAL_OK) ? 0 : 1;
}

// -------------------------- 官方标准：读寄存器 --------------------------
uint8_t LSM6DSV_ReadReg(uint8_t reg)
{
  uint8_t data = 0;
  HAL_I2C_Mem_Read(&hi2c2, LSM6DSV_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
  return data;
}

// -------------------------- 官方标准：读取16位数据 --------------------------
int16_t LSM6DSV_Read16Bit(uint8_t reg)
{
  uint8_t buf[2];
  HAL_I2C_Mem_Read(&hi2c2, LSM6DSV_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buf, 2, 1000);
  return (int16_t)(buf[1] << 8 | buf[0]);
}














//#include "i2c.h"
//#define MS6DSV_SCL_GPIO_PORT GPIOB
//#define MS6DSV_SCL_GPIO_PIN GPIO_PIN_10
//
//#define MS6DSV_SDA_GPIO_PORT GPIOB
//#define MS6DSV_SDA_GPIO_PIN GPIO_PIN_11
//static uint16_t  g_fac_us = 9;
////void delay_us(uint32_t nus)
////{
////    uint32_t temp;
////    SysTick->LOAD = nus * g_fac_us;
////    SysTick->VAL = 0x00;
////    SysTick->CTRL |= 1 << 0 ;
////
////    do
////    {
////        temp = SysTick->CTRL;
////    } while ((temp & 0x01) && !(temp & (1 << 16)));
////
////    SysTick->CTRL &= ~(1 << 0) ;
////    SysTick->VAL = 0X00;
////}
//void delay_us(uint32_t nus)
//{
//    uint32_t ticks = nus * (SystemCoreClock / 1000000);
//    uint32_t start = SysTick->VAL;
//    while((start - SysTick->VAL) < ticks);
//}
//
//void IIC_W_SCL(uint8_t x)
//{
//	if(x)
//	{
//		HAL_GPIO_WritePin(MS6DSV_SCL_GPIO_PORT, MS6DSV_SCL_GPIO_PIN, GPIO_PIN_SET);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(MS6DSV_SCL_GPIO_PORT, MS6DSV_SCL_GPIO_PIN, GPIO_PIN_RESET);
//	}
//	delay_us(4);
//
//}
//void IIC_W_SDA(uint8_t x)
//{
//	if(x)
//	{
//		HAL_GPIO_WritePin(MS6DSV_SDA_GPIO_PORT, MS6DSV_SDA_GPIO_PIN, GPIO_PIN_SET);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(MS6DSV_SDA_GPIO_PORT, MS6DSV_SDA_GPIO_PIN, GPIO_PIN_RESET);
//	}
//	delay_us(4);
//}
//uint8_t IIC_R_SDA(void)
//{
//	uint8_t x;
//	x = HAL_GPIO_ReadPin(MS6DSV_SDA_GPIO_PORT, MS6DSV_SDA_GPIO_PIN);
//	delay_us(4);
//	return x;
//}
//
//
//void IIC_Start()
//{
//	IIC_W_SDA(1);
//	IIC_W_SCL(1);
//	IIC_W_SDA(0);
//	IIC_W_SCL(0);
//}
//void IIC_Stop()
//{
//	IIC_W_SDA(0);
//	IIC_W_SCL(1);
//	IIC_W_SDA(1);
//}
//
//void IIC_Send_Byte(uint8_t Byte)
//{
//	uint8_t i;
//	for (i=0;i<8;i++)
//	{
//		IIC_W_SDA( (Byte & (0x80 >> i)) ? 1 : 0 );
//		IIC_W_SCL(1);
//		IIC_W_SCL(0);
//	}
//	IIC_W_SDA(1);
//}
//
//uint8_t IIC_Receive_Byte(void)
//{
//	uint8_t Byte=0x00,i;
//	IIC_W_SDA(1);
//	for (i=0;i<8;i++)
//	{
//		IIC_W_SCL(1);
//		if(IIC_R_SDA())
//		{
//			Byte|=(0x80>>i);
//		}
//		IIC_W_SCL(0);
//	}
//	return Byte;
//}
//
//
//void IIC_Send_Ack(uint8_t Ack)
//{
//	IIC_W_SDA(Ack);
//	IIC_W_SCL(1);
//	IIC_W_SCL(0);
//}
//
//uint8_t IIC_Receive_Ack(void)
//{
//	uint8_t Ack;
//	IIC_W_SDA(1);
//	IIC_W_SCL(1);
//	Ack=IIC_R_SDA();
//	IIC_W_SCL(0);
//	return Ack;
//}
//
//// 带ACK校验的写寄存器函数（核心修复）
//uint8_t MS6DSV_WriteReg(uint8_t reg, uint8_t data)
//{
//    uint8_t ack;
//    IIC_Start();
//
//    // 1. 发送设备写地址，等待ACK
//    IIC_Send_Byte(0xD4);
//    ack = IIC_Receive_Ack();
//    if (ack != 0) {  // NACK，通信失败
//        IIC_Stop();
//        return 1;
//    }
//
//    // 2. 发送寄存器地址，等待ACK
//    IIC_Send_Byte(reg);
//    ack = IIC_Receive_Ack();
//    if (ack != 0) {
//        IIC_Stop();
//        return 1;
//    }
//
//    // 3. 发送数据，等待ACK
//    IIC_Send_Byte(data);
//    ack = IIC_Receive_Ack();
//    if (ack != 0) {
//        IIC_Stop();
//        return 1;
//    }
//
//    IIC_Stop();
//    HAL_Delay(1);
//    return 0;  // 写入成功
//}
//// 单字节读寄存器 (核心函数，你缺失的)
//uint8_t MS6DSV_ReadReg(uint8_t reg)
//{
//    uint8_t data;
//    IIC_Start();
//    IIC_Send_Byte(0xD4);    // 写地址
//    IIC_Receive_Ack();
//    IIC_Send_Byte(reg);     // 寄存器地址
//    IIC_Receive_Ack();
//
//    IIC_Start();
//    IIC_Send_Byte(0xD5);    // 读地址
//    IIC_Receive_Ack();
//    data = IIC_Receive_Byte();
//    IIC_Send_Ack(1);        // NACK
//    IIC_Stop();
//    return data;
//}
//
//// 读取16位数据 (陀螺仪/角度专用)
//int16_t MS6DSV_Read16Bit(uint8_t reg_L)
//{
//    uint8_t low, high;
//    IIC_Start();
//    IIC_Send_Byte(0xD4);
//    IIC_Receive_Ack();
//    IIC_Send_Byte(reg_L);
//    IIC_Receive_Ack();
//
//    IIC_Start();
//    IIC_Send_Byte(0xD5);
//    IIC_Receive_Ack();
//    low = IIC_Receive_Byte();
//    IIC_Send_Ack(0);        // ACK
//    high = IIC_Receive_Byte();
//    IIC_Send_Ack(1);        // NACK
//    IIC_Stop();
//    return (int16_t)((high << 8) | low);
//}
