/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#include "daima.h"






void SystemClock_Config(void);
uint8_t mode=0;
float location=0;
float angle_z=0;
float base_speed = 150;       // 基础速度
float target_angle = 0.0f;    // 目标角度（0=直线）
extern uint8_t IR[7]; //七个红外对管
extern uint8_t IRz;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
float Speed_left,Speed_right,Location;		//速度，位置
//
//PID_t Inner_left = {
//    .Kp = 0.3f,
//    .Ki = 0.09f,
//    .Kd = 0.0f,
//    .OutMax = 99,
//    .OutMin = -99,
//};
//PID_t Inner_right = {
//    .Kp = 0.3f,
//    .Ki = 0.09f,
//    .Kd = 0.0f,
//    .OutMax = 99,
//    .OutMin = -99,
//};


// 内环 速度环 稳定不啸叫
PID_t Inner_left = {
    .Kp = 0.3f,
    .Ki = 0.09f,   // 积分压到极低，不冲
    .Kd = 0.0f,
    .OutMax = 99,
    .OutMin = -99,
};

PID_t Inner_right = {
    .Kp = 0.3f,
    .Ki = 0.09f,
    .Kd = 0.0f,
    .OutMax = 99,
    .OutMin = -99,
};

// 角度环（转向环）PID —— 负责走直线、修正方向
PID_t Angle_PID = {
//    .Kp = 2.2f,     // 转向力度
//    .Ki = 0.0f,    // 小积分
//    .Kd = 0.5f,     // 微阻尼
////		.Kp = 8.0f,     // 转向力度
////		.Ki = 0.00f,    // 小积分
////		.Kd = 0.5f,     // 微阻尼
//    .OutMax = 60,   // 转向最大修正量（不能太大）
//    .OutMin = -60,
//		 .Kp = 3.0f,     // 转向力度
//		    .Ki = 0.0f,    // 小积分
//		    .Kd = 0.5f,     // 微阻尼
//		//		.Kp = 8.0f,     // 转向力度
//		//		.Ki = 0.00f,    // 小积分
//		//		.Kd = 0.5f,     // 微阻尼
//		    .OutMax = 60,   // 转向最大修正量（不能太大）
//		    .OutMin = -60,          //90度
//		.Kp = 3.0f,     // 转向力度
//				.Ki = 0.00f,    // 小积分
//				.Kd = 0.5f,     // 微阻尼
//		    .OutMax = 50,   // 转向最大修正量（不能太大）
//		    .OutMin = -50,          //90度

//		.Kp = 3.0f,     // 转向力度
//				.Ki = 0.04f,    // 小积分
//				.Kd = 0.0f,     // 微阻尼
//		    .OutMax = 50,   // 转向最大修正量（不能太大）
//		    .OutMin = -50,
		.Kp = 3.0f,     // 转向力度
				.Ki = 0.1f,    // 小积分
				.Kd = 0.2f,     // 微阻尼
		    .OutMax = 40,   // 转向最大修正量（不能太大）
		    .OutMin = -40,


};

// 外环 位置环 不冲、稳停车
Motor Outer = {

//		.Kp = 0.1f,
//		.Ki = 0.0f,
//		.Kd = 0.1f,
//    .OutMax = 140,
//    .OutMin = -140,
		.Kp = 0.1f,
		.Ki = 0.0f,
		.Kd = 0.1f,
    .OutMax = 140,
    .OutMin = -140,

};
//Motor Outer = {
//    .Kp = 6.0f,      // 更稳
//    .Ki = 0.0f,
//    .Kd = 1.2f,      // 降低，减少抖动
//    .OutMax = 420,   // 正常速度（不会飞）
//    .OutMin = -420,
//};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// 全局变量（放在 main.c 最上面）


uint32_t delay_ms = 0;    // 计时用
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t go_trace_flag = 0;  // 0=走直线  1=循迹
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 两个速度环PID，完全独立！

// 速度环PID初始化


// 输出限幅（你的PWM最大值 99）

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  // 开启外设
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
  MX_TIM1_Init();
  LSM6DSV_Init();
  HAL_TIM_Base_Start_IT(&htim1); // 开启TIM1中断
//  Inner_left.Target=60;
//  Inner_right.Target=60;
//  base_speed=90;
//  target_angle=-30; //斜线角度


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int16_t temp1=0;
int16_t temp2=0;
int16_t du1(void)
{
    temp1 = __HAL_TIM_GET_COUNTER(&htim3);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    return temp1;
}
int16_t du2(void)
{

    temp2= __HAL_TIM_GET_COUNTER(&htim4);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    return temp2;
}



char text[80];
uint16_t count=0;
extern Motor_1 L_PID;
extern Motor_1 R_PID;

// uint8_t first_run = 0;
// uint8_t num1=0;

	uint8_t  this_white_flag  = 0;
	uint8_t  last_white_flag  = 0;
uint8_t flag11=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
		static uint16_t Count1=0;
//		static uint16_t tim=0;
		Count1++;
//		tim++;
//		if(tim>3000)
//		{
//			tim=0;
//			Outer.Out+=40;
//			if(Outer.Out>250)
//			{
//				Outer.Out=-250;
//
//			}
//		}
		// 40ms 中断
		if (Count1 >= 40)
		{
			Count1=0;

//			target_angle =90;
//			//Outer.Target  = 5000;
//			jiaodusuduhuan();
//			jiaoduweizhisuduhuan();
//			sprintf(text, "%.2f,%.2f,%.2f\r\n",Inner_left.Actual,Inner_right.Actual,Angle_PID.Actual);
//			HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), 100);

//			aaa();
////
////			Inner_left.Target=1000;
////			Inner_right.Target=1000;
////			suduhuan();

//**************************第三题半圈****************************
//			uint8_t status = LSM6DSV_ReadReg(0x1E);
//			if((status & 0x03) == 0x03)
//			{
//				int16_t gx, gy, gz;
//				int16_t ax, ay, az;
//				LSM6DSV_ReadGyro(&gx, &gy, &gz);
//				LSM6DSV_ReadAccel(&ax, &ay, &az);
//
//				float gz_dps = gz * 4.375f / 1000.0f;
//				if(fabs(gz_dps) < 3.0f) gz_dps = 0.0f;
//				angle_z += gz_dps * 0.04f;
//			}
//
//			// 读取7路红外
//			IR[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
//			IR[1] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
//			IR[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
//			IR[3] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
//			IR[4] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
//			IR[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
//			IR[6] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
//
//			// ==============================================
//			// 核心逻辑：斜走完成后 → 循迹 / 左转找线
//			// ==============================================
//			if(go_trace_flag == 0)
//			{
//				// ======================
//				// 阶段1：还没走完 → 斜走 -45° 9500
//				// ======================
//				target_angle = -45;
//				Outer.Target  = 9500;
//				jiaoduweizhisuduhuan();
//			}
//			else
//			{
//				// ======================
//				// 阶段2：斜走完成 → 判断是否在线上
//				// ======================
//				uint8_t has_black = 0;
//				if(IR[0]==1 || IR[1]==1 || IR[2]==1 || IR[3]==1 || IR[4]==1 || IR[5]==1 || IR[6]==1)
//				{
//					has_black = 1; // 有黑线 → 在线上
//				}
//				else
//				{
//					has_black = 0; // 全白 → 不在线上
//				}
//
//				// ======================
//				// 执行：在线循迹 / 不在线左转
//				// ======================
//				if(has_black == 1)
//				{
//					// ✅ 在线 → 循迹
//					Track_7IR();
//					suduhuan();
//				}
//				else
//				{
//					// ❌ 不在线 → 原地左转找线
//					suduhuan_zizhuan(0.0f, 30.0f);
//				}
//			}
//						sprintf(text, "%.2f,%.2f,%.2f\r\n",Inner_left.Actual,Inner_right.Actual,Angle_PID.Actual);
//						HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), 100);
//**************************第三题半圈****************************




//			**************************第三题1圈****************************
//			uint8_t status = LSM6DSV_ReadReg(0x1E);
//			if((status & 0x03) == 0x03)
//			{
//			    int16_t gx, gy, gz;
//			    int16_t ax, ay, az;
//			    LSM6DSV_ReadGyro(&gx, &gy, &gz);
//			    LSM6DSV_ReadAccel(&ax, &ay, &az);
//
//			    float gz_dps = gz * 4.375f / 1000.0f;
//			    if(fabs(gz_dps) < 3.0f) gz_dps = 0.0f;
//			    angle_z += gz_dps * 0.04f;
//			}
//
//			// 读取红外
//			IR[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
//			IR[1] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
//			IR[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
//			IR[3] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
//			IR[4] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
//			IR[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
//			IR[6] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
//
//			// 判断有没有黑线
//
//
//			uint8_t has_black = 0;
//
////			uint8_t white=0;
////			if(IR[0]==0 && IR[1]==0 && IR[2]==0 && IR[3]==0 && IR[4]==0 && IR[5]==0 && IR[6]==0)
////			{
////				white = 1;
////			}
//			if(IR[0]==1 || IR[1]==1 || IR[2]==1 || IR[3]==1 || IR[4]==1 || IR[5]==1 || IR[6]==1)
//			{
//
//			    has_black = 1;
//			}
//
//			// ==============================================
//			// 你要的最终完美逻辑
//			// ==============================================
//			if(go_trace_flag == 0)       // 1. 先斜着走 -45° 9500
//			{
//
//				if(flag==0)
//				{
//
//					Inner_left.Error0 = 0;
//					Inner_left.Error1 = 0;
//					Inner_left.ErrorInt = 0;
//					Inner_right.Error0 = 0;
//					Inner_right.Error1 = 0;
//					Inner_right.ErrorInt = 0;
//					Outer.Error0=0;
//					Outer.Error1=0;
//					Outer.ErrorInt=0;
//					Angle_PID.Error0 = 0;
//					Angle_PID.Error1 = 0;
//					Angle_PID.ErrorInt = 0;
////				    Outer.Out = 0;
////				    Angle_PID.Out = 0;
//				    location = 0;
//
//					flag=1;
//				}
//
//			    target_angle = -35;
//			    Outer.Target  = 9500;
//			    jiaoduweizhisuduhuan();
//			}
//			else if(go_trace_flag == 1)  // 2. -45°走完：判断是否要左转找线
//			{
//			    if(has_black)
//			    {
//			        // 有黑线 → 直接循迹
////			    	if(flag11==0)
////			    	{
////			    		Inner_left.Error0 = 0;
////						Inner_left.Error1 = 0;
////						Inner_left.ErrorInt = 0;
////						Inner_right.Error0 = 0;
////						Inner_right.Error1 = 0;
////						Inner_right.ErrorInt = 0;
////						flag11=1;
////			    	}
//
//
//			        go_trace_flag = 2;
//			    }
//			    else
//			    {
//			        // 全白 → 左转找线
//			        suduhuan_zizhuan(0.0f, 40.0f);
//			    }
//			}
//			else if(go_trace_flag == 2)  // 3. 循迹模式
//			{
//			    if(has_black)
//			    {
//			        // 有黑线 → 正常循迹
//			        Track_7IR();
//			        suduhuan();
//			    }
//			    else
//			    {
//			        // 全白 → 去走 215°
//			    	static uint8_t out_times = 0;
//					out_times++;
//
//					if(out_times >= 2)   // 两次离线 = 一圈跑完
//					{
//						go_trace_flag = 0;   // ✅ 回到起点 -40°
//						out_times = 0;       // 清零计数
//						location = 0;
//					}
//					else
//					{
//						go_trace_flag = 3;  // 第一次离线 → 走220°
//					}
//			        location = 0;
//
//			    }
//			}
//			else if(go_trace_flag == 3)  // 4. 走 215° 9500
//			{
//
//
//				if(flag==1)
//				{
//					Inner_left.Error0 = 0;
//					Inner_left.Error1 = 0;
//					Inner_left.ErrorInt = 0;
//					Inner_right.Error0 = 0;
//					Inner_right.Error1 = 0;
//					Inner_right.ErrorInt = 0;
//					Outer.Error0=0;
//					Outer.Error1=0;
//					Outer.ErrorInt=0;
//					Angle_PID.Error0 = 0;
//					Angle_PID.Error1 = 0;
//					Angle_PID.ErrorInt = 0;
////				    Outer.Out = 0;
////				    Angle_PID.Out = 0;
//				    location = 0;
//					flag=0;
//				}
//				target_angle = 225;
//				Outer.Target  = 10000;
//				jiaoduweizhisuduhuan();
//
//
//			}
//			else if(go_trace_flag == 4)  // 5. 215°走完：判断是否要右转找线
//			{
//
//			    if(has_black)
//			    {
//
//
//			        // 有黑线 → 直接循迹
//			        go_trace_flag = 2;
//
//
//			    }
//			    else
//			    {
//			        // 全白 → 右转找线
//			        suduhuan_zizhuan(40.0f, 0.0f);
//
//			    }
//			}

//	        Track_7IR();
//	        suduhuan();
			//**************************第三题1圈****************************




			//**************************第三题测试****************************
						uint8_t status = LSM6DSV_ReadReg(0x1E);
						if((status & 0x03) == 0x03)
						{
							int16_t gx, gy, gz;
							int16_t ax, ay, az;
							LSM6DSV_ReadGyro(&gx, &gy, &gz);
							LSM6DSV_ReadAccel(&ax, &ay, &az);

							float gz_dps = gz * 4.375f / 1000.0f;
							if(fabs(gz_dps) < 3.0f) gz_dps = 0.0f;
							angle_z += gz_dps * 0.04f;
						}

						// 读取红外
//						IR[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
//						IR[1] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
//						IR[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
//						IR[3] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
//						IR[4] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
//						IR[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
//						IRz = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
			            Track_7IR();
			            du1();
			            du2();



						switch(mode)
						{
						case 0:Mode_1();break;
						case 1:Mode_2();break;
						case 2:Mode_3();break;
						case 3:Mode_4();break;
						case 4:Mode_5();break;
						case 5:Mode_6();break;
						}

//						// 判断有没有黑线
//						static uint8_t quanshu=0;
//						static uint8_t quanshu2=0;
//
//
//						uint8_t has_black = 0;
//
//			//			uint8_t white=0;
//			//			if(IR[0]==0 && IR[1]==0 && IR[2]==0 && IR[3]==0 && IR[4]==0 && IR[5]==0 && IR[6]==0)
//			//			{
//			//				white = 1;
//			//			}
//						if(IR[0]==1 || IR[1]==1 || IR[2]==1 || IR[3]==1 || IR[4]==1 || IR[5]==1 || IR[6]==1)
//						{
//
//						    has_black = 1;
//						}
//
//						// ==============================================
//						// 你要的最终完美逻辑
//						// ==============================================
//						if(go_trace_flag == 0)       // 1. 先斜着走 -45° 9500
//						{
//
//							if(flag==0)
//							{
//								quanshu++;
//
//								Inner_left.Error0 = 0;
//								Inner_left.Error1 = 0;
//								Inner_left.ErrorInt = 0;
//								Inner_right.Error0 = 0;
//								Inner_right.Error1 = 0;
//								Inner_right.ErrorInt = 0;
//								Outer.Error0=0;
//								Outer.Error1=0;
//								Outer.ErrorInt=0;
//								Angle_PID.Error0 = 0;
//								Angle_PID.Error1 = 0;
//								Angle_PID.ErrorInt = 0;
//							    Outer.Out = 0;
//							    Angle_PID.Out = 0;
//							    location = 0;
//
//								flag=1;
//							}
//
//							if(quanshu==1)
//							{
//								angle_z=0;
//								quanshu=2;
//
//							}
//							else if(quanshu==2)
//							{
//								angle_z=20;
//								quanshu=1;
////								target_angle = (-40+(quanshu-1)*(15-(quanshu-1)));
////								 target_angle = (-40+(quanshu-1)*15);
////								 Outer.Target  = (9500+(quanshu-1)*(600-quanshu*50));
////								Outer.Target  = 9500+(quanshu-1)*600;
//							}
////							else if(quanshu==3)
////							{
////								target_angle = -8;
////								Outer.Target  =10000;
//////								target_angle = (-40+(quanshu-1)*(15-(quanshu-1)));
//////								 target_angle = (-40+(quanshu-1)*15);
//////								 Outer.Target  = (9500+(quanshu-1)*(600-quanshu*50));
//////								Outer.Target  = 9500+(quanshu-1)*600;
////							}
////							else if(quanshu==4)
////							{
////								target_angle =10;
////								Outer.Target  =10000;
//////								target_angle = (-40+(quanshu-1)*(15-(quanshu-1)));
//////								 target_angle = (-40+(quanshu-1)*15);
//////								 Outer.Target  = (9500+(quanshu-1)*(600-quanshu*50));
//////								Outer.Target  = 9500+(quanshu-1)*600;
////							}
//							 target_angle = -45;
//							Outer.Target  = 10000;
//
//						    jiaoduweizhisuduhuan();
//						}
//						else if(go_trace_flag == 1)  // 2. -45°走完：判断是否要左转找线
//						{
//						    if(has_black)
//						    {
//						        // 有黑线 → 直接循迹
//			//			    	if(flag11==0)
//			//			    	{
//			//			    		Inner_left.Error0 = 0;
//			//						Inner_left.Error1 = 0;
//			//						Inner_left.ErrorInt = 0;
//			//						Inner_right.Error0 = 0;
//			//						Inner_right.Error1 = 0;
//			//						Inner_right.ErrorInt = 0;
//			//						flag11=1;
//			//			    	}
//
//
//						        go_trace_flag = 2;
//						    }
//						    else
//						    {
//						        // 全白 → 左转找线
//						        suduhuan_zizhuan(0.0f, 45.0f);
//						    }
//						}
//						else if(go_trace_flag == 2)  // 3. 循迹模式
//						{
//						    if(has_black)
//						    {
//						        // 有黑线 → 正常循迹
//						        Track_7IR();
//						        suduhuan();
//						    }
//						    else
//						    {
//						        // 全白 → 去走 215°
//						    	static uint8_t out_times = 0;
//								out_times++;
//
//								if(out_times >= 2)   // 两次离线 = 一圈跑完
//								{
//									go_trace_flag = 0;   // ✅ 回到起点 -40°
//									out_times = 0;       // 清零计数
//									location = 0;
//								}
//								else
//								{
//									go_trace_flag = 3;  // 第一次离线 → 走220°
//								}
//						        location = 0;
//
//						    }
//						}
//						else if(go_trace_flag == 3)  // 4. 走 215° 9500
//						{
//
//
//
//							if(flag==1)
//							{
//								quanshu2=1;
//
////								Inner_left.Error0 = 0;
////								Inner_left.Error1 = 0;
////								Inner_left.ErrorInt = 0;
////								Inner_right.Error0 = 0;
////								Inner_right.Error1 = 0;
////								Inner_right.ErrorInt = 0;
////								Outer.Error0=0;
////								Outer.Error1=0;
////								Outer.ErrorInt=0;
////								Angle_PID.Error0 = 0;
////								Angle_PID.Error1 = 0;
////								Angle_PID.ErrorInt = 0;
////							    Outer.Out = 0;
////							    Angle_PID.Out = 0;
//							    location = 0;
//								flag=0;
//							}
//							if(quanshu2==1)
//							{
//								angle_z=-20;
//								quanshu2=0;
//							}
////							else if(quanshu2==4||quanshu2==5||quanshu2==6||quanshu2==7||quanshu2==8)
////							{
////
////							}
////							else if(quanshu2==2)
////							{
////								target_angle = 215;
////								Outer.Target  = 10200;
//////								target_angle = 220+(quanshu2-1)*15;
//////								Outer.Target  = 10000+(quanshu2-1)*500;
////
//////								Outer.Target  = (10000+(quanshu2-1)*(500-quanshu2*50));
//////								target_angle = (220+(quanshu2-1)*(15-quanshu2));
////
////							}
////							else if(quanshu2==3)
////							{
////								target_angle = 235;
////								Outer.Target  = 10400;
//////								target_angle = 220+(quanshu2-1)*15;
//////								Outer.Target  = 10000+(quanshu2-1)*500;
////
//////								Outer.Target  = (10000+(quanshu2-1)*(500-quanshu2*50));
//////								target_angle = (220+(quanshu2-1)*(15-quanshu2));
////
////							}
////							else if(quanshu2==4)
////							{
////								target_angle = 245;
////								Outer.Target  = 10600;
//////								target_angle = 220+(quanshu2-1)*15;
//////								Outer.Target  = 10000+(quanshu2-1)*500;
////
//////								Outer.Target  = (10000+(quanshu2-1)*(500-quanshu2*50));
//////								target_angle = (220+(quanshu2-1)*(15-quanshu2));
////
////							}
//
//
//
//
//
//							target_angle = 45;
//							Outer.Target  = 10000;
//							jiaoduweizhisuduhuan();
//
//
//						}
//						else if(go_trace_flag == 4)  // 5. 215°走完：判断是否要右转找线
//						{
//
//						    if(has_black)
//						    {
//
//
//						        // 有黑线 → 直接循迹
//						        go_trace_flag = 2;
//
//
//						    }
//						    else
//						    {
//						        // 全白 → 右转找线
//						        suduhuan_zizhuan(45.0f, 0.0f);
//
//						    }
//						}




			//**************************第三题测试****************************











//
//
//			Track_7IR();
//							 suduhuan();

//第二题*******************************************************************
//			uint8_t status = LSM6DSV_ReadReg(0x1E);
//			if((status & 0x03) == 0x03)
//			{
//				int16_t gx, gy, gz;
//				int16_t ax, ay, az;
//				LSM6DSV_ReadGyro(&gx, &gy, &gz);
//				LSM6DSV_ReadAccel(&ax, &ay, &az);
//
//				float gz_dps = gz * 4.375f / 1000.0f;  //4.375
//				if(fabs(gz_dps) < 3.0f) gz_dps = 0.0f;
//				angle_z += gz_dps * 0.04f;
//			}
//
//			IR[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
//			IR[1] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
//			IR[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
//			IR[3] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
//			IR[4] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
//			IR[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
//			IR[6] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
//			 if(IR[0]==0 && IR[1]==0 && IR[2]==0 && IR[3]==0 && IR[4]==0 && IR[5]==0 && IR[6]==0)
//			{
//				this_white_flag = 1;
//			}
//			 if(this_white_flag == 1)
//			 {
//				 Angle_PID.Error0 = 0;
//				 Angle_PID.ErrorInt = 0;
//				 Angle_PID.Error1 = 0;
//
//				 jiaodusuduhuan();
//			 }
//			 if(IR[0]==1 || IR[1]==1 || IR[2]==1 || IR[3]==1 || IR[4]==1 || IR[5]==1 || IR[6]==1)
//			 {
//
//				 this_white_flag = 0;
////				 Angle_PID.Error0 = 0;
////				 Angle_PID.ErrorInt = 0;
////				 Angle_PID.Error1 = 0;
//				 Track_7IR();
//				 suduhuan();
//
//			 }
//			 if(this_white_flag == 0&& last_white_flag == 1)
//			 {
////				 flag++;
////				 if(flag%2==1)
////				 {
////					 target_angle=-176.0f;
////				 }
////				 else
////				 {
////					 target_angle=-90.0f;
////				 }
//				 target_angle -= 176.0f;//175往外,185往里;
//			 }
//			 last_white_flag=this_white_flag;
//第二题*******************************************************************














//			if(flag == 0)
//			{
//			    Outer.Target = 7700;
//			    // 第一次走 0，后面全部走 180
//			    if(first_run%2==0)//直线
//			    {
//			    	//angle_z=0;
//			        target_angle = 0;
//			    }
//			    else
//			    {
//			        target_angle = -180;   // 永远保持 180，不会变 0！
//			    }
//			    // 看到黑线
//			    if(IR[0]==1 || IR[1]==1 || IR[2]==1 || IR[3]==1 || IR[4]==1 || IR[5]==1 || IR[6]==1)
//			    {
//			    	this_white_flag=0;
//			    }
//			}
//			else if(flag == 1)
//			{
//			    // 全黑保护
//			    if(IR[0]==1 && IR[1]==1 && IR[2]==1 && IR[3]==1 && IR[4]==1 && IR[5]==1 && IR[6]==1)
//			    {
//			        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
//			        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
//			    }
//			    // 全白 → 回到直线
//			    else if(IR[0]==0 && IR[1]==0 && IR[2]==0 && IR[3]==0 && IR[4]==0 && IR[5]==0 && IR[6]==0)
//			    {
//			    	first_run++;
//			        target_angle = (first_run % 2 == 0) ? 0 : -180;
//			        Outer.Target = 7700;
//			        location = 0;
//			        flag = 0;
//
//			    }
//			}

//			if(flag == 0)
//			{
//				jiaoduweizhisuduhuan();
//			}
//			else
//			{
//			    Track_7IR();
//			    suduhuan();
//			}
//			last_white_flag=this_white_flag;
//
//			sprintf(text, "%.2f,%.2f,%.2f\r\n",Inner_left.Actual,Inner_right.Actual,Angle_PID.Actual);
//			HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), 100);



		}
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
