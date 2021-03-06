#include "LZD_PACKING.h"
#include "LQ_GPIO.h"


/*************************************************************************
*  函数名称：void All_Init()
*  功能说明：全局函数初始化
*  修改时间：2021年4月23日
*************************************************************************/
void All_Init()
{
    //TFT初始化
    TFTSPI_Init(1);               // TFT1.8初始化0:横屏  1：竖屏
    TFTSPI_CLS(u16BLACK);         // 清屏
    // 按键初始化
    GPIO_KEY_Init();
    // LED灯所用P10.6和P10.5初始化
    GPIO_LED_Init();
    // 串口P14.0管脚输出,P14.1输入，波特率115200
    UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
    //ADC初始化
    My_ADCInit();
    //舵机初始化

    //Sterring_PID_Init();   //舵机PID参数初始化
    ATOM_PWM_InitConfig(ATOMSERVO2, Servo_Center_Mid, 100);         //舵机初始化
    //电机初始化
    ATOM_PWM_InitConfig(ATOMPWM0, 0, 12500);
    ATOM_PWM_InitConfig(ATOMPWM1, 0, 12500);
    PID_Arg_Init();

    Reed_Init();//干簧管初始化
    // CCU6中断初始化

    CCU6_InitConfig(CCU60,CCU6_Channel0,50000);  // 小车舵机

    // 编码器init

    ENC_InitConfig(ENC6_InPut_P20_3, ENC6_Dir_P20_0);
}
