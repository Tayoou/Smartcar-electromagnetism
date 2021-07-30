/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
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

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxGtm_PinMap.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_GTM.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <LQ_UART.h>
#include <stdio.h>


#define TOMSERVO1         IfxGtm_TOM0_0_TOUT32_P33_10_OUT
#define TOMSERVO2         IfxGtm_TOM0_13_TOUT35_P33_13_OUT

/*************************************************************************
*  函数名称：void Test_Servo(void);
*  功能说明：测试舵机转动
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月22日
*  备    注：
*************************************************************************/

void LQ_Tom_Servo_2chPWM(void)
{
	unsigned short duty=0;
	char txt[16];

	TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
	TFTSPI_CLS(u16BLUE);   //蓝色屏幕
	TFTSPI_P8X16Str(0, 3, "Test Servo",u16WHITE,u16BLACK);		//字符串显示
	GPIO_KEY_Init();
	TOM_PWM_InitConfig(TOMSERVO1, 5000, 100);//初始化P33_10 作为PWM输出口 频率100Hz 占空比 百分之(5000/TOM_PWM_MAX)*100
	TOM_PWM_InitConfig(TOMSERVO2, 5000, 100);//初始化P33_13 作为PWM输出口 频率100Hz 占空比 百分之(5000/TOM_PWM_MAX)*100
	while(1)
	{
		if(KEY_Read(KEY0)==0)//按下KEY0键，占空比减小，舵机顺时针转动
		{
			//if(duty>950)
				duty-=10;
		}
		if(KEY_Read(KEY2)==0)//按下KEY2键，占空比加大，舵机逆时针转动
		{
			//if(duty<1050)
				duty+=10;
		}
		if(KEY_Read(KEY1)==0)//按下KEY1键，舵机回位中值
		{
			duty=1000;
		}

		TOM_PWM_SetDuty(TOMSERVO2, duty, 50);
		TOM_PWM_SetDuty(TOMSERVO1, duty, 50);

		sprintf(txt, "PWM: %05d;", duty);
		TFTSPI_P8X16Str(0, 5, txt,u16WHITE,u16BLACK);		//字符串显示
		UART_PutStr(UART0,txt);

    	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
		delayms(200);              //延时等待
	}
}
