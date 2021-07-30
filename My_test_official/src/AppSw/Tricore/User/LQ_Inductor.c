/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技TC264DA核心板
 【编    写】chiusir
 【E-mail】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http:// www.lqist.cn
 【淘宝店铺】http:// longqiu.taobao.com
 ------------------------------------------------
 【dev.env.】AURIX Development Studio1.2.2及以上版本
 【Target 】 TC264DA/TC264D
 【Crystal】 20.000Mhz
 【SYS PLL】 200MHz
 ________________________________________________________________
 基于iLLD_1_0_1_11_0底层程序,

 使用例程的时候，建议采用没有空格的英文路径，
 除了CIF为TC264DA独有外，其它的代码兼容TC264D
 本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
 工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
 =================================================================
 程序配套视频地址：https:// space.bilibili.com/95313236
 =================================================================
 使用说明：
 本教学演示程序适用于电磁四轮或者三轮车：
 整车资源为：
 模块：龙邱TC264DA核心板、配套母板、双路全桥电机驱动、双编码器512带方向、TFT1.8屏幕、单舵机、四路电感模块；
 车模：三轮或者四轮均可；
 电感分布：
 ||----------左------------------------------------右--------------||
 ||---------侧--------------------------------------侧-------------||
 ||--------第----------------------------------------第------------||
 ||-------1----左侧第2个电感 -------------右侧第2个电感 -----1-----------||
 ||------个--------------------------------------------个----------||
 ||-----电----------------------------------------------电---------||
 ||----感------------------------------------------------感--------||
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include <IfxCpu.h>
#include <LQ_ADC.h>
#include <LQ_CCU6.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <Main.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_MotorServo.h>
#include <LQ_GPIO_LED.h>
#include <LQ_Inductor.h>
#include <LQ_GPT12_ENC.h>
#include "FJC_ControlServo.h"


sint16 TempAngle = 0;        // 根据电感偏移量计算出的临时打角值
sint16 LastAngle = 0;        // 记忆冲出赛道前的有效偏移方向

sint16 Lleft1 = 0, Lleft2 = 0, Lright2 = 0, Lright1 = 0;  // 电感偏移量
sint16 LnowADC[6];           // 电感当前ADC数值

sint16 ad_max[6] = {2500, 2300, 999, 999, 2500, 2500}; // 新板子放到赛道标定最大值,会被程序刷新
sint16 ad_min[6] = {120, 120, 999, 999, 120, 120}; // 新板子据需要标定最小值,会被程序刷新

uint8 CircleNumber = 1;    // 入环次数，0结束；默认1次 ;环的个数一般在比赛前测试赛道时就知道了
uint8 TangentPoint = 1;    // 切点判断   0切点结束；默认1可以入环，读取脉冲为入环准备
uint8 EnterCircle = 0;     // 允许进环  默认 0不可进环；1可以进环
uint8 OutCircle = 0;       // 允许出环   默认0不可出环；1可以出环
uint8 LeftRightCircle = 0; // 左侧环还是右侧环 默认0原始；1左环；2右环

sint32 TangentPointpulse = 0; // 脉冲记忆临时变量1
sint32 EnterCirclePulse = 0;  // 脉冲记忆临时变量2
sint32 OutCirclePulse = 0;    // 脉冲记忆临时变量3
sint32 EnterCircleOKPulse = 0;// 脉冲记忆临时变量4
sint16 LowSpeed = 0;          // 圆环/十字口减速

uint16 MagneticField = 0;     // 磁场强度    magnetic field intensity,判断圆环是否出现

sint16 OffsetDelta = 0;

/*************************************************************************
 *  函数名称：void InductorInit (void)
 *  功能说明：四个电感ADC初始化函数；
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：
 *************************************************************************/
void InductorInit (void)
{
    ADC_InitConfig(ADC0, 100000); // 初始化,采样率100kHz
    ADC_InitConfig(ADC1, 100000); // 初始化
    ADC_InitConfig(ADC2, 100000); // 初始化
    ADC_InitConfig(ADC3, 100000); // 初始化
}
/*************************************************************************
 *  函数名称：void InductorNormal(void)
 *  功能说明：采集电感电压并归一化；
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：   注意要先标定运放的放大倍数，尽量四个一致或者接近
 *************************************************************************/
void InductorNormal (void)
{
    LnowADC[0] = ADC_Read(ADC0);  // 左侧第1个电感，与赛道夹角约30度，采集各个电感的AD值
    LnowADC[1] = ADC_Read(ADC1);  // 左侧第2个电感，垂直赛道，
    LnowADC[4] = ADC_Read(ADC2);  // 右侧第2个电感，垂直赛道，
    LnowADC[5] = ADC_Read(ADC3);  // 右侧第1个电感，与赛道夹角约30度
    BatVolt       = ADC_Read(ADC7);  // 刷新电池电压
    if (LnowADC[0] < ad_min[0])
        ad_min[0] = LnowADC[0];     // 刷新最小值
    else if (LnowADC[0] > ad_max[0])
        ad_max[0] = LnowADC[0];     // 刷新最大值
    if (LnowADC[1] < ad_min[1])
        ad_min[1] = LnowADC[1];
    else if (LnowADC[1] > ad_max[1])
        ad_max[1] = LnowADC[1];
    if (LnowADC[4] < ad_min[4])
        ad_min[4] = LnowADC[4];
    else if (LnowADC[4] > ad_max[4])
        ad_max[4] = LnowADC[4];
    if (LnowADC[5] < ad_min[5])
        ad_min[5] = LnowADC[5];
    else if (LnowADC[5] > ad_max[5])
        ad_max[5] = LnowADC[5];

    Lleft1 = (LnowADC[0] - ad_min[0]) * 100 / (ad_max[0] - ad_min[0]);     // 各偏移量归一化到0--100以内
    Lleft2 = (LnowADC[1] - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);
    Lright2 = (LnowADC[4] - ad_min[4]) * 100 / (ad_max[4] - ad_min[4]);
    Lright1 = (LnowADC[5] - ad_min[5]) * 100 / (ad_max[5] - ad_min[5]);

    MagneticField = Lleft1 + Lleft2 + Lright2 + Lright1;// 磁场整体强度


    if((Lleft2 + Lright2)<10)      // 两个值都小于20，电磁杆远离赛道，进入直角弯和急弯
    {
        if (Lleft1 + Lleft2 > Lright2 + Lright1)
        {
            LastAngle  = Servo_Left_Min;
            //LastAngle = Servo_Right_Max;
        }
        else
        {
           // LastAngle  = Servo_Left_Min;
            LastAngle = Servo_Right_Max;
        }

    }
    else if ((Lleft2 > 10) && (Lright2 > 10))   // 小车行走于赛道上中间
    {
        TempAngle = Servo_Center_Mid - (Lleft2 - Lright2) *10; //  根据偏移量差值小幅度打角，防止直道摇摆
        LastAngle=TempAngle;
    }
    else                                        //  小车行走于赛道上弯道区域，一大一小，需要较大程度控制转向
    {
        TempAngle = Servo_Center_Mid - (Lleft2 - Lright2) * 15; //  一般弯道转向控制，数值越大，转向越早
        LastAngle=TempAngle;
    }
}

/*************************************************************************
 *  函数名称：void CircleDetect void
 *  功能说明：识别并进入圆环的个数；
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：
 *************************************************************************/
void CircleDetect (void)
{
    if (CircleNumber) // 入环次数，0结束；默认2次
    {
        // 进环切点区域判断
        if (MagneticField > 200) // 直道进入，此值不宜太大，容易丢失切点
        {
            if (TangentPoint)
            {
                TangentPointpulse = RAllPulse; // 读取当前脉冲数值
                TangentPoint = 0;             // 禁止再次读取当前脉冲数值
            }
            else if (LeftRightCircle == 0)
            {                                 // 如果前面设置为220，此处应该大点儿，距离拉长一些，太大就走过切点了也不行！
                if (RAllPulse > TangentPointpulse + 2000)   // 进入切点后再前进3000脉冲，大约50cm，
                {
                    EnterCircle = 1;      // 通过切点区域，可以入环
                }
            }
        }

        // 。约1.2米外进环无效，则需要重新识别切点
        if ((LeftRightCircle == 0) && (RAllPulse > TangentPointpulse + 8000)) // 约1.2米外进环无效
        {
            EnterCircle = 0;   // 约1.2米外进环无效
            TangentPoint = 1;  // 重新识别切点
        }
        //入环
        if ((EnterCircle) && (MagneticField > 240)) // 约1.2米内再次发现强磁场，入环
        {
            LowSpeed = 500;    // 减速
            // 左侧入环处理
            if (Lleft1 + Lleft2 > Lright2 + Lright1)     // 左边入环，存在误判！！！
            {
                LeftRightCircle = 1;  // 左侧环为1
                EnterCircle = 0;      // 入环后禁止再次入环
                EnterCirclePulse = RAllPulse;
                ServoCtrl(Servo_Left_Min);     // 舵机PWM输出，转向舵机打角控制
                while (RAllPulse < EnterCirclePulse + 2800)
                {
                    delayms(1);       // 半打角度前进1200脉冲，约20cm，龙邱512带方向编码器1米5790个脉冲
                }
            }
            else // 右侧入环处理
            {
                LeftRightCircle = 2;  // 右侧环为2
                EnterCircle = 0;  // 入环后禁止再次入环
                EnterCirclePulse = RAllPulse;
                ServoCtrl(Servo_Right_Max);        // 舵机PWM输出，转向舵机打角控制
                while (RAllPulse < EnterCirclePulse + 1800) // 用的是右侧的编码器，实际走的距离近一点儿
                {
                    delayms(1);   // 半打角度前进1200脉冲，约20cm，龙邱512带方向编码器1米5790个脉冲
                }
            }
            EnterCircleOKPulse = RAllPulse; // 出环用
        }
        // 出环处理
        if ((LeftRightCircle > 0) && (RAllPulse > EnterCircleOKPulse + 3000))
        {
            EnterCircleOKPulse = 10000000; //禁止再次出环使能
            OutCircle = 1;  // 进环后可以出环
        }
        if ((OutCircle) && (MagneticField > 220)) // 入环标志为真才能入环
        {
            LowSpeed = 500;        // 减速
            // 左侧出环处理
            if (LeftRightCircle == 1)   //左边入环
            {
                //OutCircle = 0;    // 入环后禁止再次入环
                OutCirclePulse = RAllPulse;
                ServoCtrl(Servo_Left_Min);     // 舵机PWM输出，转向舵机打角控制
                while (RAllPulse < OutCirclePulse + 2800)
                {
                    delayms(1); // 半打角度前进1200脉冲，约20cm，龙邱512带方向编码器1米5790个脉冲
                }
                OutCirclePulse = RAllPulse;
                ServoCtrl(Servo_Center_Mid-Servo_Delta*2/3);     // 舵机PWM输出，反向打角
                while (RAllPulse < OutCirclePulse + 700)
                {
                    delayms(1);         // 半打角度前进600脉冲，约10cm，龙邱512带方向编码器1米5790个脉冲
                }
                CircleNumber--;         // 环计数
                TangentPoint = 1;       // 切点判断   0切点结束；默认1可以入环，读取脉冲为入环准备
                EnterCircle = 0;        // 允许进环  默认 0不可进环；1可以进环
                OutCircle = 0;          // 允许出环   默认0不可出环；1可以出环
                LeftRightCircle = 0;    // 左侧环还是右侧环 默认0原始；1左环；2右环
                LowSpeed = 0;           // 恢复速度
                //Reed_Init();            // 干簧管GPIO及中断初始化函数,为停车入库做准备
            }
            // 右侧出环处理
            else if (LeftRightCircle == 2)  //右边入环
            {
                //OutCircle = 0;     // 入环后禁止再次入环
                OutCirclePulse = RAllPulse;
                ServoCtrl(Servo_Right_Max);     // 舵机PWM输出，转向舵机打角控制
                while (RAllPulse < OutCirclePulse + 2500)
                {
                    delayms(1);   // 半打角度前进1200脉冲，约20cm，龙邱512带方向编码器1米5790个脉冲
                }
                OutCirclePulse = RAllPulse;
                ServoCtrl(Servo_Center_Mid+Servo_Delta*2/3);     // 舵机PWM输出，反向打角
                while (RAllPulse < OutCirclePulse + 1400)
                {
                    delayms(1);         // 半打角度前进600脉冲，约10cm，龙邱512带方向编码器1米5790个脉冲
                }
                CircleNumber--;         // 环计数
                TangentPoint = 1;       // 切点判断   0切点结束；默认1可以入环，读取脉冲为入环准备
                EnterCircle = 0;        // 允许进环  默认 0不可进环；1可以进环
                OutCircle = 0;          // 允许出环   默认0不可出环；1可以出环
                LeftRightCircle = 0;    // 左侧环还是右侧环 默认0原始；1左环；2右环
                LowSpeed = 0;           // 恢复速度
                Reed_Init();            // 干簧管GPIO及中断初始化函数,为停车入库做准备
            }
        }
    }
}

/*************************************************************************
 *  函数名称：void TFT_Show_EleMag_Info(void)
 *  功能说明：显示各种所需信息
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：
 *************************************************************************/
void TFT_Show_EleMag_Info(void)
{
    char txt[16] = "X:";

    sint16 mps = 0, dmm = 0;    // 速度：m/s,毫米数值
    sint16 pulse100 = 0;
    uint16 bat=0;

    dmm = (sint16) (RAllPulse * 100 / 579);           // 龙邱512带方向编码器1米5790个脉冲，数值太大，除以100
    pulse100 = (sint16) (RAllPulse / 100);
    sprintf(txt, "AP:%05d00", pulse100);              //
    TFTSPI_P8X16Str(3, 1, txt, u16RED, u16BLACK);     // 显示赛道偏差参数

    NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // 获取STM0 当前时间，得到毫秒
    mps = (sint16) (dmm / (NowTime / 1000));          // 计算速度mm/s
    // 调试信息
    sprintf(txt, "%04d %04d %04d ", TempAngle, ECPULSE1, ECPULSE2);   // 显示舵机角度数值，电机占空比数值，编码器数值
    TFTSPI_P8X16Str(1, 0, txt, u16WHITE, u16BLACK);      // 字符串显示
    //显示各电感归一化后的偏移量  当前各电感电压值 各电感开机后历史最小值 各电感开机后历史最大值
    sprintf(txt, "0:%04d %04d %04d ", Lleft1, LnowADC[0], ad_max[0]);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "1:%04d %04d %04d ", Lleft2, LnowADC[1], ad_max[1]);
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "2:%04d %04d %04d ", Lright2, LnowADC[4], ad_max[4]);
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "3:%04d %04d %04d ", Lright1, LnowADC[5], ad_max[5]);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);

    sprintf(txt, "Ring num: %d ", CircleNumber);
    TFTSPI_P8X16Str(2, 6, txt, u16GREEN, u16BLACK);
    sprintf(txt, "M:%03d Q:%d J:%d ", MagneticField, TangentPoint, EnterCircle);
    TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
    if (LeftRightCircle == 1)
        TFTSPI_P8X16Str(0, 8, "Left Ring ", u16WHITE, u16BLACK);
    else if (LeftRightCircle == 2)
        TFTSPI_P8X16Str(0, 8, "Right Ring", u16WHITE, u16BLACK);
    else
        TFTSPI_P8X16Str(0, 8, "No Ring   ", u16WHITE, u16BLACK);

    bat = BatVolt * 11 / 25;  // x/4095*3.3*100*5.7
    sprintf(txt, "B:%d.%02dV %d.%02dm/s", bat / 100, bat % 100, mps / 1000, (mps / 10) % 100);  // *3.3/4095*3
    TFTSPI_P8X16Str(0, 9, txt, u16PURPLE, u16BLACK);   // 字符串显示
}
/*************************************************************************
 *  函数名称：void ElectroMagneticCar(void)
 *  功能说明：电磁车双电机差速控制
 -->1.入门算法：简单的分段比例控制算法，教学演示控制算法；
 ---2.进阶算法：PID典型应用控制算法，教学演示控制算法；
 ---3.高端算法：暂定改进粒子群协同控制算法；
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年10月28日
 *  备    注：驱动2个电机
 *************************************************************************/
void ElectroMagneticCar (void)
{
    sint16 bat=0;

    CircleNumber = 1;   // 入环次数，0结束；默认1次
    TangentPoint = 1;   // 切点判断   0切点结束；默认1可以入环，读取脉冲为入环准备
    EnterCircle = 0;    // 允许进环  默认 0不可进环；1可以进环
    OutCircle = 0;      // 允许出环   默认0不可出环；1可以出环
    LeftRightCircle = 0;// 左侧环还是右侧环 默认0原始；1左环；2右环
    LowSpeed = 0;       // 速度差

    // 切记CPU0,CPU1...不可以同时开启屏幕显示，否则冲突不显示
    mutexCpu0TFTIsOk=0;             // CPU1： 0占用/1释放 TFT
    CircleNumber = SetCircleNum();  // 设置需要进入圆环的个数；

    // 。根据需要设置出入库，出库是固定执行，
    // 。入库需要干簧管和外部中断配合实现
    // 。本例程中，干簧管在通过圆环后开启，不会出现起跑触发的可能性
   // OutInGarage(OUT_GARAGE,ReadOutInGarageMode()); // 测试出库，拨码在上左侧出入库，反之右侧出入库
    //OutInGarage(IN_GARAGE,ReadOutInGarageMode());  // 测试入库

    TFTSPI_CLS(u16BLACK);            // 清屏
    // 切记CPU0,CPU1...不可以同时开启屏幕显示，否则冲突不显示
    mutexCpu0TFTIsOk=1;             // CPU1： 0占用/1释放 TFT

    RAllPulse = 0;                  // 全局变量，脉冲计数总数
    NowTime = STM_GetNowUs(STM0);   // 获取STM0 当前时间

    while (1)
    {
        InductorNormal();           // 采集电感电压并并归一化；
        if (MagneticField > 220)    // 直道进入，此值不宜太大，容易丢失切点
        {
            LowSpeed = 500;         // 减速
        }
        else if (MagneticField < 200)
        {
            LowSpeed = 0; // 恢复速度
        }
        CircleDetect();             // 识别并进入圆环的个数；

        ServoDuty = TempAngle;
        ServoCtrl(ServoDuty);       // 舵机PWM输出，转向舵机打角控制
        //
        OffsetDelta = (Lleft2 - Lright2);  // 直道偏差
        bat=(BatVolt * 11 / 25-750)*Kbat;
        MotorDuty1 = MtTargetDuty-bat + ECPULSE1 * Kencoder  - LowSpeed;  // 有差速控制，右转偏差为负，左侧加速
       // MotorDuty2 = MtTargetDuty-bat - ECPULSE2 * Kencoder + OffsetDelta * Koffset - LowSpeed;  // 有差速控制，左转偏差为正，右侧加速

        if (MagneticField < 15)     // 判断是否冲出赛道
        {
            MotorCtrl(0);        // 冲出赛道停车
            delayms(200);
        }
        else
        {
            MotorCtrl(MotorDuty1);   // 四轮双电机驱动
            // MotorCtrl(MtTargetDuty-TempAngle*8/5, MtTargetDuty+TempAngle*8/5);// 三轮车，无舵机
        }

       /* if(Game_Over)
        {
            OutInGarage(IN_GARAGE, ReadOutInGarageMode());
        }*/
    } // WHILE(1)
} // MAIN()

