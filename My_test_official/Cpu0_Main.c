#include <include.h>//各个模块的头文件
#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <LQ_CAMERA.h>
#include "LQ_CCU6.h"
#include<LQ_GPIO.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_MotorServo.h>
#include <LQ_SOFTI2C.h>
#include <LQ_UART.h>
#include <LQ_Inductor.h>
#include <Main.h>
#include "LQ_ImageProcess.h"
/*******************自己写的头文件********************************/
#include "CXJ_Motor.h"
#include "FJC_Servo_t.h"
#include "FJC_ControlServo.h"
#include "FJC_Servo_up.h"
#include "LZD_PACKING.h"
#include "FJC_NewCS.h"


App_Cpu0 g_AppCpu0; // brief CPU 0 global data //
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 初始化完成标志位
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0占用/1释放 TFT
extern unsigned char  Game_Over; // 小车完成全部任务，停车
extern unsigned char goflag;

extern char TIMER_CC60_CH0;
extern char TIMER_CC60_CH1;
extern char TIMER_CC61_CH0;
extern char TIMER_CC61_CH1;
extern char txt[10];

int core0_main (void)
{
    /* 系统初始化 */
	IfxCpu_disableInterrupts();
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
	g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
	g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

	//用户全部初始化
	All_Init();
	Game_Over=0;// 小车完成全部任务，停车
	// 开启CPU总中断
	IfxCpu_enableInterrupts();
	// 通知CPU1，CPU0初始化完成
	IfxCpu_releaseMutex(&mutexCpu0InitIsOk);


	// 电机恒定速度
    ATOM_PWM_SetDuty(ATOMPWM0,3000, 12500);    //设定舵机的占空比
    ATOM_PWM_SetDuty(ATOMPWM1,0, 12500);    //设定舵机的占空比
    /* 用户测试函数 */
    //          SteeringConfig();  // 根据电感判断车身位置测试
    //        SteeringCrol();      // 舵机pd调试程序(电机恒定速度)
    //        Key_Control();       // 按键调节pid参数
//            Test_Origin();       // 标定电感值
    //        Test_error()
//            Get_ADC();           // 将电感值进行归一化后得到
     //         TestServo();
	/* 用户代码 */
    while (1)	//主循环
    {
        sprintf(txt,"Over:%d\r\n",Game_Over);
        UART_PutStr(UART0,txt);
        if(Game_Over == 0)
            DJCrol();
        else
        {
            ATOM_PWM_SetDuty(ATOMPWM0,0, 12500);    //设定舵机的占空比
            ATOM_PWM_SetDuty(ATOMPWM1,0, 12500);    //设定舵机的占空比
        }


//        Mode_Choose();

//            DJCrol2();



//        DJCrol2();
//       DJCrol();
//       LQ_GPT_4mini512();
        //标定
//        Test_Origin();
        //静态误差
//        Get_ADC();
//        Test_error();

//          Set_Speed(3000);
//        Test_BMQ();
        //电机测试
//        for(int i=0;i<500;i++)
//        {
//            Set_Speed(3000);
//            Test_BMQ();
//        }
//        for(int i=0;i<500;i++)
//        {
//            Set_Speed(2000);
////            Test_BMQ();
//        }
    }



}



