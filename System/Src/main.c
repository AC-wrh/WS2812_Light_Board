/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-11 16:40:39
 * @LastEditTime: 2020-04-11 17:47:48
 */
#include "core.h"


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
            PWM_Dimming(MODE_CHASE, COLOR_CHANGE_INTERVAL_NORMAL, 120);
            timer0_flag = 0;
            TR0 = 1;
        }
    }

    return 0;
}





















//     WsDat_RUN4_R = 0;
//     WsDat_RUN4_G = 0;
//     WsDat_RUN4_B = 0;
    
// }

