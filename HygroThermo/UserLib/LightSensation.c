#include <include/LightSensation.h>

float volatile lightValue=0;

void MY_ADC_Init(void)
{
    static char onceThread=0;
    if(onceThread == 0)onceThread =1;
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4; //4分频，ADCCLK = PER_CK/4 = 64/4 = 16MHz
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_16B;
    ADC1_Handler.Init.ScanConvMode=DISABLE;
    ADC1_Handler.Init.EOCSelection=ADC_EOC_SINGLE_CONV;
    ADC1_Handler.Init.LowPowerAutoWait=DISABLE;
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;
    ADC1_Handler.Init.NbrOfConversion=1;
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;
    ADC1_Handler.Init.NbrOfDiscConversion=0;
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;
//    ADC1_Handler.Init.BoostMode=ENABLE;
    ADC1_Handler.Init.Overrun=ADC_OVR_DATA_OVERWRITTEN;
    ADC1_Handler.Init.OversamplingMode=DISABLE;
    ADC1_Handler.Init.ConversionDataManagement=ADC_CONVERSIONDATA_DR;
    HAL_ADC_Init(&ADC1_Handler);

    HAL_ADCEx_Calibration_Start(&ADC1_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);
}
unsigned long Get_Adc(unsigned long ch)
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;

    ADC1_ChanConf.Channel=ch;
    ADC1_ChanConf.Rank=ADC_REGULAR_RANK_1;
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_64CYCLES_5;
    ADC1_ChanConf.SingleDiff=ADC_SINGLE_ENDED;
    ADC1_ChanConf.OffsetNumber=ADC_OFFSET_NONE;
    ADC1_ChanConf.Offset=0;
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);
    HAL_ADC_Start(&ADC1_Handler);
    HAL_ADC_PollForConversion(&ADC1_Handler,10);
    return (unsigned long)HAL_ADC_GetValue(&ADC1_Handler);
}
unsigned long Get_Adc_Average(unsigned long ch,unsigned char times)
{
    unsigned long temp_val=0;
    unsigned char t;
    for(t=0;t<times;t++)
    {
        temp_val+= Get_Adc(ch);
        rt_thread_delay(5);
    }
    return temp_val/times;
}

//#define TPAD_PIN            GET_PIN(B, 0)
//#define TPAD_DEFAULT_VAL    1500
//#define TPAD_AD_VAL         100
//void TPAD_reset(void)
//{
//    rt_pin_mode(TPAD_PIN, PIN_MODE_OUTPUT_OD);
//    rt_pin_write(TPAD_PIN, PIN_LOW);
//    rt_thread_delay(5);
//    rt_pin_mode(TPAD_PIN, PIN_MODE_INPUT);
//}
//unsigned char TPAD_Scan(void)
//{
//    unsigned long get_Val;
//    TPAD_reset();
//    get_Val = Get_Adc_Average(ADC_CHANNEL_9,20);
//    LOG_D("LOG_D: TPAD_Adc_Average = %d",get_Val);
//    if(get_Val > TPAD_DEFAULT_VAL+TPAD_AD_VAL)return 1;
//    else return 0;
//}

void adc_vol_sample(int argc, char *argv[])
{
    MY_ADC_Init();
//    LOG_D("LOG_D: TPAD_Adc_Average = %d", (Get_Adc_Average(ADC_CHANNEL_9,20)));//TPAD
    LOG_D("LOG_D: Light_Adc_Average = %f", (float)(Get_Adc_Average(ADC_CHANNEL_1,20)*(3.3/65536)));
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(adc_vol_sample, adc voltage convert sample);

void set_NixieTube4P_light(char s)
{
    switch(s)
    {
        case 1:CH455_Write(CH455_SYSON | CH455_BIT_INTENS1);
            break;
        case 2:CH455_Write(CH455_SYSON | CH455_BIT_INTENS4);
            break;
        case 3:CH455_Write(CH455_SYSON | CH455_BIT_INTENS8);
            break;
   }
}
void light_sensation_Run(void* parameter)
{
    static float old_lightValue=0;
    MY_ADC_Init();
    while(1)
    {
        lightValue = (float)(Get_Adc_Average(ADC_CHANNEL_1,20)*(3.3/65536));
        if(fabs(lightValue - old_lightValue) > 0.3)
        {
            if(lightValue > 2)set_NixieTube4P_light(1);
            else if(lightValue > 1)set_NixieTube4P_light(2);
            else set_NixieTube4P_light(3);
            old_lightValue = lightValue;
        }
        rt_thread_mdelay(1000);
    }
}
static rt_thread_t tid_light_sensation = RT_NULL;//LightSensation
void light_sensation(void)
{
    static u8 onceThread = 0;
    if(onceThread)return;else onceThread=1;
    /* 创建线程1，名称是thread1，入口是thread1_entry*/
    tid_light_sensation = rt_thread_create("light_sensation_Run", light_sensation_Run, RT_NULL, 1024, 10, 10);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid_light_sensation != RT_NULL)
        {rt_thread_startup(tid_light_sensation);LOG_D("light_sensation thread_startup");}
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(light_sensation, light_sensation sample);

