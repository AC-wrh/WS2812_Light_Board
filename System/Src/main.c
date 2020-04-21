/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:40:39
 * @LastEditTime: 2020-04-21 13:37:51
 */
#include "core.h"

unsigned char ws_mode = 0;

int main(void)
{
    P3M1 = 0x80;
    P3M0 = 0x80;

    timer0_init();
    exint1_init();

    while (1)
    {
        if (infrared_signal)
        {
            TR0 = 0;
            PWM_Dimming(MODE_WHITE_FLASHING, 6, 180);
            TR0 = 1;
        }
        else if (timer0_flag == 1)
        {
            TR0 = 0;
            PWM_Dimming(2, 51, 120);
            timer0_flag = 0;
            TR0 = 1;
        }
    }

    return 0;
}
