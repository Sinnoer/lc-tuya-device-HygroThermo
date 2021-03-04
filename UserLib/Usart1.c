/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-10     Sinnoer       the first version
 */
#include "include/Usart1.h"
/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static QUEUE que;

rt_device_t usart1_serial;

void queue_reset()
{
    que._head = que._tail = 0;
}
//压栈
void queue_push(qdata _data)
{
    qsize pos = (que._head+1)%QUEUE_MAX_SIZE;
    if(pos!=que._tail)
    {
        que._data[que._head] = _data;
        que._head = pos;
    }
}
//出栈
static void queue_pop(qdata* _data)
{
    if(que._tail!=que._head)
    {
        *_data = que._data[que._tail];
        que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
    }
}
static qsize queue_size()
{
    return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}
u8 CheckCRC16(qdata *buffer, qsize buf_len)
{
    u32 rcr = 0;
    for(int i=0; i < buf_len; i++)
        rcr += buffer[i];
    if((rcr&0xFF) == buffer[buf_len])return 1;
    else return 0;
}
qsize queue_find_cmd(qdata *buffer,qsize buf_len)
{
    u16 _frame_leng =0;
    u16 cmd_state = 0;
    qsize cmd_pos = 0;
    qdata _data = 0;
    buffer[0] = 0x55;
    while(queue_size()>0)
    {
        queue_pop(&_data);
        if(cmd_pos==0)cmd_state = ((cmd_state<<8)|_data);
        if(cmd_pos==0&&cmd_state!=CMD_HEAD)
        {
            continue;
        }else cmd_pos++;
        if(cmd_pos<buf_len)
            buffer[cmd_pos] = _data;
//        LOG_D("usart1_serial_thread_entry: %x \n",_data);
        if(cmd_pos==5)_frame_leng = (buffer[4]<<8|buffer[5])+6;
        if(cmd_pos==_frame_leng)
        {
            if(CheckCRC16(buffer,cmd_pos))
            {
                buffer[++cmd_pos] = '\0';
                return cmd_pos;
            }
            _frame_leng =0;
            cmd_state = 0;
            cmd_pos = 0;
            _data = 0;
         }
    }
    return 0;
}
static void tuya_sample(int argc, char *argv[])
{
    qdata cmd[20];
    qsize leng = queue_find_cmd(cmd,20);
    if(leng)
        for(int i=0; i < leng; i++)
            rt_kprintf("%x ", cmd[i]);
    else rt_kprintf("tuya_sample find cmd failed!");
    rt_kprintf("\n");
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(tuya_sample, tuya_sample device sample);

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}
#include "wifi.h"
static void usart1_serial_thread_entry(void *parameter)
{
    char ch;
    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(usart1_serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* 读取到的数据通过串口错位输出 */
//        queue_push(ch);
        uart_receive_input(ch);
    }
}
static int uart_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */
    static u8 onceThread = 0;
    if(onceThread)return ret;else onceThread=1;
    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }
    /* 查找系统中的串口设备 */
    usart1_serial = rt_device_find(uart_name);
    if (!usart1_serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }
    /* step2：修改串口配置参数 */
    config.baud_rate = BAUD_RATE_9600;        //修改波特率为 9600
    config.data_bits = DATA_BITS_8;           //数据位 8
    config.stop_bits = STOP_BITS_1;           //停止位 1
    config.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    config.parity    = PARITY_NONE;           //无奇偶校验位
    /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    if(rt_device_control(usart1_serial, RT_DEVICE_CTRL_CONFIG, &config) == RT_EOK)
        rt_kprintf("rt_device_control OK!\n", uart_name);
    else rt_kprintf("rt_device_control failed!\n", uart_name);
    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    if(rt_device_open(usart1_serial, RT_DEVICE_FLAG_INT_RX) == RT_EOK)
        rt_kprintf("rt_device_open OK!\n", uart_name);
    else rt_kprintf("rt_device_open failed!\n", uart_name);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(usart1_serial, uart_input);
    /* 发送字符串 */
//    rt_device_write(usart1_serial, 0, str, (sizeof(str) - 1));
    /* 创建 usart1_serial 线程 */
    rt_thread_t thread = rt_thread_create("usart1_serial", usart1_serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }
    return ret;
}
int uart1_start(void)
{
    return uart_sample(0,0);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(uart_sample, uart device sample);
