/*
 * @Description: Control WS2812 through STC15W204S
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-09 16:25:14
 * @LastEditTime: 2020-04-10 14:27:05
 */

#include "main.h"

int main(void)
{
    
    
    return 0;
}


#define FOSC 11059200L
#define T1MS (65536 - FOSC / 1000) //1Tģʽ  1ms
//#define T1MS (65536-FOSC/12/1000) //12Tģʽ
#define DENGSHU 24
/*
,
ȫ�� LED ws2812  �� ���������ܵ���̳ zhuls ��������
ʵ�������ҵ�8��оƬ��STC15F104WоƬ�ϵ���������
�޸��ڲ����� @ 12MHZ�� STC Y5�ںˣ�һ��  _nop_();  Ϊ 0.25 usʱ��Ϊ��׼������WS2812
�˳���ʵ����������WS2812������˸�������ƣ�
��
���������úܳ�һ��WS2812�ƴ����ڳ�88��88����һ���������״��
Ȼ���õ�Ƭ��һ��IO��������ʵ��ȫ�������ӣ����๦�ܣ�������˵��Ҫ��ʱʱ�䵽�ˣ�����+������ɫ��RR=0xFF;;����
����˯��ǰ���Խ�ɫ�����Ƶķ�ʽ��ֱ����ҹ�������Զ��䰵��RR=0x01;�� GG=0x01; BB=0x01;
,
*/

sfr P3M1 = 0x80; //  10 000000   ȫ������Ϊ˫��IO�ڣ�������25K
sfr P3M0 = 0x80; //  10 000000
sfr AUXR = 0x8e; //Auxiliary register

sbit sda = P3 ^ 2;  // 8
sbit KEY1 = P3 ^ 3; // 1

uint MS1;
uchar i;
uchar send;
uchar WsDat_RUN4_R = 0; //��ɫ����0-255 ����FF����
uchar WsDat_RUN4_G = 0;
uchar WsDat_RUN4_B = 0;
uchar WsDat_R[DENGSHU];
uchar WsDat_G[DENGSHU];
uchar WsDat_B[DENGSHU];
unsigned char color_change_speed = 75;
//=============================================

void delayus(uint us) //     ��ʱn��1us    @12M����  STC15F104WоƬ
{
    uint us2;
    for (us2 = us; us2 > 0; us2--)
    {
        _nop_();
        _nop_();
    }
}

//==================================================��ʱ���� 12mHZ
// void delayms(uint z) //X  MS��ʱ==@ 12MHZ
// {
//     uint x, y;
//     for (x = z; x > 0; x--)
//         for (y = 920; y > 0; y--)
//             ;
// }
//=========================================================================

void bit1() //H=0.7us.L=0.6us  ����1�ı�ʾ   оƬSTC15F104W  @ 12MHZ

//1US= 4�� _nop_();  ������Ҫ�õ�2��_nop_(); �������õ�2��_nop_();
{
    sda = 1;
    _nop_();
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
}

void bit0() //H=0.35us.L=0.8us  ==����0�ı�ʾ
{
    sda = 1;
    _nop_();
    _nop_();
    sda = 0;
    _nop_();
    _nop_();
}

//����8λ��=1�ŵƵ�һ����ɫ��WS2812������Ϊ24һ��24λ���������Cÿ����ɫ8λ����������������ɫ��Ȼ���Ǻ�ɫ���������ɫ
void send_sda(uint dat)

{
    uchar count1; //λ�����������ֽ�Ϊ8λ
    for (count1 = 8; count1 > 0; count1--)
    {
        if (dat & 0x80) //ȡ��λ�ȷ�����dat & 0x80
            bit1();
        else
            bit0();
        dat <<= 1; //�������ƣ���һ��λ������,����λ�ȷ���dat<<=1
    }
}
//=============================================
void Init(void) //�жϳ�ʼ��
{
    AUXR |= 0x80; //��ʱ��0Ϊ1Tģʽ
                  //  AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ
    TMOD = 0x00;  //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
    TL0 = T1MS;   //��ʼ����ʱֵ
    TH0 = T1MS >> 8;
    TR0 = 1; //��ʱ��0��ʼ��ʱ
    ET0 = 1; //ʹ�ܶ�ʱ��0�ж�
    EA = 1;
}

void WS_Set_Date(unsigned char len, unsigned char *r, unsigned char *g, unsigned char *b) // ȫ������ͬ��������
{
    unsigned char i;
    //unsigned long dat;
    //dat = (r<<16) | (g<<8) | b;
    for (i = 0; i < len; i++)
    {
        send_sda(*r); //
        send_sda(*g); //
        send_sda(*b); //
        r++;
        g++;
        b++;
    }
    sda = 0;
    delayus(70); // ����50US�ĵ͵�ƽ�ź�=��λ��
}

// LED cycle gradient
// R = FF           G = 0 -> FF         B = 0           yellow  #FFFF00
// R = FF -> 0      G = FF              B = 0           green   #00FF00
// R = 0            G = FF              B = 0 -> FF     cyan    #00FFFF
// R = 0            G = FF -> 0         B = FF          blue    #0000FF
// R = 0 -> FF      G = 0               B = FF          purple  #FF00FF
// R = FF           G = 0               B = FF -> 0     red     #FF0000

#define COLOR_CHANGE_INTERVAL_VERY_SMALL        1
#define COLOR_CHANGE_INTERVAL_SMALL             3
#define COLOR_CHANGE_INTERVAL_NORMAL            5
#define COLOR_CHANGE_INTERVAL_BIG               17
#define COLOR_CHANGE_INTERVAL_VERY_BIG          51

#define COLOR_MIN       0
#define COLOR_MAX       255

void color_cycle_gradient(unsigned char interval) 
{
    static unsigned char cycle_gradient_flag = 0;

    switch (cycle_gradient_flag)
    {
    case 0: // R = 00 -> FF, G = 0, B = 0   #000000 -> #FF0000
        WsDat_RUN4_R += interval;
        // WsDat_RUN4_G += interval;
        // WsDat_RUN4_B += interval;
        // WsDat_RUN4_R = 255;
        if (WsDat_RUN4_R == COLOR_MAX)
        {
            cycle_gradient_flag = 1;
        }
        break;

    case 1: // R = FF, G = 0 -> FF, B = 0   #FF0000 -> #FFFF00
        WsDat_RUN4_G += interval;
        if (WsDat_RUN4_G == COLOR_MAX)
        {
            cycle_gradient_flag = 2;
        }
        break;

    case 2: // R = FF -> 0, G = FF, B = 0   #FFFF00 -> #00FF00
        WsDat_RUN4_R -= interval;
        if (WsDat_RUN4_R == COLOR_MIN)
        {
            cycle_gradient_flag = 3;
        }
        break;

    case 3: // R = 0, G = FF, B = 0 -> FF   #00FF00 -> #00FFFF
        WsDat_RUN4_B += interval;
        if (WsDat_RUN4_B == COLOR_MAX)
        {
            cycle_gradient_flag = 4;
        }
        break;

    case 4: // R = 0, G = FF -> 0, B = FF   #00FFFF -> #0000FF
        WsDat_RUN4_G -= interval;
        if (WsDat_RUN4_G == COLOR_MIN)
        {
            cycle_gradient_flag = 5;
        }
        break;

    case 5: // R = 0 -> FF, G = 0, B = FF   #0000FF -> #FF00FF
        WsDat_RUN4_R += interval;
        if (WsDat_RUN4_R == COLOR_MAX)
        {
            cycle_gradient_flag = 6;
        }
        break;

    case 6: // R = FF, G = 0, B = 0 -> FF   #FF00FF -> #FF0000
        WsDat_RUN4_B -= interval;
        if (WsDat_RUN4_B == COLOR_MIN)
        {
            cycle_gradient_flag = 1;
        }
        break;

    default:
        break;
    }
}

void PWM_Dimming(unsigned char mode, unsigned char interval, unsigned char speed)
{
    unsigned char count;

    color_change_speed = speed;

    if (mode) //�м������߽���
    {
        for (count = 0; count < (DENGSHU - 1) / 2; count++)
        {
            WsDat_R[count + 1] = WsDat_R[count];
            WsDat_G[count + 1] = WsDat_G[count];
            WsDat_B[count + 1] = WsDat_B[count];

            WsDat_R[23 - (count + 1)] = WsDat_R[23 - count];
            WsDat_G[23 - (count + 1)] = WsDat_G[23 - count];
            WsDat_B[23 - (count + 1)] = WsDat_B[23 - count];
        }
        color_cycle_gradient(interval);
        WsDat_R[0] = WsDat_RUN4_R;
        WsDat_G[0] = WsDat_RUN4_G;
        WsDat_B[0] = WsDat_RUN4_B;

        WsDat_R[23] = WsDat_RUN4_R;
        WsDat_G[23] = WsDat_RUN4_G;
        WsDat_B[23] = WsDat_RUN4_B;
    }
    else //��ˮ�ƽ���
    {
        for (count = 0; count < DENGSHU - 1; count++)
        {
            WsDat_R[count + 1] = WsDat_R[count];
            WsDat_G[count + 1] = WsDat_G[count];
            WsDat_B[count + 1] = WsDat_B[count];
        }
        color_cycle_gradient(interval);
        WsDat_R[0] = WsDat_RUN4_R;
        WsDat_G[0] = WsDat_RUN4_G;
        WsDat_B[0] = WsDat_RUN4_B;
    }

    WS_Set_Date(DENGSHU, WsDat_R, WsDat_G, WsDat_B);
}

//=============================================
void timer0(void) interrupt 1 using 1 //---��ʱ��t0-----
{
    TF0 = 0;
    MS1++;
    if (MS1 == color_change_speed) //�ı��ֵ���Ե��ڱ仯�ٶ�
    {
        MS1 = 0;
        send = 1;
    }
}
//=========================================================================

//==========================  ������ʼ  ==================================

void main()
{
    //uchar i;
    unsigned char color_mode = 0;

    P3M1 = 0x80;
    P3M0 = 0x80;
    MS1 = 0;
    send = 0;
    WsDat_RUN4_R = 0;
    WsDat_RUN4_G = 0;
    WsDat_RUN4_B = 0;
    Init();
    while (1) //��ѭ����
    {
        if (send == 1)
        {
            TR0 = 0;
            PWM_Dimming(color_mode, COLOR_CHANGE_INTERVAL_NORMAL, 25);
            send = 0;
            TR0 = 1;
        }
    }
}

//=========================================================================
//=========================      END     ==================================
//=========================================================================
