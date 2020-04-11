/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:41:41
 * @LastEditTime: 2020-04-11 17:48:00
 */
#ifndef __WS2812_H__
#define __WS2812_H__

#define WS2812_QUANTITY 24
#define COLOR_CHANGE_INTERVAL_VERY_SMALL        1
#define COLOR_CHANGE_INTERVAL_SMALL             3
#define COLOR_CHANGE_INTERVAL_NORMAL            5
#define COLOR_CHANGE_INTERVAL_BIG               17
#define COLOR_CHANGE_INTERVAL_VERY_BIG          51

#define COLOR_MIN       0
#define COLOR_MAX       255

#define MODE_GRADIENT       0
#define MODE_BOTH_GRADIENT  1
#define MODE_CHASE          2

#define CHASE_QUANTITY      3
#define CHASE_INTERVAL      2

void PWM_Dimming(unsigned char mode, unsigned char interval, unsigned char speed);

#endif

