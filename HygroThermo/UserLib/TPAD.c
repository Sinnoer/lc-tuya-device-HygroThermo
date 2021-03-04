#include "include/TPAD.h"

TIM_HandleTypeDef TIM3_Handler;
volatile u16 tpad_default_val = 0;

void TIM3_CH3_Cap_Init(u32 arr, u16 psc)
{
    TIM_IC_InitTypeDef TIM3_CH3Config;

    TIM3_Handler.Instance = TIM3;
    TIM3_Handler.Init.Prescaler = psc;
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM3_Handler.Init.Period = arr;
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM3_Handler);

    TIM3_CH3Config.ICPolarity = TIM_ICPOLARITY_RISING;
    TIM3_CH3Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
    TIM3_CH3Config.ICPrescaler = TIM_ICPSC_DIV1;
    TIM3_CH3Config.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&TIM3_Handler, &TIM3_CH3Config, TIM_CHANNEL_3);
    HAL_TIM_IC_Start(&TIM3_Handler, TIM_CHANNEL_3);
}
void TPAD_Reset(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin = GPIO_PIN_0;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLDOWN;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    rt_thread_delay(5);
    __HAL_TIM_CLEAR_IT(&TIM3_Handler, TIM_IT_CC1|TIM_IT_UPDATE);
    __HAL_TIM_SET_COUNTER(&TIM3_Handler, 0); //¹é0

    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}
u16 TPAD_Get_Val(void)
{
    TPAD_Reset();
    while (__HAL_TIM_GET_FLAG(&TIM3_Handler,TIM_FLAG_CC1) == RESET)
    {
        if (__HAL_TIM_GET_COUNTER(&TIM3_Handler) > TPAD_ARR_MAX_VAL - 500)
            return __HAL_TIM_GET_COUNTER(&TIM3_Handler);
    };
    return HAL_TIM_ReadCapturedValue(&TIM3_Handler, TIM_CHANNEL_3);
}
u8 TPAD_Init(u8 psc)
{
    u16 buf[10];
    u16 temp;
    u8 j, i;
    TIM3_CH3_Cap_Init(TPAD_ARR_MAX_VAL, psc - 1);
    for (i = 0; i < 10; i++)
    {
        buf[i] = TPAD_Get_Val();
        rt_thread_delay(10);
    }
    for (i = 0; i < 9; i++)
    {
        for (j = i + 1; j < 10; j++)
        {
            if (buf[i] > buf[j])
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    temp = 0;
    for (i = 2; i < 8; i++)
        temp += buf[i];
    tpad_default_val = temp / 6;
    LOG_D("tpad_default_val:%d\r\n", tpad_default_val);
    if (tpad_default_val > TPAD_ARR_MAX_VAL / 2)
        return 1;
    return 0;
}
u16 TPAD_Get_MaxVal(u8 n)
{
    u16 temp = 0;
    u16 res = 0;
    while (n--)
    {
        temp = TPAD_Get_Val();
        if (temp > res)
            res = temp;
    };
    return res;
}
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM3_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;

    GPIO_Initure.Pin = GPIO_PIN_0;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initure.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}

u8 TPAD_Scan(int mode)
{
    static u8 keyen = 0, onceThread = 0;
    u8 res = 0;
    u8 sample = 3;
    u16 rval = 0;
    if (onceThread == 0)
    {
        TPAD_Init(8);
        onceThread = 1;
    }
    if (mode)
    {
        sample = 6;
        keyen = 0;
    }
    rval = TPAD_Get_MaxVal(sample);
    if (rval > (tpad_default_val * 4 / 3) && rval < (10 * tpad_default_val))
    {
        LOG_D("TPAD_Get_MaxVal %d", rval);
        if (keyen == 0)
            res = 1;
        keyen = 3;
    }
    if (keyen)
        keyen--;
    rt_thread_delay(5);
    return res;
}
void TPAD_Run(void* parameter)
{
    int flag_moto = 0;
    while (1)
    {
        if (TPAD_Scan(0))
        {
            if (flag_moto)
            {
                emoto_end();
                flag_moto = 0;
            }
            else
            {
                ibmoto_begin(1);
                flag_moto = 1;
            }
        }
        rt_thread_delay(50);
    }
}
static rt_thread_t tid_Tpad = RT_NULL;
int TPAD(void)
{
    static u8 onceThread = 0;
    if(onceThread)return 0;else onceThread=1;
    /* 创建线程1，名称是thread1，入口是thread1_entry*/
    tid_Tpad = rt_thread_create("TPAD_Run", TPAD_Run, RT_NULL, 1024, 10, 10);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid_Tpad != RT_NULL)
        {rt_thread_startup(tid_Tpad);LOG_D("Tpad thread_startup");}
    return 0;
}
MSH_CMD_EXPORT(TPAD, TPAD_Run);

