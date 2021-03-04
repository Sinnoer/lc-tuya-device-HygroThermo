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
/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static QUEUE que;

rt_device_t usart1_serial;

void queue_reset()
{
    que._head = que._tail = 0;
}
//ѹջ
void queue_push(qdata _data)
{
    qsize pos = (que._head+1)%QUEUE_MAX_SIZE;
    if(pos!=que._tail)
    {
        que._data[que._head] = _data;
        que._head = pos;
    }
}
//��ջ
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
/* ������ msh �����б��� */
MSH_CMD_EXPORT(tuya_sample, tuya_sample device sample);

/* �������ݻص����� */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}
#include "wifi.h"
static void usart1_serial_thread_entry(void *parameter)
{
    char ch;
    while (1)
    {
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(usart1_serial, -1, &ch, 1) != 1)
        {
            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ�����ڴ�λ��� */
//        queue_push(ch);
        uart_receive_input(ch);
    }
}
static int uart_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* ��ʼ�����ò��� */
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
    /* ����ϵͳ�еĴ����豸 */
    usart1_serial = rt_device_find(uart_name);
    if (!usart1_serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }
    /* step2���޸Ĵ������ò��� */
    config.baud_rate = BAUD_RATE_9600;        //�޸Ĳ�����Ϊ 9600
    config.data_bits = DATA_BITS_8;           //����λ 8
    config.stop_bits = STOP_BITS_1;           //ֹͣλ 1
    config.bufsz     = 128;                   //�޸Ļ����� buff size Ϊ 128
    config.parity    = PARITY_NONE;           //����żУ��λ
    /* step3�����ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
    if(rt_device_control(usart1_serial, RT_DEVICE_CTRL_CONFIG, &config) == RT_EOK)
        rt_kprintf("rt_device_control OK!\n", uart_name);
    else rt_kprintf("rt_device_control failed!\n", uart_name);
    /* ��ʼ���ź��� */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    if(rt_device_open(usart1_serial, RT_DEVICE_FLAG_INT_RX) == RT_EOK)
        rt_kprintf("rt_device_open OK!\n", uart_name);
    else rt_kprintf("rt_device_open failed!\n", uart_name);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(usart1_serial, uart_input);
    /* �����ַ��� */
//    rt_device_write(usart1_serial, 0, str, (sizeof(str) - 1));
    /* ���� usart1_serial �߳� */
    rt_thread_t thread = rt_thread_create("usart1_serial", usart1_serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* �����ɹ��������߳� */
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
/* ������ msh �����б��� */
MSH_CMD_EXPORT(uart_sample, uart device sample);
