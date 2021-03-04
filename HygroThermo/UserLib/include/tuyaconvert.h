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

#define MOTO_DELAY                          3//调节风扇速度
//////////////////////////////////////////////////////////
#define DEFAULT_THERMO                      0//1温度值
#define DEFAULT_HYGRO                       0//2湿度值
#define DEFAULT_BATTERY_LEVEL               2//3电池百分比0:低;1:中;2:高;
#define DEFAULT_BATTERY_PERCENT             100//4电池电量
#define DEFAULT_ALARM_DISMANTLE             0//5防拆警报
#define DEFAULT_SAMPLING_TIME_THERMO        100//6温度采样时间
#define DEFAULT_SAMPLING_TIME_HYGRO         100//7湿度采样时间
#define DEFAULT_CHARGE_STATE                0//8充电状态
#define DEFAULT_SCALE_SWITCH                0//9温标切换
#define DEFAULT_LIMIT_SET_THERMO_UP         600//10温度上限设置
#define DEFAULT_LIMIT_SET_THERMO_DOWN       0//11温度下限设置
#define DEFAULT_LIMIT_SET_HYGRO_UP          100//12湿度上限设置
#define DEFAULT_LIMIT_SET_HYGRO_DOWN        0//13湿度下限设置
#define DEFAULT_ALARM_THERMO                0//14温度报警
#define DEFAULT_ALARM_HYGRO                 0//15湿度报警
#define DEFAULT_CURRENT_BRIGHTNESS          1000//16当前亮度
#define DEFAULT_PERIODIC_REPORTING_THERMO   60//17温度周期上报
#define DEFAULT_PERIODIC_REPORTING_HYGRO    60//18湿度周期上报
#define DEFAULT_SENSITIVITY_THERMO          3//19温度灵敏度
#define DEFAULT_SENSITIVITY_HYGRO           3//20湿度灵敏度
#define DEFAULT_ON_OFF                      0//21开关
#define DEFAULT_SETING_THERMO               50//22温度设置
#define DEFAULT_OTHER_FUNCTION              0//101自定义心跳（风扇）

////////////////////////////////////////////////////////////////////////
//extern u16 thermo  ;//1温度值
//extern u16 hygro ;//2湿度值
extern u8 battery_level ;//3电池百分比
extern u8 battery_percent ;//4电池电量
extern u8 alarm_dismantle ;//5防拆警报
extern u16 sampling_time_thermo ;//6温度采样时间
extern u16 sampling_time_hygro ;//7湿度采样时间
extern u8 charge_state ;//8充电状态
extern u8 scale_switch ;//9温标切换
extern u16 limit_set_thermo_up ;//10温度上限设置
extern u16 limit_set_thermo_down ;//11温度下限设置
extern u16 limit_set_hygro_up ;//12湿度上限设置
extern u16 limit_set_hygro_down ;//13湿度下限设置
extern u8 alarm_thermo ;//14温度报警
extern u8 alarm_hygro ;//15湿度报警
extern u16 current_brightness ;//16当前亮度
extern u8 periodic_reporting_thermo ;//17温度周期上报
extern u8 periodic_reporting_hygro ;//18湿度周期上报
extern u8 sensitivity_thermo ;//19温度灵敏度
extern u8 sensitivity_hygro ;//20湿度灵敏度
extern u8 on_off ;//21开关
extern u16 seting_thermo ;//22温度设置
extern u8 other_function ;//101自定义心跳（风扇）extern u16 send_thermo(void);//1温度值

////////////////////////////////////////////////////////////////////
extern u16 send_thermo(void);//1温度值
extern u16 send_hygro(void);//2湿度值
extern u8 send_battery_level(void);//3电池电量状态
extern u8 send_battery_percent(void);//4电池百分比
extern u8 send_alarm_dismantle(void);//5防拆警报
extern u16 send_sampling_time_thermo(void);//6温度采样时间
extern u16 send_sampling_time_hygro(void);//7湿度采样时间
extern u8 send_charge_state(void);//8充电状态
extern u8 send_scale_switch(void);//9温标切换
extern u16 send_limit_set_thermo_up(void);//10温度上限设置
extern u16 send_limit_set_thermo_down(void);//11温度下限设置
extern u16 send_limit_set_hygro_up(void);//12湿度上限设置
extern u16 send_limit_set_hygro_down(void);//13湿度下限设置
extern u8 send_alarm_thermo(void);//14温度报警
extern u8 send_alarm_hygro(void);//15湿度报警
extern u16 send_current_brightness(void);//16当前亮度
extern u8 send_periodic_reporting_thermo(void);//17温度周期上报1-60
extern u8 send_periodic_reporting_hygro(void);//18湿度周期上报1-60
extern u8 send_sensitivity_thermo(void);//19温度灵敏度3-10
extern u8 send_sensitivity_hygro(void);//20湿度灵敏度3-10
extern u8 send_on_off(void);//21开关
extern u16 send_seting_thermo(void);//22温度设置
extern u8 send_other_function(void);//101自定义心跳（风扇）




#endif /* USERLIB_INCLUDE_TUYACONVERT_H_ */
