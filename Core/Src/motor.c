/*
 * motor.c
 *
 *  Created on: Apr 21, 2026
 *      Author: LENOVO
 */

#include "motor.h"
#include "main.h"
#include "tim.h"


// ==================== 电机PWM输出 ====================
void Motor_SetPWM(int16_t pwm)
{
    if(pwm > 99) pwm = 99;
    if(pwm < 0)   pwm = 0;

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm);
}

// ==================== 前进 ====================
void Qian_Jin(int16_t pwm)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);

}

// ==================== 倒车 ====================
void Dao_Che(int16_t pwm)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
}

// ==================== 停车 ====================
void Ting_Che(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);
Motor_SetPWM(0);
}

#include "motor.h"
#include "tim.h"

// 左轮控制（用你封装的宏）
void Motor_SetLeft(int16_t pwm)
{
    if (pwm > 0)
    {
        // 正转
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm);
    }
    else if (pwm < 0)
    {
        // 反转
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, -pwm);
    }
    else
    {
        // 停止
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
    }
}

// 右轮控制（用你封装的宏）
void Motor_SetRight(int16_t pwm)
{
    if (pwm > 0)
    {
        // 正转（按你宏的定义，BIN1_ON是RESET，BIN1_OFF是SET）
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm);
    }
    else if (pwm < 0)
    {
        // 反转
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -pwm);
    }
    else
    {
        // 停止
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
    }
}


