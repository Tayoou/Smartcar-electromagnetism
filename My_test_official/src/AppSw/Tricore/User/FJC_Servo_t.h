/*
 * FJC_Servo.h
 *
 *  Created on: 2021Äê4ÔÂ3ÈÕ
 *      Author: 96556
 */

#ifndef SRC_APPSW_TRICORE_USER_FJC_SERVO_H_
#define SRC_APPSW_TRICORE_USER_FJC_SERVO_H_
#define MID_SER 1280

#include "LQ_ADC.h"
#include "LQ_STM.h"
#include "LQ_UART.h"
#include <stdio.h>
#include "LQ_TFT18.h"
#include "FJC_NEWCS.h"


float ForServoMID(float i,float j,float k);
void Get_ADC(void);
void My_ReadADC();
void My_ADCDisplay();
void My_ADCInit();
void Test_Origin();
void Test_error();


#endif /* SRC_APPSW_TRICORE_USER_FJC_SERVO_H_ */
