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

#include <LQ_GPIO.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>

/*************************************************************************
*  函数名称：void GPIO_KEY_Init(void)
*  功能说明：GPIO初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void GPIO_KEY_Init(void)
{
	  // 初始化,输入口，高电平
	  PIN_InitConfig(KEY0p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(KEY1p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(KEY2p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(DSW0p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(DSW1p, PIN_MODE_INPUT, 1);
}
/*************************************************************************
*  函数名称：void Reed_Init(void)
*  功能说明：干簧管GPIO及中断初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月21日
*  备    注：   中断在那个核初始化，中断VECTABNUM号必须对应该cpu,否则不会调用中断函数
*          中断服务函数PIN_INT0_IRQHandler在LQ_GPIO.c中
*************************************************************************/
void Reed_Init(void)
{
      // 初始化,输入口，高电平
      PIN_InitConfig(REEDp, PIN_MODE_INPUT, 1);
      // 使能场中断 ，下降沿触发
      PIN_Exti(REEDp, PIN_IRQ_MODE_FALLING);
}
#pragma warning 544         // 屏蔽警告

/*************************************************************************
*  函数名称：unsigned char KEY_Read(KEYn_e KEYno)
*  功能说明：读取按键状态
*  参数说明：KEYn_e KEYno按键编号
*  函数返回：按键状态，0/1
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
unsigned char KEY_Read(KEYn_e KEYno)
{
    switch(KEYno)
    {
      case KEY0:
        return PIN_Read(KEY0p);//母板上按键0
      break;

      case KEY1:
        return PIN_Read(KEY1p);//母板上按键1
      break;

      case KEY2:
        return PIN_Read(KEY2p);//母板上按键2
      break;

      case DSW0:
        return PIN_Read(DSW0p);//母板上拨码开关0
      break;

      case DSW1:
        return PIN_Read(DSW1p);//母板上拨码开关1
      break;
      default:
        return 0XFF;
    }
    return 0;
}
#pragma warning default     // 打开警告


/*************************************************************************
*  函数名称：unsigned char KEY_Read_All(void)
*  功能说明：读取全部按键状态
*  参数说明：无
*  函数返回：按键组合状态，0--7八种状态
*  修改时间：2020年3月10日
*  备    注：读取三个按键状态，方便组合键使用
*************************************************************************/
unsigned char KEY_Read_All(void)
{
   unsigned char tm=0;

   tm = (PIN_Read(KEY0p)|(PIN_Read(KEY1p)<<1)|(PIN_Read(KEY2p)<<2));//读取各个按键状态并编码
   if(tm==0x07)
	{
	   return 0;
	}
//   while(tm == (PIN_Read(KEY0p)|(PIN_Read(KEY1p)<<1)|(PIN_Read(KEY2p)<<2)));//等待按键释放

   return  (~tm)&0X07;
}


/*************************************************************************
*  函数名称：void Test_GPIO_KEY(void)
*  功能说明：测试程序
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：按下KEY0/1/2任意键 LED全亮
*************************************************************************/
void Test_GPIO_KEY(void)
 {
	unsigned char k=0xFF;

	GPIO_KEY_Init();
 	while(1)
 	{
 		k= KEY_Read(KEY0)& KEY_Read(KEY1)& KEY_Read(KEY2);

 		if(k==0) LED_Ctrl(LEDALL, ON); //按下KEY0/1/2任意键 LED全亮
 		else     LED_Ctrl(LEDALL, OFF);//松开KEY0/1/2任意键 LED全灭

 		delayms(100);                //延时等待
 	}
 }


/*************************************************************************
*  函数名称：void Test_ComKEY_Tft(void)
*  功能说明：测试组合按键并在彩屏显示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：按键状态彩屏显示
*************************************************************************/
void Test_ComKEY_Tft(void)
 {
	unsigned char k=0xFF;

	TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏
	TFTSPI_CLS(u16BLUE);   //蓝色屏幕
	TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
	TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);		//字符串显示
	TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);		//字符串显示
	delayms(1000);              //延时等待
	TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
	TFTSPI_CLS(u16BLACK);   //黑色屏幕
	TFTSPI_P8X16Str(0,1,"KEY Pressed:  ",u16WHITE,u16BLACK);		//字符串显示

	GPIO_KEY_Init();
 	while(1)
 	{
 		k= KEY_Read_All();
 		switch(k)
 		{
 		  case NOKEYDOWN:
 			 //TFTSPI_P8X16Str(35,3,"NO key!  ",u16WHITE,u16BLACK);		//字符串显示
 		  break;
 		  case KEY0DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0!    ",u16WHITE,u16BLACK);		//字符串显示
 		  break;
 		  case KEY1DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY1!    ",u16WHITE,u16BLACK);		//字符串显示
 		  break;
 		  case KEY2DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY2!    ",u16WHITE,u16BLACK);		//字符串显示
 		  break;
 		  case KEY01DOWN:
 			 TFTSPI_P8X16Str(35,3,"KEY0&1!  ",u16WHITE,u16BLACK);		//字符串显示
 		  break;
 		  case KEY02DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0&2!  ",u16WHITE,u16BLACK);		//字符串显示
 		  break;
 		  case KEY12DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY1&2!  ",u16WHITE,u16BLACK);		//字符串显示
 		  break;
 		  case KEY012DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0&1&2!",u16WHITE,u16BLACK);		//字符串显示
 		  break;
 		  default:
 		  break;
 		 }
 		LED_Ctrl(LED0,RVS);        //四个LED同时闪烁
 		delayms(100);              //延时等待
 	}
 }



