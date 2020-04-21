/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:41:41
 * @LastEditTime: 2020-04-21 11:46:02
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

#define MODE_GRADIENT               0
#define MODE_BOTH_GRADIENT          1
#define MODE_SYMMETRICAL_REBOUND    2
#define MODE_CHASE                  3
#define MODE_WHITE_FLASHING         4

#define SIGNLE_MODE_CYCLE   1

#define CHASE_QUANTITY      6
#define CHASE_INTERVAL      4

void PWM_Dimming(unsigned char mode, unsigned char interval, unsigned int speed);

#endif