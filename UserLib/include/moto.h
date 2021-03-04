/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     Sinnoer       the first version
 */

#ifndef USERLIB_MOTO_H_
#define USERLIB_MOTO_H_
#include <rtdevice.h>
#include "drv_common.h"
#include <stm32h7xx.h>
#include <rtthread.h>
#include <ulog.h>

#define MOTO_P GET_PIN(H, 2)
#define MOTO_N GET_PIN(B, 1)

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

TIM_HandleTypeDef TIM3_Handler;
TIM_OC_InitTypeDef TIM3_CH4Handler;

extern void ibmoto_begin(int peri);/* @peri定时长度，以 OS Tick 为单位，即 1个 OS Tick */
extern void emoto_end(void);

#endif /* USERLIB_MOTO_H_ */
