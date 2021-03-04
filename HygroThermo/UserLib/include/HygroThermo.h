/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-07     Sinnoer       the first version
 */
#ifndef USERLIB_INCLUDE_HYGROTHERMO_H_
#define USERLIB_INCLUDE_HYGROTHERMO_H_
#include <rtdevice.h>
#include <stm32h7xx.h>
#include <rtthread.h>
#include <ulog.h>
#include "drv_common.h"
#include "NixieTube4P.h"
#include "string.h"

#define DHT11_IO_PIN        GET_PIN(A, 15)
#define DHT11_IO_TOIN       rt_pin_mode(DHT11_IO_PIN, PIN_MODE_INPUT)
#define DHT11_IO_TOOUT      rt_pin_mode(DHT11_IO_PIN, PIN_MODE_OUTPUT)
#define DHT11_DQ_OUT_H      rt_pin_write(DHT11_IO_PIN, PIN_HIGH)
#define DHT11_DQ_OUT_L      rt_pin_write(DHT11_IO_PIN, PIN_LOW)
#define DHT11_DQ_IN         rt_pin_read(DHT11_IO_PIN)

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

extern u16 temperature;
extern u16 humidity;

extern u8 num[16];
extern void nixieTube4P_init(void);
extern void CH455_Write(u16 cmd );  //–¥√¸¡Ó


#endif /* USERLIB_INCLUDE_HYGROTHERMO_H_ */
