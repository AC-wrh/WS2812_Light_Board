/*
 * @Description: 
 * @version: 
 * @Author: Adol
 * @Date: 2020-04-10 14:25:47
 * @LastEditTime: 2020-04-10 17:20:48
 */

uint MS1;

void delayus(uint us) //     ��ʱn��1us    @12M����  STC15F104WоƬ
{
    uint us2;
    for (us2 = us; us2 > 0; us2--)
    {
        _nop_();
        _nop_();
    }
}

void timer0_init(void)      //��ʱ��0��ʼ��
{
    AUXR |= 0x80;           //��ʱ��0Ϊ1Tģʽ
    // AUXR &= 0x7f;           //��ʱ��0Ϊ12Tģʽ

    TMOD = 0x00;            //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
    TL0 = T1MS_1T;          //��ʼ����ʱֵ
    TH0 = T1MS_1T >> 8;
    TR0 = 1;                //��ʱ��0��ʼ��ʱ
    ET0 = 1;                //ʹ�ܶ�ʱ��0�ж�
    EA  = 1;                //�������ж�
}

void timer0_IRQ_handler(void) interrupt 1 using 1 //---��ʱ��t0-----
{
    TF0 = 0;
    MS1++;
    if (MS1 == color_change_speed) //�ı��ֵ���Ե��ڱ仯�ٶ�
    {
        MS1 = 0;
        send = 1;
    }
}
