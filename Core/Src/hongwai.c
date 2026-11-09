#include "hongwai.h"
#include "main.h"
#include "pid.h"

extern PID_t Inner_left;
extern PID_t Inner_right;

//#define BASE_SPEED_LEFT  90    // 基础速度（可改）
//#define BASE_SPEED_RIGHT  90    // 基础速度（可改）
//void hw(void)
//{
//	uint8_t ir1=0, ir2=0, ir3=0, ir4=0, ir5=0, ir6=0;
//	uint8_t all_white=0;
//	static uint8_t last_white = 0;
//
//	// 读取6路红外
//	ir1 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);  // 最外侧左
//	ir2 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);  // 中左
//	ir3 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);  // 内侧左
//	ir4 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);  // 内侧右
//	ir5 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);  // 中右
//	ir6 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);  // 最外侧右
//
//	// 判断是否全白（所有都亮）
//	all_white = (ir1 && ir2 && ir3 && ir4 && ir5 && ir6);
//
//	// ====================== 停车逻辑 ======================
//	if(last_white == 0 && all_white == 1)
//	{
//		// 第一次全白 → 停车
//		Inner_left.Target = 0;
//		Inner_right.Target = 0;
//		return;
//	}
//	last_white = all_white;
//
//	// ====================== 循迹逻辑 ======================
//	// 内侧 → 小转弯
//	// 外侧 → 大转弯
//
//	// ================= 左边碰到黑线 → 往右修 =================
//	if(ir3 == 0) // 内侧左 → 最小转弯
//	{
//		Inner_left.Target  = BASE_SPEED_LEFT + 6;
//		Inner_right.Target = BASE_SPEED_RIGHT - 6;
//	}
//	else if(ir2 == 0) // 中左 → 中等转弯
//	{
//		Inner_left.Target  = BASE_SPEED_LEFT + 13;
//		Inner_right.Target = BASE_SPEED_RIGHT - 13;
//	}
//	else if(ir1 == 0) // 最外侧左 → 最大转弯
//	{
//		Inner_left.Target  = BASE_SPEED_LEFT + 20;
//		Inner_right.Target = BASE_SPEED_RIGHT - 20;
//	}
//
//	// ================= 右边碰到黑线 → 往左修 =================
//	else if(ir4 == 0) // 内侧右 → 最小转弯
//	{
//		Inner_left.Target  = BASE_SPEED_LEFT - 6;
//		Inner_right.Target = BASE_SPEED_RIGHT + 6;
//	}
//	else if(ir5 == 0) // 中右 → 中等转弯
//	{
//		Inner_left.Target  = BASE_SPEED_LEFT - 13;
//		Inner_right.Target = BASE_SPEED_RIGHT + 13;
//	}
//	else if(ir6 == 0) // 最外侧右 → 最大转弯
//	{
//		Inner_left.Target  = BASE_SPEED_LEFT - 20;
//		Inner_right.Target = BASE_SPEED_RIGHT + 20;
//	}
//
//	// 没碰到 → 直行
//	else
//	{
//		Inner_left.Target  = BASE_SPEED_LEFT;
//		Inner_right.Target = BASE_SPEED_RIGHT;
//	}
//}



// 正确的函数定义！！！
//uint8_t Tracking(double *LB_Target,double *RB_Target)
//{
//	uint8_t Infrared[7];
//	uint8_t Infrared_temp[7];
//	uint8_t i;
//	static uint8_t white_sum;
//	int16_t deviation=0;
//	int16_t weight_sum=0;
//	int16_t dev_values_sum=0;
//	uint8_t weights[7]={1,2,3,4,3,2,1};
//	int8_t dev_values[7]={-45,-35,-15,5,15,35,45};
//	Infrared[0]=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
//	Infrared[1]=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
//	Infrared[2]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
//	Infrared[3]=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15);
//	Infrared[4]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
//	Infrared[5]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
//	Infrared[6]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
//	for(i=0;i<7;i++)
//	{
//		if(Infrared[i]==1)
//		{
//			weight_sum += weights[i];
//			dev_values_sum += dev_values[i]*weights[i];
//		}
//	}
//	if(weight_sum==0)
//	{
//		white_sum++;
//		if(white_sum>=3)
//		{
//			white_sum=0;
//			*LB_Target=0;
//			*RB_Target=0;
//			dev_values_sum=0;
//			deviation=0;
//			return 1;
//		}
//	}
//	else
//	{
//		white_sum=0;
//		deviation = dev_values_sum/weight_sum;
//		*LB_Target=90+deviation;
//		*RB_Target=90-deviation;
//	}
//	return 0;
//
//}
//uint8_t Tracking(double *LB_Target,double *RB_Target)
//{
//	uint8_t Infrared[7];
//	uint8_t i;
//	static uint8_t white_sum = 0;
//
//	int16_t deviation = 0;
//	int16_t weight_sum = 0;
//	int16_t dev_values_sum = 0;
//
//	// 7路红外权重（中间最大，越外侧越小）
//	uint8_t weights[7]    = {1, 2, 3, 4, 3, 2, 1};
//
//	// 7路偏移值：中间=0（直行），越外侧偏移越大
//	// 0:左6  1:左5  2:左4  3:中间  4:右4  5:右5  6:右6
//	int8_t  dev_values[7] = {-45, -25, -10, 0, 10, 25, 45};
//
//	// ========== 读取7路红外 ==========
//	Infrared[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
//	Infrared[1] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
//	Infrared[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
//	Infrared[3] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);  // 第4路（中间）
//	Infrared[4] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
//	Infrared[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
//	Infrared[6] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
//
//	// ========== 加权计算 ==========
//	for(i=0;i<7;i++)
//	{
//		// 0 = 碰到黑线，1 = 白色
//		if(Infrared[i] == 0)
//		{
//			weight_sum += weights[i];
//			dev_values_sum += dev_values[i] * weights[i];
//		}
//	}
//
//	// ========== 全白检测（停车逻辑） ==========
//	if(weight_sum == 0)
//	{
//		white_sum++;
//		if(white_sum >= 3)  // 连续3次全白再停，防止误停
//		{
//			white_sum = 0;
//			*LB_Target = 0;
//			*RB_Target = 0;
//			return 1;
//		}
//	}
//	else
//	{
//		white_sum = 0;
//		deviation = dev_values_sum / weight_sum;
//
//		// 基础速度 35（你原来90太快，会飞车！）
//		*LB_Target = 35 + deviation;
//		*RB_Target = 35 - deviation;
//	}
//
//	return 0;
//}

//void Trace_task(void)
//{
//	uint8_t X1=0,X2=0,X3=0,X4=0,X5=0,X6=0,X7=0;
//	X1=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
//	X2=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
//	X3=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
//	X4=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15);
//	X5=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
//	X6=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
//	X7=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
//
//
//	    // 基础速度（和你原来的4000对应，可改）
//	    const int16_t BASE_SPEED = 30;
//
//	    // ========== 中间碰到黑线 → 直行 ==========
//	    if (X4 == 0)
//	    {
//	    	Inner_left.Target=BASE_SPEED;
//	    	Inner_right.Target=BASE_SPEED;
//
//	    }
//
//	    // ========== 左半部分碰到黑线 → 右转 ==========
//	    else if (X3 == 0)   // 内侧左 → 轻微右转
//	    {
//	    	Inner_left.Target=BASE_SPEED;
//	        Inner_right.Target=BASE_SPEED*0.7f;
//
//	    }
//	    else if (X2 == 0)   // 中左 → 中度右转
//	    {
//	    	Inner_left.Target=BASE_SPEED;
//	        Inner_right.Target=BASE_SPEED*0.3f;
//
//	    }
//	    else if (X1 == 0)   // 最外侧左 → 原地右转
//	    {
//	    	Inner_left.Target=BASE_SPEED;
//	        Inner_right.Target=0;
//
//	    }
//
//	    // ========== 右半部分碰到黑线 → 左转 ==========
//	    else if (X5 == 0)   // 内侧右 → 轻微左转
//	    {
//	    	Inner_left.Target=BASE_SPEED* 0.7f;
//	        Inner_right.Target=BASE_SPEED;
//	    }
//	    else if (X6 == 0)   // 中右 → 中度左转
//	    {
//	    	Inner_left.Target=BASE_SPEED* 0.3f;
//	        Inner_right.Target=BASE_SPEED;
//	    }
//	    else if (X7 == 0)   // 最外侧右 → 原地左转
//	    {
//	    	Inner_left.Target=0;
//	    	Inner_right.Target=BASE_SPEED;
//	    }
//
//	    // ========== 没碰到任何黑线 → 直行 ==========
//	    else
//	    {
//	    	Inner_left.Target=BASE_SPEED;
//	    		    	Inner_right.Target=BASE_SPEED;
//	    }
//
////	if(Infrared[2]==1&&Infrared[3]==0&&Infrared[4]==1)
////	{
////		Inner_left.Target=60.0;
////		Inner_right.Target=60.0;
////	}
////	if(Infrared[2]==0&&Infrared[3]==0&&Infrared[4]==1)
////	{
////		Inner_left.Target=0;
////		Inner_right.Target=60.0;
////	}
////	if(Infrared[2]==1&&Infrared[3]==0&&Infrared[4]==0)
////	{
////		Inner_left.Target=60.0;
////		Inner_right.Target=0;
////	}
//}
//uint8_t Tracking(double *LB_Target,double *RB_Target)
//{
//	uint8_t Infrared[6];
//	uint8_t Infrared_temp[6];
//	uint8_t i;
//	static uint8_t white_sum;
//	int16_t deviation=0;
//	int16_t weight_sum=0;
//	int16_t dev_values_sum=0;
//	uint8_t infrar;
//	uint8_t weights[6]={1,2,3,3,2,1};
//	int8_t dev_values[6]={-20,-15,-5,5,15,20};
//	Infrared[0]=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
//	Infrared[1]=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
//	Infrared[2]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
//	Infrared[3]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
//	Infrared[4]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
//	Infrared[5]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
//	infrar=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15);
//	for(i=0;i<6;i++)
//	{
//		if(Infrared[i]==1)
//		{
//			weight_sum += weights[i];
//			dev_values_sum += dev_values[i]*weights[i];
//		}
//	}
//	if(weight_sum==0 && infrar==0)
//	{
//
//			white_sum=0;
////			LB_PID.Target=0;
////			RB_PID.Target=0;
//			dev_values_sum=0;
//			deviation=0;
//			return 1;
//
//	}
//	else
//	{
//		white_sum=0;
//		deviation = dev_values_sum/weight_sum;
//		*LB_Target=60+deviation;
//		*RB_Target=60-deviation;
//	}
//
//	return 0;
//
//}
// 7路红外循迹（只用速度环，无角度环，不转圈）
float weight_sum=0,value_sum = 0.0f ;
uint8_t weights[6] =    {1,2,3,3,2,1};
//int8_t  dev_values[6] = {-45,-35,-5,5,35,45};
////int8_t  dev_values[6] = {-45,-20,-5,5,20,45};
//int8_t  dev_values[6] = {-45,-25,-5,5,25,45};
//int8_t  dev_values[6] = {-35,-25,-5,5,25,35};
//int8_t  dev_values[6] = {-15,-10,-5,5,10,15};
int8_t  dev_values[6] = {-55,-45,-15,15,45,55};
float deviation = 0;
uint8_t IR[7];
uint8_t IRz;
void Track_7IR(void)
{

	weight_sum = 0.0f;    // 所有检测到黑线的红外管的权重总和
	value_sum = 0.0f;     // 所有检测到黑线的红外管的"加权偏差值"总和

	    // 读取 7 路红外（按你实际引脚）
	    IR[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
	    IR[1] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	    IR[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
	    IR[3] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	    IR[4] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
	    IR[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
	    IRz = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
		int i = 0;
		for(i = 0; i < 6; i++)
		{
		    if(IR[i] == 1)
		    {

		        weight_sum += weights[i];// 累加该红外管的权重
		        value_sum += dev_values[i] * weights[i];// 累加"偏差值 × 权重"
		    }
		}
		if(weight_sum > 0)
		{
		    deviation = value_sum / weight_sum;// 最终偏差 = 加权偏差总和 / 权重总和
		}
		else
		{
		    deviation = 0.0f; // 没线直行
		}
		// 最佳循迹限幅（不抖、不冲、稳得一批）
//		if(deviation > 40) deviation = 40;   // 右极限
//		if(deviation < -40) deviation = -40; // 左极限

}
