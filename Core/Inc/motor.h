/*
 * motor.h
 *
 *  Created on: Apr 21, 2026
 *      Author: LENOVO
 */

#ifndef INC_MOTOR_H
#define INC_MOTOR_H
#include "stdint.h"
#include "main.h"


void Qian_Jin(int16_t pwm);
void Dao_Che(int16_t pwm);
void Ting_Che(void);
void Motor_Init(void);
void Motor_SetLeft(int16_t pwm);
void Motor_SetRight(int16_t pwm);
void Motor_Stop(void);


#endif /* INC_MOTOR_H_ */
