#ifndef __OLED_UI_CONFIG_H
#define __OLED_UI_CONFIG_H
/*==============================================================================*/
/*============================此文件用于配置一些宏================================*/
/*==============================================================================*/

// OLED_UI 版本号
#define OLED_UI_VERSION "2.0.0"

//中断时间间隔（单位：ms）【您的中断时间是多少ms，就请将这个宏改为多少ms】
#define OLED_UI_INTERRUPT_TIME                                            (20)

// 单击按键结束判定的时间（单位：ms）【在第一次松开按键之后，如果在此时间内还没有再次按下，则判定为单击事件】
#define OLED_UI_ENTER_CLICK_TIME_MS                                           (300)
#define OLED_UI_BACK_CLICK_TIME_MS                                           (300)
#define OLED_UI_UP_CLICK_TIME_MS                                           (20)
#define OLED_UI_DOWN_CLICK_TIME_MS                                         (20)
#define OLED_UI_ENCODER_KEY_CLICK_TIME_MS                                         (300)
// 长按按键结束判定的时间（单位：ms）【如果持续按下按键的时间超过此时间，则判定为长按事件】
#define OLED_UI_ENTER_LONG_PRESS_TIME_MS                                      (800)
#define OLED_UI_BACK_LONG_PRESS_TIME_MS                                      (800)
#define OLED_UI_UP_LONG_PRESS_TIME_MS                                      (800)
#define OLED_UI_DOWN_LONG_PRESS_TIME_MS                                    (800)
#define OLED_UI_ENCODER_KEY_LONG_PRESS_TIME_MS                                    (800)

// 短时间按住按键不放的的判断时间（单位：ms）【如果在此时间内没有松开按键，则判定为短时间按住事件】
#define OLED_UI_SHORT_HOLD_TIME_MS                                      (1000)

// 长时间按住按键不放的的判断时间（单位：ms）【如果在此时间内没有松开按键，则判定为长时间按住事件】
#define OLED_UI_LONG_HOLD_TIME_MS                                       (3000)

// pid动画的参数
#define OLED_UI_PID_ANIM_KP                                               (0.02f)
#define OLED_UI_PID_ANIM_KI                                               (0.005f)
#define OLED_UI_PID_ANIM_KD                                               (0.002f)

// 长菜单项每帧滚动的像素数
#define OLED_UI_LONG_MENU_SCROLL_PIXELS                                    (0.2f)

// 渐隐时间（单位：ms）【菜单项渐隐的时间】
#define OLED_UI_FADE_TIME_MS                                               (40)



// 窗口当中如果有数据显示，数据与前文本之间的距离
#define OLED_UI_WINDOW_DATA_GAP                                             (5)

// 上/下按键长按达到一定时间后，是否触发菜单项的滚动。由于触发的快速滚动和中断绑定，当触发短时间按下时，中断每经过OLED_UI_PRESS_SHORT轮，菜单光标移动1格。触发长时间按下时，中断每经过OLED_UI_PRESS_LONG轮，菜单光标移动1格。
#define OLED_UI_PRESSED_SHORT                                               (5)
#define OLED_UI_PRESSED_LONG                                                (1)

#endif

