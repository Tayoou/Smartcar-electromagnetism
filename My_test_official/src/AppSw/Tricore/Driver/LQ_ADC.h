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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

理论有8个转换器，可同步转换；每个转换8个通道，精度为8/10/12位。默认为最高精度12位。
ADC端口AN可以作为输入口，不可作为输出口，与ARM单片机不同！
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_ADC_H_
#define _LQ_ADC_H_

#include "stdint.h"
#include "IfxVadc_reg.h"
#include "IfxVadc_regdef.h"
#include "Ifx_TypesReg.h"
#include "Vadc/Std/IfxVadc.h"
#include "Vadc/Adc/IfxVadc_Adc.h"
#include "Cpu\Std\Platform_Types.h"

/*
 * adc 通道 枚举   ADC0对应管脚 AN0
 */
typedef enum
{
	ADC0 = 0x0000, ADC1 = 0x0101, ADC2 = 0x0202, ADC3 = 0x0303, ADC4 = 0x0404, ADC5 = 0x0505, ADC6 = 0x0606, ADC7 = 0x0707,
	ADC8 = 0x0808, ADC10= 0x0A09, ADC11= 0x0B0A, ADC12= 0x0C0B, ADC13= 0x0D0C,

	ADC16= 0x100D, ADC17= 0x110E, ADC20= 0x140F, ADC25= 0x1910,

	ADC35= 0x2311, ADC36= 0x2412, ADC37= 0x2513, ADC38= 0x2614, ADC39= 0x2715, ADC44= 0x2A16, ADC45= 0x2B17, ADC46= 0x2C18,
	ADC47= 0x2D19, ADC48= 0x2E1A, ADC49= 0x2F1B
}ADC_Channel_t;

/* 获取ADC对应组 */
#define ADC_GetGroup(ADC_Channel)   ((ADC_Channel & 0xF000)>>12)
/* 获取ADC对应通道 */
#define ADC_GetChannel(ADC_Channel) ((ADC_Channel & 0x0F00)>>8)
/* 获取ADC对应管脚序列号 宏定义 */
#define ADC_GetIndex(ADC_Channel)   (unsigned char)(ADC_Channel & 0x00FF)

void ADC_InitConfig(ADC_Channel_t channel, unsigned long Freq_Hz);
unsigned short ADC_Read(ADC_Channel_t chn);
unsigned short ADC_ReadAverage(ADC_Channel_t  chn, unsigned char count);


#endif /* 0_APPSW_TRICORE_APP_LQ_ADC_H_ */
