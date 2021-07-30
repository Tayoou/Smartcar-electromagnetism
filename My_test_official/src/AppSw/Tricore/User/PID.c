#include "PID.h"
extern PID sptr;
extern PID s_pid;

/*************************************************************************
*  函数名称：int PID_MOTOR_Contrl(int set,int NowPoint)
*  功能说明：PID控制电机
*  参数说明：sptr为期PID结构体，NowPoint为实际速度（通过编码器测得）
*  函数返回：返回PID增量
*  修改时间：2021年4月4日
*************************************************************************/
int PID_MOTOR_Contrl(int set,int NowPoint)
{
    int iError,iIncPid;                     //iError:误差, iIncPid:增量值
    sptr.SetPoint=set;                      //设置目标值
    iError = sptr.SetPoint - NowPoint;      //当前误差  设定的目标值和实际值的偏差

    iIncPid = sptr.P * (iError - sptr.LastError)             //增量计算
              + sptr.I * iError;
              //+ sptr.D * (iError - 2*sptr.LastError + sptr.PrevError);

    /*存储误差  用于下次计算*/
    sptr.PrevError = sptr.LastError;
    sptr.LastError = iError;
    // 限幅(小峻你自己看下这个增量的阈值控制多少)
    constrain_float((float)iIncPid, 0.0, 1000.0);
    return iIncPid;                         //返回增量值
}


/*************************************************************************
*  函数名称：int PID_Servo_Contrl(int set,int NowPoint)
*  功能说明：PD控制舵机(位置式)
*  参数说明：sptr为期PID结构体，NowPoint为实际速度（通过编码器测得）
*  函数返回：返回PID增量
*  修改时间：2021年4月4日
*************************************************************************/
int PID_Servo_Contrl(int set,int NowPoint)
{
    int iError,output;             //iError:误差
    s_pid.SetPoint=set;              //设置目标值

    iError = s_pid.SetPoint - NowPoint;      //当前误差  设定的目标值和实际值的偏差

    output = s_pid.P * iError               //增量计算
              + s_pid.D * (iError - s_pid.LastError);

    /*存储误差  用于下次计算*/
    s_pid.PrevError = s_pid.LastError;
    s_pid.LastError = iError;

    return output;                         //返回位置值
}

/*************************************************************************
 *  函数名称constrain_( amt,  low,  high)
 *  功能说明：限幅函数
 *  参数说明：
  * @param    amt   ： 参数
  * @param    low   ： 最低值
  * @param    high  ： 最高值
 *************************************************************************/
float constrain_float(float amt, float low, float high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

unsigned int constrain_int(int amt, int low, int high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}




