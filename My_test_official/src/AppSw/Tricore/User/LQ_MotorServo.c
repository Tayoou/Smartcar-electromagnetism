/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技TC264DA核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【dev.env.】AURIX Development Studio1.2.2及以上版本
 【Target 】 TC264DA/TC264D
 【Crystal】 20.000Mhz
 【SYS PLL】 200MHz
 ________________________________________________________________
 基于iLLD_1_0_1_11_0底层程序,
 使用例程的时候，建议采用没有中文及空格的英文路径，
 除了CIF为TC264DA独有外，其它的代码兼容TC264D
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include <ANO_DT.h>
#include <IfxGtm_PinMap.h>
#include <LQ_GPT12_ENC.h>
#include <LQ_GTM.h>
#include <stdint.h>
#include <IfxGtm_PinMap.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_GTM.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <LQ_UART.h>
#include <stdio.h>
#include <LQ_Inductor.h>
#include <LQ_MotorServo.h>
#include <LQ_CCU6.h>
#include "FJC_ControlServo.h"

unsigned char Game_Over = 0; // 小车完成全部任务，停车
sint16 ServoDuty = Servo_Center_Mid;
sint16 MotorDuty1 = 500;      // 电机驱动占空比数值
//sint16 MotorDuty2 = 500;      // 电机驱动占空比数值
sint32 NowTime = 0;
uint16 BatVolt = 0;           // 电池电压采集
//电机频率
#define MOTOR_FREQUENCY    12500

//电机PWM 宏定义
#define MOTOR1_P          IfxGtm_ATOM0_6_TOUT42_P23_1_OUT
#define MOTOR1_N          IfxGtm_ATOM0_5_TOUT40_P32_4_OUT

//#define MOTOR2_P          IfxGtm_ATOM0_0_TOUT53_P21_2_OUT
//#define MOTOR2_N          IfxGtm_ATOM0_4_TOUT50_P22_3_OUT

#define ATOMSERVO1       IfxGtm_ATOM2_0_TOUT32_P33_10_OUT
#define ATOMSERVO2       IfxGtm_ATOM2_5_TOUT35_P33_13_OUT

/*************************************************************************
 *  函数名称：void MotorInit(void)
 *  功能说明：电机PWM初始化
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：驱动2个电机
 *************************************************************************/
void MotorInit (void)
{
    ATOM_PWM_InitConfig(MOTOR1_P, 0, MOTOR_FREQUENCY);
    ATOM_PWM_InitConfig(MOTOR1_N, 0, MOTOR_FREQUENCY);
   // ATOM_PWM_InitConfig(MOTOR2_P, 0, MOTOR_FREQUENCY);
   // ATOM_PWM_InitConfig(MOTOR2_N, 0, MOTOR_FREQUENCY);
}

/*************************************************************************
 *  函数名称：void EncInit (void)
 *  功能说明：编码器初始化函数
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：驱动2个编码器
 *************************************************************************/
void EncInit (void)
{
    ENC_InitConfig(ENC2_InPut_P33_7, ENC2_Dir_P33_6);
   // ENC_InitConfig(ENC4_InPut_P02_8, ENC4_Dir_P33_5);
}

/*************************************************************************
 *  函数名称：void MotorCtrl(float motor1, float motor2)
 *  功能说明：舵机转角函数，由于小车拉杆范围限制，较小
 *  参数说明：   @param    motor1   ： 电机1占空比
 @param    motor2   ： 电机2占空比
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：驱动2个舵机，普通四轮只需要一个舵机即可
 *************************************************************************/
void MotorCtrl (sint32 motor1)
{
    if (motor1 > 0)
    {
        ATOM_PWM_SetDuty(MOTOR1_P, motor1, MOTOR_FREQUENCY);
        ATOM_PWM_SetDuty(MOTOR1_N, 0, MOTOR_FREQUENCY);
    }
    else
    {
        ATOM_PWM_SetDuty(MOTOR1_P, 0, MOTOR_FREQUENCY);
        ATOM_PWM_SetDuty(MOTOR1_N, (-motor1), MOTOR_FREQUENCY);
    }

   /* if (motor2 > 0)
    {
        ATOM_PWM_SetDuty(MOTOR2_P, motor2, MOTOR_FREQUENCY);
        ATOM_PWM_SetDuty(MOTOR2_N, 0, MOTOR_FREQUENCY);
    }
    else
    {
        ATOM_PWM_SetDuty(MOTOR2_P, 0, MOTOR_FREQUENCY);
        ATOM_PWM_SetDuty(MOTOR2_N, (-motor2), MOTOR_FREQUENCY);
    }*/
}

/*************************************************************************
 *  函数名称：TestMotor(void)
 *  功能说明：测试标定输出PWM控制电机
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：驱动2个电机
 【注意事项】注意，一定要对舵机打角进行限制
 使用龙邱母板测试流程：
 1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
 2.接好母板到驱动板的信号线及电源线；
 3.接好驱动板到电机的导线；
 4.烧写程序并运行，确定电机能正常转动后，开启驱动板电源开关；
 5.按键K0/K1确定电机转动速度及方向；
 6.如果出现疯转，按下K2键返回低速模式，或者直接关闭驱动板电源！
 *************************************************************************/
void TestMotor (void)
{
    short duty = 1500;
    char txt[16];

    TFTSPI_CLS(u16BLUE);          //清屏
    TFTSPI_P8X16Str(2, 0, "LQ Motor Test", u16RED, u16BLUE);
    MotorInit();
    MotorCtrl(1500);
    while (1)
    {
        if (KEY_Read(KEY0) == 0)      //按下KEY0键，占空比减小
        {
            if (duty > 100)
                duty -= 100;
        }
        if (KEY_Read(KEY2) == 0)      //按下KEY2键，占空比加大
        {
            if (duty < ATOM_PWM_MAX)      //满占空比为12500
                duty += 100;
        }
        if (KEY_Read(KEY1) == 0)      //按下KEY1键，占空比中值
        {
            duty = 1500;
        }

        MotorCtrl(duty);

        sprintf(txt, "PWM: %05d;", duty);
        TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);       //字符串显示
        UART_PutStr(UART0, txt);

        LED_Ctrl(LED0, RVS);       //电平翻转,LED闪烁
        delayms(200);              //延时等待
    }
}
/*************************************************************************
 *  函数名称：void ServoInit(void)
 *  功能说明：舵机PWM初始化
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：驱动2个舵机
 *************************************************************************/
void ServoInit (void)
{
    ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, 100);  //舵机频率为100HZ，初始值为1.5ms中值
    ATOM_PWM_InitConfig(ATOMSERVO2, Servo_Center_Mid, 100);  //舵机理论范围为：0.5ms--2.5ms，大多舵机实际比这个范围小
}

/*************************************************************************
 *  函数名称：void ServoCtrl(uint32 duty)
 *  功能说明：舵机转角函数，由于小车拉杆范围限制，较小
 *  参数说明：short duty，占空比
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：驱动2个舵机，普通四轮只需要一个舵机即可
 *************************************************************************/
void ServoCtrl (uint32 duty)
{
    if (duty >= Servo_Right_Max)                  //限制幅值
        duty = Servo_Right_Max;
    else if (duty <= Servo_Left_Min)            //限制幅值
        duty = Servo_Left_Min;

    ATOM_PWM_InitConfig(ATOMSERVO1, duty, 100);  //舵机频率为100HZ，初始值为1.5ms中值
    ATOM_PWM_InitConfig(ATOMSERVO2, duty, 100);  //舵机理论范围为：0.5ms--2.5ms，大多舵机实际比这个范围小
}

/*************************************************************************
 *  函数名称：Test_Servo(void)
 *  功能说明：舵机PWM初始化，测试标定输出PWM控制SD5/S3010舵机
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：驱动2个舵机
 【注意事项】注意，一定要对舵机打角进行限制
 使用龙邱母板测试流程：
 1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
 2.然后确定舵机供电电压，SD5舵机用5V供电，S3010用6-7V供电！！！
 3.把舵机的舵盘去掉，让舵机可以自由转动；
 4.烧写程序并运行，让舵机转动到中值附近；如果没反应重复1-2步，或者调整舵机的PWM频率计占空比，能受控为准；
 5.舵机受控后用手轻转，舵机会吱吱响，对抗转动，此时可以装上舵盘；
 6.按键K0/K1确定舵机的左右转动极限，并记下来，作为后续限幅防止舵机堵转烧毁！
 *************************************************************************/
void TestServo (void)
{
    char txt[16] = "X:";
    signed short duty = Servo_Center_Mid;

    TFTSPI_CLS(u16BLUE);          //清屏
    TFTSPI_P8X16Str(2, 0, "LQ Servo Test", u16RED, u16BLUE);
    ServoInit();
    ServoCtrl(Servo_Center_Mid);  // 先让舵机打到中值
    while (1)
    {
        if (!KEY_Read(KEY0))
        {
            if (duty > 10)  // 减小
            {
                duty -= 10;
            }
        }
        if (!KEY_Read(KEY1))
        {
            duty = Servo_Center_Mid;
        }
        if (!KEY_Read(KEY2))  // 增大
        {
            duty += 10;
        }

        ATOM_PWM_SetDuty(ATOMSERVO2, duty, 100);
        ATOM_PWM_SetDuty(ATOMSERVO1, duty, 100);
        sprintf(txt, "Servo duty:%04d ", duty);
        TFTSPI_P8X16Str(1, 2, txt, u16BLACK, u16GREEN); //显示出库实际脉冲数，以便灵活调整
        LED_Ctrl(LEDALL, RVS);        //四个LED同时闪烁;
        delayms(100);
    }
}

/*************************************************************************
 *  函数名称：void TestEncoder(void)
 *  功能说明：测试程序，TFT1.8显示
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年4月10日
 *  备    注：
 *************************************************************************/
void TestEncoder(void)
{
    char txt[32];

    TFTSPI_CLS(u16BLUE);   //蓝色屏幕
    TFTSPI_P8X16Str(0, 0, "Test Encoder", u16WHITE, u16BLACK);      //字符串显示
    EncInit();
    while (1)
    {
        /* 获取编码器值 */
        ECPULSE1 = ENC_GetCounter(ENC2_InPut_P33_7); //左电机 母板上编码器1，小车前进为负值
       // ECPULSE2 = ENC_GetCounter(ENC4_InPut_P02_8); //右电机 母板上编码器2，小车前进为正值

        sprintf(txt, "Enc1: %05d;", ECPULSE1);
        TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);       //字符串显示
    //    sprintf(txt, "Enc2: %05d;", ECPULSE2);
   //     TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);       //字符串显示

        LED_Ctrl(LED0, RVS);        //电平翻转,LED闪烁
        delayms(200);              //延时等待
    }
}

/*************************************************************************
 *  函数名称：uint8 SetCircleNum (void)
 *  功能说明：设置需要进入圆环的个数；
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：
 *************************************************************************/
uint8 SetCircleNum (void)
{
    char txt[16] = " ";
    uint8 num = 1;

    TFTSPI_CLS(u16BLACK);            // 清屏
    TFTSPI_P8X16Str(2, 1, "LQ Smart Car", u16RED, u16BLUE);
    TFTSPI_P8X16Str(2, 3, "K2 num +", u16RED, u16BLUE);
    TFTSPI_P8X16Str(2, 4, "K1 set OK", u16RED, u16BLUE);
    TFTSPI_P8X16Str(2, 5, "K0 num -", u16RED, u16BLUE);
    TFTSPI_P8X16Str(2, 7, "Ring num:  ", u16RED, u16BLUE);

    while (KEY_Read(KEY1))
    {
        if (KEY_Read(KEY2) == 0)
        {
            if (num < 10)
                num++;
        }
        else if (KEY_Read(KEY0) == 0)
        {
            if (num > 0)
                num--;
        }
        sprintf(txt, "Ring num: %d ", num);
        TFTSPI_P8X16Str(2, 7, txt, u16WHITE, u16BLUE);

        delayms(100);
    }
    return num;
}

