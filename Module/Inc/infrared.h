/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-20 10:55:10
 * @LastEditTime: 2020-04-24 17:29:41
 */
#ifndef __INFRARED_H__
#define __INFRARED_H__

sbit KEY1 = P3 ^ 3;

void exint1_init(void);

extern bit infrared_signal_flag;

#endif