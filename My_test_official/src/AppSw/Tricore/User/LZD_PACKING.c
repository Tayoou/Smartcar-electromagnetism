#include "LZD_PACKING.h"
#include "LQ_GPIO.h"


/*************************************************************************
*  �������ƣ�void All_Init()
*  ����˵����ȫ�ֺ�����ʼ��
*  �޸�ʱ�䣺2021��4��23��
*************************************************************************/
void All_Init()
{
    //TFT��ʼ��
    TFTSPI_Init(1);               // TFT1.8��ʼ��0:����  1������
    TFTSPI_CLS(u16BLACK);         // ����
    // ������ʼ��
    GPIO_KEY_Init();
    // LED������P10.6��P10.5��ʼ��
    GPIO_LED_Init();
    // ����P14.0�ܽ����,P14.1���룬������115200
    UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
    //ADC��ʼ��
    My_ADCInit();
    //�����ʼ��

    //Sterring_PID_Init();   //���PID������ʼ��
    ATOM_PWM_InitConfig(ATOMSERVO2, Servo_Center_Mid, 100);         //�����ʼ��
    //�����ʼ��
    ATOM_PWM_InitConfig(ATOMPWM0, 0, 12500);
    ATOM_PWM_InitConfig(ATOMPWM1, 0, 12500);
    PID_Arg_Init();

    Reed_Init();//�ɻɹܳ�ʼ��
    // CCU6�жϳ�ʼ��

    CCU6_InitConfig(CCU60,CCU6_Channel0,50000);  // С�����

    // ������init

    ENC_InitConfig(ENC6_InPut_P20_3, ENC6_Dir_P20_0);
}