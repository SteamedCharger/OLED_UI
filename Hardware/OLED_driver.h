#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H



#include "stm32f10x.h"         
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>







#ifdef __cplusplus
extern "C" {
#endif



//	oled��ʼ������
void OLED_Init(void);
//	oledȫ��ˢ�º���
void OLED_Update(void);
//	oled�ֲ�ˢ�º���
void OLED_UpdateArea(int16_t X, int16_t Y, int16_t Width, int16_t Height);
// ������ɫģʽ
void OLED_SetColorMode(bool colormode);
// OLED �������Ⱥ���
void OLED_SetBrightness(int16_t Brightness);
// �Դ������Ƿ�仯
bool OLED_IfChangedScreen(void);
// ��ʱ����
void OLED_DelayMs(uint32_t xms);

#ifdef __cplusplus
}
#endif


#endif







