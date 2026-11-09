#include "main.h"
#include "math.h"
#include "tim.h"
#include "usart.h"
#include "stdio.h"
#include "gpio.h"
#include "string.h"
#include "motor.h"
#include "OLED.h"
#include "pid.h"
#include "hongwai.h"
#include "my_i2c.h"
#include "ms6dsv(1).h"
#include "stdlib.h"
#include "stdint.h"
#include "i2c.h"
extern PID_t Inner_left;
extern PID_t Inner_right;
extern PID_t Angle_PID;
extern Motor Outer;
extern float angle_z;
extern float Speed_left,Speed_right,Location;
extern float base_speed;       // 基础速度
extern float target_angle;  // 目标角度（0=直线）
extern uint8_t num;
extern uint8_t IR[7];
extern float location;
extern uint8_t go_trace_flag;
extern uint8_t mode;
extern float weight_sum;
extern float deviation;
extern int16_t temp1;
extern int16_t temp2;
uint8_t flag=0;
extern uint8_t IRz;
Motor_1 L_PID={
		.Kp=0.3f,.Ki=0.09f,.Kd=0.0f,
		.Target=100.0f,.OutMax=99.0f,.OutMin=-99.0f,
};
Motor_1 R_PID={
		.Kp=0.3f,.Ki=0.09f,.Kd=0.0f,
		.Target=100.0f,.OutMax=99.0f,.OutMin=-99.0f,
};
//void aaa()
//{
//	L_PID.Actual=du2();
//	R_PID.Actual=du1();
//
//	Motor_PID_Calculate_zl(&L_PID);
//	Motor_PID_Calculate_zl(&R_PID);
//	Motor_pwm(L_PID.Out,R_PID.Out);
//
//
//
//	char text[80];
//	sprintf(text, "%.2f,%.2f,%.2f,%.2f",L_PID.Actual,L_PID.Target,L_PID.Out,L_PID.Error[0]);
//	HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), 100);
//}




void Motor_pwm(float pwm_left,float pwm_right )
{
	// ==================== 左轮驱动 ====================
	if(pwm_left > 0)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm_left);
	}
	else if(pwm_left < 0)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -pwm_left);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	}

	// ==================== 右轮驱动 ====================
	if(pwm_right > 0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_right);
	}
	else if(pwm_right < 0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, -pwm_right);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	}
}



//void jiaoduweizhisuduhuan()
//{
//	uint8_t status = LSM6DSV_ReadReg(0x1E);
//	if((status & 0x03) == 0x03)
//	{
//		int16_t gx, gy, gz;
//		int16_t ax, ay, az;
//		LSM6DSV_ReadGyro(&gx, &gy, &gz);
//		LSM6DSV_ReadAccel(&ax, &ay, &az);
//
//		float gz_dps = gz * 4.375f / 1000.0f;
//		if(fabs(gz_dps) < 3.0f) gz_dps = 0.0f;
//		angle_z += gz_dps * 0.04f;
//	}
//	Speed_left  = du2();
//	Speed_right = du1();
//
//	// ==================== 2. 角度环（外环） ====================
//	Angle_PID.Target = target_angle;       // 目标角度
//	Angle_PID.Actual = angle_z;           // 实际角度（来自陀螺仪）
//	PID_Update(&Angle_PID);
//	if(fabs(Angle_PID.Out) < 1.0f)
//	{
//		Angle_PID.Out = 0.0f;
//	}
//	Inner_left.Actual  = Speed_left;
//	Inner_right.Actual = Speed_right;
//	location+=Inner_left.Actual+Inner_right.Actual;
//	Outer.Actual = (double)location/2.0;
//	Motor_PID_Calculate_wz(&Outer);
//	if(fabs(Outer.Out) < 50)
//	{
//		Outer.Out = 0.0f;
//	}
//	Inner_left.Target=Outer.Out-Angle_PID.Out;
//	Inner_right.Target=Outer.Out+Angle_PID.Out;
//	PID_Update(&Inner_left);
//	PID_Update(&Inner_right);
//	float pwm_left  = Inner_left.Out;
//	float pwm_right = Inner_right.Out;
//
//	// ==================== 左轮驱动 ====================
//	if(pwm_left > 0)
//	{
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm_left);
//	}
//	else if(pwm_left < 0)
//	{
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -pwm_left);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
//	}
//
//	// ==================== 右轮驱动 ====================
//	if(pwm_right > 0)
//	{
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_right);
//	}
//	else if(pwm_right < 0)
//	{
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, -pwm_right);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
//	}
//
//
//}
//void jiaodusuduhuan()
//{
//	uint8_t status = LSM6DSV_ReadReg(0x1E);
//	if((status & 0x03) == 0x03)
//	{
//		int16_t gx, gy, gz;
//		int16_t ax, ay, az;
//		LSM6DSV_ReadGyro(&gx, &gy, &gz);
//		LSM6DSV_ReadAccel(&ax, &ay, &az);
//		float gz_dps = gz * 4.375f / 1000.0f;
//		if(fabs(gz_dps) < 3.0f) gz_dps = 0.0f;
//		angle_z += gz_dps * 0.04f;
//	}
//
//	// ==================== 双环PID配置 ====================
//
//	// ==================== 1. 读取速度 ====================
//	Inner_left.Actual  = du2();
//	Inner_right.Actual = du1();
//
//	// ==================== 2. 角度环（外环） ====================
//	Angle_PID.Target = target_angle;       // 目标角度
//	Angle_PID.Actual = angle_z;           // 实际角度（来自陀螺仪）
//	PID_Update(&Angle_PID);
//	float turn = Angle_PID.Out;           // 转向修正值
//	//turn *= 2.0f;   // 放大2倍，让车有足够转向力
////	if(turn>25)turn=25;   // 把限幅从25改成40，足够拉动车
////	if(turn<-25)turn=-25;
//
//	Inner_left.Target  = base_speed - turn;
//	Inner_right.Target = base_speed + turn;
//
//	PID_Update(&Inner_left);
//	PID_Update(&Inner_right);
//		float pwm_left  = Inner_left.Out;
//		float pwm_right = Inner_right.Out;
//
//		// ==================== 左轮驱动 ====================
//		if(pwm_left > 0)
//		{
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm_left);
//		}
//		else if(pwm_left < 0)
//		{
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -pwm_left);
//		}
//		else
//		{
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
//		}
//
//		// ==================== 右轮驱动 ====================
//		if(pwm_right > 0)
//		{
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_right);
//		}
//		else if(pwm_right < 0)
//		{
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, -pwm_right);
//		}
//		else
//		{
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
//		}
//}

void jiaoduweizhisuduhuan()
{
//	uint8_t status = LSM6DSV_ReadReg(0x1E);
//	if((status & 0x03) == 0x03)
//	{
//		int16_t gx, gy, gz;
//		int16_t ax, ay, az;
//		LSM6DSV_ReadGyro(&gx, &gy, &gz);
//		LSM6DSV_ReadAccel(&ax, &ay, &az);
//
//		float gz_dps = gz * 4.375f / 1000.0f;  //4.375
//		if(fabs(gz_dps) < 3.0f) gz_dps = 0.0f;
//		angle_z += gz_dps * 0.04f;
//	}
//


	Angle_PID.Target = target_angle;
	Angle_PID.Actual = angle_z;
	PID_Update(&Angle_PID);

	if(fabs(Angle_PID.Out) < 1.0f)
	{
		Angle_PID.Out = 0.0f;
	}
	Inner_left.Actual  = temp2;
	Inner_right.Actual = temp1;
	location += Inner_left.Actual + Inner_right.Actual;
	Outer.Actual = (double)location / 2.0;
	PID_Update(&Outer);
	Motor_PID_Calculate_wz(&Outer);
	Inner_left.Target  = Outer.Out - Angle_PID.Out;
	Inner_right.Target = Outer.Out + Angle_PID.Out;
	PID_Update(&Inner_left);
	PID_Update(&Inner_right);
	Motor_pwm(Inner_left.Out,Inner_right.Out );

}
void suduhuan()
{
	Inner_left.Actual  = temp2;
	Inner_right.Actual = temp1;
	Inner_left.Target=150+deviation;
	Inner_right.Target=150-deviation;
	PID_Update(&Inner_left);
	PID_Update(&Inner_right);
	Motor_pwm(Inner_left.Out,Inner_right.Out );
}
void suduhuan_zizhuan(float left,float right)
{
	Inner_left.Actual  = temp2;
	Inner_right.Actual = temp1;
	Inner_left.Target=left;
	Inner_right.Target=right;
	PID_Update(&Inner_left);
	PID_Update(&Inner_right);
	Motor_pwm(Inner_left.Out,Inner_right.Out );
}
void Reset_PID(PID_t *p)
{
    p->Error0 = 0;
    p->Error1 = 0;
    p->ErrorInt = 0;
    p->Out = 0;
}
void weizhi(Motor *p)
{
	p->Error0 = 0;
	p->Error1 = 0;
	p->ErrorInt = 0;
	p->Out = 0;
}

void Reset_All_PID(void)
{
    Reset_PID(&Angle_PID);
    weizhi(&Outer);
    Reset_PID(&Inner_left);
    Reset_PID(&Inner_right);
}
void Mode_1()//模式一：-21.8度转弯+位置环定位
{

	target_angle = -34.0;
	Outer.Target  = 9700;
	jiaoduweizhisuduhuan();
	if(Outer.Actual>=9700)
	{
		if(flag==0)
		{
			Reset_PID(&Angle_PID);
			flag=1;
		}
		mode++;
	}

}
void Mode_2()//模式二：扫描
{

	if(weight_sum==0&&IRz==0)
	{
//		suduhuan_zizhuan(-10.0f, 10.0f);
		Motor_pwm(70,-70);
	}
	else if(weight_sum != 0 || IRz!=0)
	{
		Reset_All_PID();
			mode++;
	}

}
void Mode_3()//模式三：巡线
{

//	Track_7IR();
	suduhuan();
	if(weight_sum == 0&&IRz==0)
	{
		location=0;
		Reset_All_PID();
		angle_z = -12.0f;//出弯偏移角度补偿
		mode++;
	}
}
void Mode_4()//模式四：更新角度 + +21.8度转弯+位置环定位
{
	target_angle = 45.0;
	Outer.Target  = 10500;
	jiaoduweizhisuduhuan();
	if(Outer.Actual>=10500)
	{
		if(flag==1)
		{
			Reset_PID(&Angle_PID);
			flag=0;
		}
			mode++;
	}
}
void Mode_5()//模式五：扫描
{
//	static uint8_t init = 0;
//	if(init == 0)
//	{
//		Reset_All_PID();   // ✅ 复位所有PID
//		Motor_pwm(0, 0);   // ✅ 先停止电机
//		HAL_Delay(50);     // ✅ 等待电机完全停止
//		init = 1;
//	}
	if(weight_sum == 0&&IRz==0)
	{
//		suduhuan_zizhuan(45.0f, 0.0f);
		Motor_pwm(-70,70);
	}
	else if(weight_sum != 0||IRz!=0)
	{
		Reset_All_PID();
			mode++;
	}
}
void Mode_6() //模式六：设置目标角度为+-41.8
{
	suduhuan();
	if(weight_sum == 0&&IRz==0)
	{
		location=0;
		Reset_All_PID();
		angle_z = 24.0f;
		mode= 0;
	}
}
