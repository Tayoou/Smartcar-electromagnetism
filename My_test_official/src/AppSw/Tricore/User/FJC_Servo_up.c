#include "FJC_Servo_up.h"


unsigned char transter_buff_s[30];
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )


void Servo_Send()
{
    int _cnt=0;//_cnt用来计数
    unsigned short int tmp = 0;

    /* 功能字段 */
    transter_buff_s[_cnt++]=0xAA;    //帧头：AAAA
    transter_buff_s[_cnt++]=0xAA;
    transter_buff_s[_cnt++]=0x01;    //功能字：OXFn只接受数据,这里0x02对应第二帧，需要对应到上位机的第二帧，然后修改数据位
    transter_buff_s[_cnt++]=0;       //需要发送数据的字节数，暂时给0，后面在赋值。

    /* 数据位6个uint16 */
    for(int i=0;i<6;i++)        // 6是因为目前只有6个电感
    {
        tmp = ad_VAL[i];
        transter_buff_s[_cnt++]= BYTE1(tmp);
        transter_buff_s[_cnt++]= BYTE0(tmp);
    }

    transter_buff_s[3]=_cnt-4;
    int sum=0;
    /* 和校验 */
    for(int i=0;i<_cnt;i++)
        sum += transter_buff_s[i];

    transter_buff_s[_cnt++] = sum;
    UART_PutBuff(UART0,transter_buff_s,_cnt); //见LQ_UART.c 文件，查看UART_PutBuff 函数用法。
}
