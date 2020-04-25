/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:41:33
 * @LastEditTime: 2020-04-25 17:57:20
 */
#include "core.h"

unsigned char ws_data_r = 0; //颜色变量0-255 就是FF以内
unsigned char ws_data_g = 0;
unsigned char ws_data_b = 0;
unsigned char ws_data_buf_r[WS2812_QUANTITY];
unsigned char ws_data_buf_g[WS2812_QUANTITY];
unsigned char ws_data_buf_b[WS2812_QUANTITY];
unsigned char ws_rgb_data[3]; // 0 - r, 1 - g, 2 - b
// unsigned char mode_switch_flag = 0;

/**
 * @brief: 发送一位数据 -- 1
 * @note: H=0.7us.L=0.6us，1US= 4个 _nop_(); 进程序要用掉2个_nop_(); 出程序用掉2个_nop_();
 * @param void
 * @retval: none
 */
void ws_send_bit1(void)
{
    sda = 1;
    _nop_();
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
}

/**
 * @brief: 发送一位数据 -- 0
 * @note: H=0.35us.L=0.8us
 * @param void
 * @retval: none
 */
void ws_send_bit0(void)
{
    sda = 1;
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
    _nop_();
}

/**
 * @brief: 发送8位数据
 * @note: none
 * @param dat 颜色数据（8位二进制数）
 * @retval: 
 */
void ws_send_single_data(unsigned int dat) //发送8位，=1颗灯的一种颜色，。
{
    unsigned char bit_count; //位计数器，单字节为8位

    for (bit_count = 8; bit_count > 0; bit_count--)
    {
        if (dat & 0x80) //取高位先发，则dat & 0x80
            ws_send_bit1();
        else
            ws_send_bit0();
        dat <<= 1; //数据右移，下一个位的数据,若高位先发则dat<<=1
    }
}

/**
 * @brief: 发送RGB颜色数据
 * @note: WS2812灯数据为24一串24位二进制数，每种颜色8位二进制数
 * @param len   数据长度
 * @param r     红色数据
 * @param g     绿色数据
 * @param b     蓝色数据
 * @retval: none
 */
void ws_send_data(unsigned char len, unsigned char *r, unsigned char *g, unsigned char *b) // 全部发送同样的数据
{
    unsigned char count;

    for (count = 0; count < len; count++)
    {
        ws_send_single_data(*r);
        ws_send_single_data(*g);
        ws_send_single_data(*b);
        r++;
        g++;
        b++;
    }
    sda = 0;
    dev_ws_delayus(70); // 高于50US的低电平信号=复位，
}

/**
 * @brief: 颜色循环表
 * @note: LED cycle gradient
 *      R = FF          G = 0 -> FF     B = 0              red to yellow    #FF0000 -> #FFFF00
 *      R = FF -> 0     G = FF          B = 0           yellow to green     #FFFF00 -> #00FF00
 *      R = 0           G = FF          B = 0 -> FF      green to cyan      #00FF00 -> #00FFFF
 *      R = 0           G = FF -> 0     B = FF            cyan to blue      #00FFFF -> #0000FF
 *      R = 0 -> FF     G = 0           B = FF            blue to purple    #0000FF -> #FF00FF
 *      R = FF          G = 0           B = FF -> 0     purple to red       #FF00FF -> #FF0000
 * @param interval 颜色变化间隔
 * @retval: none
 */
void ws_color_gradient_cycle_table(unsigned char interval)
{
    static unsigned char gradient_cycle_flag = 0;

    switch (gradient_cycle_flag)
    {
    case 0: // R = 00 -> FF, G = 0, B = 0   #000000 -> #FF0000

        ws_data_r += interval;
        if (ws_data_r == COLOR_MAX)
        {
            gradient_cycle_flag = 1;
        }

        break;

    case 1: // R = FF, G = 0 -> FF, B = 0   #FF0000 -> #FFFF00

        ws_data_g += interval;
        if (ws_data_g == COLOR_MAX)
        {
            gradient_cycle_flag = 2;
        }

        break;

    case 2: // R = FF -> 0, G = FF, B = 0   #FFFF00 -> #00FF00

        ws_data_r -= interval;
        if (ws_data_r == COLOR_MIN)
        {
            gradient_cycle_flag = 3;
        }

        break;

    case 3: // R = 0, G = FF, B = 0 -> FF   #00FF00 -> #00FFFF

        ws_data_b += interval;
        if (ws_data_b == COLOR_MAX)
        {
            gradient_cycle_flag = 4;
        }

        break;

    case 4: // R = 0, G = FF -> 0, B = FF   #00FFFF -> #0000FF

        ws_data_g -= interval;
        if (ws_data_g == COLOR_MIN)
        {
            gradient_cycle_flag = 5;
        }

        break;

    case 5: // R = 0 -> FF, G = 0, B = FF   #0000FF -> #FF00FF

        ws_data_r += interval;
        if (ws_data_r == COLOR_MAX)
        {
            gradient_cycle_flag = 6;
        }

        break;

    case 6: // R = FF, G = 0, B = FF -> 0   #FF00FF -> #FF0000

        ws_data_b -= interval;
        if (ws_data_b == COLOR_MIN)
        {
            gradient_cycle_flag = 1;
            if (++ws_mode == 7)
            {
                ws_mode = 0;
            }
        }

        break;

    default:
        break;
    }

    ws_rgb_data[0] = ws_data_r;
    ws_rgb_data[1] = ws_data_g;
    ws_rgb_data[2] = ws_data_b;
}

void reindex_array(unsigned char mode, unsigned char *arr, unsigned char len, unsigned char num)
{
    unsigned char idata *arr_buf;
    // unsigned char arr_buf;
    unsigned char count;

    switch (mode)
    {
    // case 0: //Rotate left
    //     while (num--)
    //     {
    //         arr_buf = arr[0];
    //         for (count = 0; count < len - 1; count++)
    //         {
    //             arr[count] = arr[count + 1];
    //         }
    //         arr[count] = arr_buf;
    //     }
    //     break;

    // case 1: //Rotate Right
    //     while (num--)
    //     {
    //         arr_buf = arr[len - 1];
    //         for (count = len - 1; count > 0; count--)
    //         {
    //             arr[count] = arr[count - 1];
    //         }
    //         arr[0] = arr_buf;
    //     }
    //     break;

    case 0: //Rotate left

        for (count = 0; count < num; count++)
        {
            *arr_buf++ = arr[count];
        }

        for (count = 0; count < len - num; count++)
        {
            arr[count] = arr[count + num];
        }

        while (num--)
        {
            arr[count + num] = *(--arr_buf);
        }

        break;

    case 1: //Rotate Right

        for (count = len - num; count < len; count++)
        {
            *arr_buf++ = arr[count];
        }

        for (count = len - 1; count > num - 1; count--)
        {
            arr[count] = arr[count - num];
        }

        while (num--)
        {
            arr[count--] = *(--arr_buf);
        }

        break;

    default:
        break;
    }
}

void get_chase_data(unsigned char chase_quantity, unsigned char chase_interval) //single LED chase
{
    // unsigned char chase_data_max[CHASE_QUANTITY] = {0};
    unsigned char num;
    static unsigned char chase_count = 0;

    for (num = 0; num < chase_quantity; num++)
    {
        // chase_data_max[num] = WS2812_QUANTITY + num * (chase_interval + 1);

        // if (chase_count >= (CHASE_INTERVAL + 1) * num && chase_count < chase_data_max[num])
        // {
        //     ws_data_buf_r[chase_count - (CHASE_INTERVAL + 1) * num] = ws_rgb_data[0];
        //     ws_data_buf_g[chase_count - (CHASE_INTERVAL + 1) * num] = ws_rgb_data[1];
        //     ws_data_buf_b[chase_count - (CHASE_INTERVAL + 1) * num] = ws_rgb_data[2];
        // }
        // else
        // {
        //     ws_data_buf_r[(chase_count - (CHASE_INTERVAL + 1) * num) % 23] = ws_rgb_data[0];
        //     ws_data_buf_g[(chase_count - (CHASE_INTERVAL + 1) * num) % 23] = ws_rgb_data[1];
        //     ws_data_buf_b[(chase_count - (CHASE_INTERVAL + 1) * num) % 23] = ws_rgb_data[2];;
        // }

        if (chase_count >= (chase_interval + 1) * num)
        {
            ws_data_buf_r[(chase_count - (chase_interval + 1) * num) % 24] = ws_rgb_data[0];
            ws_data_buf_g[(chase_count - (chase_interval + 1) * num) % 24] = ws_rgb_data[1];
            ws_data_buf_b[(chase_count - (chase_interval + 1) * num) % 24] = ws_rgb_data[2];
            ;
        }
        else
        {
            ;
        }
        reindex_array(1, ws_rgb_data, sizeof(ws_rgb_data), 1); // 将rgb数据进行调换
    }

    if (++chase_count == 255)
    {
        chase_count = 23;
    }
}

void PWM_Dimming(unsigned char mode, unsigned char interval, unsigned int speed)
{
    unsigned char count = 0, cycle_count = 0;
    static unsigned char direction_change_flag = 0, direction_count = 0;
    static unsigned char test_count = 0;
    timing_time = speed;

    switch (mode)
    {
    case MODE_CCW_GRADIENT:

        for (count = WS2812_QUANTITY - 1; count > 0; count--)
        {
            ws_data_buf_r[count] = ws_data_buf_r[count - 1];
            ws_data_buf_g[count] = ws_data_buf_g[count - 1];
            ws_data_buf_b[count] = ws_data_buf_b[count - 1];
        }

        ws_color_gradient_cycle_table(interval);

        ws_data_buf_r[0] = ws_rgb_data[0];
        ws_data_buf_g[0] = ws_rgb_data[1];
        ws_data_buf_b[0] = ws_rgb_data[2];

        break;

    case MODE_BOTH_GRADIENT:

        for (count = (WS2812_QUANTITY - 1) / 2; count > 0; count--)
        {
            ws_data_buf_r[count] = ws_data_buf_r[count - 1];
            ws_data_buf_g[count] = ws_data_buf_g[count - 1];
            ws_data_buf_b[count] = ws_data_buf_b[count - 1];
        }

        for (count = WS2812_QUANTITY / 2; count < WS2812_QUANTITY - 1; count++)
        {
            ws_data_buf_r[count] = ws_data_buf_r[count + 1];
            ws_data_buf_g[count] = ws_data_buf_g[count + 1];
            ws_data_buf_b[count] = ws_data_buf_b[count + 1];
        }

        ws_color_gradient_cycle_table(interval);

        ws_data_buf_r[0] = ws_rgb_data[0];
        ws_data_buf_g[0] = ws_rgb_data[1];
        ws_data_buf_b[0] = ws_rgb_data[2];

        ws_data_buf_r[23] = ws_rgb_data[0];
        ws_data_buf_g[23] = ws_rgb_data[1];
        ws_data_buf_b[23] = ws_rgb_data[2];

        break;

    case MODE_SYMMETRICAL_REBOUND:

        ws_color_gradient_cycle_table(interval);

        if (direction_change_flag)
        {
            ws_data_buf_r[direction_count] = ws_rgb_data[0];
            ws_data_buf_g[direction_count] = ws_rgb_data[1];
            ws_data_buf_b[direction_count] = ws_rgb_data[2];

            ws_data_buf_r[23 - direction_count] = ws_rgb_data[0];
            ws_data_buf_g[23 - direction_count] = ws_rgb_data[1];
            ws_data_buf_b[23 - direction_count] = ws_rgb_data[2];

            if (--direction_count == 0)
            {
                direction_change_flag = 0;
            }
        }
        else
        {
            ws_data_buf_r[direction_count] = ws_rgb_data[0];
            ws_data_buf_g[direction_count] = ws_rgb_data[1];
            ws_data_buf_b[direction_count] = ws_rgb_data[2];

            ws_data_buf_r[23 - direction_count] = ws_rgb_data[0];
            ws_data_buf_g[23 - direction_count] = ws_rgb_data[1];
            ws_data_buf_b[23 - direction_count] = ws_rgb_data[2];

            if (++direction_count == 11)
            {
                direction_change_flag = 1;
            }
        }

        break;

    case MODE_CHASE:

        for (count = 0; count < WS2812_QUANTITY; count++)
        {
            ws_data_buf_r[count] = 0;
            ws_data_buf_g[count] = 0;
            ws_data_buf_b[count] = 0;
        }
        ws_color_gradient_cycle_table(interval);
        get_chase_data(CHASE_QUANTITY, CHASE_INTERVAL);

        break;

    case MODE_WHITE_FLASHING:
        for (cycle_count = 0; cycle_count < interval; cycle_count++)
        {
            if (cycle_count % 2 == 0)
            {
                for (count = 0; count < WS2812_QUANTITY; count++)
                {
                    ws_data_buf_r[count] = 0x4f;
                    ws_data_buf_g[count] = 0x4f;
                    ws_data_buf_b[count] = 0x4f;
                }
            }
            else
            {
                for (count = 0; count < WS2812_QUANTITY; count++)
                {
                    ws_data_buf_r[count] = 0;
                    ws_data_buf_g[count] = 0;
                    ws_data_buf_b[count] = 0;
                }
            }
            ws_send_data(WS2812_QUANTITY, ws_data_buf_r, ws_data_buf_g, ws_data_buf_b);
            dev_ws_delayms(speed);
        }

        infrared_signal_flag = 0;
        break;

    case 5:
        ws_color_gradient_cycle_table(interval);

        ws_data_buf_r[test_count] = ws_rgb_data[0];
        ws_data_buf_g[test_count] = ws_rgb_data[1];
        ws_data_buf_b[test_count] = ws_rgb_data[2];

        if (++test_count == 24)
        {
            test_count = 0;
        }
        break;

    case 6:
        ws_color_gradient_cycle_table(interval);

        ws_data_buf_r[23 - test_count] = ws_rgb_data[0];
        ws_data_buf_g[23 - test_count] = ws_rgb_data[1];
        ws_data_buf_b[23 - test_count] = ws_rgb_data[2];

        if (++test_count == 24)
        {
            test_count = 0;
        }
        break;

    default:
        break;
    }

    ws_send_data(WS2812_QUANTITY, ws_data_buf_r, ws_data_buf_g, ws_data_buf_b);
}
