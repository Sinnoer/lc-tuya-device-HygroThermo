#include "include/moto.h"

//void TIM3_PWM_Init(unsigned short arr,unsigned short psc)
//{
//    rt_pin_mode(MOTO_P, PIN_MODE_OUTPUT);
//    rt_pin_mode(MOTO_N, PIN_MODE_OUTPUT);
//
//    TIM3_Handler.Instance=TIM3;
//    TIM3_Handler.Init.Prescaler=psc;
//    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;
//    TIM3_Handler.Init.Period=arr;
//    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
//    HAL_TIM_PWM_Init(&TIM3_Handler);
//
//    TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1;
//    TIM3_CH4Handler.Pulse=arr/2;
//
//    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW;
//    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);
//    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);
//}
//void moto_start(void *param)
//{
//    static char flag=0;
//    if(flag == 0){TIM3_PWM_Init(500-1,200-1);flag =1;}
//    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);
//}
//void moto_stop(void *param)
//{
//    HAL_TIM_PWM_Stop(&TIM3_Handler,TIM_CHANNEL_4);
//}
//void moto_set(int argc, char *argv[])
//{
//    int compare=0;
//    if(argc < 2)
//    {
//        rt_kprintf("Please input <compare>(150-400)\n");
//        return;
//    }
//    compare = atoi(argv[1]);
//    if(compare<150 || compare>400)
//    {
//        rt_kprintf("Please input <compare>(150-400):no %d\n",compare);
//        return;
//    }
//    else TIM3->CCR4=(compare);
//    rt_kprintf("<compare>set:%d\n",compare);
//}

/* 定时器的控制块 */
static struct rt_timer moto_SoftTimer;
static u8 is_moto_init=0;
/* 定时器 1 超时函数 */
void moto_SoftTimer_Handl(void* parameter)
{
    static unsigned char stat = 0;
    if (stat)
        rt_pin_write(MOTO_P, PIN_LOW);
    else
        rt_pin_write(MOTO_P, PIN_HIGH);
    stat = !stat;
}
u8 moto_init(int argc)
{
    static char onceThread = 0;
    if (onceThread || argc<0)return 0;else onceThread = 1;
    rt_pin_mode(MOTO_P, PIN_MODE_OUTPUT);
    rt_pin_mode(MOTO_N, PIN_MODE_OUTPUT);
    rt_pin_write(MOTO_P, PIN_LOW);
    rt_pin_write(MOTO_N, PIN_LOW);
    /* 初始化定时器 */
    rt_timer_init(&moto_SoftTimer, "moto_SoftTimer", /* 定时器名字是 timer1 */
    moto_SoftTimer_Handl, /* 超时时回调的处理函数 */
    RT_NULL, /* 超时函数的入口参数 */
    argc, /* 定时长度，以 OS Tick 为单位，即 10 个 OS Tick */
    RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
    return 1;
}
void ibmoto_begin(int peri)
{
    if(moto_init(peri))is_moto_init=1;
    /* 启动定时器 */
    rt_timer_start(&moto_SoftTimer);
}
void bmoto_begin(int argc, char *argv[])
{
    if (argv[1] == RT_NULL || atoi(argv[1]) > 3)
    {
        LOG_E("Error argv<peri>(1-3)");
        return;
    }
    if(moto_init(atoi(argv[1])))is_moto_init=1;;
    /* 启动定时器 */
    rt_timer_start(&moto_SoftTimer);
}
void emoto_end(void)
{
    if(is_moto_init==0)return;
    rt_timer_stop(&moto_SoftTimer);
    rt_pin_write(MOTO_P, PIN_LOW);
}
void smoto_set(int argc, char *argv[])
{
    if(is_moto_init==0)return;
    if (argv[1] == RT_NULL || atoi(argv[1]) > 3)
    {
        LOG_E("Error argv<peri>(1-3)");
        return;
    }
    int peri = atoi(argv[1]);
    rt_timer_control(&moto_SoftTimer, RT_TIMER_CTRL_SET_TIME, (void*) &peri);
    LOG_D("RT_TIMER_CTRL_SET_TIME: %d ", peri);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(bmoto_begin, moto_start);
MSH_CMD_EXPORT(emoto_end, moto_stop);
MSH_CMD_EXPORT(smoto_set, set moto peri);

//void moto_start(int argc, char *argv[])
//{
//    rt_pin_mode(MOTO_P, PIN_MODE_OUTPUT);
//    rt_pin_mode(MOTO_N, PIN_MODE_OUTPUT);
//    rt_pin_write(MOTO_P, PIN_HIGH);
//    rt_pin_write(MOTO_N, PIN_LOW);
//}
//MSH_CMD_EXPORT(moto_start, moto_start);
//MSH_CMD_EXPORT(moto_stop, moto_stop);
//MSH_CMD_EXPORT(moto_set, atcmd sample: set <compare>(150-400));
