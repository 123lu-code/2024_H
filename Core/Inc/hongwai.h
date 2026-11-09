#ifndef _HONGWAI_H
#define _HONGWAI_H

#include "main.h"
#include "stdint.h"
#include "pid.h"   // 必须加
void hw(void);
uint8_t Tracking(double *LB_Target,double *RB_Target);
void Trace_task(void);
void six_red(void);
void Track_7IR(void);
#endif
