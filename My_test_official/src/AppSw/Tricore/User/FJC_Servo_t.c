/*
 * FJC_Servo.c
 *
 *  Created on: 2021年4月3日
 *      Author: 96556
 */

#include <FJC_Servo_t.h>

#define N 3     // 多少组电容



float a[6][3];  // 电感值收集数组
float ad_VAL[6];           //滤波归一化后ad数据
float AD_VAL[6];



char txt1[10] = {0};
char txt2[10] = {0};
char txt3[10] = {0};
char txt4[10] = {0};
char txt5[10] = {0};
char txt6[10] = {0};
char txt9[10] = {0};
char txte1[10] = {0};
char txte2[10] = {0};
char txte3[10] = {0};


/*************************************************************************
*  函数名称：float ForServoMID(float i,float j,float k)
*  功能说明：电磁传感器取传入中值
*  参数说明：无
*  函数返回：无
*  修改时间：2021年4月3日
*  备    注：中值滤波法
*************************************************************************/
float ForServoMID(float i,float j,float k)
{
    float tmp;
    if (i > j)
    {
        tmp = i; i = j; j = tmp;
    }
    if (k > j)
        tmp = j;
    else if(k > i)
        tmp = k;
    else
        tmp = i;
    return tmp;
}
/*************************************************************************
*  函数名称：void My_ReadADC(void)
*  功能说明：读取ADC的值存入a数组中
*  参数说明：无
*  函数返回：无
*  修改时间：2021年4月3日
*  备    注：中值平均滤波归一
*************************************************************************/
void My_ReadADC()
{
    a[0][0]=ADC_Read(ADC1);
    a[1][0]=ADC_Read(ADC2);
    a[2][0]=ADC_Read(ADC3);
    a[3][0]=ADC_Read(ADC4);
    a[4][0]=ADC_Read(ADC5);
    a[5][0]=ADC_Read(ADC6);

    a[0][1]=ADC_Read(ADC1);
    a[1][1]=ADC_Read(ADC2);
    a[2][1]=ADC_Read(ADC3);
    a[3][1]=ADC_Read(ADC4);
    a[4][1]=ADC_Read(ADC5);
    a[5][1]=ADC_Read(ADC6);

    a[0][2]=ADC_Read(ADC1);
    a[1][2]=ADC_Read(ADC2);
    a[2][2]=ADC_Read(ADC3);
    a[3][2]=ADC_Read(ADC4);
    a[4][2]=ADC_Read(ADC5);
    a[5][2]=ADC_Read(ADC6);
}

/*************************************************************************
*  函数名称：void My_ReadADC(void)
*  功能说明：读取ADC的值存入a数组中
*  参数说明：无
*  函数返回：无
*************************************************************************/
void Test_Origin()
{
//    static float  AD_val_min[6]={120,0,7,37,0,11};
//    static float  AD_val_max[6]={1551,143,881,147,474,830};
    float sum[6]={0},mid[3]={0};
    // 每一个电感值进行5次采样
    for(int y = 0;y < 5;y++)
    {
        My_ReadADC();  //读取电感值到a数组
        for(int i = 0;i < 6;i++)
        {
            mid[i] = ForServoMID(a[i][0],a[i][1],a[i][2]);  //取三次采样的中值
            sum[i] += mid[i];
        }
    }
    // 10次采样求平均
    for(int i = 0;i < 6;i++)
    {
        AD_VAL[i] = sum[i]/5;  //求5次采样的平均值

//        if(AD_VAL[i]>AD_val_max[i])  //对采集数据限幅
//            AD_VAL[i]=AD_val_max[i];
    }

    sprintf(txt1,"%.1f ",AD_VAL[0]);
    sprintf(txt2,"%.1f ",AD_VAL[1]);
    sprintf(txt3,"%.1f ",AD_VAL[2]);
    sprintf(txt4,"%.1f ",AD_VAL[3]);
    sprintf(txt5,"%.1f ",AD_VAL[4]);
    sprintf(txt6,"%.1f ",AD_VAL[5]);
    sprintf(txt9,"%d",9999);

//    TFTSPI_P8X16Str(0, 0, txt1,u16WHITE,u16BLACK);       //字符串显示
//    TFTSPI_P8X16Str(0, 1, txt2,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 2, txt3,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 3, txt4,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 4, txt5,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 5, txt6,u16WHITE,u16BLACK);



    UART_PutStr(UART0,txt1);               //串口发送到上位机
    UART_PutStr(UART0,txt2);
    UART_PutStr(UART0,txt3);
    UART_PutStr(UART0,txt4);
    UART_PutStr(UART0,txt5);
    UART_PutStr(UART0,txt6);
    UART_PutStr(UART0,txt9);
    UART_PutStr(UART0,"\r\n");
}


/*************************************************************************
*  函数名称：void ForServo(void)
*  功能说明：ADC通道初始化函数
*************************************************************************/
void My_ADCInit()
{
    ADC_InitConfig(ADC0, 80000); //初始化
    ADC_InitConfig(ADC1, 80000); //初始化
    ADC_InitConfig(ADC2, 80000); //初始化
    ADC_InitConfig(ADC3, 80000); //初始化
    ADC_InitConfig(ADC4, 80000); //初始化
    ADC_InitConfig(ADC5, 80000); //初始化
    ADC_InitConfig(ADC6, 80000); //初始化

}

/*************************************************************************
*  函数名称：void My_ADCDisplay()
*  功能说明：结果显示和传输函数
*************************************************************************/
void My_ADCDisplay()
{
//    TFTSPI_P8X16Str(0, 0, txt1,u16WHITE,u16BLACK);       //字符串显示
//    TFTSPI_P8X16Str(0, 1, txt2,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 2, txt3,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 3, txt4,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 4, txt5,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 5, txt6,u16WHITE,u16BLACK);

    UART_PutStr(UART0,txt1);               //串口发送到上位机
    UART_PutStr(UART0,txt2);
    UART_PutStr(UART0,txt3);
    UART_PutStr(UART0,txt4);
    UART_PutStr(UART0,txt5);
    UART_PutStr(UART0,txt6);
    UART_PutStr(UART0,"\r\n");
}
/*************************************************************************
*  函数名称：void Get_ADC(void)
*  功能说明：得到6个电感的值,并均值滤波归一化
*  参数说明：无
*  函数返回：无
*  修改时间：2021年4月3日
*  备    注：中值平均滤波归一
*************************************************************************/
void Get_ADC(void)
{
    /***************************要标定的参数*************************/
    static float  AD_val_min[6]={0,0,0,0,0,0};
    static float  AD_val_max[6]={1380,1226,1440,1343,1340,1594};
    /***************************要标定的参数*************************/

    static unsigned int i;
    static float MID[3]={0};                //中值
    static float AD_VAL[6],num;
    //初始化
    float sum[6]={0,0,0,0,0,0};              // 每次都要重新初始化


    // 每一个电感值进行5次采样
    for(num = 0;num < 10;num++)
    {
        My_ReadADC();  //读取电感值到a数组
        for(i = 0;i < 6;i++)
        {
            MID[i] = ForServoMID(a[i][0],a[i][1],a[i][2]);  //取三次采样的中值
            sum[i] += MID[i];
        }
    }
    // 10次采样求平均
    for(i = 0;i < 6;i++)
    {
        AD_VAL[i] = sum[i]/10;  //求5次采样的平均值

        if(AD_VAL[i] < AD_val_min[i])
            AD_VAL[i]=AD_val_min[i];
    }
    // 归一化
    for(i = 0;i < 6;i++)
    {
        ad_VAL[i]=100*(AD_VAL[i] - AD_val_min[i])/(AD_val_max[i]-AD_val_min[i]);    //归一化处理采集数据
    }


    // 将值写入字符数组
    sprintf(txt1,"[1]%.2f ",ad_VAL[0]);
    sprintf(txt2,"[2]%.2f ",ad_VAL[1]);
    sprintf(txt3,"[3]%.2f ",ad_VAL[2]);
    sprintf(txt4,"[4]%.2f ",ad_VAL[3]);
    sprintf(txt5,"[5]%.2f ",ad_VAL[4]);
    sprintf(txt6,"[6]%.2f ",ad_VAL[5]);
////    // TFT和UART的展示6个电感的值
//    My_ADCDisplay();

}


/*************************************************************************
*  函数名称：void Test_error()
*  功能说明：测试小车左右电感的误差值
*  修改时间：2021年4月3日
*  备    注：用于标定车身在
*************************************************************************/
void Test_error()
{
    float err_group[3];
    // 拿到归一化后误差
    for(int i=0;i<3;i++)
    {
        err_group[i] = ad_VAL[i] - ad_VAL[5-i];
    }
    /* 误差值显示 */
    sprintf(txte1,"e1:%.2f ",err_group[0]);
    sprintf(txte2,"e2:%.2f ",err_group[1]);
    sprintf(txte3,"e3:%.2f \r\n",err_group[2]);
//    TFTSPI_P8X16Str(0, 1, txte1,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 2,txte2,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 3,txte3,u16WHITE,u16BLACK);
    UART_PutStr(UART0,txte1);               //串口发送到上位机
    UART_PutStr(UART0,txte2);
    UART_PutStr(UART0,txte3);
}



