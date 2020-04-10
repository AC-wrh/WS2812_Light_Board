/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-10 14:25:15
 * @LastEditTime: 2020-04-10 17:57:43
 */
#ifndef __WS2812_H__
#define __WS2812_H__

#include "main.h"

#define WS2812_QTY      24

#define COLOR_CHANGE_INTERVAL_VERY_SMALL 1
#define COLOR_CHANGE_INTERVAL_SMALL 3
#define COLOR_CHANGE_INTERVAL_NORMAL 5
#define COLOR_CHANGE_INTERVAL_BIG 17
#define COLOR_CHANGE_INTERVAL_VERY_BIG 51

#define COLOR_MIN 0
#define COLOR_MAX 255

typedef struct
{
    uint8 r_dat;
    uint8 g_dat;
    uint8 b_dat;
    uint8 ws_dat_r[WS2812_QTY];
    uint8 ws_dat_g[WS2812_QTY];
    uint8 ws_dat_b[WS2812_QTY];
} dev_color_t;

void PWM_Dimming(uint8 mode, uint8 interval, uint8 speed);

#endif
 
