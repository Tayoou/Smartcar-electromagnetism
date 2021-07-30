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
  */ 
#ifndef SRC_APPSW_TRICORE_DRIVER_LQ_GPSR_H_
#define SRC_APPSW_TRICORE_DRIVER_LQ_GPSR_H_

#include "Tricore/_Impl/IfxSrc_cfg.h"

/***********************************************************************************************/
/***********************************************************************************************/
/*****************    CPU中断优先级 和 指向内核配置 用户可以自行修改*******************************/
/***********************************************************************************************/
/***********************************************************************************************/

/** CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CPU0_SOFT_IRQ0_PRIORITY    240

/** CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CPU0_SOFT_IRQ1_PRIORITY    241

/** CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CPU0_SOFT_IRQ2_PRIORITY    242

/** CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CPU0_SOFT_IRQ3_PRIORITY    243


/** CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CPU1_SOFT_IRQ0_PRIORITY    244

/** CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CPU1_SOFT_IRQ1_PRIORITY    245

/** CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CPU1_SOFT_IRQ2_PRIORITY    246

/** CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CPU1_SOFT_IRQ3_PRIORITY    247



/**
 * 软件中的序号枚举
 */
typedef enum
{
	SoftIRQ0,
	SoftIRQ1,
	SoftIRQ2,
	SoftIRQ3
}SOFT_IRQ;

void GPSR_InitConfig(IfxSrc_Tos cpu, SOFT_IRQ index);
void CPSR_Trig(IfxSrc_Tos cpu, SOFT_IRQ index);

#endif /* SRC_APPSW_TRICORE_DRIVER_LQ_GPSR_H_ */
