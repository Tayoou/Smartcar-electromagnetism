
/*
 * FJC_ControlServo.c
 *  Created on: 2021年4月3日
 */
#define ATOMSERVO1       IfxGtm_ATOM2_0_TOUT32_P33_10_OUT
#define ATOMSERVO2       IfxGtm_ATOM2_5_TOUT35_P33_13_OUT


#include "FJC_ControlServo.h"

PID s_pid;  // 舵机PD控制结构体


/* 全局变量 */
extern unsigned int ad_VAL[6];
unsigned int s_duty = Servo_Center_Mid;  // 默认中值
int group_err[3]={0};
int test1,test2;




/* 显示变量 */
char txt[20] = {0};
char txt_sum[20] = {0};
char txt_e1[10] = {0};
char txt_e2[10] = {0};
char txt_e3[10] = {0};
char txt_output[15] = {0};
char txt_pid[15] = {0};

char txt_kp[10] = {0};
char txt_kd[10] = {0};


/*************************************************************************
*  函数名称：signed int Dir_Judge()
*  功能说明：判断舵机偏转方向函数
*  函数返回：无
*  修改时间：2021年4月3日
*  备    注：左右传感器作差（注意：只作为测试，并未考虑电感排布情况！！）
*************************************************************************/
unsigned char Dir_Judge()
{
    static unsigned char res = 0;  // 小车当运行位置:0表示直道
    // 阈值判断方向
    if((ad_VAL[0] >= 40 && res == 0) && (ad_VAL[2] <=92 && ad_VAL[3] <= 92) || (res == 9 && ad_VAL[5] >= 90) || ((ad_VAL[0] + ad_VAL[5] >= 135) && res!=2))
        res = 1;        // 左转弯
    else if(ad_VAL[5] >= 40 && res == 0 && (ad_VAL[2] <=92 && ad_VAL[3] <= 92))
        res = 2;        // 右转弯
    // 直道
    else if((ad_VAL[2] >= 88 || ad_VAL[3] >= 88) && (ad_VAL[0]<=45 || ad_VAL[5] <= 45) &&
            (ad_VAL[0]<=75 && ad_VAL[5] <= 75))  // 这个阈值很重要
        res = 0;

    // 过渡状态的判断
    if(res == 2 && group_err[0] >= 0 && ad_VAL[0] != 100)
        res = 8;
    else if(res == 1 && group_err[0] <= 0 && ad_VAL[5] != 100 && ((ad_VAL[0]+ad_VAL[1])>ad_VAL[5]+ad_VAL[4]))
        res = 9;
    return res;
}

/*************************************************************************
*  函数名称：void Sterring_PID_Init(void)
*  功能说明：舵机PD参数初始化
*  函数调用：初始化调用即可
*************************************************************************/
void Sterring_PID_Init()
{
    s_pid.P = 1.5;
    s_pid.I = 0;
    s_pid.D = 0.0;
}

/*************************************************************************
*  函数名称：void SteeringCrol(void)
*  功能说明：舵机方向偏转,与舵机打角反应
*  参数说明：无
*  函数返回：无
*  修改时间：2021年4月3日
*  备    注：电磁传感器传入值作差
*************************************************************************/
void SteeringCrol()
{
    unsigned char pos = 0;
    int input_error = 0;
    int sum_err=0;
    int sum_dg = 0;           //滤波归一化后ad数据
    My_ReadADC();  // 采集电感值
    Get_ADC();
    /* 针对电感值进行做差计算 */
    for(int i=0;i<3;i++)
    {
//        group_err[i] = ((ad_VAL[i] - ad_VAL[5-i]) / (ad_VAL[i] + ad_VAL[5-i]+1)) * 100;  //注:这里通过两边电感值的偏差来得到车身相对与中心的位置

        group_err[i] =   ad_VAL[i] - ad_VAL[5-i];
    }

    /* 静态误差补偿 */
    group_err[0] += CPS_E1;
    group_err[1] += CPS_E2;
    group_err[2] += CPS_E3;

    /* TFT误差标定和测试 */
//    Err_Test();

    /* 停车判断 */
    for(int i=0;i<5;i++)
        sum_dg+=ad_VAL[i];
    if(sum_dg < 10)
        ATOM_PWM_SetDuty(ATOMPWM0, 0, 12500);


    /* 小车姿态判断 */
    pos = Dir_Judge();
    // 变量显示调试
    sprintf(txt,"flag:%d",pos);
    TFTSPI_P8X16Str(0, 0, txt,u16WHITE,u16BLACK);       //字符串显示
    sprintf(txt_e1,"e1:%d",group_err[0]);
    TFTSPI_P8X16Str(0, 1, txt_e1,u16WHITE,u16BLACK);       //字符串显示
    sprintf(txt_e2,"e2:%d",group_err[1]);
    TFTSPI_P8X16Str(0, 2, txt_e2,u16WHITE,u16BLACK);       //字符串显示

    sprintf(txt_sum,"AD0:%d",ad_VAL[0]);
    TFTSPI_P8X16Str(0, 4, txt_sum,u16WHITE,u16BLACK);       //字符串显示
    sprintf(txt_output,"AD5:%d",ad_VAL[5]);
    TFTSPI_P8X16Str(0, 5, txt_output,u16WHITE,u16BLACK);       //字符串显示
    sprintf(txt_pid,"AD3:%d",ad_VAL[2]);
    TFTSPI_P8X16Str(0, 6, txt_pid,u16WHITE,u16BLACK);       //字符串显示
    sprintf(txt_e3,"AD4:%d",ad_VAL[3]);
    TFTSPI_P8X16Str(0, 3, txt_e3,u16WHITE,u16BLACK);       //字符串显示
    switch(pos)
    {
        /* 直道 */
        case 0:
            /* 两组水平电感误差求平均值 */
            sum_err =  group_err[1] + group_err[2];
            input_error =  sum_err / 2;
            /* 误差位置值输入 */
            input_error =  input_error*2.5 + Servo_Center_Mid;
            /* 分段PID */
            Set_PID(1.2,0,0.1);
            // PD控制+限幅
            s_duty = Servo_Center_Mid + PID_Servo_Contrl(Servo_Center_Mid,input_error);
            s_duty = constrain_int(s_duty,820,1600);  // 限幅
            /* 舵机电机赋值 */
            ATOM_PWM_SetDuty(ATOMSERVO2, s_duty, 100);
            ATOM_PWM_SetDuty(ATOMPWM0,2500, 12500);    //电机
        break;
        /* 左直角弯 */
        case 1:
//            ATOM_PWM_SetDuty(ATOMSERVO2, Servo_LEFT_MAX, 100);
            /* 三组电感误差求平均值 */
            sum_err =  group_err[0];
            input_error =  sum_err;
            /* 误差位置值输入 */
            input_error =  input_error*2.5 + Servo_Center_Mid;
            Set_PID(8.0,0,0.5);
            // PD控制+限幅
            s_duty = Servo_Center_Mid + PID_Servo_Contrl(Servo_Center_Mid,input_error);
            s_duty = constrain_int(s_duty,820,1610);  // 限幅
            /* 舵机电机赋值 */
            ATOM_PWM_SetDuty(ATOMSERVO2, s_duty, 100);
            ATOM_PWM_SetDuty(ATOMPWM0,1700, 12500);    //设定舵机的占空比
        break;
        /* 右直角弯 */
        case 2:
//            ATOM_PWM_SetDuty(ATOMSERVO2, Servo_RIGHT_MAX, 100);
            /* 三组电感误差求平均值 */
            sum_err =  group_err[0];
            input_error =  sum_err;
            /* 误差位置值输入 */
            input_error =  input_error*2.5 + Servo_Center_Mid;
            Set_PID(8.0,0,0.5);
            //PD控制
            s_duty = Servo_Center_Mid + PID_Servo_Contrl(Servo_Center_Mid,input_error);
            s_duty = constrain_int(s_duty,820,1610);  // 限幅
            ATOM_PWM_SetDuty(ATOMSERVO2, s_duty, 100);
            ATOM_PWM_SetDuty(ATOMPWM0,1700, 12500);    //设定舵机的占空比
        break;
        /* 左直角弯的中间状态 */
        case 8:
            ATOM_PWM_SetDuty(ATOMSERVO2, Servo_RIGHT_MAX, 100);
        break;
        case 9:
            ATOM_PWM_SetDuty(ATOMSERVO2, Servo_LEFT_MAX, 100);
        break;

    }

}


/*************************************************************************
*  函数名称：void Key_Control()
*  功能说明：按键调参
*************************************************************************/
void Key_Control()
{
    if(!KEY_Read(KEY0))
        s_pid.D = 0;
    if(!KEY_Read(KEY1))
        s_pid.P -= 0.03;
    if(!KEY_Read(KEY2))
        s_pid.P += 0.03;

    sprintf(txt_kp," Kp:%.2f\n ",s_pid.P);
    TFTSPI_P8X16Str(0, 5, txt_kp,u16WHITE,u16BLACK);
    sprintf(txt_kd," Kd:%.2f\n ",s_pid.D);
    TFTSPI_P8X16Str(0, 6, txt_kd,u16WHITE,u16BLACK);
}
/*************************************************************************
*  函数名称：void Set_PID(float P,float I,float D)
*  功能说明：调整PID参数
*************************************************************************/
void Set_PID(float P,float I,float D)
{
    s_pid.P = P;
    s_pid.I = I;
    s_pid.D = D;
}

/*************************************************************************
*  函数名称：void Err_Test()
*  功能说明：用于标定误差(TFT显示)
*************************************************************************/
void Err_Test()
{
    sprintf(txt_e1,"e1:%d",group_err[0]);
    sprintf(txt_e2,"-:%d",test1);
    sprintf(txt_e3,"+:%d",test2);
    TFTSPI_P8X16Str(0, 0, txt_e1,u16WHITE,u16BLACK);       //字符串显示
    TFTSPI_P8X16Str(0, 1, txt_e2,u16WHITE,u16BLACK);
    TFTSPI_P8X16Str(0, 2, txt_e3,u16WHITE,u16BLACK);
}











