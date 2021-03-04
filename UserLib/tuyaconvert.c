/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-14     Sinnoer       the first version
 */
#include "include/tuyaconvert.h"
///////////////////////////////////
/// /////////////////////////////////
//u16 thermo =DEFAULT_THERMO ;//1温度值//定义在温湿度文件中
//u16 hygro =DEFAULT_HYGRO;//2湿度值
u8 battery_level =DEFAULT_BATTERY_LEVEL;//3电池百分比
u8 battery_percent =DEFAULT_BATTERY_PERCENT;//4电池电量
u8 alarm_dismantle =DEFAULT_ALARM_DISMANTLE;//5防拆警报
u16 sampling_time_thermo =DEFAULT_SAMPLING_TIME_THERMO;//6温度采样时间
u16 sampling_time_hygro =DEFAULT_SAMPLING_TIME_HYGRO;//7湿度采样时间
u8 charge_state =DEFAULT_CHARGE_STATE;//8充电状态
u8 scale_switch =DEFAULT_SCALE_SWITCH;//9温标切换
u16 limit_set_thermo_up =DEFAULT_LIMIT_SET_THERMO_UP;//10温度上限设置
u16 limit_set_thermo_down =DEFAULT_LIMIT_SET_THERMO_DOWN;//11温度下限设置
u16 limit_set_hygro_up =DEFAULT_LIMIT_SET_HYGRO_UP;//12湿度上限设置
u16 limit_set_hygro_down =DEFAULT_LIMIT_SET_HYGRO_DOWN;//13湿度下限设置
u8 alarm_thermo =DEFAULT_ALARM_THERMO;//14温度报警
u8 alarm_hygro =DEFAULT_ALARM_HYGRO;//15湿度报警
u16 current_brightness =DEFAULT_CURRENT_BRIGHTNESS;//16当前亮度
u8 periodic_reporting_thermo =DEFAULT_PERIODIC_REPORTING_THERMO;//17温度周期上报
u8 periodic_reporting_hygro =DEFAULT_PERIODIC_REPORTING_HYGRO;//18湿度周期上报
u8 sensitivity_thermo =DEFAULT_SENSITIVITY_THERMO;//19温度灵敏度
u8 sensitivity_hygro =DEFAULT_SENSITIVITY_HYGRO;//20湿度灵敏度
u8 on_off =DEFAULT_ON_OFF;//21开关
u16 seting_thermo =DEFAULT_SETING_THERMO;//22温度设置
u8 other_function =DEFAULT_OTHER_FUNCTION;//101自定义心跳（风扇）
///////////////////////////////////////////////////
/// //////////////////////////////////////////////
u16 send_thermo(void)//1温度值
{
    u16 tem=DEFAULT_THERMO;
    tem = ((temperature&0xFF00)>>8)*10+(temperature&0xFF)/10;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_hygro(void)//2湿度值
{
   u16 tem=DEFAULT_HYGRO;
   tem = ((humidity&0xFF00)>>8);
//   rt_kprintf("send_hygro: %d\n",tem);
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_battery_level(void)//3电池电量状态
{
    u16 tem=DEFAULT_BATTERY_LEVEL;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_battery_percent(void)//4电池百分比
{
    u16 tem=DEFAULT_BATTERY_PERCENT;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_alarm_dismantle(void)//5防拆警报
{
    u16 tem=DEFAULT_ALARM_DISMANTLE;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_sampling_time_thermo(void)//6温度采样时间
{
    u16 tem=DEFAULT_SAMPLING_TIME_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_sampling_time_hygro(void)//7湿度采样时间
{
    u16 tem=DEFAULT_SAMPLING_TIME_HYGRO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_charge_state(void)//8充电状态
{
    u16 tem=DEFAULT_CHARGE_STATE;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_scale_switch(void)//9温标切换
{
    u16 tem=DEFAULT_SCALE_SWITCH;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_limit_set_thermo_up(void)//10温度上限设置
{
    u16 tem=DEFAULT_LIMIT_SET_THERMO_UP;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_limit_set_thermo_down(void)//11温度下限设置
{
    u16 tem=DEFAULT_LIMIT_SET_THERMO_DOWN;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_limit_set_hygro_up(void)//12湿度上限设置
{
    u16 tem=DEFAULT_LIMIT_SET_HYGRO_UP;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_limit_set_hygro_down(void)//13湿度下限设置
{
    u16 tem=DEFAULT_LIMIT_SET_HYGRO_DOWN;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_alarm_thermo(void)//14温度报警
{
    u16 tem=DEFAULT_ALARM_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_alarm_hygro(void)//15湿度报警
{
    u16 tem=DEFAULT_ALARM_HYGRO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_current_brightness(void)//16当前亮度
{
    u16 tem=DEFAULT_CURRENT_BRIGHTNESS;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_periodic_reporting_thermo(void)//17温度周期上报1-60
{
    u16 tem=DEFAULT_PERIODIC_REPORTING_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_periodic_reporting_hygro(void)//18湿度周期上报1-60
{
    u16 tem=DEFAULT_PERIODIC_REPORTING_HYGRO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_sensitivity_thermo(void)//19温度灵敏度3-10
{
    u16 tem=DEFAULT_SENSITIVITY_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_sensitivity_hygro(void)//20湿度灵敏度3-10
{
    u16 tem=DEFAULT_SENSITIVITY_HYGRO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_on_off(void)//21开关
{
    u16 tem=DEFAULT_ON_OFF;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_seting_thermo(void)//22温度设置
{
    u16 tem=DEFAULT_SETING_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_other_function(void)//101自定义心跳（风扇）
{
    u16 tem=DEFAULT_OTHER_FUNCTION;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
////////////////////////////////////////////////////////

void hygro_thermo_Run(void* parameter)
{
    static u16 old_temperature = 0, old_humidity = 0;
    //temperature humidity
    while (1)
    {
        if (temperature != old_temperature || humidity != old_humidity)
        {
            old_temperature = temperature;
            old_humidity = humidity;
            mcu_dp_value_update(DPID_TEMP_CURRENT,send_thermo()); //VALUE型数据上报;
            mcu_dp_value_update(DPID_HUMIDITY_VALUE,send_hygro()); //VALUE型数据上报;
        }
        rt_thread_delay(10000);
    }
}
static rt_thread_t tid_hygro_thermo_Run = RT_NULL;
int hygro_thermo_updata(int argc, char *argv[])
{
    static u8 onceThread = 0;
    if(onceThread)return 0;else onceThread=1;
    /* 创建线程1，名称是thread1，入口是thread1_entry*/
    tid_hygro_thermo_Run = rt_thread_create("hygro_thermo_Run", hygro_thermo_Run, RT_NULL, 1024, 10, 10);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid_hygro_thermo_Run != RT_NULL)
        {rt_thread_startup(tid_hygro_thermo_Run);LOG_D("tid_hygro_thermo_Run thread_startup");}
    return 0;
}
MSH_CMD_EXPORT(hygro_thermo_updata, hygro_thermo_updata_Run);

void wifi_reset_by(int argc, char *argv[])
{
    mcu_reset_wifi();
    mcu_set_wifi_mode(2);   //防误触模式(特殊配网方式)CONFIG_MODE_SPECIAL=2;
}
MSH_CMD_EXPORT(wifi_reset_by, wifi_reset_by);
