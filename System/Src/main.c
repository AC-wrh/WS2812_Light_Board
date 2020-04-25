/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:40:39
 * @LastEditTime: 2020-04-24 17:19:59
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
        if (infrared_signal_flag)
        {
            TR0 = 0;
            PWM_Dimming(MODE_WHITE_FLASHING, 6, 180);
            PWM_Dimming(MODE_SYMMETRICAL_REBOUND, 51, 120);
            TR0 = 1;
            infrared_signal_flag = 0;
        }
        else if (timer0_flag == 1)
        {
            TR0 = 0;
            PWM_Dimming(ws_mode, COLOR_CHANGE_INTERVAL_VERY_BIG, 1000);
            timer0_flag = 0;
            TR0 = 1;
        }
    }

    return 0;
}
