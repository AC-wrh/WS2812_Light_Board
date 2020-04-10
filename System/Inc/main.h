/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-09 17:41:54
 * @LastEditTime: 2020-04-10 17:44:13
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg51.h>
#include <intrins.h>
#include "stdio.h"
#include "ws2812.h"
#include "timer.h"

#define uint8   unsigned char
#define uint16  unsigned short
#define uint32  unsigned int

sfr P3M1 = 0x80;    //  10 000000   全部设置为双向IO口，弱上拉25K
sfr P3M0 = 0x80;    //  10 000000
sfr AUXR = 0x8e;    //Auxiliary register

sbit sda  = P3 ^ 2; // 8
sbit KEY1 = P3 ^ 3; // 1

#endif
