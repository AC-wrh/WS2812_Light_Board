/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-10 14:25:47
 * @LastEditTime: 2020-04-10 14:39:22
 */

void Timer0_Init(void)
{
    AUXR |= 0x80;           //定时器0为1T模式
    // AUXR &= 0x7f;           //定时器0为12T模式

    TMOD = 0x00;            //设置定时器为模式0(16位自动重装载)
    TL0 = T1MS_1T;          //初始化计时值
    TH0 = T1MS_1T >> 8;
    TR0 = 1;                //定时器0开始计时
    ET0 = 1;                //使能定时器0中断
    EA  = 1;                //开启总中断
}

void Timer0_IRQHandler(void) interrupt 1 using 1 //---定时器t0-----
{
    TF0 = 0;
    MS1++;
    if (MS1 == color_change_speed) //改变该值可以调节变化速度
    {
        MS1 = 0;
        send = 1;
    }
}