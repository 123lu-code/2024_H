#include "main.h"
#include "pid.h"

/**
  * 函    数：PID计算及结构体变量值更新
  * 参    数：PID_t * 指定结构体的地址
  * 返 回 值：无
  */
void Motor_PID_Calculate_zl(Motor_1 *pid)//增量式
{
	pid->Error[2]=pid->Error[1];
	pid->Error[1]=pid->Error[0];
	pid->Error[0]=pid->Target-pid->Actual;
	pid->Out+= pid->Kp * (pid->Error[0] - pid->Error[1])
			 + pid->Ki * pid->Error[0]
			 + pid->Kd * (pid->Error[0] - 2*pid->Error[1] + pid->Error[2]);
	if(pid->Out>pid->OutMax)
	{
		pid->Out=pid->OutMax;
	}
	if(pid->Out<pid->OutMin)
	{
		pid->Out=pid->OutMin;
	}

}
void Motor_PID_Calculate_zl_angle(Motor_1 *pid)//增量式（角度）
{
	pid->Error[2]=pid->Error[1];
	pid->Error[1]=pid->Error[0];
	pid->Error[0]=pid->Target-pid->Actual;
	pid->Out+= pid->Kp * (pid->Error[0] - pid->Error[1])
			 + pid->Ki * pid->Error[0]
			 + pid->Kd * (pid->Error[0] - 2*pid->Error[1] + pid->Error[2]);
	if(fabs(pid->Error[1])<1)
	{
		pid->Out=0;
	}
	if(pid->Out>pid->OutMax)
	{
		pid->Out=pid->OutMax;
	}
	if(pid->Out<pid->OutMin)
	{
		pid->Out=pid->OutMin;
	}

}





void PID_Update(PID_t *p)
{
	/*获取本次误差和上次误差*/
	p->Error1 = p->Error0;					//获取上次误差
	p->Error0 = p->Target - p->Actual;		//获取本次误差，目标值减实际值，即为误差值

	/*外环误差积分（累加）*/
	/*如果Ki不为0，才进行误差积分，这样做的目的是便于调试*/
	/*因为在调试时，我们可能先把Ki设置为0，这时积分项无作用，误差消除不了，误差积分会积累到很大的值*/
	/*后续一旦Ki不为0，那么因为误差积分已经积累到很大的值了，这就导致积分项疯狂输出，不利于调试*/
	if (p->Ki != 0)					//如果Ki不为0
	{
		p->ErrorInt += p->Error0;	//进行误差积分
	}
	else							//否则
	{
		p->ErrorInt = 0;			//误差积分直接归0
	}

	/*PID计算*/
	/*使用位置式PID公式，计算得到输出值*/
	p->Out = p->Kp * p->Error0
		   + p->Ki * p->ErrorInt
		   + p->Kd * (p->Error0 - p->Error1);

	/*输出限幅*/
	if (p->Out > p->OutMax) {p->Out = p->OutMax;}	//限制输出值最大为结构体指定的OutMax
	if (p->Out < p->OutMin) {p->Out = p->OutMin;}	//限制输出值最小为结构体指定的OutMin
}

//void PID_Update_zl(PID_t *p)
//{
//	p->ErrorInt=p->Error1;
//	p->Error1=p->Error0;
//	p->Error0=p->Target-p->Actual;
//	p->Out+= p->Kp * (p->Error0 - p->Error1)
//			 + p->Ki * p->Error0
//			 + p->Kd * (p->Error0 - 2*p->Error1 +p-> ErrorInt);
//	if(fabs(p->Error1)<1)
//	{
//		p->Out=0;
//	}
//	if(p->Out>p->OutMax)
//	{
//		p->Out=p->OutMax;
//	}
//	if(p->Out<p->OutMin)
//	{
//		p->Out=p->OutMin;
//	}
//}



void Motor_PID_Calculate_wz(Motor *p)//位置式
{
	p->Error1=p->Error0;
	p->Error0=p->Target-p->Actual;
	if(p->Ki !=0)
	{
		p->ErrorInt+=p->Error0;
	}
	else
	{
		p->ErrorInt = 0;
	}
	p->Out = p->Kp * p->Error0
			 + p->Ki * p->ErrorInt
			 + p->Kd * (p->Error0 - p->Error1);
	if(fabs(p->Error0)<50)
	{
		p->Out=0;
	}

	if(p->Out > p->OutMax)
	{
		p->Out=p->OutMax;
	}
	if(p->Out < p->OutMin)
	{
		p->Out=p->OutMin;
	}

}

