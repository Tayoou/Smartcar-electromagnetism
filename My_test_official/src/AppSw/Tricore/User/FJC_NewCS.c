/*
 * FJC_NewCS.c
 *
 *  Created on: 2021年4月29日
 *      Author: 96556
 */

#include "FJC_NewCS.h"
#include <LQ_STM.h>
#include <LQ_CCU6.h>

/****************模糊pid控制板块**************************************/
#define PMAX 100
#define PMIN -100
#define DMAX 100
#define DMIN -100
#define FMAX 100       //语言值的满幅值

extern float ad_VAL[6];           //滤波归一化后ad数据
extern float AD_VAL[6];

/***************记忆控制板块*************************************/
extern volatile int encValue5;
unsigned char RemLength = 1;         //区分正常距离与倒车距离，1为直道记录，2为直角弯/小角度弯道记录，3为十字记录，4为环岛记录

volatile float HPsum = 0;                     //编码器求和辅助变量
volatile float ZTsum = 0;                     //直道编码器积分
float DCsum = 0;                     //倒车编码器积分
float SZsum = 0;                     //十字编码器积分
float HDsum = 0;                     //环岛编码器积分

float Remforward = 40.36;                //记忆直角路程
float Remsforward = 5;               //记忆大角度路程

extern volatile uint8 Game_Over; // 小车完成全部任务，停车
volatile float distance[50] = {0};           //赛道直角距离 （编码器数值/512）
volatile int direction[50] = {0};            //赛道直角方向（1为左，2为右，3为左小角度，4为右小角度)

float laternum = 16.135;                  //直角滞后
int goflag = 1;               //电机使能
/***************倒车与状态板块**********************************/
extern unsigned char dao_flag;
unsigned char res = 0;


float forward = 320;               //直角倒车前进距离
float backward = -240;              //直角倒车后退距离

float sforward = 10.3;           //小角度倒车前进距离
float sbackward = -8.97;            //小角度倒车后退距离

float szkeep = 50;                //十字保持时间
float hdkeep = 150;                //环岛保持时间
float out_hdkeep = 55;

unsigned char slow_flag = 0;

unsigned int fjc_duty = Servo_Center_Mid;
int set_speed = 3000;
int back_speed = 0;
unsigned int help_slow = 0;

unsigned char hd_dir = 0;
unsigned char hd_in = 0;
unsigned int speed = 2000;
extern unsigned char hd_flag;

extern volatile int cnt;
extern unsigned int cnt_return;
extern unsigned char dao_flag;

extern int realpwm;

int PFF[4] = {0,11,20,28};    //PFF[3]为电感最大值，其余值等值增加即可
int DFF[4] = {0,7,13,19};      //DFF[3]为电感变化量的最大值，其余值等值增加即可
int UFF[7] = {0,180,350,500,600,680,720};     //舵机打角过程占空比
float curr_val = 0.0;                       //电磁偏差，差和比运算的结果
float delta_val = 0.0;                      //电磁值偏差变化率
float last_val = 0.0;                       //上一次的电磁值
float up_curr_val = 0.0;                    //竖直电感偏差
float up_last_val = 0.0;               //竖直电感上一次的值
float up_delta_val = 0.0;                   //竖直电感电磁变化率
float elect_M ;                             //中间两个电感的平均值
int fuzzy_txt[10];                          //串口传送调用规则表坐标
float fuzzy_txt1[3] = {0};                         //屏幕输出U、差比和差结果的大小
int fuzzy_txt2[1] = {0};
int rule[7][7]={//误差变化率 -3,-2,-1, 0, 1, 2, 3                                                                                                                        使用sendFuzzyData()函数校准模糊pid规则表。在临界丢线处进行规则表调整
            {-6,-6,-6,-6,-5,-5,-4,},                      //   -3      0
            {-6,-6,-6,-6,-4,-2,-1,},                      //   -2      1
            {-6,-6,-3,-1, 0, 1, 1,},                      //   -1      2
            {-1,-1,-2, 0, 4, 5, 6,},                      //    0      3
            {-2,-2, 0, 1, 6, 6, 6,},                      //    1      4
            { 1, 1, 5, 6, 6, 6, 6,},                      //    2      5
            { 4, 5, 5, 5, 6, 6, 6} };                     //    3      6


char txtq[15] = {0};
char txtb[15] = {0};
char txtd[15] = {0};
char txta[15] = {0};
char txtj[15] = {0};
char txte[15] = {0};
char txts[15] = {0};
extern char txt1[10];
extern char txt2[10];
extern char txt3[10] ;
extern char txt4[10] ;
extern char txt5[10] ;
extern char txt6[10] ;

extern char txt_fz[10] = {0};
/******************环岛相关变量***********************/
unsigned int Circle_Flag = 0;             //环岛标志位
unsigned int LeftCircle_Flag = 0;         //左环
unsigned int RightCircle_Flag = 0;        //右环

/*************************************************************************
*  函数名称：float my_sqrt(float number)
*  功能说明：牛顿迭代法计算平方根
*  参数说明：float number：原始数值输入
*  函数返回：返回平方根
*  修改时间：2021年4月28日
*  备    注：0x5f3759df的来历比较复杂，详见维基百科
*************************************************************************/
float my_sqrt(float number)
{
    long i;
    float x, y;
    const float f = 1.5F;
    x = number * 0.5F;
    y = number;
    i = * ( long * ) &y;
    i = 0x5f3759df - ( i >> 1 );

    y = * ( float * ) &i;
    y = y * ( f - ( x * y * y ) );
    y = y * ( f - ( x * y * y ) );
    return number * y;
}

/*************************************************************************
*  函数名称：float my_absolute(float number)
*  功能说明：求绝对值
*  参数说明：float number：原始数值输入
*  函数返回：返回平方根
*  修改时间：2021年4月29日
*  备    注：对输入值做绝对值处理
*************************************************************************/
float my_absolute(float number)
{
    if(number < 0)
    {
        number = -number;
    }
    return number;
}

/*************************************************************************
*  函数名称：void aricomsumope()
*  功能说明：差比和差运算(由于四个系数难以确定,因此弃用差比和差算法）
*  参数说明：acso_abcp的值需要后期调整得到
*  函数返回：无
*  修改时间：2021年4月29日
*  备    注：差比和差算法，差比和差算法相较差比和算法拥有更加优秀的鲁棒性
*************************************************************************/
void aricomsumope()
{
    const float acso_a = 0.4;
    const float acso_b = 0.6;
    const float acso_c = 0.6;
    const float acso_p = 1;      //比例系数p
    last_val = curr_val;
    curr_val =  acso_p*((acso_a*(ad_VAL[4] - ad_VAL[1])+acso_b*(ad_VAL[5] - ad_VAL[0]))/(acso_a*(ad_VAL[4] + ad_VAL[1])+acso_c*(my_absolute(ad_VAL[5]-ad_VAL[0]))));
    delta_val = curr_val - last_val;
    fuzzy_txt1[0] = curr_val;
}

/*************************************************************************
*  函数名称：float Vag_pid(float currval,float deltaval)
*  功能说明：舵机模糊pid控制
*  参数说明：currval:电磁偏差，差比和差运算的结果；deltaval：电磁值偏差变化率;U:占空比变化量
*  函数返回：float类型的占空比
*  修改时间：2021年5月5日
*  备    注：利用模糊pid对舵机打角进行实时参数调整
*************************************************************************/
float Vag_pid(float currval,float deltaval)
{
    float U;                //偏差,偏差微分以及输出值的精确量
    float PF[2], DF[2], UF[4];//偏差,偏差微分以及输出值的隶属度
    int Pn, Dn, Un[4];        //Un对应表中四个调用元素
    float temp1, temp2;

    fuzzy_txt1[0] = currval;
    fuzzy_txt1[1] = deltaval;
    /*隶属度的确定*/
    /*根据PD的指定语言值获得有效隶属度*/
    if (currval > -PFF[3] && currval < PFF[3])
    {
        if (currval <= -PFF[2])
        {
            Pn = -2;
            PF[0] = FMAX * ((float)(-PFF[2] - currval) / (PFF[3] - PFF[2]));
        }
        else if (currval <= -PFF[1])
        {
            Pn = -1;
            PF[0] = FMAX * ((float)(-PFF[1] - currval) / (PFF[2] - PFF[1]));
        }
        else if (currval <= PFF[0])
        {
            Pn = 0;
            PF[0] = FMAX * ((float)(-PFF[0] - currval) / (PFF[1] - PFF[0]));
        }
        else if (currval <= PFF[1])
        {
            Pn = 1; PF[0] = FMAX * ((float)(PFF[1] - currval) / (PFF[1] - PFF[0]));
        }
        else if (currval <= PFF[2])
        {
            Pn = 2; PF[0] = FMAX * ((float)(PFF[2] - currval) / (PFF[2] - PFF[1]));
        }
        else if (currval <= PFF[3])
        {
            Pn = 3; PF[0] = FMAX * ((float)(PFF[3] - currval) / (PFF[3] - PFF[2]));
        }
    }
    else if (currval <= -PFF[3])
    {
        Pn = -2; PF[0] = FMAX;
    }
    else if (currval >= PFF[3])
    {
        Pn = 3; PF[0] = 0;
    }
    PF[1] = FMAX - PF[0];

    if (deltaval > -DFF[3] && deltaval < DFF[3])
    {
        if (deltaval <= -DFF[2])
        {
            Dn = -2; DF[0] = FMAX * ((float)(-DFF[2] - deltaval) / (DFF[3] - DFF[2]));
        }
        else if (deltaval <= -DFF[1])
        {
            Dn = -1;
            DF[0] = FMAX * ((float)(-DFF[1] - deltaval) / (DFF[2] - DFF[1]));
        }
        else if (deltaval <= DFF[0])
        {
            Dn = 0;
            DF[0] = FMAX * ((float)(-DFF[0] - deltaval) / (DFF[1] - DFF[0]));
        }
        else if (deltaval <= DFF[1])
        {
            Dn = 1;
            DF[0] = FMAX * ((float)(DFF[1] - deltaval) / (DFF[1] - DFF[0]));
        }
        else if (deltaval <= DFF[2])
        {
            Dn = 2; DF[0] = FMAX * ((float)(DFF[2] - deltaval) / (DFF[2] - DFF[1]));
        }
        else if (deltaval <= DFF[3])
        {
            Dn = 3; DF[0] = FMAX * ((float)(DFF[3] - deltaval) / (DFF[3] - DFF[2]));
        }
    }
    else if (deltaval <= -DFF[3])
    {
        Dn = -2;
        DF[0] = FMAX;
    }
    else if (deltaval >= DFF[3])
    {
        Dn = 3;
        DF[0] = 0;
    }
    DF[1] = FMAX - DF[0];

    /*使用误差范围优化后的规则表rule[7][7]*/
    /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
    /*一般都是四个规则有效*/
    fuzzy_txt[0]=Pn - 1 + 3;
    fuzzy_txt[1]=Dn - 1 + 3;
    fuzzy_txt[2]=Pn + 3;
    fuzzy_txt[3]=Dn - 1 + 3;
    fuzzy_txt[4]=Pn - 1 + 3;
    fuzzy_txt[5]=Dn + 3;
    fuzzy_txt[6]=Pn + 3;
    fuzzy_txt[7]=Dn + 3;
    // 拿到规则表中的四个值
    Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
    Un[1] = rule[Pn + 3][Dn - 1 + 3];
    Un[2] = rule[Pn - 1 + 3][Dn + 3];
    Un[3] = rule[Pn + 3][Dn + 3];

    if (PF[0] <= DF[0]) UF[0] = PF[0];
    else    UF[0] = DF[0];
    if (PF[1] <= DF[0]) UF[1] = PF[1];
    else    UF[1] = DF[0];
    if (PF[0] <= DF[1]) UF[2] = PF[0];
    else    UF[2] = DF[1];
    if (PF[1] <= DF[1]) UF[3] = PF[1];
    else    UF[3] = DF[1];

    /*同隶属函数输出语言值求大*/
    if (Un[0] == Un[1])
    {
        if (UF[0] > UF[1])  UF[1] = 0;
        else    UF[0] = 0;
    }
    if (Un[0] == Un[2])
    {
        if (UF[0] > UF[2])  UF[2] = 0;
        else    UF[0] = 0;
    }
    if (Un[0] == Un[3])
    {
        if (UF[0] > UF[3])  UF[3] = 0;
        else    UF[0] = 0;
    }
    if (Un[1] == Un[2])
    {
        if (UF[1] > UF[2])  UF[2] = 0;
        else    UF[1] = 0;
    }
    if (Un[1] == Un[3])
    {
        if (UF[1] > UF[3])  UF[3] = 0;
        else    UF[1] = 0;
    }
    if (Un[2] == Un[3])
    {
        if (UF[2] > UF[3])  UF[3] = 0;
        else    UF[2] = 0;
    }
//
    /*重心法反模糊*/
    /*Un[]原值为输出隶属函数标号，转换为隶属函数值*/
    if (Un[0] >= 0) Un[0] = UFF[Un[0]];
    else            Un[0] = -UFF[-Un[0]];
    if (Un[1] >= 0) Un[1] = UFF[Un[1]];
    else            Un[1] = -UFF[-Un[1]];
    if (Un[2] >= 0) Un[2] = UFF[Un[2]];
    else            Un[2] = -UFF[-Un[2]];
    if (Un[3] >= 0) Un[3] = UFF[Un[3]];
    else            Un[3] = -UFF[-Un[3]];

    temp1 = UF[0] * Un[0] + UF[1] * Un[1] + UF[2] * Un[2] + UF[3] * Un[3];
    temp2 = UF[0] + UF[1] + UF[2] + UF[3];
    U = temp1 / temp2;



//    sendFuzzyData();
    return U;
}

/*************************************************************************
*  函数名称：void sendFuzzyData()
*  功能说明：校准规则表中的数值
*  参数说明：无
*  函数返回：无
*  修改时间：2021年5月5日
*  备    注：无
*************************************************************************/
void sendFuzzyData(){
  char txt[10];

   UART_PutStr(UART0, "-");

  for(int i = 0;i<8;i++){
    sprintf(txt,"%d",fuzzy_txt[i]);
    if(i==0 || i == 2 || i == 4 || i == 6){
        UART_PutStr (UART0, "[");
        UART_PutStr (UART0, txt);
        UART_PutStr (UART0, ".");
    }
    else{
        UART_PutStr (UART0, txt);
        UART_PutStr (UART0, "]");
    }
  }
  UART_PutStr (UART0, "\r\n");
}


/*************************************************************************
*  函数名称：void DJCrol()
*  功能说明：舵机控制函数
*  参数说明：无
*  函数返回：无
*  修改时间：2021年7月16日
*  备    注：无
*************************************************************************/
void DJCrol()
{
    Get_ADC();
    Datadeal();
    DirJudge();
    sprintf(txta,"res=%d\r\n",res);
    UART_PutStr (UART0, txta);

    if(RemLength == 1)            //直角调整状态下不记录距离
    {
        HPsum = encValue5/512.0;      //累计距离
        ZTsum += HPsum;
//        sprintf(txte, "Enc5: %05d;", encValue5);
//        sprintf(txts, "ZTsum: %.3f;", ZTsum);
//        UART_PutStr(UART0,txte);
//        UART_PutStr(UART0,txts);
//        UART_PutStr(UART0,"\r\n");

    }
    else if(RemLength == 2)
    {
        HPsum = encValue5/512.0;      //累计距离
        DCsum += HPsum;
    }
    else if(RemLength == 3)
    {
        HPsum = encValue5/512.0;      //累计距离
        SZsum += HPsum;
    }
    else if(RemLength == 4)
    {
        HPsum = encValue5/512.0;      //累计距离
        HDsum += HPsum;
    }

    if(Game_Over == 0)
    {
        /* 舵机电机赋值 */
        if(res == 1)    //左转
        {
            if(RemLength == 1)
            {
                AddListTill(ZTsum,1);              //将此时的累计脉冲数加入到距离数组中
                ZTsum = 0;
            }
            if(dao_flag == 0)
            {
                fjc_duty = 570;
                set_speed = 4000;
            }
            if(DCsum >= forward || dao_flag == 1)
            {
                dao_flag = 1;
                fjc_duty = 1840;
                back_speed = 4000;
                if(DCsum <= (forward + backward))
                {
                    res = 0;
                    DCsum = 0;
                    dao_flag = 0;
                }
            }
            RemLength = 2;
        }
        else if(res == 2)
        {
            if(RemLength == 1)
            {
                AddListTill(ZTsum,2);              //将此时的累计脉冲数加入到距离数组中
                ZTsum = 0;
            }
            if(dao_flag == 0)
            {
                fjc_duty = 1840;
                set_speed = 4000;
            }
            if(DCsum >= forward || dao_flag == 1)
            {
                dao_flag = 1;
                fjc_duty = 570;
                back_speed = 4000;
                if(DCsum <= (forward + backward))
                {
                    res = 0;
                    DCsum = 0;
                    dao_flag = 0;
                }
            }
            RemLength = 2;
        }
        else if(res == 3)           //十字
        {
            RemLength = 3;
            fjc_duty = 1240;
            set_speed = 3200;
            if(SZsum >= szkeep)
            {
                res = 0;
                SZsum = 0;
            }
        }
        else if(res == 4)
        {
            RemLength = 4;
            fjc_duty = 1840;
            set_speed = 3500;
            if(HDsum >= hdkeep)
            {
                res = 0;
                HDsum = 0;
                hd_in = 1;     //右入环
            }
        }
        else if(res == 5)
        {
            RemLength = 4;
            fjc_duty = 1290;
            set_speed = 3700;
            if(HDsum >= out_hdkeep)
            {
                res = 0;
                HDsum = 0;
            }
        }
        //左小角度
        else if(res == 8)
        {
//            float left_f = 0;
//            float left_b = 0;
//            sprintf(txt_fz,"var:%f",left_f);
//            UART_PutStr (UART0, txt_fz);
//
//            while(left_f < 160)
//            {
//                UART_PutStr (UART0, "zuofor\r\n");
//                left_f += encValue5/512.0;
//                ATOM_PWM_SetDuty(ATOMSERVO2, 570 , 100);
//                Set_Speed(1000);
//            }
//            while(left_b < 270)
//            {
//                UART_PutStr (UART0, "zuodao\r\n");
//
//                Set_Speedback(4200);
//                left_b += -encValue5/512.0;
//                ATOM_PWM_SetDuty(ATOMSERVO2, 1840 , 100);
//            }
//            res = 0;

            if(RemLength == 1)
            {
                AddListTill(ZTsum,3);              //将此时的累计脉冲数加入到距离数组中
                ZTsum = 0;
            }
            if(dao_flag == 0)
            {
//                UART_PutStr (UART0, "zuofor\r\n");
                fjc_duty = 570;
                set_speed = 2000;
            }
            if(DCsum >= sforward || dao_flag == 1)
            {
                dao_flag = 1;
                fjc_duty = 1840;
                back_speed = 4000;
                if(DCsum <= (sforward + sbackward))
                {
                    res = 0;
                    DCsum = 0;
                    dao_flag = 0;
                }
            }
            RemLength = 2;

        }
        //右小角度
        else if(res == 9)
        {
            if(RemLength == 1)
            {
                AddListTill(ZTsum,4);              //将此时的累计脉冲数加入到距离数组中
                ZTsum = 0;
            }
            if(dao_flag == 0)
            {
                fjc_duty = 1840;
                set_speed = 4000;
            }
            if(DCsum >= sforward || dao_flag == 1)
            {
                dao_flag = 1;
                fjc_duty = 570;
                back_speed = 4000;
                if(DCsum <= (sforward + sbackward))
                {
                    res = 0;
                    DCsum = 0;
                    dao_flag = 0;
                }
            }
            RemLength = 2;
        }
        else
        {
            RemLength = 1;
            fjc_duty = Servo_Center_Mid + Vag_pid(curr_val*45,delta_val*100);

            //直道加速
            if(ad_VAL[2] + ad_VAL[3] > 150)
                set_speed = set_speed + 30;

            // 弯道
            if(ad_VAL[0] > 15 || ad_VAL[5] > 30 && (ad_VAL[2]+ad_VAL[3] >= 10))
            {
                if(fjc_duty >= 550 && fjc_duty <= 580)
                {

                    if(set_speed >= 3000)
                    {
                        back_speed = 1;
                    }
                    else
                        set_speed = 2900;
                }
                if(fjc_duty >= 1730 && fjc_duty <= 1850)
                {
                    if(set_speed >= 3000)
                    {
                        back_speed = 800;
                    }
                    else
                        set_speed = 2700;
                }

                else if(fjc_duty > 1600 && fjc_duty <= 1740)
                {
                    set_speed = (unsigned int)(-1.33*fjc_duty + 5133);
                }
                /*else if(fjc_duty >= 570 && fjc_duty <= 729)
                {
                    set_speed = (unsigned int)(2.5*fjc_duty + 1175);
                }*/
                else
                    set_speed = 3000;
            }
        }
//        sprintf(txt_fz,"DUTY:%d\r\n",fjc_duty);
//        UART_PutStr (UART0, txt_fz);
        fjc_duty = constrain_int(fjc_duty,570,1840);  // 限幅
        ATOM_PWM_SetDuty(ATOMSERVO2, fjc_duty , 100);

/***********************调试***********************/
//        sprintf(txt4,"DCsum:%.3f\r\n",DCsum);
//        UART_PutStr (UART0, txt4);
//
//        sprintf(txt5,"back_speed:%d\r\n",back_speed);
//        UART_PutStr (UART0, txt5);
//        sprintf(txt6,"set_speed:%d\r\n",set_speed);
//        UART_PutStr (UART0, txt6);

        set_speed = constrain_int(set_speed,2000,4000);

        if(back_speed == 0)
            Set_Speed(set_speed);
        else
        {
            Set_Speedback(back_speed);
            back_speed = 0;
        }


        //调试
//        sprintf(txtq,"%.2f",fuzzy_txt1[0]);
//        sprintf(txtd,"%.2f",fuzzy_txt1[1]);
//        UART_PutStr (UART0, txtq);
//        UART_PutStr (UART0, " ");
//        UART_PutStr (UART0, txtd);
//        UART_PutStr (UART0, "\r\n");
    }
    else
    {
        SearchListTill();
        //TFTSPI_P8X16Str(0,0,"Mode is Rem",u16WHITE,u16BLACK);
        UART_PutStr (UART0, "Mode is Rem");

        sprintf(txtj,"later:%.3f",laternum);
        UART_PutStr (UART0, txtj);
        ZTsum = 0;
        UART_PutStr(UART0,"/***************************************/");
        //TFTSPI_P8X16Str(0,1, txtj,u16WHITE,u16BLACK);
    }
}

/*************************************************************************
*  函数名称：float KalmanFilter_Elect(float curr_elect_val,float last_elect_val)
*  功能说明：卡尔曼滤波
*  参数说明：无
*  函数返回：无
*  修改时间：2021年5月9日
*  备    注：无
*************************************************************************/
float KalmanFilter_Elect(float curr_elect_val,float last_elect_val)
{
  static float Q_curr = 0.1;//1.0           //Q增大，动态响应增大，过程噪声的协方差
  static float Q_last = 0.0001;         //过程噪声的协方差，过程噪声的协方差为一个一行两列矩阵
  static float R_elect = 10.0;                  //测量噪声的协方差 即测量偏差
  static float dt = 0.006;   //0.0055                    //积分间隔

  static float Pk[2][2] = { {1, 0}, {0, 1 }};

  static float Pdot[4] = {0,0,0,0};

  static float q_bias = 0.0;
  static float elect_err = 0.0;
  static float PCt_0 = 0.0;
  static float PCt_1 = 0.0;
  static float E = 0.0;
  static float K_0 = 0.0, K_1 = 0.0, t_0 = 0.0, t_1 = 0.0;

  Pdot[0] = Q_curr - Pk[0][1] - Pk[1][0];       //Pk-先验估计误差协方差的微分
  Pdot[1] = -Pk[1][1];
  Pdot[2] = -Pk[1][1];
  Pdot[3] = Q_last;

  Pk[0][0] += Pdot[0] * dt;             //Pk-先验估计误差的协方差微分的积分
  Pk[0][1] += Pdot[1] * dt;             //先验估计误差协方差
  Pk[1][0] += Pdot[2] * dt;
  Pk[1][1] += Pdot[3] * dt;

  elect_err = curr_elect_val - last_elect_val;          //偏差 = 测量值 - 预测值，先验估计

  PCt_0 = Pk[0][0];
  PCt_1 = Pk[1][0];

  E = R_elect + PCt_0;

  K_0 = PCt_0 / E;
  K_1 = PCt_1 / E;

  t_0 = PCt_0;
  t_1 = Pk[0][1];

  Pk[0][0] -= K_0 * t_0;                    //后验估计误差协方差
  Pk[0][1] -= K_0 * t_1;
  Pk[1][0] -= K_1 * t_0;
  Pk[1][1] -= K_1 * t_1;

  curr_elect_val += K_0 * elect_err;                //后验估计 更新最优电磁值 最优电磁值 = 预测值 + 卡尔曼增益*(测量值-预测值)
  q_bias += K_1 * elect_err;                //后验估计 更新误差

  return curr_elect_val;

}


/*************************************************************************
*  函数名称：void Datadeal()
*  功能说明：数据处理函数
*  参数说明：无
*  函数返回：无
*  修改时间：2021年5月10日
*  备    注：无
*************************************************************************/
void Datadeal()
{

    last_val = curr_val;
    up_last_val = up_curr_val;
    elect_M = 25.5;
      //水平电感差比和
    curr_val =  (my_sqrt(ad_VAL[3]) - my_sqrt(ad_VAL[2] ))/ (my_sqrt(ad_VAL[3]) + my_sqrt(ad_VAL[2])+ my_sqrt(elect_M));
    //竖直电感差比和
    up_curr_val =(my_sqrt(ad_VAL[5] ) - my_sqrt(ad_VAL[0])) / (my_sqrt(ad_VAL[5]) + my_sqrt(ad_VAL[0]) + my_sqrt(elect_M));

    curr_val = KalmanFilter_Elect(curr_val,last_val);                 //卡尔曼滤波
    up_curr_val = KalmanFilter_Elect(up_curr_val,up_last_val);

    fuzzy_txt1[0] = curr_val;
    fuzzy_txt1[1] = up_curr_val;

    delta_val = curr_val - last_val;                                            //电磁变化率
    up_delta_val = up_curr_val - up_last_val;


    if(ad_VAL[0] > 15 || ad_VAL[5] > 20)
    {//两边电感差比和大，则忽略中间电感值
        if(fabs(curr_val) < fabs(up_curr_val) || (ad_VAL[0] > 80 || ad_VAL[5] > 80))
        {
//          UART_PutStr (UART0, "ininin");
          curr_val = up_curr_val;
          delta_val = up_delta_val;
        }
    }

}

/*************************************************************************
*  函数名称：void DirJudge()
*  功能说明：判断舵机偏转方向函数
*  函数返回：res：弯道判断结果
*  修改时间：2021年4月3日
*  备    注：特征弯道判断（一定要使用电机进行特征值采集，切勿使用手推！）
*************************************************************************/
void DirJudge()
{
    if(ad_VAL[0] + ad_VAL[5] >=160)
    {
        res = 3;         //十字
    }

    else if(ad_VAL[0] - ad_VAL[5] >= 90 && res == 0  && ad_VAL[2]<=60)
    {
        res = 1;        //左直角
        cnt = 0;
        dao_flag = 0;
        cnt_return = 0;
    }

    else  if(ad_VAL[5] - ad_VAL[0] >= 82 && res == 0 && ad_VAL[3] <= 75)
    {
        res = 2;        //右直角
        cnt = 0;
        dao_flag = 0;
        cnt_return = 0;
    }

//    else if(ad_VAL[5] - ad_VAL[0] >= 63 && res == 0 && (ad_VAL[3] + ad_VAL[2]) < 130 && realpwm >= 3000)
//    {
//        res = 9;       //右小角度
//    }
//    else if(ad_VAL[0] >= 88 && res == 0  && realpwm >= 2400 && ad_VAL[2] > 80) //48  2+3>165
//    {
//
//        res = 8;       //左小角度
//    }

    if((ad_VAL[2] >= 60 && ad_VAL[3] >= 103) && (ad_VAL[5] >= 67 ) && res == 0 )
    {
        res = 4;
    }

    else if((ad_VAL[2] >= 95 && ad_VAL[3] >= 90) && ad_VAL[5]>60 && hd_in == 1)
    {
        res = 5;      //右出环
        hd_in = 0;
    }
}

/*************************************************************************
*  函数名称：void DirJudge2()
*  功能说明：记忆舵机偏转方向函数
*  函数返回：res：弯道判断结果
*  修改时间：2021年4月3日
*  备    注：特征弯道判断（一定要使用电机进行特征值采集，切勿使用手推！）
*************************************************************************/
void DirJudge2()
{
    if(ad_VAL[0] + ad_VAL[5] >=160)
    {
        res = 3;         //十字
    }
    if((ad_VAL[2] >= 90 || ad_VAL[3] >=90) && (ad_VAL[1] >= 90 || ad_VAL[4] >= 90) && ad_VAL[0] - ad_VAL[5] > 90 && res == 0 )
    {
        hd_in = 1;     //右入环
        res = 4;
    }
    else if((ad_VAL[2] >= 90 || ad_VAL[3] >= 90) && (ad_VAL[1] >= 90 || ad_VAL[4] >= 90) && hd_in == 1)
    {
        res = 5;      //右出环
        hd_in = 0;
    }
}

/*************************************************************************
*  函数名称：void Loseline()
*  功能说明：丢线检测函数
*  参数说明：无
*  函数返回：无
*  修改时间：2021年7月16日
*  备    注：无
*************************************************************************/
void Loseline()
{
    if(abs(fjc_duty - 1280) <=120 && ad_VAL[2] + ad_VAL[3] < 5 && res == 0)
    {
        if(fjc_duty > 1280)
        {
            fjc_duty = 1840;
        }
        else
        {
            fjc_duty = 570;
        }
    }
}

void AddListTill(float a,unsigned char Rem)
{
    unsigned char i = 0;
    //直角距离、方向赋值
    distance[i] = a;
    direction[i]=Rem;
}

/*************************************************************************
*  函数名称：void SearchListTill(void)
*  功能说明：遍历距离、方向数组
*  函数返回：无
*  修改时间：2021年7月10日
*  备    注：用于显示/通信到屏幕或串口
*************************************************************************/
void SearchListTill(void)
{
    unsigned char i = 0;
    unsigned char txtS[20];
    while(direction[i])
    {
        sprintf(txtS[i],"%.6f",distance[i]);


        UART_PutStr (UART0, txtS[i]);
        UART_PutStr (UART0, "\r\n");

        i++;
    }
}

/*************************************************************************
*  函数名称：void DJCrol2()
*  功能说明：舵机控制模式二
*  参数说明：无
*  函数返回：无
*  修改时间：2021年7月10日
*  备    注：无
*************************************************************************/
void DJCrol2()
{
    static unsigned char i = 0;
    Get_ADC();
    Datadeal();
    DirJudge2();

    if(RemLength == 1)            //直角调整状态下不记录距离
    {
        HPsum = encValue5/512.0;      //累计距离
        ZTsum += HPsum;
//            sprintf(txte, "Enc5: %05d;", encValue5);
//            sprintf(txts, "ZTsum: %.3f;", ZTsum);
//            UART_PutStr(UART0,txte);
//            UART_PutStr(UART0,txts);
//            UART_PutStr(UART0,"\r\n");

    }
    else if(RemLength == 3)
    {
        HPsum = encValue5/512.0;      //累计距离
        SZsum += HPsum;
    }
    else if(RemLength == 4)
    {
        HPsum = encValue5/512.0;      //累计距离
        HDsum += HPsum;
    }


    if((ZTsum >= (distance[i] - laternum)) && direction[i] != 0 && distance[i] > 0 && res == 0)
    {
        if(direction[i] == 1)
        {
            res = 5;
            i++;
        }
        if(direction[i] == 2)
        {
            res = 6;
            i++;
        }
        if(direction[i] == 3)
        {
            res = 8;
            i++;
        }
        if(direction[i] == 40)
        {
            res = 9;
            i++;
        }
    }

    /* 舵机电机赋值 */
    if(res == 5)   //记忆左转
    {
        fjc_duty = 570;
        set_speed = 2200;
        if(ZTsum >= Remforward)
        {
            ZTsum = 0;
            res = 0;
        }
    }
    else if(res == 6)   //记忆右转
    {
        fjc_duty = 1840;
        set_speed = 2200;
        if(ZTsum >= Remforward)
       {
           ZTsum = 0;
           res = 0;
       }
    }
    else if(res == 3)           //十字
    {
        RemLength = 3;
        fjc_duty = 1280;
        set_speed = 3200;
        if(SZsum >= szkeep)
        {
            res = 0;
            SZsum = 0;
        }
    }
    else if(res == 4)
    {
        RemLength = 4;
        fjc_duty = 1500;
        set_speed = 3200;
        if(HDsum >= hdkeep)
        {
            res = 0;
            HDsum = 0;
        }
    }
    else if(res == 5)
    {
        RemLength = 4;
        fjc_duty = 1300;
        set_speed = 3200;
        if(HDsum >= hdkeep)
        {
            res = 0;
            HDsum = 0;
        }
    }
    else if(res == 8)                    //记忆左小
    {
        fjc_duty = 570;
        set_speed = 2200;
        if(ZTsum >= Remsforward)
        {
            ZTsum = 0;
            res = 0;
        }
    }
    else if(res == 9)                   //记忆右小
    {
        fjc_duty = 1840;
        set_speed = 2200;
        if(ZTsum >= Remsforward)
        {
            ZTsum = 0;
            res = 0;
        }
    }
    else
    {
        RemLength = 1;
        fjc_duty = Servo_Center_Mid + Vag_pid(curr_val*45,delta_val*100);

        //直到
        set_speed = set_speed + 30;

        // 弯道
        if(ad_VAL[0] > 15 || ad_VAL[5] > 35 && (ad_VAL[2]+ad_VAL[3] >= 10))
        {
            if(fjc_duty >= 565 && fjc_duty <= 630)
            {

                if(set_speed >= 3000)
                {
                    back_speed = 100;
                }
                else
                    set_speed = 1900;
            }
            else if(fjc_duty >= 1480 && fjc_duty <= 1510)
            {
                if(set_speed >= 3000)
                {
                    back_speed = 1700;
                }
                else
                    set_speed = 100;
            }
            else if(fjc_duty > 1600 && fjc_duty <= 1900)
            {
                set_speed = (unsigned int)(-1.3793*fjc_duty + 5020);
            }
            else if(fjc_duty >= 570 && fjc_duty <= 729)
            {
                set_speed = (unsigned int)(2.515*fjc_duty + 966);
            }
            else
                set_speed = 2200;
        }
    }


        fjc_duty = constrain_int(fjc_duty,570,1840);  // 限幅
        ATOM_PWM_SetDuty(ATOMSERVO2, fjc_duty , 100);



//        sprintf(txt_fz,"back_speed:%d\r\n",back_speed);
//        UART_PutStr (UART0, txt_fz);
//        sprintf(txt_fz,"set_speed:%d\r\n",set_speed);
//        UART_PutStr (UART0, txt_fz);

        set_speed = constrain_int(set_speed,2000,4000);

        if(back_speed == 0)
            Set_Speed(set_speed);
        else
        {
            Set_Speedback(back_speed);
            back_speed = 0;
        }


}

/*************************************************************************
*  函数名称：void Mode_Choose(void)
*  功能说明：按键控制启停
*  参数说明：无
*  函数返回：无
*  修改时间：2021年7月10日
*  备    注：无
*************************************************************************/
void Mode_Choose(void)
{
//    if(!KEY_Read(KEY2))
//    {
//        goflag = 1;
//        ZTsum = 0;
//        UART_PutStr(UART0,"******key2");
//    }
    if(!KEY_Read(KEY0))
    {
        laternum += 0.1;
        UART_PutStr(UART0,"******key0");
    }
    if(!KEY_Read(KEY1))
    {
        goflag = 1;
        ZTsum = 0;
        UART_PutStr(UART0,"******key1");
    }
}

