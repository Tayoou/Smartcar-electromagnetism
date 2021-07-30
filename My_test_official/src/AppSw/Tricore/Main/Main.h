/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Hightec4.9.3/Tasking6.3及以上版本
【Target 】 TC264DA
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
基于iLLD_1_0_1_11_0底层程序
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_MAIN_MAIN_H_
#define SRC_APPSW_TRICORE_MAIN_MAIN_H_

#include <include.h>//各个模块的头文件
#include <IfxCpu.h>
#include <stdint.h>

extern App_Cpu0 g_AppCpu0; // brief CPU 0 global data
extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 初始化完成标志位  */
extern volatile char mutexCpu0TFTIsOk;       /** TFT18使用标志位  */
/**
 * 定时器 5ms和50ms标志位
 */
extern volatile uint8 cpu1Flage5ms;
extern volatile uint8 cpu1Flage50ms;

/* 期望速度  */
extern volatile sint16 targetSpeed;

/* 避障标志位 */
extern volatile uint8 evadibleFlage;

int core0_main (void);

#endif /* SRC_APPSW_TRICORE_MAIN_MAIN_H_ */
