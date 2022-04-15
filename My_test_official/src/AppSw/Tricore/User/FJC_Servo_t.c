/*
 * FJC_Servo.c
 *
 *  Created on: 2021��4��3��
 *      Author: 96556
 */

#include <FJC_Servo_t.h>

#define N 3     // ���������



float a[6][3];  // ���ֵ�ռ�����
float ad_VAL[6];           //�˲���һ����ad����
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
*  �������ƣ�float ForServoMID(float i,float j,float k)
*  ����˵������Ŵ�����ȡ������ֵ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2021��4��3��
*  ��    ע����ֵ�˲���
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
*  �������ƣ�void My_ReadADC(void)
*  ����˵������ȡADC��ֵ����a������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2021��4��3��
*  ��    ע����ֵƽ���˲���һ
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
*  �������ƣ�void My_ReadADC(void)
*  ����˵������ȡADC��ֵ����a������
*  ����˵������
*  �������أ���
*************************************************************************/
void Test_Origin()
{
//    static float  AD_val_min[6]={120,0,7,37,0,11};
//    static float  AD_val_max[6]={1551,143,881,147,474,830};
    float sum[6]={0},mid[3]={0};
    // ÿһ�����ֵ����5�β���
    for(int y = 0;y < 5;y++)
    {
        My_ReadADC();  //��ȡ���ֵ��a����
        for(int i = 0;i < 6;i++)
        {
            mid[i] = ForServoMID(a[i][0],a[i][1],a[i][2]);  //ȡ���β�������ֵ
            sum[i] += mid[i];
        }
    }
    // 10�β�����ƽ��
    for(int i = 0;i < 6;i++)
    {
        AD_VAL[i] = sum[i]/5;  //��5�β�����ƽ��ֵ

//        if(AD_VAL[i]>AD_val_max[i])  //�Բɼ������޷�
//            AD_VAL[i]=AD_val_max[i];
    }

    sprintf(txt1,"%.1f ",AD_VAL[0]);
    sprintf(txt2,"%.1f ",AD_VAL[1]);
    sprintf(txt3,"%.1f ",AD_VAL[2]);
    sprintf(txt4,"%.1f ",AD_VAL[3]);
    sprintf(txt5,"%.1f ",AD_VAL[4]);
    sprintf(txt6,"%.1f ",AD_VAL[5]);
    sprintf(txt9,"%d",9999);

//    TFTSPI_P8X16Str(0, 0, txt1,u16WHITE,u16BLACK);       //�ַ�����ʾ
//    TFTSPI_P8X16Str(0, 1, txt2,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 2, txt3,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 3, txt4,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 4, txt5,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 5, txt6,u16WHITE,u16BLACK);



    UART_PutStr(UART0,txt1);               //���ڷ��͵���λ��
    UART_PutStr(UART0,txt2);
    UART_PutStr(UART0,txt3);
    UART_PutStr(UART0,txt4);
    UART_PutStr(UART0,txt5);
    UART_PutStr(UART0,txt6);
    UART_PutStr(UART0,txt9);
    UART_PutStr(UART0,"\r\n");
}


/*************************************************************************
*  �������ƣ�void ForServo(void)
*  ����˵����ADCͨ����ʼ������
*************************************************************************/
void My_ADCInit()
{
    ADC_InitConfig(ADC0, 80000); //��ʼ��
    ADC_InitConfig(ADC1, 80000); //��ʼ��
    ADC_InitConfig(ADC2, 80000); //��ʼ��
    ADC_InitConfig(ADC3, 80000); //��ʼ��
    ADC_InitConfig(ADC4, 80000); //��ʼ��
    ADC_InitConfig(ADC5, 80000); //��ʼ��
    ADC_InitConfig(ADC6, 80000); //��ʼ��

}

/*************************************************************************
*  �������ƣ�void My_ADCDisplay()
*  ����˵���������ʾ�ʹ��亯��
*************************************************************************/
void My_ADCDisplay()
{
//    TFTSPI_P8X16Str(0, 0, txt1,u16WHITE,u16BLACK);       //�ַ�����ʾ
//    TFTSPI_P8X16Str(0, 1, txt2,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 2, txt3,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 3, txt4,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 4, txt5,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 5, txt6,u16WHITE,u16BLACK);

    UART_PutStr(UART0,txt1);               //���ڷ��͵���λ��
    UART_PutStr(UART0,txt2);
    UART_PutStr(UART0,txt3);
    UART_PutStr(UART0,txt4);
    UART_PutStr(UART0,txt5);
    UART_PutStr(UART0,txt6);
    UART_PutStr(UART0,"\r\n");
}
/*************************************************************************
*  �������ƣ�void Get_ADC(void)
*  ����˵�����õ�6����е�ֵ,����ֵ�˲���һ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2021��4��3��
*  ��    ע����ֵƽ���˲���һ
*************************************************************************/
void Get_ADC(void)
{
    /***************************Ҫ�궨�Ĳ���*************************/
    static float  AD_val_min[6]={0,0,0,0,0,0};
    static float  AD_val_max[6]={1380,1226,1440,1343,1340,1594};
    /***************************Ҫ�궨�Ĳ���*************************/

    static unsigned int i;
    static float MID[3]={0};                //��ֵ
    static float AD_VAL[6],num;
    //��ʼ��
    float sum[6]={0,0,0,0,0,0};              // ÿ�ζ�Ҫ���³�ʼ��


    // ÿһ�����ֵ����5�β���
    for(num = 0;num < 10;num++)
    {
        My_ReadADC();  //��ȡ���ֵ��a����
        for(i = 0;i < 6;i++)
        {
            MID[i] = ForServoMID(a[i][0],a[i][1],a[i][2]);  //ȡ���β�������ֵ
            sum[i] += MID[i];
        }
    }
    // 10�β�����ƽ��
    for(i = 0;i < 6;i++)
    {
        AD_VAL[i] = sum[i]/10;  //��5�β�����ƽ��ֵ

        if(AD_VAL[i] < AD_val_min[i])
            AD_VAL[i]=AD_val_min[i];
    }
    // ��һ��
    for(i = 0;i < 6;i++)
    {
        ad_VAL[i]=100*(AD_VAL[i] - AD_val_min[i])/(AD_val_max[i]-AD_val_min[i]);    //��һ�������ɼ�����
    }


    // ��ֵд���ַ�����
    sprintf(txt1,"[1]%.2f ",ad_VAL[0]);
    sprintf(txt2,"[2]%.2f ",ad_VAL[1]);
    sprintf(txt3,"[3]%.2f ",ad_VAL[2]);
    sprintf(txt4,"[4]%.2f ",ad_VAL[3]);
    sprintf(txt5,"[5]%.2f ",ad_VAL[4]);
    sprintf(txt6,"[6]%.2f ",ad_VAL[5]);
////    // TFT��UART��չʾ6����е�ֵ
//    My_ADCDisplay();

}


/*************************************************************************
*  �������ƣ�void Test_error()
*  ����˵��������С�����ҵ�е����ֵ
*  �޸�ʱ�䣺2021��4��3��
*  ��    ע�����ڱ궨������
*************************************************************************/
void Test_error()
{
    float err_group[3];
    // �õ���һ�������
    for(int i=0;i<3;i++)
    {
        err_group[i] = ad_VAL[i] - ad_VAL[5-i];
    }
    /* ���ֵ��ʾ */
    sprintf(txte1,"e1:%.2f ",err_group[0]);
    sprintf(txte2,"e2:%.2f ",err_group[1]);
    sprintf(txte3,"e3:%.2f \r\n",err_group[2]);
//    TFTSPI_P8X16Str(0, 1, txte1,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 2,txte2,u16WHITE,u16BLACK);
//    TFTSPI_P8X16Str(0, 3,txte3,u16WHITE,u16BLACK);
    UART_PutStr(UART0,txte1);               //���ڷ��͵���λ��
    UART_PutStr(UART0,txte2);
    UART_PutStr(UART0,txte3);
}


