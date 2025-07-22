/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

#define LED3_ON() gpio_set_level(P20_8,0)
#define LED4_ON() gpio_set_level(P20_9,0)

#define LED3_OFF() gpio_set_level(P20_8,1)
#define LED4_OFF() gpio_set_level(P20_9,1)

void led_init()
{

    gpio_init(P20_8, GPO, 1, GPO_PUSH_PULL);
    gpio_init(P20_9, GPO, 1, GPO_PUSH_PULL);
}

// **************************** �������� ****************************
int core0_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
    tft180_init();
    key_init(5);
    pit_ms_init(CCU60_CH0, 5);

    switch_init();
    led_init();

    // �˴���д�û����� ���������ʼ�������
    BEEP_init();
    liushuideng_Init();
    //BEEP_on ();
    //system_delay_ms(100);
    //BEEP_off();
    cpu_wait_event_ready();  // �ȴ����к��ĳ�ʼ�����



    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        //tft180_show_int(0, 0, count, 3);

        HuXi();
        if(Switch_Get(SWITCH_3) == SWITCH_ON) LED3_ON();
        if(Switch_Get(SWITCH_4) == SWITCH_ON) LED4_ON();

        //��ʽ�����궨���ͺ�����ȡ

                if(!GET_SWITCH3()) LED3_OFF();
                if(!GET_SWITCH4()) LED4_OFF();


        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************
