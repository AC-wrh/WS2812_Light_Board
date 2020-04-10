/*
 * @Description: Control WS2812 through STC15W204S
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-09 16:25:14
 * @LastEditTime: 2020-04-10 14:27:05
 */

#include "main.h"

int main(void)
{
    
    
    return 0;
}


#define FOSC 11059200L
#define T1MS (65536 - FOSC / 1000) //1T模式  1ms
//#define T1MS (65536-FOSC/12/1000) //12T模式
#define DENGSHU 24
/*
,
全彩 LED ws2812  灯 驱动程序，受到论坛 zhuls 的启发，
实现了在我的8脚芯片，STC15F104W芯片上的驱动，、
修改内部晶振 @ 12MHZ， STC Y5内核，一个  _nop_();  为 0.25 us时间为基准来驱动WS2812
此程序实验完美驱动WS2812，无闪烁，无跳灯，
，
接下来想用很长一个WS2812灯带，摆成88：88这样一个数码管形状，
然后用单片机一个IO口驱动，实现全彩数字钟，（多功能），比如说重要定时时间到了，狂闪+高亮红色，RR=0xFF;;这样
或者睡觉前，以渐色呼吸灯的方式，直到深夜，亮度自动变暗，RR=0x01;， GG=0x01; BB=0x01;
,
*/

sfr P3M1 = 0x80; //  10 000000   全部设置为双向IO口，弱上拉25K
sfr P3M0 = 0x80; //  10 000000
sfr AUXR = 0x8e; //Auxiliary register

sbit sda = P3 ^ 2;  // 8
sbit KEY1 = P3 ^ 3; // 1

uint MS1;
uchar i;
uchar send;
uchar WsDat_RUN4_R = 0; //颜色变量0-255 就是FF以内
uchar WsDat_RUN4_G = 0;
uchar WsDat_RUN4_B = 0;
uchar WsDat_R[DENGSHU];
uchar WsDat_G[DENGSHU];
uchar WsDat_B[DENGSHU];
unsigned char color_change_speed = 75;
//=============================================

void delayus(uint us) //     延时n个1us    @12M晶振  STC15F104W芯片
{
    uint us2;
    for (us2 = us; us2 > 0; us2--)
    {
        _nop_();
        _nop_();
    }
}

//==================================================延时代码 12mHZ
// void delayms(uint z) //X  MS延时==@ 12MHZ
// {
//     uint x, y;
//     for (x = z; x > 0; x--)
//         for (y = 920; y > 0; y--)
//             ;
// }
//=========================================================================

void bit1() //H=0.7us.L=0.6us  数据1的表示   芯片STC15F104W  @ 12MHZ

//1US= 4个 _nop_();  进程序要用掉2个_nop_(); 出程序用掉2个_nop_();
{
    sda = 1;
    _nop_();
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
}

void bit0() //H=0.35us.L=0.8us  ==数据0的表示
{
    sda = 1;
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
    _nop_();
}

//发送8位，=1颗灯的一种颜色，WS2812灯数据为24一串24位二进制数–每种颜色8位二进制数。先是绿色，然后是红色，最后是蓝色
void send_sda(uint dat)

{
    uchar count1; //位计数器，单字节为8位
    for (count1 = 8; count1 > 0; count1--)
    {
        if (dat & 0x80) //取高位先发，则dat & 0x80
            bit1();
        else
            bit0();
        dat <<= 1; //数据右移，下一个位的数据,若高位先发则dat<<=1
    }
}
//=============================================
void Init(void) //中断初始化
{
    AUXR |= 0x80; //定时器0为1T模式
                  //  AUXR &= 0x7f;                   //定时器0为12T模式
    TMOD = 0x00;  //设置定时器为模式0(16位自动重装载)
    TL0 = T1MS;   //初始化计时值
    TH0 = T1MS >> 8;
    TR0 = 1; //定时器0开始计时
    ET0 = 1; //使能定时器0中断
    EA = 1;
}

void WS_Set_Date(unsigned char len, unsigned char *r, unsigned char *g, unsigned char *b) // 全部发送同样的数据
{
    unsigned char i;
    //unsigned long dat;
    //dat = (r<<16) | (g<<8) | b;
    for (i = 0; i < len; i++)
    {
        send_sda(*r); //
        send_sda(*g); //
        send_sda(*b); //
        r++;
        g++;
        b++;
    }
    sda = 0;
    delayus(70); // 高于50US的低电平信号=复位，
}

// LED cycle gradient
// R = FF           G = 0 -> FF         B = 0           yellow  #FFFF00
// R = FF -> 0      G = FF              B = 0           green   #00FF00
// R = 0            G = FF              B = 0 -> FF     cyan    #00FFFF
// R = 0            G = FF -> 0         B = FF          blue    #0000FF
// R = 0 -> FF      G = 0               B = FF          purple  #FF00FF
// R = FF           G = 0               B = FF -> 0     red     #FF0000

#define COLOR_CHANGE_INTERVAL_VERY_SMALL        1
#define COLOR_CHANGE_INTERVAL_SMALL             3
#define COLOR_CHANGE_INTERVAL_NORMAL            5
#define COLOR_CHANGE_INTERVAL_BIG               17
#define COLOR_CHANGE_INTERVAL_VERY_BIG          51

#define COLOR_MIN       0
#define COLOR_MAX       255

void color_cycle_gradient(unsigned char interval) 
{
    static unsigned char cycle_gradient_flag = 0;

    switch (cycle_gradient_flag)
    {
    case 0: // R = 00 -> FF, G = 0, B = 0   #000000 -> #FF0000
        WsDat_RUN4_R += interval;
        // WsDat_RUN4_G += interval;
        // WsDat_RUN4_B += interval;
        // WsDat_RUN4_R = 255;
        if (WsDat_RUN4_R == COLOR_MAX)
        {
            cycle_gradient_flag = 1;
        }
        break;

    case 1: // R = FF, G = 0 -> FF, B = 0   #FF0000 -> #FFFF00
        WsDat_RUN4_G += interval;
        if (WsDat_RUN4_G == COLOR_MAX)
        {
            cycle_gradient_flag = 2;
        }
        break;

    case 2: // R = FF -> 0, G = FF, B = 0   #FFFF00 -> #00FF00
        WsDat_RUN4_R -= interval;
        if (WsDat_RUN4_R == COLOR_MIN)
        {
            cycle_gradient_flag = 3;
        }
        break;

    case 3: // R = 0, G = FF, B = 0 -> FF   #00FF00 -> #00FFFF
        WsDat_RUN4_B += interval;
        if (WsDat_RUN4_B == COLOR_MAX)
        {
            cycle_gradient_flag = 4;
        }
        break;

    case 4: // R = 0, G = FF -> 0, B = FF   #00FFFF -> #0000FF
        WsDat_RUN4_G -= interval;
        if (WsDat_RUN4_G == COLOR_MIN)
        {
            cycle_gradient_flag = 5;
        }
        break;

    case 5: // R = 0 -> FF, G = 0, B = FF   #0000FF -> #FF00FF
        WsDat_RUN4_R += interval;
        if (WsDat_RUN4_R == COLOR_MAX)
        {
            cycle_gradient_flag = 6;
        }
        break;

    case 6: // R = FF, G = 0, B = 0 -> FF   #FF00FF -> #FF0000
        WsDat_RUN4_B -= interval;
        if (WsDat_RUN4_B == COLOR_MIN)
        {
            cycle_gradient_flag = 1;
        }
        break;

    default:
        break;
    }
}

void PWM_Dimming(unsigned char mode, unsigned char interval, unsigned char speed)
{
    unsigned char count;

    color_change_speed = speed;

    if (mode) //中间往两边渐变
    {
        for (count = 0; count < (DENGSHU - 1) / 2; count++)
        {
            WsDat_R[count + 1] = WsDat_R[count];
            WsDat_G[count + 1] = WsDat_G[count];
            WsDat_B[count + 1] = WsDat_B[count];

            WsDat_R[23 - (count + 1)] = WsDat_R[23 - count];
            WsDat_G[23 - (count + 1)] = WsDat_G[23 - count];
            WsDat_B[23 - (count + 1)] = WsDat_B[23 - count];
        }
        color_cycle_gradient(interval);
        WsDat_R[0] = WsDat_RUN4_R;
        WsDat_G[0] = WsDat_RUN4_G;
        WsDat_B[0] = WsDat_RUN4_B;

        WsDat_R[23] = WsDat_RUN4_R;
        WsDat_G[23] = WsDat_RUN4_G;
        WsDat_B[23] = WsDat_RUN4_B;
    }
    else //流水灯渐变
    {
        for (count = 0; count < DENGSHU - 1; count++)
        {
            WsDat_R[count + 1] = WsDat_R[count];
            WsDat_G[count + 1] = WsDat_G[count];
            WsDat_B[count + 1] = WsDat_B[count];
        }
        color_cycle_gradient(interval);
        WsDat_R[0] = WsDat_RUN4_R;
        WsDat_G[0] = WsDat_RUN4_G;
        WsDat_B[0] = WsDat_RUN4_B;
    }

    WS_Set_Date(DENGSHU, WsDat_R, WsDat_G, WsDat_B);
}

//=============================================
void timer0(void) interrupt 1 using 1 //---定时器t0-----
{
    TF0 = 0;
    MS1++;
    if (MS1 == color_change_speed) //改变该值可以调节变化速度
    {
        MS1 = 0;
        send = 1;
    }
}
//=========================================================================

//==========================  主程序开始  ==================================

void main()
{
    //uchar i;
    unsigned char color_mode = 0;

    P3M1 = 0x80;
    P3M0 = 0x80;
    MS1 = 0;
    send = 0;
    WsDat_RUN4_R = 0;
    WsDat_RUN4_G = 0;
    WsDat_RUN4_B = 0;
    Init();
    while (1) //主循环。
    {
        if (send == 1)
        {
            TR0 = 0;
            PWM_Dimming(color_mode, COLOR_CHANGE_INTERVAL_NORMAL, 25);
            send = 0;
            TR0 = 1;
        }
    }
}

//=========================================================================
//=========================      END     ==================================
//=========================================================================
