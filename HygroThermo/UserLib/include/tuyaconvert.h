/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-14     Sinnoer       the first version
 */
#ifndef USERLIB_INCLUDE_TUYACONVERT_H_
#define USERLIB_INCLUDE_TUYACONVERT_H_
#include "HygroThermo.h"
#include "NixieTube4P.h"
#include "moto.h"
#include "wifi.h"

#define MOTO_DELAY                          3//���ڷ����ٶ�
//////////////////////////////////////////////////////////
#define DEFAULT_THERMO                      0//1�¶�ֵ
#define DEFAULT_HYGRO                       0//2ʪ��ֵ
#define DEFAULT_BATTERY_LEVEL               2//3��ذٷֱ�0:��;1:��;2:��;
#define DEFAULT_BATTERY_PERCENT             100//4��ص���
#define DEFAULT_ALARM_DISMANTLE             0//5���𾯱�
#define DEFAULT_SAMPLING_TIME_THERMO        100//6�¶Ȳ���ʱ��
#define DEFAULT_SAMPLING_TIME_HYGRO         100//7ʪ�Ȳ���ʱ��
#define DEFAULT_CHARGE_STATE                0//8���״̬
#define DEFAULT_SCALE_SWITCH                0//9�±��л�
#define DEFAULT_LIMIT_SET_THERMO_UP         600//10�¶���������
#define DEFAULT_LIMIT_SET_THERMO_DOWN       0//11�¶���������
#define DEFAULT_LIMIT_SET_HYGRO_UP          100//12ʪ����������
#define DEFAULT_LIMIT_SET_HYGRO_DOWN        0//13ʪ����������
#define DEFAULT_ALARM_THERMO                0//14�¶ȱ���
#define DEFAULT_ALARM_HYGRO                 0//15ʪ�ȱ���
#define DEFAULT_CURRENT_BRIGHTNESS          1000//16��ǰ����
#define DEFAULT_PERIODIC_REPORTING_THERMO   60//17�¶������ϱ�
#define DEFAULT_PERIODIC_REPORTING_HYGRO    60//18ʪ�������ϱ�
#define DEFAULT_SENSITIVITY_THERMO          3//19�¶�������
#define DEFAULT_SENSITIVITY_HYGRO           3//20ʪ��������
#define DEFAULT_ON_OFF                      0//21����
#define DEFAULT_SETING_THERMO               50//22�¶�����
#define DEFAULT_OTHER_FUNCTION              0//101�Զ������������ȣ�

////////////////////////////////////////////////////////////////////////
//extern u16 thermo  ;//1�¶�ֵ
//extern u16 hygro ;//2ʪ��ֵ
extern u8 battery_level ;//3��ذٷֱ�
extern u8 battery_percent ;//4��ص���
extern u8 alarm_dismantle ;//5���𾯱�
extern u16 sampling_time_thermo ;//6�¶Ȳ���ʱ��
extern u16 sampling_time_hygro ;//7ʪ�Ȳ���ʱ��
extern u8 charge_state ;//8���״̬
extern u8 scale_switch ;//9�±��л�
extern u16 limit_set_thermo_up ;//10�¶���������
extern u16 limit_set_thermo_down ;//11�¶���������
extern u16 limit_set_hygro_up ;//12ʪ����������
extern u16 limit_set_hygro_down ;//13ʪ����������
extern u8 alarm_thermo ;//14�¶ȱ���
extern u8 alarm_hygro ;//15ʪ�ȱ���
extern u16 current_brightness ;//16��ǰ����
extern u8 periodic_reporting_thermo ;//17�¶������ϱ�
extern u8 periodic_reporting_hygro ;//18ʪ�������ϱ�
extern u8 sensitivity_thermo ;//19�¶�������
extern u8 sensitivity_hygro ;//20ʪ��������
extern u8 on_off ;//21����
extern u16 seting_thermo ;//22�¶�����
extern u8 other_function ;//101�Զ������������ȣ�extern u16 send_thermo(void);//1�¶�ֵ

////////////////////////////////////////////////////////////////////
extern u16 send_thermo(void);//1�¶�ֵ
extern u16 send_hygro(void);//2ʪ��ֵ
extern u8 send_battery_level(void);//3��ص���״̬
extern u8 send_battery_percent(void);//4��ذٷֱ�
extern u8 send_alarm_dismantle(void);//5���𾯱�
extern u16 send_sampling_time_thermo(void);//6�¶Ȳ���ʱ��
extern u16 send_sampling_time_hygro(void);//7ʪ�Ȳ���ʱ��
extern u8 send_charge_state(void);//8���״̬
extern u8 send_scale_switch(void);//9�±��л�
extern u16 send_limit_set_thermo_up(void);//10�¶���������
extern u16 send_limit_set_thermo_down(void);//11�¶���������
extern u16 send_limit_set_hygro_up(void);//12ʪ����������
extern u16 send_limit_set_hygro_down(void);//13ʪ����������
extern u8 send_alarm_thermo(void);//14�¶ȱ���
extern u8 send_alarm_hygro(void);//15ʪ�ȱ���
extern u16 send_current_brightness(void);//16��ǰ����
extern u8 send_periodic_reporting_thermo(void);//17�¶������ϱ�1-60
extern u8 send_periodic_reporting_hygro(void);//18ʪ�������ϱ�1-60
extern u8 send_sensitivity_thermo(void);//19�¶�������3-10
extern u8 send_sensitivity_hygro(void);//20ʪ��������3-10
extern u8 send_on_off(void);//21����
extern u16 send_seting_thermo(void);//22�¶�����
extern u8 send_other_function(void);//101�Զ������������ȣ�




#endif /* USERLIB_INCLUDE_TUYACONVERT_H_ */
