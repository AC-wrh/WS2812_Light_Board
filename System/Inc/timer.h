/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:41:17
 * @LastEditTime: 2020-04-13 09:36:43
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#define FOSC        11059200L
#define T1MS_1T     (65536 - FOSC / 1000)   //1T模式
#define T1MS_12T    (65536-FOSC/12/1000)    //12T模式

void dev_ws_delayus(unsigned int us);
void timer0_init(void);

extern unsigned char timer0_flag;
extern unsigned int timing_time;

#endif