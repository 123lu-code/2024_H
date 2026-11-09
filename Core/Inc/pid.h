#ifndef __PID_H
#define __PID_H

typedef struct {
	float Target;
	float Actual;

	float Out;

	float Kp;
	float Ki;
	float Kd;

	float Error0;
	float Error1;
	float ErrorInt;

	float OutMax;
	float OutMin;
} PID_t,Motor;
typedef struct {
	float Target;
	float Actual;

	float Out;

	float Kp;
	float Ki;
	float Kd;

	float Error0;
	float Error1;
	float Error2;
	float ErrorInt;

	float OutMax;
	float OutMin;
}Moter2;



typedef struct {
	float Target;
	float Actual;

	float Out;

	float Kp;
	float Ki;
	float Kd;

	float Error[3];


	float OutMax;
	float OutMin;
} Motor_1;

void PID_Update(PID_t *p);
void Motor_PID_Calculate_wz(Motor *p);//位置式
void Motor_PID_Calculate_zl(Motor_1 *pid);//增量式
void Motor_PID_Calculate_zl_angle(Motor_1 *pid);//增量式（角度）
void Motor_pwm(float pwm_left,float pwm_right );//车轮PWM
void PID_Update_zl(PID_t *p);
#endif
