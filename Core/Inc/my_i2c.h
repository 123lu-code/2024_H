#ifndef _my_i2c_H
#define _my_i2c_H
#include "gpio.h"

// 官方手册寄存器地址（严格对标）
#define STATUS_REG      0x1E    // 状态寄存器
#define CTRL1_XL        0x10    // 加速度配置
#define CTRL2_G         0x11    // 陀螺仪配置
#define CTRL3_C         0x12    // 控制寄存器（BDU）
#define OUTX_L_G        0x22    // 陀螺仪低字节
#define OUTX_L_A        0x28    // 加速度低字节
#define	CTRL6_G			0x15

// 传感器I2C地址
#define LSM6DSV_ADDR    (0x6A << 1)

// -------------------------- 官方标准：写寄存器 --------------------------
uint8_t LSM6DSV_WriteReg(uint8_t reg, uint8_t data);

// -------------------------- 官方标准：读寄存器 --------------------------
uint8_t LSM6DSV_ReadReg(uint8_t reg);

// -------------------------- 官方标准：读取16位数据 --------------------------
int16_t LSM6DSV_Read16Bit(uint8_t reg);
















//void IIC_Start();
//void IIC_Stop();
//void IIC_Send_Byte(uint8_t Byte);
//uint8_t IIC_Receive_Byte(void);
//void IIC_Send_Ack(uint8_t Ack);
//uint8_t IIC_Receive_Ack(void);
//
//
//
//uint8_t MS6DSV_WriteReg(uint8_t reg, uint8_t data);
//uint8_t MS6DSV_ReadReg(uint8_t reg);
//int16_t MS6DSV_Read16Bit(uint8_t reg_L);



#endif




