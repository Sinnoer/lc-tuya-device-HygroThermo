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
//u16 thermo =DEFAULT_THERMO ;//1�¶�ֵ//��������ʪ���ļ���
//u16 hygro =DEFAULT_HYGRO;//2ʪ��ֵ
u8 battery_level =DEFAULT_BATTERY_LEVEL;//3��ذٷֱ�
u8 battery_percent =DEFAULT_BATTERY_PERCENT;//4��ص���
u8 alarm_dismantle =DEFAULT_ALARM_DISMANTLE;//5���𾯱�
u16 sampling_time_thermo =DEFAULT_SAMPLING_TIME_THERMO;//6�¶Ȳ���ʱ��
u16 sampling_time_hygro =DEFAULT_SAMPLING_TIME_HYGRO;//7ʪ�Ȳ���ʱ��
u8 charge_state =DEFAULT_CHARGE_STATE;//8���״̬
u8 scale_switch =DEFAULT_SCALE_SWITCH;//9�±��л�
u16 limit_set_thermo_up =DEFAULT_LIMIT_SET_THERMO_UP;//10�¶���������
u16 limit_set_thermo_down =DEFAULT_LIMIT_SET_THERMO_DOWN;//11�¶���������
u16 limit_set_hygro_up =DEFAULT_LIMIT_SET_HYGRO_UP;//12ʪ����������
u16 limit_set_hygro_down =DEFAULT_LIMIT_SET_HYGRO_DOWN;//13ʪ����������
u8 alarm_thermo =DEFAULT_ALARM_THERMO;//14�¶ȱ���
u8 alarm_hygro =DEFAULT_ALARM_HYGRO;//15ʪ�ȱ���
u16 current_brightness =DEFAULT_CURRENT_BRIGHTNESS;//16��ǰ����
u8 periodic_reporting_thermo =DEFAULT_PERIODIC_REPORTING_THERMO;//17�¶������ϱ�
u8 periodic_reporting_hygro =DEFAULT_PERIODIC_REPORTING_HYGRO;//18ʪ�������ϱ�
u8 sensitivity_thermo =DEFAULT_SENSITIVITY_THERMO;//19�¶�������
u8 sensitivity_hygro =DEFAULT_SENSITIVITY_HYGRO;//20ʪ��������
u8 on_off =DEFAULT_ON_OFF;//21����
u16 seting_thermo =DEFAULT_SETING_THERMO;//22�¶�����
u8 other_function =DEFAULT_OTHER_FUNCTION;//101�Զ������������ȣ�
///////////////////////////////////////////////////
/// //////////////////////////////////////////////
u16 send_thermo(void)//1�¶�ֵ
{
    u16 tem=DEFAULT_THERMO;
    tem = ((temperature&0xFF00)>>8)*10+(temperature&0xFF)/10;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_hygro(void)//2ʪ��ֵ
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
u8 send_battery_level(void)//3��ص���״̬
{
    u16 tem=DEFAULT_BATTERY_LEVEL;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_battery_percent(void)//4��ذٷֱ�
{
    u16 tem=DEFAULT_BATTERY_PERCENT;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_alarm_dismantle(void)//5���𾯱�
{
    u16 tem=DEFAULT_ALARM_DISMANTLE;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_sampling_time_thermo(void)//6�¶Ȳ���ʱ��
{
    u16 tem=DEFAULT_SAMPLING_TIME_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_sampling_time_hygro(void)//7ʪ�Ȳ���ʱ��
{
    u16 tem=DEFAULT_SAMPLING_TIME_HYGRO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_charge_state(void)//8���״̬
{
    u16 tem=DEFAULT_CHARGE_STATE;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_scale_switch(void)//9�±��л�
{
    u16 tem=DEFAULT_SCALE_SWITCH;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_limit_set_thermo_up(void)//10�¶���������
{
    u16 tem=DEFAULT_LIMIT_SET_THERMO_UP;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_limit_set_thermo_down(void)//11�¶���������
{
    u16 tem=DEFAULT_LIMIT_SET_THERMO_DOWN;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_limit_set_hygro_up(void)//12ʪ����������
{
    u16 tem=DEFAULT_LIMIT_SET_HYGRO_UP;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_limit_set_hygro_down(void)//13ʪ����������
{
    u16 tem=DEFAULT_LIMIT_SET_HYGRO_DOWN;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_alarm_thermo(void)//14�¶ȱ���
{
    u16 tem=DEFAULT_ALARM_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_alarm_hygro(void)//15ʪ�ȱ���
{
    u16 tem=DEFAULT_ALARM_HYGRO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_current_brightness(void)//16��ǰ����
{
    u16 tem=DEFAULT_CURRENT_BRIGHTNESS;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_periodic_reporting_thermo(void)//17�¶������ϱ�1-60
{
    u16 tem=DEFAULT_PERIODIC_REPORTING_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_periodic_reporting_hygro(void)//18ʪ�������ϱ�1-60
{
    u16 tem=DEFAULT_PERIODIC_REPORTING_HYGRO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_sensitivity_thermo(void)//19�¶�������3-10
{
    u16 tem=DEFAULT_SENSITIVITY_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_sensitivity_hygro(void)//20ʪ��������3-10
{
    u16 tem=DEFAULT_SENSITIVITY_HYGRO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_on_off(void)//21����
{
    u16 tem=DEFAULT_ON_OFF;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u16 send_seting_thermo(void)//22�¶�����
{
    u16 tem=DEFAULT_SETING_THERMO;
    if(0)
    {   //Erro

    }
    else
        return tem;
}
u8 send_other_function(void)//101�Զ������������ȣ�
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
            mcu_dp_value_update(DPID_TEMP_CURRENT,send_thermo()); //VALUE�������ϱ�;
            mcu_dp_value_update(DPID_HUMIDITY_VALUE,send_hygro()); //VALUE�������ϱ�;
        }
        rt_thread_delay(10000);
    }
}
static rt_thread_t tid_hygro_thermo_Run = RT_NULL;
int hygro_thermo_updata(int argc, char *argv[])
{
    static u8 onceThread = 0;
    if(onceThread)return 0;else onceThread=1;
    /* �����߳�1��������thread1�������thread1_entry*/
    tid_hygro_thermo_Run = rt_thread_create("hygro_thermo_Run", hygro_thermo_Run, RT_NULL, 1024, 10, 10);

    /* �������߳̿��ƿ飬��������߳� */
    if (tid_hygro_thermo_Run != RT_NULL)
        {rt_thread_startup(tid_hygro_thermo_Run);LOG_D("tid_hygro_thermo_Run thread_startup");}
    return 0;
}
MSH_CMD_EXPORT(hygro_thermo_updata, hygro_thermo_updata_Run);

void wifi_reset_by(int argc, char *argv[])
{
    mcu_reset_wifi();
    mcu_set_wifi_mode(2);   //����ģʽ(����������ʽ)CONFIG_MODE_SPECIAL=2;
}
MSH_CMD_EXPORT(wifi_reset_by, wifi_reset_by);
