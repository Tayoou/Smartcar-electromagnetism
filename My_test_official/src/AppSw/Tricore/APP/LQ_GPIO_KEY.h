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
#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

//定义模块号
typedef enum
{
    KEY0 = 0,  //母板上按键0
    KEY1 = 1,
    KEY2 = 2,
    DSW0 = 3,    //母板上拨码开关0
    DSW1 = 4,
} KEYn_e;

typedef enum
{
    LOW = 0,  //按下
    HIGH = 1, //松开
    FAIL = 0xff, //错误
} KEYs_e;

typedef enum //
{
    NOKEYDOWN = 0, KEY0DOWN = 0x01,  //母板上按键0
    KEY1DOWN = 0x02,
    KEY2DOWN = 0x04,
    KEY01DOWN = 0x03,  //母板上按键0
    KEY02DOWN = 0x05,
    KEY12DOWN = 0x06,
    KEY012DOWN = 0x07,
    KEYError = 0xFF,
} KEYdown_e;
//定义的管脚要对应实际按键
#define KEY0p      P22_0  //母板上按键0
#define KEY1p      P22_1  //母板上按键1
#define KEY2p      P22_2  //母板上按键2
#define DSW0p      P33_9  //母板上拨码开关0,默认拨到下方是高电平，上方为低电平
#define DSW1p      P33_11 //母板上拨码开关1
#define REEDp      P15_4  //母板上干簧管
/*********************** UART功能函数 **************************/
//初始化
void GPIO_KEY_Init (void);
unsigned char KEY_Read (KEYn_e KEYno);
unsigned char KEY_Read_All (void);
void Test_GPIO_KEY (void);
void Test_ComKEY_Tft (void);
/*************************************************************************
*  函数名称：void Reed_Init(void)
*  功能说明：干簧管GPIO及中断初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月21日
*  备    注：   中断在那个核初始化，中断VECTABNUM号必须对应该cpu,否则不会调用中断函数
*          中断服务函数PIN_INT0_IRQHandler在LQ_GPIO.c中
*************************************************************************/
void Reed_Init(void);
#endif/* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */
