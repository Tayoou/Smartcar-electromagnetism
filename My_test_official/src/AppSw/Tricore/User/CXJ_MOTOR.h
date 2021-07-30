/*
 * PID.h
 *
 *  Created on: 2021Äê4ÔÂ5ÈÕ
 *      Author: admin
 */

#ifndef SRC_APPSW_TRICORE_USER_PID_H_
#define SRC_APPSW_TRICORE_USER_PID_H_
#include "..\Driver\include.h"
#include "PID.h"
#include "stdbool.h"

#include <LQ_GPIO_LED.h>
#include <LQ_GPT12_ENC.h>
#include <LQ_OLED096.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <LQ_UART.h>
#include <stdint.h>
#include <stdio.h>

void Set_Speed(unsigned int tar_speed);
void Set_Speedback(unsigned int tar_speed);
void Test_BMQ();
void PID_Arg_Init();



#endif /* SRC_APPSW_TRICORE_USER_PID_H_ */
