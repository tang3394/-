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
* �ļ�����          zf_device_mt9v03x2
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
* 2023-04-28       pudding            ��������ע��˵��
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                  ------------------------------------
*                  ģ��ܽ�             ��Ƭ���ܽ�
*                  TXD                �鿴 zf_device_mt9v03x2.h �� MT9V03X2_COF_UART_TX �궨��
*                  RXD                �鿴 zf_device_mt9v03x2.h �� MT9V03X2_COF_UART_RX �궨��
*                  PCLK               �鿴 zf_device_mt9v03x2.h �� MT9V03X2_PCLK_PIN �궨��
*                  VSY                �鿴 zf_device_mt9v03x2.h �� MT9V03X2_VSYNC_PIN �궨��
*                  D0-D7              �鿴 zf_device_mt9v03x2.h �� MT9V03X2_DATA_PIN �궨�� �Ӹö��忪ʼ�������˸�����
*                  VCC                3.3V��Դ
*                  GND                ��Դ��
*                  ������������
*                  ------------------------------------
********************************************************************************************************************/


#include "zf_common_interrupt.h"
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_driver_soft_iic.h"
#include "zf_driver_delay.h"
#include "zf_driver_dma.h"
#include "zf_driver_exti.h"
#include "zf_driver_gpio.h"
#include "zf_device_camera.h"
#include "zf_device_config.h"
#include "isr_config.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_mt9v03x2.h"
vuint8  mt9v03x2_finish_flag = 0;                             // һ��ͼ��ɼ���ɱ�־λ
IFX_ALIGN(4) uint8  mt9v03x2_image[MT9V03X2_H][MT9V03X2_W];   // ����4�ֽڶ���

static  m9v03x2_type_enum mt9v03x2_type;                      // ��������ͷ����

int16   timeout_2 = MT9V03X2_INIT_TIMEOUT;                    // ���峬ʱ���ʱ��

uint8   mt9v03x2_lost_flag = 1;                               // ͼ��ʧ��־λ
uint8   mt9v03x2_dma_int_num;                                 // ��ǰDMA�жϴ���
uint8   mt9v03x2_dma_init_flag;                               // �Ƿ���Ҫ���³�ʼ��
uint8   mt9v03x2_link_list_num;

int16   mt9v03x2_dma_run_flag = 1;
uint8   mt9v03x2_init_flag = 0;

// ��Ҫ���õ�����ͷ������ �����������޸Ĳ���
static int16 mt9v03x2_set_confing_buffer[MT9V03X2_CONFIG_FINISH][2]=
{
    {MT9V03X2_INIT,              0},                                             // ����ͷ��ʼ��ʼ��
    {MT9V03X2_AUTO_EXP,          MT9V03X2_AUTO_EXP_DEF},                          // �Զ��ع�����   ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
    {MT9V03X2_EXP_TIME,          MT9V03X2_EXP_TIME_DEF},                          // �ع�ʱ��      ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
    {MT9V03X2_FPS,               MT9V03X2_FPS_DEF},                               // ͼ��֡��      ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
    {MT9V03X2_SET_COL,           MT9V03X2_W},                                     // ͼ��������    ��Χ1-752
    {MT9V03X2_SET_ROW,           MT9V03X2_H},                                     // ͼ��������    ��Χ1-480
    {MT9V03X2_LR_OFFSET,         MT9V03X2_LR_OFFSET_DEF},                         // ͼ������ƫ����  ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {MT9V03X2_UD_OFFSET,         MT9V03X2_UD_OFFSET_DEF},                         // ͼ������ƫ����  ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {MT9V03X2_GAIN,              MT9V03X2_GAIN_DEF},                              // ͼ������      ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
    {MT9V03X2_PCLK_MODE,         MT9V03X2_PCLK_MODE_DEF},                         // ����ʱ��ģʽ   �������MT9V034 V2.0�Լ����ϰ汾֧�ָ�����
};



//-------------------------------------------------------------------------------------------------------------------
//  �������      MT9V03X2����ͷ���ж�
//  ����˵��      void
//  ���ز���      void
//  ʹ��ʾ��      mt9v03x2_vsync_handler();
//-------------------------------------------------------------------------------------------------------------------
static void mt9v03x2_vsync_handler(void)
{
    exti_flag_clear(MT9V03X2_VSYNC_PIN);

    if ((mt9v03x_dma_run_flag == 1) && (mt9v03x_init_flag))                         //  ��ʱ�ɼ�
    {
        return ;
    }
    mt9v03x2_dma_int_num = 0;
    if((mt9v03x2_dma_init_flag))
    {
        mt9v03x2_dma_init_flag = 0;
        IfxDma_resetChannel(&MODULE_DMA, MT9V03X2_DMA_CH);

        mt9v03x2_link_list_num = dma_init(MT9V03X2_DMA_CH,
                                         MT9V03X2_DATA_ADD,
                                         mt9v03x2_image[0],
                                         MT9V03X2_PCLK_PIN,
                                         EXTI_TRIGGER_RISING,
                                         MT9V03X2_IMAGE_SIZE,
                                         DMA_INT_PRIO_2);           // �����Ƶ��300M �����ڶ�������������ΪFALLING

        dma_enable(MT9V03X2_DMA_CH);

    }
    else
    {
        if(1 == mt9v03x2_link_list_num)
        {
            dma_set_destination(MT9V03X2_DMA_CH, mt9v03x2_image[0]);      // û�в������Ӵ���ģʽ ��������Ŀ�ĵ�ַ
        }
        dma_enable(MT9V03X2_DMA_CH);

    }

    mt9v03x2_lost_flag = 1;
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      MT9V03X2����ͷDMA����ж�
//  ����˵��      void
//  ���ز���      void
//  ʹ��ʾ��      mt9v03x2_dma_handler();
//-------------------------------------------------------------------------------------------------------------------
static void mt9v03x2_dma_handler(void)
{

    clear_dma_flag(MT9V03X2_DMA_CH);

    if(IfxDma_getChannelTransactionRequestLost(&MODULE_DMA, MT9V03X2_DMA_CH)) // ͼ���λ�ж�
    {
        mt9v03x2_finish_flag = 0;
        dma_disable(MT9V03X2_DMA_CH);
        IfxDma_clearChannelInterrupt(&MODULE_DMA, MT9V03X2_DMA_CH);
        mt9v03x2_dma_init_flag = 1;
    }
    else
    {
        mt9v03x2_dma_int_num++;
        if(mt9v03x2_dma_int_num >= mt9v03x2_link_list_num)
        {
            // �ɼ����
            // һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ3.8MS����(50FPS��188*120�ֱ���)
            mt9v03x2_dma_int_num = 0;
            mt9v03x2_lost_flag   = 0;
            mt9v03x2_finish_flag = 1;


            mt9v03x2_dma_run_flag = 0;
            mt9v03x_dma_run_flag = 1;
            dma_disable(MT9V03X2_DMA_CH);


        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     MT9V03X2 ����ͷ��ʼ��
// ����˵��     void
// ���ز���     uint8           1-ʧ�� 0-�ɹ�
// ʹ��ʾ��     zf_log(mt9v03x2_init(), "mt9v03x2 init error");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 mt9v03x2_init (void)
{
    uint8 return_state = 0;
    soft_iic_info_struct mt9v03x2_iic_struct;
    do
    {
//        system_delay_ms(200);
        // ���ȳ���SCCBͨѶ
        mt9v03x2_type = MT9V03X2_SCCB;
        set_camera_type_2(CAMERA_GRAYSCALE2, mt9v03x2_vsync_handler, mt9v03x2_dma_handler, NULL);
        soft_iic_init(&mt9v03x2_iic_struct, 0, MT9V03X2_COF_IIC_DELAY, MT9V03X2_COF_IIC_SCL, MT9V03X2_COF_IIC_SDA);
        if(mt9v03x_set_config_sccb(&mt9v03x2_iic_struct, mt9v03x2_set_confing_buffer))
        {
            // SCCBͨѶʧ��
            // �������������˶�����Ϣ ������ʾ����λ��������
            // ��ô���Ǵ���ͨ�ų�����ʱ�˳���
            // ���һ�½�����û������ ���û������ܾ��ǻ���
            zf_log(0, "MT9V03X2 set sccb error.");
           return_state = 1;
            break;
        }
        mt9v03x2_link_list_num = camera_init_2(MT9V03X2_DATA_ADD, mt9v03x2_image[0], MT9V03X2_IMAGE_SIZE);
    }while(0);

    mt9v03x2_init_flag = 1;
    return return_state;
}

