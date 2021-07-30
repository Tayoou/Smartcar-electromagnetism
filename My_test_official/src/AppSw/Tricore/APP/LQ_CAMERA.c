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
 *  备    注：TC264只有15个固定的GPIO支持外部中断 这15个固定GPIO分为4组，每组只能选择其中一个作为外部中断管脚使用
 *           0：P15_4  P33_7  P15_5                             1：P14_3  P15_8
 *           2：P10_2  P02_1  P00_4  P20_0  P11_10              3：P10_3  P14_1  P02_0  P20_9  P15_1
 *			外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
 *
 ________________________________________________________________
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 摄像头管脚  TC264DA管脚   说明              DMA方式采集所用管脚通过电阻跳线
 D0        P02_0       摄像头数据bit0
 D1        P02_1       摄像头数据bit1
 D2        P02_2       摄像头数据bit2
 D3        P02_3       摄像头数据bit3
 D4        P02_4       摄像头数据bit4
 D5        P02_5       摄像头数据bit5
 D6        P02_6       摄像头数据bit6
 D7        P02_7       摄像头数据bit7
 CLK       P00_4       像素时钟        外部中断第2组：P00_4
 VSNC      P15_1       场信号             外部中断第3组：P15_1

 SCL       P11_2       兼容模拟SCCB SCL
 SDA       P11_3       兼容模拟SCCB SDA
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu.h>
#include <IfxPort_reg.h>
#include <LQ_CAMERA.h>
#include <LQ_DMA.h>
#include <LQ_GPIO.h>
#include <LQ_GPIO_LED.h>
#include <LQ_TFT18.h>
#include <LQ_UART.h>
#include <LQ_STM.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <include.h>

/** 图像原始数据存放 */
unsigned char Image_Data[IMAGEH][IMAGEW];

/** 压缩后之后用于存放屏幕显示数据  */
unsigned char Image_Use[LCDH][LCDW];

/** 二值化后用于OLED显示的数据 */
unsigned char Bin_Image[LCDH][LCDW];

sint16 OFFSET0 = 0;      //最远处，赛道中心值综合偏移量
sint16 OFFSET1 = 0;      //第二格
sint16 OFFSET2 = 0;      //最近，第三格
sint16 TXV = 0;          //梯形的左高度，右高度

/*!
 * @brief    摄像头测试例程
 *
 * @param
 *
 * @return
 *
测试MT9V034  注意需要使用  带与非门版（白色）转接座
 *
 * @example
 *
 * @date     2019/10/22 星期二
 */
void Test_CAMERA (void)
{

#ifdef USEOLED
    OLED_Init();
    OLED_CLS();                   //LCD清屏
#else
    TFTSPI_Init(0);               //TFT1.8初始化0:横屏  1：竖屏
    TFTSPI_CLS(u16BLUE);          //清屏
#endif

    /* 摄像头初始化 */
    CAMERA_Init(50);

    while (1)
    {
        if (Camera_Flag == 2)
        {

            //上报数据给上位机 串口速度比较慢 注意上位机图像宽高设置为120*188
            CAMERA_Reprot();

            /* 提取部分使用的数据 */
            Get_Use_Image();

            /* 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据 */
            Camera_Flag = 0;

#ifdef USEOLED

            /* 二值化 */
            Get_Bin_Image(2);


            // OLED动态显示摄像头图像
            OLED_Road(LCDH, LCDW, (unsigned char *)Bin_Image);
#else

#if 0       //显示原始图像
            //TFT1.8动态显示摄像头图像
            TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);

#else       //显示二值化图像

            /* 二值化 */
            Get_Bin_Image(2);

            // 显示摄像头图像
            TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *) Bin_Image);
#endif
#endif
            LED_Ctrl(LED0, RVS);

        }
    }
}

/*!
 * @brief    串口上报上位机
 *
 * @param    无
 *
 * @return   无
 *
上位机的帧头可能有所区别
 *
 * @see      CAMERA_Reprot();
 *
 * @date     2019/9/24 星期二
 */
void CAMERA_Reprot (void)
{
    short j, i;

    UART_PutChar(UART0, 0xfe);  //帧头
    UART_PutChar(UART0, 0xef);  //帧头

    for (i = 0; i < IMAGEH; i++)
    {
        for (j = 0; j < IMAGEW; j++)
        {
            if (Image_Data[i][j] == 0xfe)  //防止错误发送帧尾
            {
                Image_Data[i][j] = 0xff;
            }
            UART_PutChar(UART0, Image_Data[i][j]); //发送数据

        }
    }
    UART_PutChar(UART0, 0xef);  //帧尾
    UART_PutChar(UART0, 0xfe);  //帧尾

}

/*!
 * @brief    摄像头初始化
 *
 * @param    fps:  帧率
 *
 * @return   无
 *
摄像头的一些参数，在LQ_MT9V034.c中的宏定义中修改
 *
 * @see      CAMERA_Init(50);   //初始化MT9V034  50帧 注意使用白色带与非门版转接座
 *
 * @date     2019/10/22 星期二
 */
void CAMERA_Init (unsigned char fps)
{
    //关闭CPU总中断
    IfxCpu_disableInterrupts();
    
    /* 初始化摄像头 数据IO */
    PIN_InitConfig(P02_0, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_1, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_2, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_3, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_4, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_5, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_6, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P02_7, PIN_MODE_INPUT_PULLDOWN, 0);

    MT9V034_Init(fps);

    /* DMA 触发源初始化 */
    PIN_Exti(P00_4, PIN_IRQ_MODE_FALLING);

    /* DMA 初始化 */
    DMA_CameraInitConfig((unsigned long) (&MODULE_P02.IN.U), (unsigned long) Image_Data, PIN_INT2_PRIORITY);
    
    /* 使能场中断 */
    PIN_Exti(P15_1, PIN_IRQ_MODE_RISING);

    /* 初始化完成 打开中断 */
    IfxCpu_enableInterrupts();

}

/*************************************************************************
 *  函数名称：void Get_Use_Image (void)
 *  功能说明：把摄像头采集到原始图像，缩放到赛道识别所需大小
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年10月28日
 *  备    注：  IMAGEW为原始图像的宽度，神眼为188，OV7725为320
 *       IMAGEH为原始图像的高度，神眼为120，OV7725为240
 *************************************************************************/
void Get_Use_Image(void)
{
    short i = 0, j = 0, row = 0, line = 0;

    for (i = 0; i < IMAGEH; i += 2)          //神眼高 120 / 2  = 60，
    // for (i = 0; i < IMAGEH; i += 3)       //OV7725高 240 / 3  = 80，
    {
        for (j = 0; j <= IMAGEW; j += 2)     //神眼宽188 / 2  = 94，
        // for (j = 0; j <= IMAGEW; j += 3)  //OV7725宽320 / 3  = 106，
        {
            Image_Use[row][line] = Image_Data[i][j];
            line++;
        }
        line = 0;
        row++;
    }
}

/*************************************************************************
 *  函数名称：void Get_Bin_Image (unsigned char mode)
 *  功能说明：图像二值化到Bin_Image[][]
 *  参数说明：mode  ：
 *    0：使用大津法阈值
 *    1：使用平均阈值
 *    2: sobel 算子改进型  手动阈值，同时输出改为提取边沿的图像
 *    3：sobel 算子改进型   动态阈值，同时输出改为提取边沿的图像
 *  函数返回：无
 *  修改时间：2020年10月28日
 *  备    注：  Get_Bin_Image(0); //使用大津法二值化
 *************************************************************************/
void Get_Bin_Image (unsigned char mode)
{
    unsigned short i = 0, j = 0;
    unsigned short Threshold = 0;
    unsigned long tv = 0;
    //char txt[16];

    if (mode == 0)
    {
        Threshold = GetOSTU(Image_Use);  //大津法阈值
    }
    if (mode == 1)
    {
        //累加
        for (i = 0; i < LCDH; i++)
        {
            for (j = 0; j < LCDW; j++)
            {
                tv += Image_Use[i][j];   //累加
            }
        }
        Threshold =(unsigned short)(tv / LCDH / LCDW);   //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
        Threshold = Threshold + 20;      //此处阈值设置，根据环境的光线来设定
    }
    else if (mode == 2)
    {
        Threshold = 80;                          //手动调节阈值
        lq_sobel(Image_Use, Bin_Image, (unsigned char) Threshold);

        return;

    }
    else if (mode == 3)
    {
        lq_sobelAutoThreshold(Image_Use, Bin_Image);  //动态调节阈值
        return;
    }
/*
#ifdef USEOLED
    sprintf(txt,"%03d",Threshold);//显示阈值
	OLED_P6x8Str(80,0,txt);
#else
    sprintf(txt, "%03d", Threshold);  //显示阈值
    TFTSPI_P6X8Str(0,7, txt, u16RED, u16BLUE);
#endif
*/
    /* 二值化 */
    for (i = 0; i < LCDH; i++)
    {
        for (j = 0; j < LCDW; j++)
        {
            if (Image_Use[i][j] > Threshold) //数值越大，显示的内容越多，较浅的图像也能显示出来
                Bin_Image[i][j] = 1;
            else
                Bin_Image[i][j] = 0;
        }
    }
}
/*************************************************************************
 *  函数名称：short GetOSTU (unsigned char tmImage[LCDH][LCDW])
 *  功能说明：大津法求阈值大小
 *  参数说明：tmImage ： 图像数据
 *  函数返回：无
 *  修改时间：2011年10月28日
 *  备    注：  GetOSTU(Image_Use);//大津法阈值
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代 1
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像
        的比例w0，        并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背
        景像素)  * 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7) 将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
*************************************************************************/
short GetOSTU (unsigned char tmImage[LCDH][LCDW])
{
    signed short i, j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelshortegralBack = 0;
    unsigned long Pixelshortegral = 0;
    signed long PixelshortegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    signed short MinValue, MaxValue;
    signed short Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < LCDH; j++)
    {
        for (i = 0; i < LCDW; i++)
        {
            HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++);        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--); //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j];        //  像素总数

    Pixelshortegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        Pixelshortegral += HistoGram[j] * j;        //灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];     //前景像素点数
        PixelFore = Amount - PixelBack;           //背景像素点数
        OmegaBack = (float) PixelBack / Amount;   //前景像素百分比
        OmegaFore = (float) PixelFore / Amount;   //背景像素百分比
        PixelshortegralBack += HistoGram[j] * j;  //前景灰度值
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //背景灰度值
        MicroBack = (float) PixelshortegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float) PixelshortegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);   //计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //返回最佳阈值;
}

/*!
 * @brief    基于soble边沿检测算子的一种边沿检测
 *
 * @param    imageIn    输入数组
 *           imageOut   输出数组      保存的二值化后的边沿信息
 *           Threshold  阈值
 *
 * @return
 *
 * @note
 *
 * @example
 *
 * @date     2020/5/15
 */
void lq_sobel (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold)
{
    /** 卷积核大小 */
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = LCDW - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = LCDH - KERNEL_SIZE / 2;
    short i, j, k;
    short temp[4];
    for (i = yStart; i < yEnd; i++)
    {
        for (j = xStart; j < xEnd; j++)
        {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j + 1]     //{{-1, 0, 1},
            - (short) imageIn[i][j - 1] + (short) imageIn[i][j + 1]        // {-1, 0, 1},
            - (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j + 1];    // {-1, 0, 1}};

            temp[1] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j - 1]     //{{-1, -1, -1},
            - (short) imageIn[i - 1][j] + (short) imageIn[i + 1][j]       // { 0,  0,  0},
            - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];    // { 1,  1,  1}};

            temp[2] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j - 1]       //  0, -1, -1
            - (short) imageIn[i][j + 1] + (short) imageIn[i + 1][j]       //  1,  0, -1
            - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j - 1];    //  1,  1,  0

            temp[3] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j + 1]       // -1, -1,  0
            - (short) imageIn[i][j - 1] + (short) imageIn[i + 1][j]       // -1,  0,  1
            - (short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j + 1];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for (k = 1; k < 4; k++)
            {
                if (temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            if (temp[0] > Threshold)
            {
                imageOut[i][j] = 1;
            }
            else
            {
                imageOut[i][j] = 0;
            }
        }
    }
}

/*!
 * @brief    基于soble边沿检测算子的一种自动阈值边沿检测
 *
 * @param    imageIn    输入数组
 *           imageOut   输出数组      保存的二值化后的边沿信息
 *
 * @return
 *
 * @note
 *
 * @example
 *
 * @date     2020/5/15
 */
void lq_sobelAutoThreshold (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW])
{
    /** 卷积核大小 */
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = LCDW - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = LCDH - KERNEL_SIZE / 2;
    short i, j, k;
    short temp[4];
    for (i = yStart; i < yEnd; i++)
    {
        for (j = xStart; j < xEnd; j++)
        {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j + 1]     //{{-1, 0, 1},
            - (short) imageIn[i][j - 1] + (short) imageIn[i][j + 1]       // {-1, 0, 1},
            - (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j + 1];    // {-1, 0, 1}};

            temp[1] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j - 1]     //{{-1, -1, -1},
            - (short) imageIn[i - 1][j] + (short) imageIn[i + 1][j]       // { 0,  0,  0},
            - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];    // { 1,  1,  1}};

            temp[2] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j - 1]       //  0, -1, -1
            - (short) imageIn[i][j + 1] + (short) imageIn[i + 1][j]       //  1,  0, -1
            - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j - 1];    //  1,  1,  0

            temp[3] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j + 1]       // -1, -1,  0
            - (short) imageIn[i][j - 1] + (short) imageIn[i + 1][j]       // -1,  0,  1
            - (short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j + 1];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for (k = 1; k < 4; k++)
            {
                if (temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            /* 使用像素点邻域内像素点之和的一定比例    作为阈值  */
            temp[3] = (short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j] + (short) imageIn[i - 1][j + 1]
                    + (short) imageIn[i][j - 1] + (short) imageIn[i][j] + (short) imageIn[i][j + 1]
                    + (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j] + (short) imageIn[i + 1][j + 1];

            if (temp[0] > temp[3] / 12.0f)
            {
                imageOut[i][j] = 1;
            }
            else
            {
                imageOut[i][j] = 0;
            }

        }
    }
}

/*---------------------------------------------------------------
 【函    数】Bin_Image_Filter
 【功    能】过滤噪点
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Bin_Image_Filter (void)
{
    sint16 nr; //行
    sint16 nc; //列

    for (nr = 1; nr < LCDH - 1; nr++)
    {
        for (nc = 1; nc < LCDW - 1; nc = nc + 1)
        {
            if ((Bin_Image[nr][nc] == 0)
                    && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] > 2))
            {
                Bin_Image[nr][nc] = 1;
            }
            else if ((Bin_Image[nr][nc] == 1)
                    && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] < 2))
            {
                Bin_Image[nr][nc] = 0;
            }
        }
    }
}

/***************************************************************************
 *                                                                          *
 *  函数名称：Seek_Road(void)                                           *
 *  功能说明：寻找白色区域偏差值                                            *
 *  参数说明：无                                                            *
 *  函数返回：值的大小                                                      *
 *  修改时间：2017-07-16                                                    *
 *  备    注：以中间为0，左侧减一，右侧加一，数值代表1的面积                *
 *            计算区域从第一行开始到倒数第二行结束。                        *
 *            如果面积为负数，数值越大说明越偏左边；                        *
 *            如果面积为正数，数值越大说明越偏右边。                        *
 ***************************************************************************/
void Seek_Road (void)
{
    sint16 nr; //行
    sint16 nc; //列
    sint16 temp = 0; //临时数值
    //for(nr=1; nr<MAX_ROW-1; nr++)
    temp = 0;
    for (nr = 8; nr < 24; nr++)
    {
        for (nc = MAX_COL / 2; nc < MAX_COL; nc = nc + 1)
        {
            if (Bin_Image[nr][nc])
            {
                ++temp;
            }
        }
        for (nc = 0; nc < MAX_COL / 2; nc = nc + 1)
        {
            if (Bin_Image[nr][nc])
            {
                --temp;
            }
        }
    }
    OFFSET0 = temp;
    temp = 0;
    for (nr = 24; nr < 40; nr++)
    {
        for (nc = MAX_COL / 2; nc < MAX_COL; nc = nc + 1)
        {
            if (Bin_Image[nr][nc])
            {
                ++temp;
            }
        }
        for (nc = 0; nc < MAX_COL / 2; nc = nc + 1)
        {
            if (Bin_Image[nr][nc])
            {
                --temp;
            }
        }
    }
    OFFSET1 = temp;
    temp = 0;
    for (nr = 40; nr < 56; nr++)
    {
        for (nc = MAX_COL / 2; nc < MAX_COL; nc = nc + 1)
        {
            if (Bin_Image[nr][nc])
            {
                ++temp;
            }
        }
        for (nc = 0; nc < MAX_COL / 2; nc = nc + 1)
        {
            if (Bin_Image[nr][nc])
            {
                --temp;
            }
        }
    }
    OFFSET2 = temp;
    return;
}
