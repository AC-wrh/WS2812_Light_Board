/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-10 14:25:05
 * @LastEditTime: 2020-04-10 17:59:48
 */
#include "main.h"

dev_color_t ws_color_ctl;
uint8 color_change_speed = 75;

void ws_send_high_level() //H=0.7us.L=0.6us       数据1的表示
{   //1US= 4个 _nop_();  进程序要用掉2个_nop_(); 出程序用掉2个_nop_();
    sda = 1;
    _nop_();
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
}

void ws_send_low_level() //H=0.35us.L=0.8us      数据0的表示
{
    sda = 1;
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
    _nop_();
}

void ws_send_sda(uint32 dat)     //发送8位数据
{
    uint8 count1; //位计数器，单字节为8位
    for (count1 = 8; count1 > 0; count1--)
    {
        if (dat & 0x80) //取高位先发，则dat & 0x80
            send_high_level();
        else
            send_low_level();
        dat <<= 1; //数据右移，下一个位的数据,若高位先发则dat<<=1
    }
}

void ws_send_updata(uint8 len, uint8 *r, uint8 *g, uint8 *b) // 全部发送同样的数据
{
    uint8 i;

    for (i = 0; i < len; i++)
    {
        send_sda(*r);
        send_sda(*g);
        send_sda(*b); 
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
void color_cycle_gradient(uint8 interval)
{
    static uint8 cycle_gradient_flag = 0;

    switch (cycle_gradient_flag)
    {
    case 0: // R = 00 -> FF, G = 0, B = 0   #000000 -> #FF0000
        r_dat += interval;
        if (r_dat == COLOR_MAX)
        {
            cycle_gradient_flag = 1;
        }
        break;

    case 1: // R = FF, G = 0 -> FF, B = 0   #FF0000 -> #FFFF00
        g_dat += interval;
        if (g_dat == COLOR_MAX)
        {
            cycle_gradient_flag = 2;
        }
        break;

    case 2: // R = FF -> 0, G = FF, B = 0   #FFFF00 -> #00FF00
        r_dat -= interval;
        if (r_dat == COLOR_MIN)
        {
            cycle_gradient_flag = 3;
        }
        break;

    case 3: // R = 0, G = FF, B = 0 -> FF   #00FF00 -> #00FFFF
        b_dat += interval;
        if (b_dat == COLOR_MAX)
        {
            cycle_gradient_flag = 4;
        }
        break;

    case 4: // R = 0, G = FF -> 0, B = FF   #00FFFF -> #0000FF
        g_dat -= interval;
        if (g_dat == COLOR_MIN)
        {
            cycle_gradient_flag = 5;
        }
        break;

    case 5: // R = 0 -> FF, G = 0, B = FF   #0000FF -> #FF00FF
        r_dat += interval;
        if (r_dat == COLOR_MAX)
        {
            cycle_gradient_flag = 6;
        }
        break;

    case 6: // R = FF, G = 0, B = 0 -> FF   #FF00FF -> #FF0000
        b_dat -= interval;
        if (b_dat == COLOR_MIN)
        {
            cycle_gradient_flag = 1;
        }
        break;

    default:
        break;
    }
}

void PWM_Dimming(uint8 mode, uint8 interval, uint8 speed)
{
    uint8 count;

    color_change_speed = speed;

    if (mode) //中间往两边渐变
    {
        for (count = 0; count < (DENGSHU - 1) / 2; count++)
        {
            ws_dat_r[count + 1] = ws_dat_r[count];
            ws_dat_g[count + 1] = ws_dat_g[count];
            ws_dat_b[count + 1] = ws_dat_b[count];

            ws_dat_r[23 - (count + 1)] = ws_dat_r[23 - count];
            ws_dat_g[23 - (count + 1)] = ws_dat_g[23 - count];
            ws_dat_b[23 - (count + 1)] = ws_dat_b[23 - count];
        }
        color_cycle_gradient(interval);
        ws_dat_r[0] = r_dat;
        ws_dat_g[0] = g_dat;
        ws_dat_b[0] = b_dat;

        ws_dat_r[23] = r_dat;
        ws_dat_g[23] = g_dat;
        ws_dat_b[23] = b_dat;
    }
    else //流水灯渐变
    {
        for (count = 0; count < DENGSHU - 1; count++)
        {
            ws_dat_r[count + 1] = ws_dat_r[count];
            ws_dat_g[count + 1] = ws_dat_g[count];
            ws_dat_b[count + 1] = ws_dat_b[count];
        }
        color_cycle_gradient(interval);
        ws_dat_r[0] = r_dat;
        ws_dat_g[0] = g_dat;
        ws_dat_b[0] = b_dat;
    }

    WS_Set_Date(DENGSHU, ws_dat_r, ws_dat_g, ws_dat_b);
}
