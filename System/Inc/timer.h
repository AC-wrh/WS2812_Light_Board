/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-10 14:25:40
 * @LastEditTime: 2020-04-10 17:58:07
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#define FOSC        11059200L

#define T1MS_1T     (65536-FOSC/1000)       //1T模式
#define T1MS_12T    (65536-FOSC/12/1000)   //12T模式

void timer0_init(void);      //定时器0初始化

#endif
