#include "i2c.h"
#include "string.h"
#include <stdint.h>
#include "stdio.h"
#include "usart.h"
#include "my_i2c.h"


// -------------------------- 【官方手册】标准初始化序列 --------------------------
//void LSM6DSV_Init(void)
//{
//  // 1. 开启BDU功能（手册强制要求：防止数据错位）
//  LSM6DSV_WriteReg(CTRL3_C, 0x44);
//  HAL_Delay(10);
//
//  // 2. 加速度配置：ODR=480Hz（手册推荐 0x08）
//  LSM6DSV_WriteReg(CTRL1_XL, 0x08);
//  HAL_Delay(10);
//
//  // 3. 陀螺仪配置：ODR=480Hz（手册推荐 0x08）
//  LSM6DSV_WriteReg(CTRL2_G, 0x08);
//  HAL_Delay(10);
//}
void LSM6DSV_Init(void)
{
  // ==========================================
  // 1. CTRL3_C：BDU必须开启，同时配置地址自动递增（方便多字节读取）
  // ==========================================
  // 0x44 = BDU开启 + 地址自动递增
  LSM6DSV_WriteReg(CTRL3_C, 0x44);
  HAL_Delay(10);

  // ==========================================
  // 2. CTRL1_XL：加速度计配置（优化）
  // ==========================================
  // 0x68 = ODR=480Hz + 量程±2g + 开启低噪声模式 + 开启抗混叠滤波器
  // 说明：
  // - ODR=480Hz：和你的控制周期匹配
  // - 量程±2g：小车加速度不大，小量程精度更高
  // - 低噪声模式：降低噪声
  // - 抗混叠滤波器：滤除高频振动
  LSM6DSV_WriteReg(CTRL1_XL, 0x68);
  HAL_Delay(10);

  // ==========================================
  // 3. CTRL2_G：陀螺仪配置（核心优化！）
  // ==========================================
  // 0x68 = ODR=480Hz + 量程±250dps + 开启低噪声模式
  // 说明：
  // - 量程±250dps：小车转向角速度一般<100dps，小量程精度最高！
  // - 低噪声模式：降低陀螺仪噪声，积分角度更准
  LSM6DSV_WriteReg(CTRL2_G, 0x68);
  HAL_Delay(10);

  // ==========================================
  // 4. CTRL6_G：陀螺仪滤波器配置（额外优化！）
  // ==========================================
  // 0x00 = 开启陀螺仪低通滤波器，截止频率100Hz
  // 滤除电机震动、地面不平带来的高频噪声
  LSM6DSV_WriteReg(CTRL6_G, 0x00);
  HAL_Delay(10);
}
// 读取陀螺仪三轴数据
void LSM6DSV_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t buf[6];
    HAL_I2C_Mem_Read(&hi2c2, 0x6A << 1, 0x22, I2C_MEMADD_SIZE_8BIT, buf, 6, 1000);
    *gx = (int16_t)(buf[1] << 8 | buf[0]);
    *gy = (int16_t)(buf[3] << 8 | buf[2]);
    *gz = (int16_t)(buf[5] << 8 | buf[4]);
}

// 读取加速度计三轴数据
void LSM6DSV_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buf[6];
    HAL_I2C_Mem_Read(&hi2c2, 0x6A << 1, 0x28, I2C_MEMADD_SIZE_8BIT, buf, 6, 1000);
    *ax = (int16_t)(buf[1] << 8 | buf[0]);
    *ay = (int16_t)(buf[3] << 8 | buf[2]);
    *az = (int16_t)(buf[5] << 8 | buf[4]);
}

