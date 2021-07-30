/*
 * PID.c
 *
 *  Created on: 2021年4月5日
 *      Author: admin
 */
#include "CXJ_MOTOR.h"
PID sptr; //电机PID结构体声明
extern int encValue5;

int realpwm = 0;
/*************************************************************************
*  函数名称：void Set_Speed(unsigned int tar_speed)
*  功能说明：PID控制电机前进,只要我们设定目标PWM值即可,自动限幅(2000,6000)
*  参数说明：tar_speed是目标PWM,由我们任意设定
*  修改时间：2021年4月4日
*************************************************************************/
void Set_Speed(unsigned int tar_speed)
{
    unsigned int tmp_PWM =0;
    int tar_ENC = (int)(0.7676*tar_speed - 1048); //目标编码器的值
    static unsigned int out_ENC = 0;
    volatile  int tmp_ENC = 0;
    char txt[20],txt1[10];

    // 拿到当前编码器的值
    realpwm = (unsigned int)(1.2993*encValue5 + 1369.5);
//        sprintf(txt,"tar_enc:%d\r\n",tar_ENC);
//        UART_PutStr(UART0,txt);

   // 输入PID得到应该设置的占空比的值
   out_ENC += PID_MOTOR_Contrl(tar_ENC,encValue5);    //将目标的ENC和现在的ENC比较
//   sprintf(txt,"out_enc:%d  ",out_ENC);
//   UART_PutStr(UART0,txt);
   out_ENC = constrain_int(out_ENC,0,tar_ENC+20);
   //打印调试

//    sprintf(txt,"tmp_enc:%d    ",encValue5);
//    UART_PutStr(UART0,txt);

   tmp_PWM = (unsigned int)(1.2993*out_ENC + 1369.5);                 //将结果转化成PWM
//   sprintf(txt,"out_pwm:%d\r\n",tmp_PWM);
//   UART_PutStr(UART0,txt);
   //设置速度
   tmp_PWM = constrain_int(tmp_PWM,0,5000);
   ATOM_PWM_SetDuty(ATOMPWM0,tmp_PWM, 12500);
   ATOM_PWM_SetDuty(ATOMPWM1,0, 12500);
}


/*************************************************************************
*  函数名称：void Set_Speedback(unsigned int tar_speed)
*  功能说明：PID控制电机后退,只要我们设定目标PWM值即可,自动限幅(2000,6000)
*  参数说明：tar_speed是目标PWM,由我们任意设定
*  修改时间：2021年4月4日
*************************************************************************/
void Set_Speedback(unsigned int tar_speed)
{
    unsigned int tmp_PWM =0;
    int tar_ENC = (int)(0.7676*tar_speed - 1048); //目标编码器的值
    static short out_ENC = 0;
    volatile  int tmp_ENC = 0;
    char txt[20],txt1[10];

    // 拿到当前编码器的值

   // 输入PID得到应该设置的占空比的值
   out_ENC += PID_MOTOR_Contrl(tar_ENC,-encValue5);    //将目标的ENC和现在的ENC比较
   out_ENC = constrain_int(out_ENC,tar_ENC,tar_ENC+10);

   tmp_PWM = 1.2993*out_ENC + 1369.5;                 //将结果转化成PWM
   //设置速度
   ATOM_PWM_SetDuty(ATOMPWM0,0, 12500);
   ATOM_PWM_SetDuty(ATOMPWM1,tmp_PWM, 12500);
}

void Test_BMQ()
{
    char txt[10];
    char txt_PWM[10];
    unsigned int tmp_PWM = 0;
    volatile int tmp = 0;

//    encValue5 = -ENC_GetCounter(ENC6_InPut_P20_3);
//    tmp_PWM = 54.648*tmp + 1100;
//    sprintf(txt_PWM,"PWM:%d    ",3500);
//    UART_PutStr(UART0,txt_PWM);

    ATOM_PWM_SetDuty(ATOMPWM0,2500, 12500);
    sprintf(txt,"cur:%d\r\n",encValue5);
    UART_PutStr(UART0,txt);
//    TFTSPI_P8X16Str(0, 1, txt,u16WHITE,u16BLACK);

}


/*************************************************
 *函数名称:void PID_Arg_Init(PID* sptr)
 *功    能:参数初始化
 *************************************************/
void PID_Arg_Init()
{
    sptr.SumError = 0;         //误差累计
    sptr.LastError = 0;        //上次偏差值
    sptr.PrevError = 0;        //上上次偏差值

    sptr.P = 1.8;                //比例常数 //0.8
    sptr.I = 1.0;                //积分常数 //0.01
    sptr.D = 0;                //微分常数
}
