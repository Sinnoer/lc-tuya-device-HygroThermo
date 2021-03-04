/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-06     Sinnoer       the first version
 */
#ifndef USERLIB_INCLUDE_NIXIETUBE4P_H_
#define USERLIB_INCLUDE_NIXIETUBE4P_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <ulog.h>
////////////////////////////////////////
#define CH455_BIT_ENABLE    0x01            // 开启/关闭位
#define CH455_BIT_SLEEP     0x04            // 睡眠控制位
#define CH455_BIT_7SEG      0x08            // 7段控制位
#define CH455_BIT_INTENS1   0x10            // 1级亮度
#define CH455_BIT_INTENS2   0x20            // 2级亮度
#define CH455_BIT_INTENS3   0x30            // 3级亮度
#define CH455_BIT_INTENS4   0x40            // 4级亮度
#define CH455_BIT_INTENS5   0x50            // 5级亮度
#define CH455_BIT_INTENS6   0x60            // 6级亮度
#define CH455_BIT_INTENS7   0x70            // 7级亮度
#define CH455_BIT_INTENS8   0x00            // 8级亮度
///////////////////////////////////////
#define CH455_I2C_ADDR      0x40            // CH455的地址
#define CH455_I2C_MASK      0x3E            // CH455的高字节命令掩码
#define CH455_SYSOFF        0x0400          // 关闭显示、关闭键盘
#define CH455_SYSON         ( CH455_SYSOFF | CH455_BIT_ENABLE ) // 开启显示、键盘
#define CH455_SLEEPOFF      CH455_SYSOFF    // 关闭睡眠
#define CH455_SLEEPON       ( CH455_SYSOFF | CH455_BIT_SLEEP )  // 开启睡眠
#define CH455_7SEG_ON       ( CH455_SYSON | CH455_BIT_7SEG )    // 开启七段模式
#define CH455_8SEG_ON       ( CH455_SYSON | 0x00 )          // 开启八段模式
#define CH455_SYSON_4       ( CH455_SYSON | CH455_BIT_INTENS4 ) // 开启显示、键盘、4级亮度
#define CH455_SYSON_8       ( CH455_SYSON | CH455_BIT_INTENS8 ) // 开启显示、键盘、8级亮度
//////////////////////////////////
#define CH455_DIG0          0x1400          // 数码管位0显示,需另加8位数据
#define CH455_DIG1          0x1500          // 数码管位1显示,需另加8位数据
#define CH455_DIG2          0x1600          // 数码管位2显示,需另加8位数据
#define CH455_DIG3          0x1700          // 数码管位3显示,需另加8位数据


/*-------------------------- I2C1 CONFIG BEGIN --------------------------*/
#define I2_SCL_PIN          GET_PIN(H, 11)      /* SCL -> PC1 */
#define I2_SDA_PIN          GET_PIN(H, 12)      /* SDA -> PD6 */
#define I2_SCL_D_OUT        rt_pin_mode(I2_SCL_PIN, PIN_MODE_OUTPUT)
#define I2_SDA_D_OUT        rt_pin_mode(I2_SDA_PIN, PIN_MODE_OUTPUT)
#define I2C_DELAY           0
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
/*-------------------------- I2C1 CONFIG END --------------------------*/

extern void CH455_Write(u16 cmd );  //写命令

#endif /* USERLIB_INCLUDE_NIXIETUBE4P_H_ */
