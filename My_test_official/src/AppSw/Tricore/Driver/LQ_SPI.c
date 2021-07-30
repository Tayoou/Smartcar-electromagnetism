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


#include <LQ_SPI.h>


/**
 *  ASC SPI通信结构体
 */
IfxAsclin_Spi g_SpiConfig[4];

/** SPI中断CPU标号 */
const unsigned char SpiIrqVectabNum[4] = {SPI0_VECTABNUM, SPI1_VECTABNUM, SPI2_VECTABNUM, SPI3_VECTABNUM};

/** SPI中断优先级 */
const unsigned char SpiIrqPriority[12] = {SPI0_RX_PRIORITY, SPI0_TX_PRIORITY, SPI0_ER_PRIORITY, SPI1_RX_PRIORITY, SPI1_TX_PRIORITY, SPI1_ER_PRIORITY,
		                           SPI2_RX_PRIORITY, SPI2_TX_PRIORITY, SPI2_ER_PRIORITY, SPI3_RX_PRIORITY, SPI3_TX_PRIORITY, SPI3_ER_PRIORITY};


/***********************************************************************************************/
/***********************************************************************************************/
/*****************ASC SPI中断优先级 和 指向内核配置 用户可以自行修改************************/
/***********************************************************************************************/
/***********************************************************************************************/
/**
  * @brief    SPI0_RX_IRQHandler中断服务函数
  * @note     SPI0 接收中断服务函数
  */
void SPI0_RX_IRQHandler(void)
{
	IfxAsclin_Spi_isrReceive(&g_SpiConfig[0]);
	/* 用户代码 */
}

void SPI0_TX_IRQHandler(void)
{
	IfxAsclin_Spi_isrTransmit(&g_SpiConfig[0]);
	/* 用户代码 */
}

void SPI0_ER_IRQHandler(void)
{
	IfxAsclin_Spi_isrError(&g_SpiConfig[0]);
	/* 用户代码 */
}

void SPI1_RX_IRQHandler(void)
{
	IfxAsclin_Spi_isrReceive(&g_SpiConfig[1]);
	/* 用户代码 */
}

void SPI1_TX_IRQHandler(void)
{
	IfxAsclin_Spi_isrTransmit(&g_SpiConfig[1]);
	/* 用户代码 */
}

void SPI1_ER_IRQHandler(void)
{
	IfxAsclin_Spi_isrError(&g_SpiConfig[1]);
	/* 用户代码 */
}

void SPI2_RX_IRQHandler(void)
{
	IfxAsclin_Spi_isrReceive(&g_SpiConfig[2]);
	/* 用户代码 */
}

void SPI2_TX_IRQHandler(void)
{
	IfxAsclin_Spi_isrTransmit(&g_SpiConfig[2]);
	/* 用户代码 */
}

void SPI2_ER_IRQHandler(void)
{
	IfxAsclin_Spi_isrError(&g_SpiConfig[2]);
	/* 用户代码 */
}

void SPI3_RX_IRQHandler(void)
{
	IfxAsclin_Spi_isrReceive(&g_SpiConfig[3]);
	/* 用户代码 */
}

void SPI3_TX_IRQHandler(void)
{
	IfxAsclin_Spi_isrTransmit(&g_SpiConfig[3]);
	/* 用户代码 */
}

void SPI3_ER_IRQHandler(void)
{
	IfxAsclin_Spi_isrError(&g_SpiConfig[3]);
	/* 用户代码 */
}
/* SPI中断 */
IFX_INTERRUPT(SPI0_RX_IRQHandler, SPI0_VECTABNUM, SPI0_RX_PRIORITY);
IFX_INTERRUPT(SPI1_RX_IRQHandler, SPI1_VECTABNUM, SPI1_RX_PRIORITY);
IFX_INTERRUPT(SPI2_RX_IRQHandler, SPI2_VECTABNUM, SPI2_RX_PRIORITY);
IFX_INTERRUPT(SPI3_RX_IRQHandler, SPI3_VECTABNUM, SPI3_RX_PRIORITY);
IFX_INTERRUPT(SPI0_TX_IRQHandler, SPI0_VECTABNUM, SPI0_TX_PRIORITY);
IFX_INTERRUPT(SPI1_TX_IRQHandler, SPI1_VECTABNUM, SPI1_TX_PRIORITY);
IFX_INTERRUPT(SPI2_TX_IRQHandler, SPI2_VECTABNUM, SPI2_TX_PRIORITY);
IFX_INTERRUPT(SPI3_TX_IRQHandler, SPI3_VECTABNUM, SPI3_TX_PRIORITY);
IFX_INTERRUPT(SPI0_ER_IRQHandler, SPI0_VECTABNUM, SPI0_ER_PRIORITY);
IFX_INTERRUPT(SPI1_ER_IRQHandler, SPI1_VECTABNUM, SPI1_ER_PRIORITY);
IFX_INTERRUPT(SPI2_ER_IRQHandler, SPI2_VECTABNUM, SPI2_ER_PRIORITY);
IFX_INTERRUPT(SPI3_ER_IRQHandler, SPI3_VECTABNUM, SPI3_ER_PRIORITY);

/** SPI中断服务函数地址 */
const void *SpiIrqFuncPointer[12] = {&SPI0_RX_IRQHandler, &SPI0_TX_IRQHandler, &SPI0_ER_IRQHandler,
									&SPI1_RX_IRQHandler, &SPI1_TX_IRQHandler, &SPI1_ER_IRQHandler,
									&SPI2_RX_IRQHandler, &SPI2_TX_IRQHandler, &SPI2_ER_IRQHandler,
									&SPI3_RX_IRQHandler, &SPI3_TX_IRQHandler, &SPI3_ER_IRQHandler,};
/*************************************************************************
*  函数名称：void SPI_InitConfig(SPI_CLK_t clkPin, SPI_MISO_t misoPin, SPI_MOSI_t mosiPin, SPI_CS_t csPin, unsigned long baudrate)
*  功能说明：SPI 主机模式初始化
*  参数说明：
  * @param    clkPin      ： SPI  clk管脚
  * @param    misoPin     :  SPI  MISO管脚
  * @param    misoPin     :  SPI  MOSI管脚
  * @param    misoPin     :  SPI  CS管脚
  * @param    baudrate    ： SPI 波特率   注意最高10M
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：这里实际是将Asc口用作SPI，如果使用了Asc0当作SPI0，那么Asc0 就不可用作串口0
*  			SPI_InitConfig(SPI3_CLK_P15_6, SPI3_MISO_P15_7, SPI3_MOSI_P20_0, SPI3_CS_P21_2, 10000000U); //初始化SPI3 10M速率
*************************************************************************/
void SPI_InitConfig(SPI_CLK_t clkPin, SPI_MISO_t misoPin, SPI_MOSI_t mosiPin, SPI_CS_t csPin, unsigned long baudrate)
{
	unsigned char spiNum = SPI_GetModule(clkPin);

	IfxAsclin_Sclk_Out * IfxAsclin_Sclk = IfxAsclin_Sclk_Out_pinTable[spiNum][SPI_GetIndex(clkPin)];
	IfxAsclin_Rx_In    * IfxAsclin_MISO = IfxAsclin_Rx_In_pinTable[spiNum][SPI_GetIndex(misoPin)];
	IfxAsclin_Tx_Out   * IfxAsclin_MOSI = IfxAsclin_Tx_Out_pinTable[spiNum][SPI_GetIndex(mosiPin)];
	IfxAsclin_Slso_Out * IfxAsclin_CS   = IfxAsclin_Slso_Out_pinTable[spiNum][SPI_GetIndex(csPin)];


    /* 关中断  */
    boolean              interruptState = IfxCpu_disableInterrupts();

    /* create module config */
    IfxAsclin_Spi_Config spiConfig;
    IfxAsclin_Spi_initModuleConfig(&spiConfig, IfxAsclin_Sclk->module);

    spiConfig.frame.shiftDir        = IfxAsclin_ShiftDirection_msbFirst;/* MSB 在前 */

    /* set the desired baudrate */
    spiConfig.baudrate.prescaler    = 1;
    spiConfig.baudrate.baudrate     = (float)baudrate;                 /* spi 速度 */
    spiConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_4;

    spiConfig.inputOutput.cpol      = IfxAsclin_ClockPolarity_idleHigh; /* clk 空闲状态高电平 */

    /* ISR priorities and interrupt target */
    spiConfig.interrupt.rxPriority    = SpiIrqPriority[spiNum * 3];     /* see also \ref IfxLld_Asclin */
    spiConfig.interrupt.txPriority    = SpiIrqPriority[spiNum * 3 + 1]; /* see also \ref IfxLld_Asclin */
    spiConfig.interrupt.erPriority    = SpiIrqPriority[spiNum * 3 + 2]; /* see also \ref IfxLld_Asclin */
    spiConfig.interrupt.typeOfService = (IfxSrc_Tos)SpiIrqVectabNum[spiNum];

    /* pin configuration */
    const IfxAsclin_Spi_Pins pins = {
    		IfxAsclin_Sclk, IfxPort_OutputMode_pushPull, /* Clock out pin */
			IfxAsclin_MISO,   IfxPort_InputMode_pullUp,    /* Rx pin */
			IfxAsclin_MOSI,   IfxPort_OutputMode_pushPull, /* Tx pin */
			IfxAsclin_CS, IfxPort_OutputMode_pushPull, /* Slave select pin */
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    spiConfig.pins = &pins;

    /* initialize module */
    IfxAsclin_Spi_initModule(&g_SpiConfig[spiNum], &spiConfig);

    //接收，发送和错误中断配置
	IfxCpu_Irq_installInterruptHandler((void*)SpiIrqFuncPointer[spiNum * 3],     (unsigned long)SpiIrqPriority[spiNum * 3]);
	IfxCpu_Irq_installInterruptHandler((void*)SpiIrqFuncPointer[spiNum * 3 + 1], (unsigned long)SpiIrqPriority[spiNum * 3 + 1]);
	IfxCpu_Irq_installInterruptHandler((void*)SpiIrqFuncPointer[spiNum * 3 + 2], (unsigned long)SpiIrqPriority[spiNum * 3 + 2]);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
*  函数名称：unsigned char SPI_ReadWriteNByte(SPI_t spi, unsigned char *txData, unsigned char *rxData, unsigned long len)
*  功能说明：SPI读写N个字节
*  参数说明：
  * @param    spi     ：  SPI0 - SPI3
  * @param    txData  ：  写入的buff
  * @param    rxData  ：  读出的buff
  * @param    len     ：  长度
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：这里实际是将Asc口用作SPI，如果使用了Asc0当作SPI0，那么Asc0 就不可用作串口0
*  			SPI_ReadWriteNByte(SPI3, buff, buff, 2); //使用SPI3 读写两个字节
*************************************************************************/
unsigned char SPI_ReadWriteNByte(SPI_t spi, unsigned char *txData, unsigned char *rxData, unsigned long len)
{
	/* SPI传输结构体 */
    IfxAsclin_Spi_exchange(&g_SpiConfig[spi], txData, rxData, len);

    /* 等待传输结束  */
	while (IfxAsclin_Spi_getStatus(&g_SpiConfig[spi]) == IfxAsclin_Spi_Status_busy);

	return 0;
}



/////////////////////////////////////////////////////////////////////////////////////
