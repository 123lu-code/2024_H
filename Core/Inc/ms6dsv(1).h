#ifndef _ms6dsv_H
#define _ms6dsv_H
#include "i2c.h"
#include "my_i2c.h"
#include "gpio.h"


void LSM6DSV_Init(void);
// 读取陀螺仪三轴数据
void LSM6DSV_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz);

// 读取加速度计三轴数据
void LSM6DSV_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);
#endif
