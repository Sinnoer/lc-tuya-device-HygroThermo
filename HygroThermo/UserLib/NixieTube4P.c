/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-06     Sinnoer       the first version
 */
#include "include/NixieTube4P.h"
//           0     1    2    3    4   5     6    7    8    9    A    B    C    D    E    F
u8 num[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x5C,0x7C,0x58,0x5E,0x7B,0x71};
void CH455_I2c_Start(void)
{
    I2_SDA_D_OUT;
    rt_pin_write(I2_SCL_PIN, PIN_HIGH);   //SCL = high;
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SDA_PIN, PIN_HIGH);   //SDA = high;
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SDA_PIN, PIN_LOW);    //SDA = low;
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SCL_PIN, PIN_LOW);    //SCL = low;
    rt_thread_delay(I2C_DELAY);
}
void CH455_I2c_Stop(void)
{
    I2_SDA_D_OUT;
    rt_pin_write(I2_SCL_PIN, PIN_LOW);     //SCL = low;
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SDA_PIN, PIN_LOW);    //SDA = low;
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SCL_PIN, PIN_HIGH);    //SCL = high;
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SDA_PIN, PIN_HIGH);    //SDA = high;
    rt_thread_delay(I2C_DELAY);
}
void CH455_I2c_WrByte(u8 IIC_Byte)
{
    u8 i;
    I2_SDA_D_OUT;
    for(i = 0; i < 8; i++)
    {
        if(IIC_Byte & 0x80)
            rt_pin_write(I2_SDA_PIN, PIN_HIGH); //SDA=high;
        else
            rt_pin_write(I2_SDA_PIN, PIN_LOW);  //SDA=low;
        rt_thread_delay(I2C_DELAY);
        rt_pin_write(I2_SCL_PIN, PIN_HIGH);
        rt_thread_delay(I2C_DELAY);
        rt_pin_write(I2_SCL_PIN, PIN_LOW);
        rt_thread_delay(I2C_DELAY);
        IIC_Byte<<=1;
    }
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SDA_PIN, PIN_HIGH);
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SCL_PIN, PIN_HIGH);
    rt_thread_delay(I2C_DELAY);
    rt_pin_write(I2_SCL_PIN, PIN_LOW);
    rt_thread_delay(I2C_DELAY);
}
void CH455_Write(u16 cmd )  //写命令
{
    CH455_I2c_Start();                  //启动总线
    CH455_I2c_WrByte(((u8)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
    CH455_I2c_WrByte((u8)cmd);          //发送数据
    CH455_I2c_Stop();                   //结束总线
}
void nixieTube4P_init(void)
{
    static u8 onceThread = 0;
    if(onceThread)return;else onceThread=1;
    I2_SCL_D_OUT;
    I2_SDA_D_OUT;
    CH455_Write(CH455_SYSON_8 );       //八段式
    LOG_D("nixieTube4P_init");
}
static void nixieTube4P(void *parameter)
{
    u8 time = 0;
    nixieTube4P_init();
    CH455_Write(CH455_SYSON | CH455_BIT_INTENS1);         //抗干扰，定时刷新CH455寄存器
    while(1)
    {
        do
        {
            CH455_Write( CH455_DIG0 | num[time] );
            CH455_Write( CH455_DIG1 | num[time] );
            CH455_Write( CH455_DIG2 | num[time] );
            CH455_Write( CH455_DIG3 | num[time] );
        }while(time++<15);
        time = 0;
    }
    return;
}

static rt_thread_t nixieTube4P_tid = RT_NULL;
static void nixieTube4P_test(int argc, char *argv[])
{
        /* 创建线程1，名称是thread1，入口是thread1_entry*/
    nixieTube4P_tid = rt_thread_create("nixieTube4P",
                            nixieTube4P, RT_NULL,
                            1024,
                            10, 10);

    /* 如果获得线程控制块，启动这个线程 */
    if (nixieTube4P_tid != RT_NULL)
        rt_thread_startup(nixieTube4P_tid);
     return;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(nixieTube4P_test, i2c nixieTube4P sample);
