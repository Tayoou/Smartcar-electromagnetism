/*
 * FJC_NewCS.h
 *
 *  Created on: 2021Äê4ÔÂ29ÈÕ
 *      Author: 96556
 */

#ifndef SRC_APPSW_TRICORE_USER_FJC_NEWCS_H_
#define SRC_APPSW_TRICORE_USER_FJC_NEWCS_H_

#include "FJC_Servo_t.h"
#include "PID.h"
#include "FJC_ControlServo.h"
#include "CXJ_MOTOR.h"
#include <LQ_Atom_Motor.h>

#include <LQ_GPIO_KEY.h>
#include <IfxGtm_PinMap.h>
#include <LQ_GTM.h>
#include <LQ_MotorServo.h>
#include <LQ_ADC.h>
#include <LQ_STM.h>
#include <LQ_UART.h>
#include <stdio.h>
#include <LQ_TFT18.h>
#include <LQ_GTM.h>
#include <LQ_GPT12_ENC.h>
#include <LQ_GPT_mini512.h>


float my_sqrt(float number);
float my_absolute(float number);
void aricomsumope();
float Vag_pid(float currval,float deltaval);
void sendFuzzyData();
float KalmanFilter_Elect(float curr_elect_val,float last_elect_val);
void Datadeal();
void DirJudge();
void DirJudge2();
void Loseline();
void AddListTill(float a,unsigned char Rem);
void SearchListTill(void);
void Mode_Choose(void);
void DJCrol();
void DJCrol2();

#endif /* SRC_APPSW_TRICORE_USER_FJC_NEWCS_H_ */
