/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:41:33
 * @LastEditTime: 2020-04-11 17:49:22
 */
#include "core.h"

unsigned char ws_dat_r = 0; //颜色变量0-255 就是FF以内
unsigned char ws_dat_g = 0;
unsigned char ws_dat_b = 0;
unsigned char ws_dat_buf_r[WS2812_QUANTITY];
unsigned char ws_dat_buf_g[WS2812_QUANTITY];
unsigned char ws_dat_buf_b[WS2812_QUANTITY];

void dev_ws_send_bit1() //H=0.7us.L=0.6us  数据1的表示   芯片STC15F104W  @ 12MHZ
{//1US= 4个 _nop_();  进程序要用掉2个_nop_(); 出程序用掉2个_nop_();
    sda = 1;
    _nop_();
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
}

void dev_ws_send_bit0() //H=0.35us.L=0.8us  ==数据0的表示
{
    sda = 1;
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
    _nop_();
}


void dev_ws_send_single_data(unsigned int dat)     //发送8位，=1颗灯的一种颜色，WS2812灯数据为24一串24位二进制数–每种颜色8位二进制数。
{
    unsigned char count1; //位计数器，单字节为8位
    for (count1 = 8; count1 > 0; count1--)
    {
        if (dat & 0x80) //取高位先发，则dat & 0x80
            dev_ws_send_bit1();
        else
            dev_ws_send_bit0();
        dat <<= 1; //数据右移，下一个位的数据,若高位先发则dat<<=1
    }
}

void dev_ws_send_data(unsigned char len, unsigned char *r, unsigned char *g, unsigned char *b) // 全部发送同样的数据
{
    unsigned char i;

    for (i = 0; i < len; i++)
    {
        dev_ws_send_single_data(*r);
        dev_ws_send_single_data(*g);
        dev_ws_send_single_data(*b);
        r++;
        g++;
        b++;
    }
    sda = 0;
    dev_ws_delayus(70); // 高于50US的低电平信号=复位，
}









// LED cycle gradient
// R = FF           G = 0 -> FF         B = 0           yellow  #FFFF00
// R = FF -> 0      G = FF              B = 0           green   #00FF00
// R = 0            G = FF              B = 0 -> FF     cyan    #00FFFF
// R = 0            G = FF -> 0         B = FF          blue    #0000FF
// R = 0 -> FF      G = 0               B = FF          purple  #FF00FF
// R = FF           G = 0               B = FF -> 0     red     #FF0000

void get_chase_data(unsigned char chase_quantity, unsigned char chase_interval, unsigned char *chase_data)  //single LED chase
{
    unsigned char num;

    for (num = 0; num < chase_quantity; num++)
    {
        chase_data[num] = WS2812_QUANTITY + num * (chase_interval + 1);
    }
    
}

void color_cycle_gradient(unsigned char interval) 
{
    static unsigned char cycle_gradient_flag = 0;

    switch (cycle_gradient_flag)
    {
    case 0: // R = 00 -> FF, G = 0, B = 0   #000000 -> #FF0000
        ws_dat_r += interval;
        
        if (ws_dat_r == COLOR_MAX)
        {
            cycle_gradient_flag = 1;
        }
        break;

    case 1: // R = FF, G = 0 -> FF, B = 0   #FF0000 -> #FFFF00
        ws_dat_g += interval;
        if (ws_dat_g == COLOR_MAX)
        {
            cycle_gradient_flag = 2;
        }
        break;

    case 2: // R = FF -> 0, G = FF, B = 0   #FFFF00 -> #00FF00
        ws_dat_r -= interval;
        if (ws_dat_r == COLOR_MIN)
        {
            cycle_gradient_flag = 3;
        }
        break;

    case 3: // R = 0, G = FF, B = 0 -> FF   #00FF00 -> #00FFFF
        ws_dat_b += interval;
        if (ws_dat_b == COLOR_MAX)
        {
            cycle_gradient_flag = 4;
        }
        break;

    case 4: // R = 0, G = FF -> 0, B = FF   #00FFFF -> #0000FF
        ws_dat_g -= interval;
        if (ws_dat_g == COLOR_MIN)
        {
            cycle_gradient_flag = 5;
        }
        break;

    case 5: // R = 0 -> FF, G = 0, B = FF   #0000FF -> #FF00FF
        ws_dat_r += interval;
        if (ws_dat_r == COLOR_MAX)
        {
            cycle_gradient_flag = 6;
        }
        break;

    case 6: // R = FF, G = 0, B = 0 -> FF   #FF00FF -> #FF0000
        ws_dat_b -= interval;
        if (ws_dat_b == COLOR_MIN)
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
    unsigned char count, chase_data[CHASE_QUANTITY];
    static unsigned char chase_count = 0;

    timing_time = speed;

    switch (mode)
    {
    case MODE_GRADIENT:         //timer 25
        for (count = 0; count < WS2812_QUANTITY - 1; count++)
        {
            ws_dat_buf_r[count + 1] = ws_dat_buf_r[count];
            ws_dat_buf_g[count + 1] = ws_dat_buf_g[count];
            ws_dat_buf_b[count + 1] = ws_dat_buf_b[count];
        }
        color_cycle_gradient(interval);
        ws_dat_buf_r[0] = ws_dat_r;
        ws_dat_buf_g[0] = ws_dat_g;
        ws_dat_buf_b[0] = ws_dat_b;
        break;
    
    case MODE_BOTH_GRADIENT:    //timer 25
        for (count = 0; count < (WS2812_QUANTITY - 1) / 2; count++)
        {
            ws_dat_buf_r[count + 1] = ws_dat_buf_r[count];
            ws_dat_buf_g[count + 1] = ws_dat_buf_g[count];
            ws_dat_buf_b[count + 1] = ws_dat_buf_b[count];

            ws_dat_buf_r[23 - (count + 1)] = ws_dat_buf_r[23 - count];
            ws_dat_buf_g[23 - (count + 1)] = ws_dat_buf_g[23 - count];
            ws_dat_buf_b[23 - (count + 1)] = ws_dat_buf_b[23 - count];
        }
        color_cycle_gradient(interval);
        ws_dat_buf_r[0] = ws_dat_r;
        ws_dat_buf_g[0] = ws_dat_g;
        ws_dat_buf_b[0] = ws_dat_b;

        ws_dat_buf_r[23] = ws_dat_r;
        ws_dat_buf_g[23] = ws_dat_g;
        ws_dat_buf_b[23] = ws_dat_b;
        break;

    case MODE_CHASE:    //23 + (quantity - number) * (interval - 1);
        // get_chase_data(CHASE_QUANTITY, CHASE_INTERVAL, chase_data);
        for (count = 0; count < WS2812_QUANTITY - 1; count++)
        {
            ws_dat_buf_r[count] = 0;
            ws_dat_buf_g[count] = 0;
            ws_dat_buf_b[count] = 0;
        }
        color_cycle_gradient(interval);
        
        if (chase_count <= 23)
        {
            ws_dat_buf_r[chase_count] = ws_dat_r;
            ws_dat_buf_g[chase_count] = ws_dat_g;
            ws_dat_buf_b[chase_count] = ws_dat_b;
        }
        
        if (chase_count > 1 && chase_count <= 25)
        {
            ws_dat_buf_r[chase_count - 2] = ws_dat_b;
            ws_dat_buf_g[chase_count - 2] = ws_dat_r;
            ws_dat_buf_b[chase_count - 2] = ws_dat_g;
        }

        if (chase_count > 3 && chase_count <= 27)
        {
            ws_dat_buf_r[chase_count - 4] = ws_dat_g;
            ws_dat_buf_g[chase_count - 4] = ws_dat_b;
            ws_dat_buf_b[chase_count - 4] = ws_dat_r;
        }

        if (++chase_count == 28)
        {
            chase_count = 0;
        }
        
        break;
    
    default:
        break;
    }

    dev_ws_send_data(WS2812_QUANTITY, ws_dat_buf_r, ws_dat_buf_g, ws_dat_buf_b);
}

