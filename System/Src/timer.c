/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-10 14:25:47
 * @LastEditTime: 2020-04-10 17:20:48
 */

uint MS1;

void delayus(uint us) //     延时n个1us    @12M晶振  STC15F104W芯片
{
    uint us2;
    for (us2 = us; us2 > 0; us2--)
    {
        _nop_();
        _nop_();
    }
}

void timer0_init(void)      //定时器0初始化
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

void timer0_IRQ_handler(void) interrupt 1 using 1 //---定时器t0-----
{
    TF0 = 0;
    MS1++;
    if (MS1 == color_change_speed) //改变该值可以调节变化速度
    {
        MS1 = 0;
        send = 1;
    }
}
