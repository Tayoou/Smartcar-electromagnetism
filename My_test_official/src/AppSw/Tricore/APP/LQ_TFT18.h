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
#ifndef __LQ_SGP18T_TFTSPI_H__
#define __LQ_SGP18T_TFTSPI_H__	

#include "stdint.h"

/*******************接口定义******************************/

#define APP_LCD_WIDTH  LCD_WIDTH//液晶屏宽度
#define APP_LCD_HEIGHT LCD_HEIGHT//液晶屏高度

#define TFT18W        162
#define TFT18H        132

#define u16RED		0xf800
#define u16GREEN	0x07e0
#define u16BLUE	    0x001f
#define u16PURPLE	0xf81f
#define u16YELLOW	0xffe0
#define u16CYAN	    0x07ff 		//蓝绿色
#define u16ORANGE	0xfc08
#define u16BLACK	0x0000
#define u16WHITE	0xffff

/*!
  * @brief    TFT18初始化
  *
  * @param    type ： 0:横屏  1：竖屏
  *
  * @return   无
  *
  * @note     如果修改管脚 需要修改初始化的管脚
  *
  * @see      TFTSPI_Init(1);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Init(unsigned char type);

/*!
  * @brief    写命令
  *
  * @param    cmd ：命令
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Write_Cmd(unsigned char cmd);

/*!
  * @brief    写字节
  *
  * @param    dat ：数据
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFTSPI_Write_Byte(0x00);	//CRL=0
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Write_Byte(unsigned char dat);


/*!
  * @brief    写半字
  *
  * @param    dat ：数据
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_Write_Word(0xFFFF);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Write_Word(unsigned short dat);


/*!
  * @brief    重新定位输入信息位置
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFTSPI_Set_Pos(10, 20, 30, 40);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Set_Pos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);

/*!
  * @brief    填充指定区域
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Fill_Area(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color);

/*!
  * @brief    全屏显示单色画面
  *
  * @param    color ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_CLS(u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_CLS(unsigned short color);


/*!
  * @brief    填充矩形区域
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Draw_Part(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    画矩形边框
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Draw_Rectangle(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    画圆
  *
  * @param    x ：圆心x   (0-127)
  * @param    y ：圆心y   (0-159)
  * @param    r ：半径    (0-128)
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     圆心坐标不要超出屏幕范围
  *
  * @see      TFTSPI_Draw_Circle(50, 50, 30, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);

/*!
  * @brief    画线
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Draw_Line(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    画点
  *
  * @param    x ：x
  * @param    y ：y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Draw_Dot(10, 20, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat);

/*!
  * @brief    显示图片
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    ppic ：图片数据
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic);

/*!
  * @brief    液晶字符串输出(6*8字体)
  *
  * @param    x: 0 - 20 (行)
  * @param    y: 0 - 19 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_P6X8Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);

/*!
  * @brief    液晶字符串输出(8*8字体)
  *
  * @param    x:0 - 15 (行)
  * @param    y:0 - 19 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_P8X8Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);

/*!
  * @brief    液晶字符串输出(8*16字体)
  *
  * @param    x: x: 0 -15   (行)
  * @param    y: y: 0 -9  	 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_P8X16Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);


/*!
  * @brief    液晶汉字字符串输出(16*16字体)
  *
  * @param    x: 0 - 7	(行)
  * @param    y: 0 - 9	(列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     汉字只能是字库里的 字库没有的需要自行添加
  *
  * @see      TFTSPI_P16x16Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);


/*!
  * @brief    显示图片
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    ppic ：图片数据
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic) ;


/*!
  * @brief    显示电池图标
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_Show_Battery_Icon();
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Battery_Icon(void);

/*!
  * @brief    显示标签
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_Show_Title();
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Title(void);

/*!
  * @brief    显示龙邱LOG
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_Show_Logo();
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Logo(unsigned char xs,unsigned char ys);

/*!
  * @brief    TFT18屏 unsigned char 灰度数据显示
  *
  * @param    high_start ： 显示图像开始位置
  * @param    wide_start ： 显示图像开始位置
  * @param    high ： 显示图像高度
  * @param    wide ： 显示图像宽度
  * @param    Pixle： 显示图像数据地址
  *
  * @return   无
  *
  * @note     注意 屏幕左上为 （0，0）
  *
  * @see
  *
  * @date     2019/12/3 星期二
  */
void TFTSPI_Road( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char *Pixle);

/*!
  * @brief    TFT18屏 unsigned char 二值化数据显示
  *
  * @param    high_start ： 显示图像开始位置
  * @param    wide_start ： 显示图像开始位置
  * @param    high ： 显示图像高度
  * @param    wide ： 显示图像宽度
  * @param    Pixle： 显示图像数据地址
  *
  * @return   无
  *
  * @note     注意 屏幕左上为 （0，0）
  *
  * @see
  *
  * @date     2019/12/3 星期二
  */
void TFTSPI_BinRoad( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char *Pixle);


void Test_TFT18(void);


#endif /*SGP18T_ILI9163B.h*/
