#ifndef __OLED_UI_DRIVER_H
#define __OLED_UI_DRIVER_H
/*【如果您需要移植此项目，则需要更改以下函数的实现方式。】 */
#include "stm32f10x.h"                  // Device header


#ifdef __cplusplus
extern "C" {
#endif






//获取确认，取消，上，下按键状态的函数(【Q：为什么使用宏定义而不是函数？A：因为这样可以提高效率，减少代码量】)
#define Key_GetEnterStatus()    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define Key_GetBackStatus()     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define Key_GetUpStatus()       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define Key_GetDownStatus()     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)

//定义编码器引脚
#define Key_GetEncoderStatus()  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)



// 定时器中断初始化函数
void OLED_UI_TimerInit(void);

// 定时器中断禁止函数
void OLED_UI_TimerDisable(void);

// 定时器使能函数
void OLED_UI_TimerEnable(void);

// 按键初始化函数
void OLED_UI_KeyInit(void);

// 编码器初始化函数
void OLED_UI_EncoderInit(void);

// 编码器使能函数
void OLED_UI_EncoderEnable(void);

// 编码器失能函数
void OLED_UI_EncoderDisable(void);

// 读取编码器的增量值
int16_t OLED_UI_EncoderGet(void);



#ifdef __cplusplus
}  // extern "C"
#endif
#endif
