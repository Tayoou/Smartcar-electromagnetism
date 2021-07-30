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
#ifndef _LQOLED_H
#define _LQOLED_H

#include <stdint.h>

/*!
  * @brief    OLED初始化
  *
  * @param    无
  *
  * @return   无
  *
  * @note     如果修改管脚 需要修改初始化的管脚 
  *
  * @see      OLED_Init();
  *
  * @date     2019/6/13 星期四
  */
void OLED_Init(void);

/*!
  * @brief    OLED全亮
  *
  * @param    无   
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Fill(); //OLED全亮 用于检测坏点
  *
  * @date     2019/6/13 星期四
  */
void OLED_Fill(void);

/*!
  * @brief    OLED清屏
  *
  * @param    无  
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_CLS(); //清屏
  *
  * @date     2019/6/13 星期四
  */
void OLED_CLS(void);

/*!
  * @brief    画一个点
  *
  * @param    x  : 0-127 
  * @param    y  : 0-63
  *
  * @return   无
  *
  * @note     OLED是按字节显示的 一个字节8位，代表8个纵向像素点，
  * @note     因此如果画点 OLED_PutPixel(0,0); OLED_PutPixel(0,1); 只会显示点(0, 1) (0,0)则在画点(0,1)时清除掉
  *
  * @see      OLED_PutPixel(0, 0); //画一个点 左上角为0,0  右下角 127,63
  *
  * @date     2019/6/13 星期四
  */
void OLED_PutPixel(unsigned char x,unsigned char y);

/*!
  * @brief    清除8个点
  *
  * @param    x  : 0-127
  * @param    y  : 0-63
  *
  * @return   无
  *
  * @note     OLED是按字节显示的 一个字节8位，代表8个纵向像素点，
  *
  * @see      OLED_ClrDot(0, 0); //清除(0,0)-(0,7)8个点 左上角为0,0  右下角 127,63
  *
  * @date     2019/6/13 星期四
  */
void OLED_ClrDot(unsigned char x,unsigned char y);

/*!
  * @brief    画矩形
  *
  * @param    x1  : 0-127   左上角坐标（x1,y1）
  * @param    y1  : 0-63
  * @param    x2  : 0-127   右下角坐标（x2，y2）
  * @param    y2  : 0-63
  * @param    gif : 是否开启动画效果
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Rectangle(0, 0, 127,62,0);   //绘制矩形
  *
  * @date     2019/6/13 星期四
  */
void OLED_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);

/*!
  * @brief    写入一组标准ASCII字符串
  *
  * @param    x  : 0-127   
  * @param    y  : 0-7
  * @param    ch : 要显示的字符串
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_P6x8Str(0,0,"OLED 3.");  //显示字符串
  *
  * @date     2019/6/13 星期四
  */
void OLED_P6x8Str(unsigned char x,unsigned char y, char ch[]);

/*!
  * @brief    写入一组标准ASCII字符串
  *
  * @param    x  : 0-127   
  * @param    y  : 0-6
  * @param    ch : 要显示的字符串
  *
  * @return   无
  *
  * @note     一个字符高16  OLED高度64  
  *
  * @see      OLED_P8x16Str(0,0,"OLED 3.");  //显示字符串
  *
  * @date     2019/6/13 星期四
  */
void OLED_P8x16Str(unsigned char x,unsigned char y, char ch[]);

/*!
  * @brief    输出汉字字符串
  *
  * @param    x  : 0-127   
  * @param    y  : 0-6
  * @param    ch : 要显示的字符串
  *
  * @return   无
  *
  * @note     一个字符高16  OLED高度64   字库需要自己制作
  *
  * @see      OLED_P14x16Str(0,4,"北京龙邱");   //显示汉字
  *
  * @date     2019/6/13 星期四
  */
void OLED_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);



/*!
  * @brief    显示BMP图片 最大 128×64
  *
  * @param    x0  : 0-127   左上角坐标（x0,y0）
  * @param    y0  : 0-63
  * @param    wide: 1-127   图片宽度
  * @param    high: 1-64    图片高度
  * @param    bmp : 要显示的图片
  *
  * @return   无
  *
  * @note     图片需要用软件转化
  *
  * @see      OLED_Show_BMP(0, 0, 104, 64, longqiu104x64); //画龙邱loge
  *
  * @date     2019/6/13 星期四
  */
void OLED_Show_BMP(unsigned char x0,unsigned char y0,unsigned char wide,unsigned char high,const unsigned char * bmp);

/*!
  * @brief    显示龙邱图片128×64
  *
  * @param    无 
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Show_LQLogo();
  *
  * @date     2019/6/13 星期四
  */
void OLED_Show_LQLogo(void);

/*!
  * @brief    画边线 OV7725显示图像时 可以用这个画边线
  *
  * @param    无 
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Show_Frame80();
  *
  * @date     2019/6/13 星期四
  */
void OLED_Show_Frame80(void);

/*!
  * @brief    画边线 MT9V034显示图像时 可以用这个画边线
  *
  * @param    无 
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Show_Frame94();
  *
  * @date     2019/6/13 星期四
  */
void OLED_Show_Frame94(void);

/**
  * @brief    OLED 二值化图像显示
  *
  * @param    high ： 显示图像高度
  * @param    wide ： 显示图像宽度
  * @param    Pixle： 显示图像数据地址
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Road(OLEDH, OLEDW, (unsigned char *)Pixle);
  *
  * @date     2019/6/25 星期二
  */
void OLED_Road(unsigned short high, unsigned short wide, unsigned char *Pixle);

/*!
  * @brief    OLED命令
  *
  * @param    cmd ：命令
  *
  * @return   无
  *
  * @note     无
  *
  * @see      内部调用
  *
  * @date     2019/6/13 星期四
  */
void OLED_WrCmd(unsigned char cmd);

/*!
  * @brief    OLED设置点
  *
  * @param    x ：坐标   
  * @param    y ：坐标
  *
  * @return   无
  *
  * @note     无
  *
  * @see      内部调用
  *
  * @date     2019/6/13 星期四
  */
void OLED_Set_Pos(unsigned char x, unsigned char y);
	
/**
  * @brief    OLED 测试函数
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  Test_OLED();
  *
  * @date     2019/6/25 星期二
  */
void Test_OLED(void);

#endif


