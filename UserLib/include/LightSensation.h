/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     Sinnoer       the first version
 */
#ifndef USERLIB_INCLUDE_LIGHTSENSATION_H_
#define USERLIB_INCLUDE_ADC_H_
#include <ulog.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <stm32h7xx.h>
#include "include/NixieTube4P.h"

extern volatile float lightValue;

ADC_HandleTypeDef ADC1_Handler;


#endif /* USERLIB_INCLUDE_LIGHTSENSATION_H_ */
