#include <include.h>//����ģ���ͷ�ļ�
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
/*******************�Լ�д��ͷ�ļ�********************************/
#include "CXJ_Motor.h"
#include "FJC_Servo_t.h"
#include "FJC_ControlServo.h"
#include "FJC_Servo_up.h"
#include "LZD_PACKING.h"
#include "FJC_NewCS.h"


App_Cpu0 g_AppCpu0; // brief CPU 0 global data //
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 ��ʼ����ɱ�־λ
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0ռ��/1�ͷ� TFT
extern unsigned char  Game_Over; // С�����ȫ������ͣ��
extern unsigned char goflag;

extern char TIMER_CC60_CH0;
extern char TIMER_CC60_CH1;
extern char TIMER_CC61_CH0;
extern char TIMER_CC61_CH1;
extern char txt[10];

int core0_main (void)
{
    /* ϵͳ��ʼ�� */
	IfxCpu_disableInterrupts();
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
	g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
	g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

	//�û�ȫ����ʼ��
	All_Init();
	Game_Over=0;// С�����ȫ������ͣ��
	// ����CPU���ж�
	IfxCpu_enableInterrupts();
	// ֪ͨCPU1��CPU0��ʼ�����
	IfxCpu_releaseMutex(&mutexCpu0InitIsOk);


	// ����㶨�ٶ�
    ATOM_PWM_SetDuty(ATOMPWM0,3000, 12500);    //�趨�����ռ�ձ�
    ATOM_PWM_SetDuty(ATOMPWM1,0, 12500);    //�趨�����ռ�ձ�
    /* �û����Ժ��� */
    //          SteeringConfig();  // ���ݵ���жϳ���λ�ò���
    //        SteeringCrol();      // ���pd���Գ���(����㶨�ٶ�)
    //        Key_Control();       // ��������pid����
//            Test_Origin();       // �궨���ֵ
    //        Test_error()
//            Get_ADC();           // �����ֵ���й�һ����õ�
     //         TestServo();
	/* �û����� */
    while (1)	//��ѭ��
    {
        sprintf(txt,"Over:%d\r\n",Game_Over);
        UART_PutStr(UART0,txt);
        if(Game_Over == 0)
            DJCrol();
        else
        {
            ATOM_PWM_SetDuty(ATOMPWM0,0, 12500);    //�趨�����ռ�ձ�
            ATOM_PWM_SetDuty(ATOMPWM1,0, 12500);    //�趨�����ռ�ձ�
        }


//        Mode_Choose();

//            DJCrol2();



//        DJCrol2();
//       DJCrol();
//       LQ_GPT_4mini512();
        //�궨
//        Test_Origin();
        //��̬���
//        Get_ADC();
//        Test_error();

//          Set_Speed(3000);
//        Test_BMQ();
        //�������
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


