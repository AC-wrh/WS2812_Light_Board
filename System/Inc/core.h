/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:40:57
 * @LastEditTime: 2020-04-13 14:20:04
 */
#ifndef __CORE_H__
#define __CORE_H__

#include <reg51.h>
#include <intrins.h>

#include "timer.h"
#include "ws2812.h"

sfr P3M1 = 0x80; //  10 000000   全部设置为双向IO口，弱上拉25K
sfr P3M0 = 0x80; //  10 000000
sfr AUXR = 0x8e; //Auxiliary register

sbit sda  = P3 ^ 2;     // 8
sbit KEY1 = P3 ^ 3;     // 1

extern unsigned char ws_mode;

#endif