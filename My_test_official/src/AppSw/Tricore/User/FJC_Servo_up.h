#ifndef _FJC_SERVO_UP_H
#define _FJC_SERVO_UP_H
#include "FJC_Servo_t.h"
#include "CXJ_MOTOR.h"

extern uint16_t ad_VAL[6];           //滤波归一化后ad数据

void Servo_Send();
#endif
