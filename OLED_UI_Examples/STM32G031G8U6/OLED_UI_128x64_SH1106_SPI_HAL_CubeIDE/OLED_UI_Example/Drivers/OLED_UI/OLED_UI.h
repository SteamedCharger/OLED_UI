#ifndef __OLED_UI_H
#define __OLED_UI_H
#include "OLED.h"
#include "OLED_UI_Driver.h"
#include "OLED_UI_Config.h"

/*******************有关按键状态的宏定义***************************** */
#define OLED_UI_KEY_NONE                            (0)            //无按键按下
#define OLED_UI_KEY_CLICK                           (1)            //单击按键
#define OLED_UI_KEY_DOUBLE_CLICK                    (2)            //双击按键
#define OLED_UI_KEY_LONG_PRESS                      (3)            //长按按键


/*********************关于菜单类型的宏************************/

#define MENU_TYPE_LIST				                (0)			//列表类型
#define MENU_TYPE_TILES				                (1)			//磁贴类型

/***********有关光标类型的宏***********/
#define CURSOR_REVERSE_RECTANGLE 				    (0)			//矩形反色
#define CURSOR_REVERSE_ROUNDRECTANGLE 		        (1)			//圆角矩形反色
#define CURSOR_HOLLOW_RECTANGLE 					(2)			//空心矩形
#define CURSOR_HOLLOW_ROUNDRECTANGLE 			    (3)			//空心圆角矩形
#define CURSOR_REVERSE_BLOCK						(4)			//下划线
#define CURSOR_ARROW								(5)			//箭头
#define CURSOR_NOT_SHOW							    (6)			//不显示光标

/***********有关菜单边框类型的宏***********/
#define MENU_FRAME_RECTANGLE						(0)			//矩形边框
#define MENU_FRAME_ROUNDRECTANGLE					(1)			//圆角矩形边框
#define MENU_FRAME_NONE							    (2)			//无边框

/***************有关窗口类型的宏***********/
#define WINDOW_FRAME_RECTANGLE					    (0)			//矩形边框
#define WINDOW_FRAME_ROUNDRECTANGLE				    (1)			//圆角矩形边框

/*********************关于窗口数据类型的宏* ***************/
#define WINDOW_DATA_STYLE_NONE	                    (0)			//空
#define WINDOW_DATA_STYLE_FLOAT		                (1)			//浮点型数据
#define WINDOW_DATA_STYLE_INT8		                (2)			//整型8位数据
#define WINDOW_DATA_STYLE_INT16		                (3)			//整型16位数据
#define WINDOW_DATA_STYLE_INT32		                (4)			//整型32位数据
#define WINDOW_DATA_STYLE_INT64		                (5)			//整型64位数据

/*********************关于动画类型的宏***********************/
#define UNLINEAR 			                        (0)
#define PID_CURVE     	 	                        (1)

/************关于菜单项前缀符号的宏**********************/

#define FUNCTION_PREFIX			                    "~"			//函数前缀符号
#define SUBMENU_PREFIX				                ">"			//菜单前缀符号
#define RADIO_PREFIX				                "*"			//单选框前缀符号
#define NONE_PREFIX					                "-"			//无操作前缀符号

#define LINEPERFIX_DISTANCE			                (2)			//行前缀符号与后方文字的距离

/************关于长按不放状态的宏**********************/
#define KEY_UP_LONGPRESS_NONE                       (0)            //上没有按下
#define KEY_UP_LONGPRESS_SHORT                      (1)            //上短时间按下
#define KEY_UP_LONGPRESS_LONG                       (2)            //上长时间按下
#define KEY_DOWN_LONGPRESS_NONE                     (3)            //下没有按下
#define KEY_DOWN_LONGPRESS_SHORT                    (4)            //下短时间按下
#define KEY_DOWN_LONGPRESS_LONG                     (5)            //下长时间按下

/************关于动画类型的宏**********************/
#define ANIM_MENU                                   (0)
#define ANIM_WINDOW                                 (1)

/*表示菜单类型的数据结构*/
typedef int8_t OLED_UI_MenuStyle;

/*表示菜单ID的数据结构*/
typedef int16_t OLED_UI_MenuID;

/*表示菜单移动动画速度的数据结构*/
typedef float OLED_UI_MenuMovingSpeed;

/*表示菜单ID的数据结构*/
typedef int8_t OLED_UI_Font;

/*表示互斥锁或标志位的数据结构D*/
typedef int8_t OLED_UI_MutexFlag;

/*计数器结构体 */
typedef struct OLED_UI_Counter{
    int16_t Counter;
    int16_t Time;
    int16_t Value;
} OLED_UI_Counter;


/**OLED_UI当中用于实现窗口停留的结构体
 * 值得注意的是，当SustainFlag为false的时候，当前窗口指针并不一定是NULL，而是有可能正在退出，
 */
typedef struct OLED_UI_WindowSustainCounter{
	int16_t Counter;		//计数器
	bool SustainFlag;	//标志位，用于表示是否处于停留状态
}OLED_UI_WindowSustainCounter;

/*长时间按住不放的结构体*/
typedef struct OLED_UI_LongPress{
    int16_t DeltaData;      // 数据增量
    int16_t Counter;        // 计数器
}OLED_UI_LongPress;


// /*OLED_UI当中有关按键控制的结构体*/

typedef struct  OLED_UI_KeyControl{
    int8_t Status;	    //确认键状态
    int16_t SetClickTime;	//设置单击超时时间
    int16_t SetLongPressTime;	//设置长按时间
    
    int8_t _CurrentStatus;     //当前按键状态
    int8_t _LastStatus;        //上一次按键状态
    int8_t _ClickTimerFlag;	//确认键定时器标志位
    int8_t _ClickFrequency;	//确认键点击次数
    int16_t _ClickTime;	//确认键按下时间
} OLED_UI_KeyControl;




/*OLED_UI当中有点坐标的结构体*/
typedef struct OLED_UI_Point{
	float X; 
	float Y;
} OLED_UI_Point;

/*OLED_UI当中有关位置与大小的结构体*/
typedef struct  OLED_UI_Area{
    float X;
    float Y;
    float Width;
    float Height;
} OLED_UI_Area;

/*OLED_UI当中存放光标信息的结构体【用于制造光标移动效果】 */
typedef struct OLED_UI_AreaAnimation{
	OLED_UI_Area CurrentArea;	//当前光标区域
	OLED_UI_Area TargetArea;		//目标光标区域
	OLED_UI_Area _Error;			//误差值
	OLED_UI_Area _LastError;		//上一次的误差值
	OLED_UI_Area _Integral;		//积分值
	OLED_UI_Area _Derivative;		//微分值

}OLED_UI_AreaAnimation;

typedef struct OLED_UI_PointAnimation{
	OLED_UI_Point CurrentPoint;	//当前点坐标
	OLED_UI_Point TargetPoint;		//目标点坐标
	OLED_UI_Point _Error;			//误差值
	OLED_UI_Point _LastError;		//上一次的误差值
	OLED_UI_Point _Integral;		//积分值
	OLED_UI_Point _Derivative;		//微分值

}OLED_UI_PointAnimation;


typedef struct OLED_UI_DistanceAnimation{
	float CurrentDistance;		//当前值
	float TargetDistance;		//目标值
	float _Error;					//误差值
	float _LastError;				//上一次的误差值
	float _Integral;				//积分值
	float _Derivative;				//微分值

}OLED_UI_DistanceAnimation;



typedef struct MenuPage {
    OLED_UI_MenuStyle       General_MenuType;                                    //菜单类型
    OLED_UI_MenuMovingSpeed General_MovingSpeed;                                //菜单移动速度
    uint8_t                 General_CursorStyle;                                //光标样式
    uint8_t                 General_MoveStyle;                                    //移动样式
    OLED_UI_Font            General_FontSize;                                    //字体大小
    struct MenuPage*        General_ParentMenuPage;                            //父菜单页面
    struct MenuItem*        General_MenuItems;                                  //菜单项
    int16_t                 General_LineSpace;                                    //行间距或列间距
    OLED_UI_Area            General_MenuArea;                                //列表菜单区域
    uint8_t                 General_MenuFrameStyle;                           //绘制区域外框类型
    int16_t                 General_InitMenuID;                             //初始菜单项ID
    int16_t                 General_InitSlot;                              //初始槽位
    OLED_UI_Point           General_StartPoint;                            //列表项起始坐标【相对于菜单区域起始位置的坐标】
    void                    (*General_ShowAuxiliaryFunction)(void);             //辅助显示函数

    bool                    List_IfDrawLinePerfix;                          //是否绘制列表项前缀线

    int16_t Tiles_TileWidth;                          //磁贴宽度
    int16_t Tiles_TileHeight;                         //磁贴高度
    int16_t Tiles_FontTopDistance;                    //文字距离距页面离顶部距离
    int16_t Tiles_ScrollBarTopDistance;               //滚动条距离页面离顶部距离


    OLED_UI_MenuID _ActiveMenuID;                    //当前激活菜单ID
    OLED_UI_MenuID _Slot;                            //槽位
    OLED_UI_Point _StartPoint;                      //起始坐标
	bool _IfInit;                                     //是否初始化
} MenuPage;

typedef struct MenuItem {
    const char* General_item_text;                        //菜单项文本
    void (*General_callback)(void);                  //菜单项回调函数
    MenuPage* General_SubMenuPage;                  //子菜单页面

    bool* List_BoolRadioBox;                         //布尔单选框
    const uint8_t* Tiles_Icon;                      //磁贴图标

    float _LineSlip;                                 //行移位
    // float _LineSlip2;                                 //行移位
} MenuItem;

typedef struct MenuWindow{
    int16_t General_Width;                            //[通用属性]窗口宽度
    int16_t General_Height;                           //[通用属性]窗口高度
    float   General_StayTime;                             //[通用属性]窗口停留时间
    uint8_t General_FrameStyle;                             //[通用属性]窗口类型

    const char* Title_text;                                //[标题属性]标题文本
    uint8_t Title_FontSize;                            //[标题属性]字体大小
    int16_t Title_FontSideDistance;						//[标题属性]字体距离左侧的距离
	int16_t Title_FontTopDistance;						//[标题属性]字体距离顶部的距离

    uint8_t Prob_FontSize;                                //[进度条属性]字体大小
    uint8_t Prob_FloatPointLength;                        //[进度条属性]浮点型数据的小数点长度
    float* Prob_Data_Float;                                //[数据显示属性]浮点型数据【五选一】
    int8_t* Prob_Data_Int_8_t;                                //[数据显示属性]8位整型数据【五选一】
    int16_t* Prob_Data_Int_16_t;                                //[数据显示属性]16位整型数据【五选一】
    int32_t* Prob_Data_Int_32_t;                                //[数据显示属性]32位整型数据【五选一】
    int64_t* Prob_Data_Int_64_t;                                //[数据显示属性]64位整型数据【五选一】
    float Prob_DataStep;									//[进度条属性]窗口数据步长
	float Prob_MinData;									//[进度条属性]窗口数据的最小值
	float Prob_MaxData;									//[进度条属性]窗口数据的最大值
    int16_t Prob_SideDistance;								//[进度条属性]进度条边缘距离窗口边缘的距离
	int16_t Prob_LineHeight;								//[进度条属性]进度条线条高度
    int16_t Prob_BottomDistance;							//[进度条属性]进度条底部距离窗口底部的距离

    float _LineSlip;                                        //[私有属性]用于滚动显示长文本

} MenuWindow;


#ifdef __cplusplus
extern "C" {
#endif

// 返回函数
void OLED_UI_BackTask(void);

// OLED_UI主循环
void OLED_UI_MainLoop(void);

// 创建窗口
void OLED_UI_CreateWindow(MenuWindow* window);

// 初始化OLED_UI
void OLED_UI_Init(MenuPage* Page);

// 中断处理函数
void OLED_UI_InterruptHandler(void);



#ifdef __cplusplus
}  // extern "C"
#endif

#endif
