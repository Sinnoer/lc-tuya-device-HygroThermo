/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <ulog.h>
#include "wifi.h"

#define LED_PIN GET_PIN(I, 8)
extern void hygroThermo(void);
extern int TPAD(void);
extern int light_sensation(void);
extern int uart1_start(void);
extern void wifi_protocol_init(void);
extern void wifi_uart_service(void);
extern int hygro_thermo_updata(int argc, char *argv[]);

int main(void)
{
    rt_uint32_t count = 1;
    hygroThermo();                                  //温湿度采集
    TPAD();                                         //按键检测
    light_sensation();                              //光强采集
    if(uart1_start() == RT_ERROR)return RT_ERROR;   //串口1开启
    wifi_protocol_init();                           //涂鸦wifi开启
    hygro_thermo_updata(0,0);                       //温湿度上传
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);          //指示灯

    while(count++)
    {
        rt_enter_critical();
        wifi_uart_service();                        //涂鸦wifi服务
        rt_exit_critical();
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
        if(count>100)count=1;
    }
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);
int clock_information1(void)
{
    rt_kprintf("System Clock information\n");
    rt_kprintf("SYSCLK_Frequency = %d\n", HAL_RCC_GetSysClockFreq());
    rt_kprintf("HCLK_Frequency   = %d\n", HAL_RCC_GetHCLKFreq());
    rt_kprintf("PCLK1_Frequency  = %d\n", HAL_RCC_GetPCLK1Freq());
    rt_kprintf("PCLK2_Frequency  = %d\n", HAL_RCC_GetPCLK2Freq());
    return RT_EOK;
}
MSH_CMD_EXPORT(clock_information1, clock_information);

