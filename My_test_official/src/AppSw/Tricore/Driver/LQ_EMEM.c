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



#include <IfxEmem.h>
#include "LQ_EMEM.h"

/*************************************************************************
*  函数名称：EMEM_InitConfig(void)
*  功能说明：初始化芯片内部 EMEM内存  512K
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：TC264 DA后缀的芯片才有EMEM的512K ram
*************************************************************************/
void EMEM_InitConfig(void)
{
	IfxEmem_setClockEnableState(IfxEmem_State_enabled);

	if (IfxEmem_LockedState_locked == IfxEmem_getLockedState())
	{
		/* apply unlock sequence */
		IfxEmem_setUnlockStandbyLockFlag(0x1);
		IfxEmem_setUnlockStandbyLockFlag(0x3);
		IfxEmem_setUnlockStandbyLockFlag(0x7);
		/* wait one cycle for unlock */

		for(int i = 0; i < 10000; i++)
		{
			__asm("NOP");
		}
	}

	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_0);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_1);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_2);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_3);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_4);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_5);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_6);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_7);
}
