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
#define CH455_BIT_ENABLE    0x01            // ����/�ر�λ
#define CH455_BIT_SLEEP     0x04            // ˯�߿���λ
#define CH455_BIT_7SEG      0x08            // 7�ο���λ
#define CH455_BIT_INTENS1   0x10            // 1������
#define CH455_BIT_INTENS2   0x20            // 2������
#define CH455_BIT_INTENS3   0x30            // 3������
#define CH455_BIT_INTENS4   0x40            // 4������
#define CH455_BIT_INTENS5   0x50            // 5������
#define CH455_BIT_INTENS6   0x60            // 6������
#define CH455_BIT_INTENS7   0x70            // 7������
#define CH455_BIT_INTENS8   0x00            // 8������
///////////////////////////////////////
#define CH455_I2C_ADDR      0x40            // CH455�ĵ�ַ
#define CH455_I2C_MASK      0x3E            // CH455�ĸ��ֽ���������
#define CH455_SYSOFF        0x0400          // �ر���ʾ���رռ���
#define CH455_SYSON         ( CH455_SYSOFF | CH455_BIT_ENABLE ) // ������ʾ������
#define CH455_SLEEPOFF      CH455_SYSOFF    // �ر�˯��
#define CH455_SLEEPON       ( CH455_SYSOFF | CH455_BIT_SLEEP )  // ����˯��
#define CH455_7SEG_ON       ( CH455_SYSON | CH455_BIT_7SEG )    // �����߶�ģʽ
#define CH455_8SEG_ON       ( CH455_SYSON | 0x00 )          // �����˶�ģʽ
#define CH455_SYSON_4       ( CH455_SYSON | CH455_BIT_INTENS4 ) // ������ʾ�����̡�4������
#define CH455_SYSON_8       ( CH455_SYSON | CH455_BIT_INTENS8 ) // ������ʾ�����̡�8������
//////////////////////////////////
#define CH455_DIG0          0x1400          // �����λ0��ʾ,�����8λ����
#define CH455_DIG1          0x1500          // �����λ1��ʾ,�����8λ����
#define CH455_DIG2          0x1600          // �����λ2��ʾ,�����8λ����
#define CH455_DIG3          0x1700          // �����λ3��ʾ,�����8λ����


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

extern void CH455_Write(u16 cmd );  //д����

#endif /* USERLIB_INCLUDE_NIXIETUBE4P_H_ */
