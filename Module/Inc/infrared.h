/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-20 10:55:10
 * @LastEditTime: 2020-04-20 15:13:03
 */
#ifndef __INFRARED_H__
#define __INFRARED_H__

sbit KEY1 = P3 ^ 3;

void exint1_init(void);

extern unsigned char infrared_signal;

#endif