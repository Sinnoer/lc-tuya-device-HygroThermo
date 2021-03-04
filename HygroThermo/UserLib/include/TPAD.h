/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     Sinnoer       the first version
 */
#ifndef USERLIB_INCLUDE_TPAD_H_
#define USERLIB_INCLUDE_TPAD_H_
#include <rtdevice.h>
#include "drv_common.h"
#include <stm32h7xx.h>
#include <ulog.h>

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define TPAD_ARR_MAX_VAL  0XFFFFFFFF

extern unsigned char TPAD_Scan(int mode);
extern void ibmoto_begin(int peri);
extern void emoto_end(void);

#endif /* USERLIB_INCLUDE_TPAD_H_ */
