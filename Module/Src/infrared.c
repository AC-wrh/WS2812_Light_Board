/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-20 10:54:58
 * @LastEditTime: 2020-04-20 15:02:12
 */
#include "core.h"

unsigned char infrared_signal = 0;

void exint1_init(void)
{
    INT1 = 1;
    IT1 = 1; //设置INT1的中断类型 (1:仅下降沿 0:上升沿和下降沿)
    EX1 = 1; //使能INT1中断
    EA = 1;
}

void exint1_IRQ_handle(void) interrupt 2 //INT1中断入口
{
    infrared_signal = 1;
}