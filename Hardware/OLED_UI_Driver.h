#ifndef __OLED_UI_DRIVER_H
#define __OLED_UI_DRIVER_H
/*���������Ҫ��ֲ����Ŀ������Ҫ�������º�����ʵ�ַ�ʽ���� */
#include "stm32f10x.h"                  // Device header


#ifdef __cplusplus
extern "C" {
#endif






//��ȡȷ�ϣ�ȡ�����ϣ��°���״̬�ĺ���(��Q��Ϊʲôʹ�ú궨������Ǻ�����A����Ϊ�����������Ч�ʣ����ٴ�������)
#define Key_GetEnterStatus()    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define Key_GetBackStatus()     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define Key_GetUpStatus()       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define Key_GetDownStatus()     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)

//�������������
#define Key_GetEncoderStatus()  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)



// ��ʱ���жϳ�ʼ������
void OLED_UI_TimerInit(void);

// ��ʱ���жϽ�ֹ����
void OLED_UI_TimerDisable(void);

// ��ʱ��ʹ�ܺ���
void OLED_UI_TimerEnable(void);

// ������ʼ������
void OLED_UI_KeyInit(void);

// ��������ʼ������
void OLED_UI_EncoderInit(void);

// ������ʹ�ܺ���
void OLED_UI_EncoderEnable(void);

// ������ʧ�ܺ���
void OLED_UI_EncoderDisable(void);

// ��ȡ������������ֵ
int16_t OLED_UI_EncoderGet(void);



#ifdef __cplusplus
}  // extern "C"
#endif
#endif
