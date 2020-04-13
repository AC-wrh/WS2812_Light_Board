/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:40:46
 * @LastEditTime: 2020-04-13 15:09:57
 */
#include "core.h"

unsigned char timer0_flag = 0;
unsigned int timer0_count = 0, timing_time = 100;

void dev_ws_delayus(unsigned int us)
{
    unsigned int timer_count;
    for (timer_count = us; timer_count > 0; timer_count--)
    {
        _nop_();
        _nop_();
    }
}

void timer0_init(void) //中断初始化
{
    AUXR |= 0x80; //定时器0为1T模式
    // AUXR &= 0x7f;       //定时器0为12T模式
    TMOD = 0x00;   //设置定时器为模式0(16位自动重装载)
    TL0 = T1MS_1T; //初始化计时值
    TH0 = T1MS_1T >> 8;
    TR0 = 1; //定时器0开始计时
    ET0 = 1; //使能定时器0中断
    EA = 1;  //开启中断
}

void timer0_IRQ_handle(void) interrupt 1 using 1 //---定时器t0-----
{
    static unsigned char flag = 0;
    TF0 = 0;
    timer0_count++;
    // if (timer0_count == timing_time) //改变该值可以调节变化速度
    // {
    //     timer0_count = 0;
    //     timer0_flag = 1;
    // }
    /***************************test***************************/
    if (timer0_count % timing_time == 0) //改变该值可以调节变化速度
    {
        // timer0_count = 0;
        timer0_flag = 1;
    }
    if (timer0_count % 10000 == 0) //改变该值可以调节变化速度
    {
        if (++flag == 3)
        {
            flag = 0;
            if (++ws_mode == 3)
            {
                ws_mode = 0;
            }
        }
    }
    if (timer0_count % 60000 == 0)
    {
        timer0_count = 0;
    }
    /***************************test***************************/
}