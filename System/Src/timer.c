/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:40:46
 * @LastEditTime: 2020-04-24 17:26:59
 */
#include "core.h"

bit timer0_flag = 0;
unsigned int timer0_count = 0, timing_time = 100;

/**
 * @brief: 用于ws2812的延时（微秒级）
 * @note: none
 * @param us
 * @retval: none
 */
void dev_ws_delayus(unsigned int us)
{
    unsigned int timer_count;
    for (timer_count = us; timer_count > 0; timer_count--)
    {
        _nop_();
        _nop_();
    }
}

/**
 * @brief: 用于ws2812的延时（毫秒级）
 * @note: none
 * @param ms
 * @retval: none
 */
void dev_ws_delayms(unsigned int ms)
{
    unsigned int x, y;
    for (x = ms; x > 0; x--)
        for (y = 920; y > 0; y--);
}

/**
 * @brief: 定时器0初始化
 * @note: none
 * @param void
 * @retval: none
 */
void timer0_init(void)
{
    AUXR |= 0x80;       //定时器0为1T模式
    // AUXR &= 0x7f;       //定时器0为12T模式
    TMOD = 0x00;        //设置定时器为模式0(16位自动重装载)
    TL0 = T1MS_1T;      //初始化计时值
    TH0 = T1MS_1T >> 8;
    TR0 = 1;            //定时器0开始计时
    ET0 = 1;            //使能定时器0中断
    EA = 1;             //开启中断
}

/**
 * @brief: 定时器0中断函数
 * @note: none
 * @param void
 * @retval: none
 */
void timer0_IRQ_handle(void) interrupt 1 using 1
{
    TF0 = 0;
    timer0_count++;

    if (timer0_count % timing_time == 0)    //改变timing_time可以调节变化速度
    {
        timer0_flag = 1;
    }
    if (timer0_count == 60000)
    {
        timer0_count = 0;
    }
}