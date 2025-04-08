#ifndef __OLED_H
#define __OLED_H


#include "OLED_Driver.h"			//oled底层驱动头文件
#include "OLED_Fonts.h"				//oled字体库头文件
#include "stdbool.h"

// 检测是否是C++编译器
#ifdef __cplusplus
extern "C" {
#endif

//使用宏定义的方式确定oled的横向像素与竖向像素
#define OLED_WIDTH						(128)					
#define OLED_HEIGHT 					(128)

/*字体大小参数取值*/
#define OLED_FONT_8                          (8)                   
// #define OLED_FONT_12                         (12) 
// #define OLED_FONT_16                         (16)
// #define OLED_FONT_20                         (20)


/*字体间隔取值*/
#define OLED_CHAR_SPACING                    (0)
#define OLED_LINE_SPACING                    (2)

/*IsFilled参数数值*/
#define OLED_UNFILLED			        (0)
#define OLED_FILLED				        (1)

#define OLED_ASCII 		    (0)
#define OLED_CHINESE 		(1)

/**关于是否使用精简版vsprintf函数代替标准vsprintf函数的宏。如果开启此选项，代码体积将减少约5kb，有利于小型嵌入式系统**/
#define USE_SIMPLE_VSPRINTF   (true)
/**关于字符串最大长度的宏，用于格式化输出字符串*/
#define  MAX_STRING_LENGTH   (128)


// 工具函数，实现了一个简单的vsprintf函数，用于格式化输出字符串
int OLED_Simple_vsprintf(char* buf, const char* format, va_list args);
// 工具函数，获取字体宽度
uint8_t OLED_GetFontWidth(uint8_t oledfont, uint8_t ascii_or_chinese);
// 清除屏幕
void OLED_Clear(void);
// 清除部分屏幕
void OLED_ClearArea(int16_t X, int16_t Y, int16_t Width, int16_t Height);
// 反色屏幕
void OLED_Reverse(void);
// 反色部分屏幕
void OLED_ReverseArea(int16_t X, int16_t Y, int16_t Width, int16_t Height);

// 显示图片
void OLED_ShowImage(int16_t X, int16_t Y, uint16_t Width, uint16_t Height, const uint8_t *Image);

// 格式化输出混合字符串
void OLED_Printf(int16_t X, int16_t Y, uint8_t Font,const char *format, ...);

// 限制区域显示图片
void OLED_ShowImageArea(int16_t X_Area, int16_t Y_Area, int16_t AreaWidth, int16_t AreaHeight, int16_t X_Pic, int16_t Y_Pic, int16_t PictureWidth, int16_t PictureHeight, const uint8_t *Image);

// 限制区域格式化输出混合字符串
void OLED_PrintfArea(int16_t RangeX,int16_t RangeY,int16_t RangeWidth,int16_t RangeHeight,int16_t X, int16_t Y, uint8_t Font,const char *format, ...);


//绘制函数，绘制基础的ui
void OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, int16_t Width, int16_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, int16_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t X, int16_t Y, int16_t A, int16_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t X, int16_t Y, int16_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);
void OLED_DrawRoundedRectangle(int16_t X, int16_t Y, int16_t Width, int16_t Height, int16_t Radius, uint8_t IsFilled);

// 效果函数
void OLED_FadeOut_Masking(int16_t x0, int16_t y0, int16_t width, int16_t height, int8_t fadeLevel) ;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif

