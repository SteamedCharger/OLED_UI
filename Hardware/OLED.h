#ifndef __OLED_H
#define __OLED_H


#include "OLED_Driver.h"			//oled�ײ�����ͷ�ļ�
#include "OLED_Fonts.h"				//oled�����ͷ�ļ�
#include "stdbool.h"

// ����Ƿ���C++������
#ifdef __cplusplus
extern "C" {
#endif

//ʹ�ú궨��ķ�ʽȷ��oled�ĺ�����������������
#define OLED_WIDTH						(128)					
#define OLED_HEIGHT 					(128)

/*�����С����ȡֵ*/
#define OLED_FONT_8                          (8)                   
// #define OLED_FONT_12                         (12) 
// #define OLED_FONT_16                         (16)
// #define OLED_FONT_20                         (20)


/*������ȡֵ*/
#define OLED_CHAR_SPACING                    (0)
#define OLED_LINE_SPACING                    (2)

/*IsFilled������ֵ*/
#define OLED_UNFILLED			        (0)
#define OLED_FILLED				        (1)

#define OLED_ASCII 		    (0)
#define OLED_CHINESE 		(1)

/**�����Ƿ�ʹ�þ����vsprintf���������׼vsprintf�����ĺꡣ���������ѡ��������������Լ5kb��������С��Ƕ��ʽϵͳ**/
#define USE_SIMPLE_VSPRINTF   (true)
/**�����ַ�����󳤶ȵĺ꣬���ڸ�ʽ������ַ���*/
#define  MAX_STRING_LENGTH   (128)


// ���ߺ�����ʵ����һ���򵥵�vsprintf���������ڸ�ʽ������ַ���
int OLED_Simple_vsprintf(char* buf, const char* format, va_list args);
// ���ߺ�������ȡ������
uint8_t OLED_GetFontWidth(uint8_t oledfont, uint8_t ascii_or_chinese);
// �����Ļ
void OLED_Clear(void);
// ���������Ļ
void OLED_ClearArea(int16_t X, int16_t Y, int16_t Width, int16_t Height);
// ��ɫ��Ļ
void OLED_Reverse(void);
// ��ɫ������Ļ
void OLED_ReverseArea(int16_t X, int16_t Y, int16_t Width, int16_t Height);

// ��ʾͼƬ
void OLED_ShowImage(int16_t X, int16_t Y, uint16_t Width, uint16_t Height, const uint8_t *Image);

// ��ʽ���������ַ���
void OLED_Printf(int16_t X, int16_t Y, uint8_t Font,const char *format, ...);

// ����������ʾͼƬ
void OLED_ShowImageArea(int16_t X_Area, int16_t Y_Area, int16_t AreaWidth, int16_t AreaHeight, int16_t X_Pic, int16_t Y_Pic, int16_t PictureWidth, int16_t PictureHeight, const uint8_t *Image);

// ���������ʽ���������ַ���
void OLED_PrintfArea(int16_t RangeX,int16_t RangeY,int16_t RangeWidth,int16_t RangeHeight,int16_t X, int16_t Y, uint8_t Font,const char *format, ...);


//���ƺ��������ƻ�����ui
void OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, int16_t Width, int16_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, int16_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t X, int16_t Y, int16_t A, int16_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t X, int16_t Y, int16_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);
void OLED_DrawRoundedRectangle(int16_t X, int16_t Y, int16_t Width, int16_t Height, int16_t Radius, uint8_t IsFilled);

// Ч������
void OLED_FadeOut_Masking(int16_t x0, int16_t y0, int16_t width, int16_t height, int8_t fadeLevel) ;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif

