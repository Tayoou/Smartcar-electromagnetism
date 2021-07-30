#ifndef PID_H
#define PID_H

typedef struct pid{
    int SetPoint;       //鐩爣鍊�
    long SumError;      //璇樊绱

    float P;            //姣斾緥甯告暟
    float I;            //绉垎甯告暟
    float D;            //寰垎甯告暟

    int LastError;      //涓婃鍋忓樊鍊�
    int PrevError;      //涓婁笂娆″亸宸��
}PID;

int PID_MOTOR_Contrl(int set,int NowPoint);     // 鐢垫満pi鎺у埗
int PID_Servo_Contrl(int set,int NowPoint);     // 鑸垫満PD鎺у埗
float constrain_float(float amt, float low, float high);
unsigned int constrain_int(int amt, int low, int high);




#endif
