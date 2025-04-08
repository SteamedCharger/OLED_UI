#ifndef __OLED_UI_CONFIG_H
#define __OLED_UI_CONFIG_H
/*==============================================================================*/
/*============================���ļ���������һЩ��================================*/
/*==============================================================================*/

// OLED_UI �汾��
#define OLED_UI_VERSION "2.0.0"

//�ж�ʱ��������λ��ms���������ж�ʱ���Ƕ���ms�����뽫������Ϊ����ms��
#define OLED_UI_INTERRUPT_TIME                                            (20)

// �������������ж���ʱ�䣨��λ��ms�����ڵ�һ���ɿ�����֮������ڴ�ʱ���ڻ�û���ٴΰ��£����ж�Ϊ�����¼���
#define OLED_UI_ENTER_CLICK_TIME_MS                                           (300)
#define OLED_UI_BACK_CLICK_TIME_MS                                           (300)
#define OLED_UI_UP_CLICK_TIME_MS                                           (20)
#define OLED_UI_DOWN_CLICK_TIME_MS                                         (20)
#define OLED_UI_ENCODER_KEY_CLICK_TIME_MS                                         (300)
// �������������ж���ʱ�䣨��λ��ms��������������°�����ʱ�䳬����ʱ�䣬���ж�Ϊ�����¼���
#define OLED_UI_ENTER_LONG_PRESS_TIME_MS                                      (800)
#define OLED_UI_BACK_LONG_PRESS_TIME_MS                                      (800)
#define OLED_UI_UP_LONG_PRESS_TIME_MS                                      (800)
#define OLED_UI_DOWN_LONG_PRESS_TIME_MS                                    (800)
#define OLED_UI_ENCODER_KEY_LONG_PRESS_TIME_MS                                    (800)

// ��ʱ�䰴ס�������ŵĵ��ж�ʱ�䣨��λ��ms��������ڴ�ʱ����û���ɿ����������ж�Ϊ��ʱ�䰴ס�¼���
#define OLED_UI_SHORT_HOLD_TIME_MS                                      (1000)

// ��ʱ�䰴ס�������ŵĵ��ж�ʱ�䣨��λ��ms��������ڴ�ʱ����û���ɿ����������ж�Ϊ��ʱ�䰴ס�¼���
#define OLED_UI_LONG_HOLD_TIME_MS                                       (3000)

// pid�����Ĳ���
#define OLED_UI_PID_ANIM_KP                                               (0.02f)
#define OLED_UI_PID_ANIM_KI                                               (0.005f)
#define OLED_UI_PID_ANIM_KD                                               (0.002f)

// ���˵���ÿ֡������������
#define OLED_UI_LONG_MENU_SCROLL_PIXELS                                    (0.2f)

// ����ʱ�䣨��λ��ms�����˵������ʱ�䡿
#define OLED_UI_FADE_TIME_MS                                               (40)



// ���ڵ��������������ʾ��������ǰ�ı�֮��ľ���
#define OLED_UI_WINDOW_DATA_GAP                                             (5)

// ��/�°��������ﵽһ��ʱ����Ƿ񴥷��˵���Ĺ��������ڴ����Ŀ��ٹ������жϰ󶨣���������ʱ�䰴��ʱ���ж�ÿ����OLED_UI_PRESS_SHORT�֣��˵�����ƶ�1�񡣴�����ʱ�䰴��ʱ���ж�ÿ����OLED_UI_PRESS_LONG�֣��˵�����ƶ�1��
#define OLED_UI_PRESSED_SHORT                                               (5)
#define OLED_UI_PRESSED_LONG                                                (1)

#endif

