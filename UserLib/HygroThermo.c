/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-07     Sinnoer       the first version
 */
#include "include/HygroThermo.h"
u16 temperature = 0,humidity = 0;

u8 DHT11_Read_Data(u16 *temp,u16 *humi)
{
    u8 buf[5];
    u8 res=0,retry=0;
    memset(buf,0,5);
    DHT11_IO_TOOUT;
    DHT11_DQ_OUT_L;
    rt_thread_mdelay(20);
    DHT11_DQ_OUT_H;
    rt_hw_us_delay(13);
    DHT11_IO_TOIN;
    while (DHT11_DQ_IN&&retry<100)
    {
        retry++;
        rt_hw_us_delay(1);
    }
    if(retry>=100)return 1;
    else retry=0;
    while (!DHT11_DQ_IN&&retry<100)
    {
        retry++;
        rt_hw_us_delay(1);
    }
    if(retry>=100)return 1;
    for(int i=0;i<5;i++)
    {
        for (int j=0;j<8;j++)
        {
            buf[i]<<=1;
            retry =0;
            while(DHT11_DQ_IN&&retry<100)
            {
                retry++;
                rt_hw_us_delay(1);
            }
            if(retry>=100)return 1;
            retry=0;
            while(!DHT11_DQ_IN&&retry<100)
            {
                retry++;
                rt_hw_us_delay(1);
            }
            if(retry>=100)return 1;
            rt_hw_us_delay(33);
            if(DHT11_DQ_IN)res = 1;
            else res = 0;
            buf[i] |= res;
        }
    }
//    LOG_D("HygroThermo buf[0]%d\n buf[1]%d\n buf[2]%d\n buf[3]%d\n buf[4]%d ",buf[0],buf[1],buf[2],buf[3],buf[4]);
    if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
    {
        *humi=buf[0]<<8;
        *humi|=buf[1];
        *temp=buf[2]<<8;
        *temp|=buf[3];
    }
    rt_thread_delay(60);    //结束信号
    return 0;
}
void hygroThermo_Run(void* parameter)
{
    u8 flag =0;
    nixieTube4P_init();
    rt_thread_delay(1000);    //结束信号
    CH455_Write( CH455_SYSON | CH455_BIT_INTENS1 );         //抗干扰，定时刷新CH455寄存器
    while(1)
    {
        if(DHT11_Read_Data(&temperature,&humidity))
        {   //Erro
            CH455_Write( CH455_DIG0 | 0x7B );//E
            CH455_Write( CH455_DIG1 | 0x70 );//r
            CH455_Write( CH455_DIG2 | 0x70 );//r
            CH455_Write( CH455_DIG3 | 0x5C );//o
        }
        else if(flag)
        {
            CH455_Write( CH455_DIG0 | num[((humidity&0xFF00)>>8)/10] );
            CH455_Write( CH455_DIG1 | num[((humidity&0xFF00)>>8)%10] | 0x80);
            CH455_Write( CH455_DIG2 | num[(humidity&0xFF)/10] );
            CH455_Write( CH455_DIG3 | 0x77 );//r 湿度单位符号
        }
        else
        {
            CH455_Write( CH455_DIG0 | num[((temperature&0xFF00)>>8)/10] );
            CH455_Write( CH455_DIG1 | num[((temperature&0xFF00)>>8)%10] | 0x80);
            CH455_Write( CH455_DIG2 | num[(temperature&0xFF)/10] );
            CH455_Write( CH455_DIG3 | 0x39 );//C 温度单位符号
        }
        flag = ~flag;
        rt_thread_mdelay(1000);
    }
}
static rt_thread_t tid_hygroThermo = RT_NULL;
void hygroThermo(void)
{
    static u8 onceThread = 0;
    if(onceThread)return;else onceThread=1;
    /* 创建线程1，名称是thread1，入口是thread1_entry*/
    tid_hygroThermo = rt_thread_create("hygroThermo_Run", hygroThermo_Run, RT_NULL, 1024, 10, 10);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid_hygroThermo != RT_NULL)
        {rt_thread_startup(tid_hygroThermo);LOG_D("hygroThermo thread_startup");}
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(hygroThermo, hygroThermo sample);

