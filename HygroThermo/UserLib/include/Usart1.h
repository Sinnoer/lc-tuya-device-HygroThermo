/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-10     Sinnoer       the first version
 */
#ifndef USERLIB_INCLUDE_USART1_H_
#define USERLIB_INCLUDE_USART1_H_
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <stm32h7xx.h>
#include <ulog.h>
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef unsigned char qdata;
typedef unsigned short qsize;

#define SAMPLE_UART_NAME        "uart1"
#define QUEUE_MAX_SIZE          512
#define CMD_HEAD                0X55AA
//#define CMD_TAIL              0XFFFCFFFF
typedef struct _QUEUE
{
    qsize _head;
    qsize _tail;
    qdata _data[QUEUE_MAX_SIZE];
}QUEUE;

extern rt_device_t usart1_serial;

#endif /* USERLIB_INCLUDE_USART1_H_ */
