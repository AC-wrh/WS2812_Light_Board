/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:40:39
 * @LastEditTime: 2020-04-13 15:55:42
 */
#include "core.h"

unsigned char ws_mode = 0;
int main(void)
{
    
    
    P3M1 = 0x80;
    P3M0 = 0x80;
    
    timer0_init();
    while (1) //主循环。
    {
        if (timer0_flag == 1)
        {
            TR0 = 0;
            PWM_Dimming(ws_mode, COLOR_CHANGE_INTERVAL_NORMAL, 120);
            timer0_flag = 0;
            TR0 = 1;
        }
    }

    return 0;
}
