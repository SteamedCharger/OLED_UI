#include "OLED_UI.h"


/*=======================================OLED-UI全局数据结构======================================*/
/*=======================================OLED-UI全局数据结构======================================*/
/*=======================================OLED-UI全局数据结构======================================*/


// 全局变量，创建按键状态控制结构体.由于是上拉输入，所以要将当前与上次状态都初始化为0
OLED_UI_KeyControl OLED_UI_Enter={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_ENTER_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_ENTER_LONG_PRESS_TIME_MS};
OLED_UI_KeyControl OLED_UI_Back={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_BACK_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_BACK_LONG_PRESS_TIME_MS};
OLED_UI_KeyControl OLED_UI_Up={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_UP_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_UP_LONG_PRESS_TIME_MS};
OLED_UI_KeyControl OLED_UI_Down={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_DOWN_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_DOWN_LONG_PRESS_TIME_MS};
OLED_UI_KeyControl OLED_UI_EncoderKey={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_ENCODER_KEY_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_ENCODER_KEY_LONG_PRESS_TIME_MS};

// 全局变量，存储帧数
OLED_UI_Counter OLED_UI_FPS = {.Counter = 0,.Time = 0,.Value = 0};

OLED_UI_WindowSustainCounter OLED_UI_WindowStatus = {.SustainFlag = false};

// 全局变量，存储菜单ID偏移量
int16_t OLED_UI_DeltaMenuID = 0;

// 全局变量，长按标志位
OLED_UI_LongPress OLED_UI_UpDownLongPress = {.DeltaData = 0,.Counter = 0};



// 全局结构体指针，指向当前菜单页面
MenuPage *CurrentMenuPage = NULL;

// 全局结构体指针，指向当前菜单窗口
MenuWindow *CurrentWindow = NULL;

/*=======================================OLED-UI全局数据结构======================================*/
/*=======================================OLED-UI全局数据结构======================================*/
/*=======================================OLED-UI全局数据结构======================================*/

/*需要动画效果的结构体注册为OLED_UI_xxxAnimation全局结构体*/

// OLED_UI_Cursor是用于存储光标的结构体，控制光标的移动
OLED_UI_AreaAnimation OLED_UI_Cursor;

// OLED_UI_MenuFrame是用于存储菜单边框的结构体，控制菜单边框与内部组件的移动
OLED_UI_AreaAnimation OLED_UI_MenuFrame;

// OLED_UI_Window是用于存储窗口的结构体，控制窗口的移动
OLED_UI_AreaAnimation OLED_UI_Window;

// OLED_UI_ScrollBarHeight是用于存储滚动条高度的结构体，控制滚动条高度的变化
OLED_UI_DistanceAnimation OLED_UI_ScrollBarHeight;

// OLED_UI_ScrollBarStartPoint是用于存储滚动条起始点的结构体，控制滚动条整体的移动
OLED_UI_PointAnimation OLED_UI_ScrollBarStartPoint;

// OLED_UI_PageStartPoint是用于存储页面起始点的结构体，控制页面整体的移动
OLED_UI_PointAnimation OLED_UI_PageStartPoint;

// OLED_UI_ProbWidth是用于存储进度条宽度的结构体，控制进度条宽度的变化
OLED_UI_DistanceAnimation OLED_UI_ProbWidth;

// OLED_UI_LineStep是用于存储行间距的结构体，控制行间距的变化
OLED_UI_DistanceAnimation OLED_UI_LineStep;

// OLED_UI_Tiles_FontTopDistance是用于存储字体顶部距离的结构体，控制字体顶部距离的变化[此贴类专有]
OLED_UI_DistanceAnimation OLED_UI_Tiles_FontTopDistance;

/*=======================================OLED-UI工具函数======================================*/
    /*===================================OLED-UI工具函数==================================*/
        /*===============================OLED-UI工具函数==============================*/

/**
 * @brief 计算字符串的宽度（可以是中英文字符串）
 * @param Font 字体高度
 * @param format 格式化字符串
 * @return 字符串的宽度
 */
int16_t OLED_UI_CalcStringWidth(int8_t Font, const char *format, ...) {
    int16_t StringLength = 0;
    char String[MAX_STRING_LENGTH]; // 固定大小缓冲区
    uint8_t ASCIIFont = OLED_GetFontWidth(Font, OLED_ASCII);
    uint8_t ChineseFont = OLED_GetFontWidth(Font, OLED_CHINESE);
    
    va_list args;
    va_start(args, format);
    #if USE_SIMPLE_VSPRINTF
    OLED_Simple_vsprintf(String, format, args);
    #else
    vsprintf(String, format, args);
    #endif
    va_end(args);

    char *ptr = String;
    while (*ptr != '\0') {
        if ((unsigned char)*ptr & 0x80) { // 处理中文字符
            StringLength += ChineseFont;
            ptr += OLED_CHN_CHAR_WIDTH;
        } else {
            StringLength += ASCIIFont;
            ptr++;
        }
    }
    return StringLength;
}



/**
 * @brief 渐隐清除屏幕
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param width 宽度
 * @param height 高度
 * @note 渐隐清除屏幕，应用渐隐效果，显示更新，延时，重复5次。
 */
void OLED_UI_ClearAreaWithFadeOut(int16_t x, int16_t y, int16_t width, int16_t height){
    // 如果速度为0，则直接清除屏幕
    if(CurrentMenuPage->General_MovingSpeed <= 0){
		OLED_Clear();
		return;
	}
    for(uint8_t i=1;i<=5;i++){
		// 应用渐隐效果
			
		OLED_FadeOut_Masking(x,y,width,height,i);
		// 显示更新
        OLED_Update();
		OLED_DelayMs(OLED_UI_FADE_TIME_MS);
    }
    OLED_DelayMs(OLED_UI_FADE_TIME_MS);
}



/**
 * @brief 获取当前屏幕刷新率，结果存储在全局变量OLED_UI_FPS.value中
 * @param 无
 * @note 请确保OLED_UI_Config.h中定义的OLED_UI_INTERRUPT_TIME与实际情况相符此外，此函数需要放在中断当中。
 * @return 无
 */
void OLED_UI_CountFPS_Interrupt(void){
	if(OLED_UI_FPS.Time< 1000/OLED_UI_INTERRUPT_TIME -1){
		OLED_UI_FPS.Time++;
	}else{
		OLED_UI_FPS.Time=0;
		OLED_UI_FPS.Value = OLED_UI_FPS.Counter;
		OLED_UI_FPS.Counter=0;
	}
}

/**
 * @brief 显示FPS
 * @param 无
 * @note 在屏幕右上角显示当前帧率。
 */
void OLED_UI_ShowFPS(void)
{
    int8_t StringLength = OLED_UI_CalcStringWidth(OLED_FONT_8, "FPS:%d", OLED_UI_FPS.Value);
    OLED_Printf(OLED_WIDTH - StringLength, 0, OLED_FONT_8, "FPS:%d", OLED_UI_FPS.Value);

}

/**
 * @brief 处理按键状态，使用传递指针的方式实现复用
 * @param key 按键控制结构体指针
 * @note 处理按键状态，包括单击、双击、长按、短按、长按释放等。
 */
void OLED_UI_KeyControlProcess(OLED_UI_KeyControl *key)
{

        //【确认】按键处理逻辑
    if (key->_CurrentStatus != key->_LastStatus) {
        if (key->_CurrentStatus == 1) { 
            // 按键释放事件
            /* 新增：长按后的释放处理 */
            if (key->_ClickFrequency == 2) {
                // 清除长按特殊标记并重置状态
                key->_ClickFrequency = 0;
                key->_ClickTime = 0;
                key->_ClickTimerFlag = 0;
                return; // 直接退出，不触发后续逻辑
            }

            if (key->Status != OLED_UI_KEY_LONG_PRESS) {
                // 第一次释放：启动双击检测窗口
                if (key->_ClickFrequency == 0) {
                    key->_ClickFrequency = 1;  // 记录第一次点击
                    key->_ClickTimerFlag = 1;  // 启动双击超时检测
                    key->_ClickTime = 0;       // 重置计时器
                } 
                // 第二次释放：判定双击
                else {
                    if (key->_ClickTime <= key->SetClickTime/OLED_UI_INTERRUPT_TIME) { // 500ms内
                        key->Status = OLED_UI_KEY_DOUBLE_CLICK;
                    }
                    // 无论是否双击，处理完成后重置变量
                    key->_ClickFrequency = 0;
                    key->_ClickTime = 0;
                    key->_ClickTimerFlag = 0;
                }
            }
        } else { 
            // 按键按下事件
            // 初始化长按检测（但不重置点击次数）
            key->_ClickTimerFlag = 1;  // 启动长按计时
            key->_ClickTime = 0;       // 重置计时器
            key->Status = OLED_UI_KEY_NONE; // 清除旧状态
        }
    }

    if (key->_ClickTimerFlag) {
        // 先解引用再累加
        (key->_ClickTime) ++;

        // 长按检测（仅在按键仍处于按下状态时生效）
        if (key->_CurrentStatus == 0) { 
            // 长按阈值（假设50*20ms=1000ms）
            if (key->_ClickTime  >= key->SetLongPressTime/OLED_UI_INTERRUPT_TIME) {
                key->Status = OLED_UI_KEY_LONG_PRESS;
                // 触发长按后强制重置所有状态，并标记长按特殊状态
                key->_ClickFrequency = 2;  // 新增长按标记
                key->_ClickTime  = 0;
                key->_ClickTimerFlag = 0;
            }
        }
        // 双击超时检测（仅在按键已释放时生效）
        else { 
            // 超时阈值（25*20ms=500ms）
            if (key->_ClickTime  > key->SetClickTime/OLED_UI_INTERRUPT_TIME) {
                // 未触发长按时标记单击
                if (key->Status != OLED_UI_KEY_LONG_PRESS) {
                    key->Status = OLED_UI_KEY_CLICK;
                }
                // 重置状态
                key->_ClickFrequency = 0;
                key->_ClickTime  = 0;
                key->_ClickTimerFlag = 0;
            }
        }
    }

    // 限制长按时间（防止溢出）
    if(key->_ClickTime >= key->SetLongPressTime/OLED_UI_INTERRUPT_TIME + 1){
        key->_ClickTime = key->SetLongPressTime/OLED_UI_INTERRUPT_TIME + 1;
    }

}

/**
 * @brief 根据当前所选的动画方式，改变浮点数参数
 * @param CurrentNum 当前值的指针
 * @param TargetNum 目标值指针
 * @param StartNum 起始值指针
 * @param CurrentStepNum 当前步数指针
 * @return 无
 */
void OLED_UI_ChangeFloat(float *CurrentNum, float *TargetNum, float *ErrorNum, float *LastErrorNum, float * IntegralNum, float *DerivativeNum)  {
	if(CurrentMenuPage->General_MoveStyle ==  UNLINEAR){
		if(*CurrentNum == *TargetNum){
			return;
		}
		//如果用户将速度设置为0，那么当前值直接等于目标值
		if(CurrentMenuPage->General_MovingSpeed <= 0){
			*ErrorNum = 0;
			
		    *CurrentNum = *TargetNum;
			return;
		}
		
		//计算本轮误差值
		*ErrorNum = *TargetNum - *CurrentNum; 
		//计算当前值
		*CurrentNum += 0.02*CurrentMenuPage->General_MovingSpeed * (*ErrorNum);
		//当目标值与当前值差距小于速度值的1/20时，认为已经到达目标值
		if(fabs(*CurrentNum - *TargetNum) < fmaxf(CurrentMenuPage->General_MovingSpeed / 20.0f, 0.5f)) {

			*ErrorNum = 0;
			
		    *CurrentNum = *TargetNum;
			return;
		}
	}
	//如果当前的动画方式是PID_CURVE方式
	if(CurrentMenuPage->General_MoveStyle ==  PID_CURVE){
		/*这是一种奇特的方法，因为当当前值等于目标值的时候，其他项置零了，但是积分项并没有被置零。根据实际现象，这样的效果是最好的。 */
		//如果用户将速度设置为0，那么当前值直接等于目标值，其他所有中间值置零
		if(CurrentMenuPage->General_MovingSpeed <= 0){
			*ErrorNum = 0;
			*LastErrorNum = 0;
			*DerivativeNum = 0;
			*IntegralNum = 0;
		    *CurrentNum = *TargetNum;
			return;
		}
		// 定义PID参数
		//定义PID参数
		float Kp = OLED_UI_PID_ANIM_KP * CurrentMenuPage->General_MovingSpeed;
		float Ki = OLED_UI_PID_ANIM_KI * CurrentMenuPage->General_MovingSpeed;
		float Kd = OLED_UI_PID_ANIM_KD * CurrentMenuPage->General_MovingSpeed;
		//记录上一轮误差值
		*LastErrorNum = *ErrorNum;
		//计算本轮误差值
		*ErrorNum = *TargetNum - *CurrentNum; 
		//计算积分值
		*IntegralNum += *ErrorNum;
		//计算微分值，并假设时间间隔为0.1s
		*DerivativeNum = (*ErrorNum - *LastErrorNum) / 0.1;
		//计算当前值
		*CurrentNum += Kp * (*ErrorNum) + Ki *  (*IntegralNum) + Kd *(*DerivativeNum);
		//当目标值与当前值差距小于0.5时，将目标值强制等于当前值，除了积分项不置零，其他所有中间值置零
		if(fabs(*TargetNum - *CurrentNum) < 0.5f){
			*ErrorNum = 0;
			*LastErrorNum = 0;
			*DerivativeNum = 0;
		    *CurrentNum = *TargetNum;
			return;
		}	
	}
	
}

/**
 * @brief 改变距离参数
 * @param CurrentNum 当前值的指针
 * @param TargetNum 目标值指针
 * @param StepNum 步长指针
 */
void OLED_UI_ChangeDistance(OLED_UI_DistanceAnimation *distance){
	OLED_UI_ChangeFloat(&distance->CurrentDistance,&distance->TargetDistance,&distance->_Error,&distance->_LastError,&distance->_Integral,&distance->_Derivative);
}

/**
 * @brief 改变点坐标参数
 * @param OLED_MovingArea 结构体数组的指针
 * @return 无
 */
void OLED_UI_ChangePoint(OLED_UI_PointAnimation *point){
	OLED_UI_ChangeFloat(&point->CurrentPoint.X,&point->TargetPoint.X,&point->_Error.X,&point->_LastError.X,&point->_Integral.X,&point->_Derivative.X);
	OLED_UI_ChangeFloat(&point->CurrentPoint.Y,&point->TargetPoint.Y,&point->_Error.Y,&point->_LastError.Y,&point->_Integral.Y,&point->_Derivative.Y);
}

/**
 * @brief 改变区域参数
 * @param OLED_MovingArea 结构体数组的指针
 * @return 无
 */
void OLED_UI_ChangeArea(OLED_UI_AreaAnimation *area)	{
	OLED_UI_ChangeFloat(&area->CurrentArea.X,&area->TargetArea.X,&area->_Error.X,&area->_LastError.X,&area->_Integral.X,&area->_Derivative.X);
	OLED_UI_ChangeFloat(&area->CurrentArea.Y,&area->TargetArea.Y,&area->_Error.Y,&area->_LastError.Y,&area->_Integral.Y,&area->_Derivative.Y);
	OLED_UI_ChangeFloat(&area->CurrentArea.Width,&area->TargetArea.Width,&area->_Error.Width,&area->_LastError.Width,&area->_Integral.Width,&area->_Derivative.Width);
	OLED_UI_ChangeFloat(&area->CurrentArea.Height,&area->TargetArea.Height,&area->_Error.Height,&area->_LastError.Height,&area->_Integral.Height,&area->_Derivative.Height);
}



/** 
* @brief 获取当前菜单页面可以容纳的的槽位数量
* @param 无
* @return 当前菜单页面的槽位数量
*/
int16_t OLED_UI_GetMaxSlotNum(void){
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
        return (int16_t)(CurrentMenuPage->General_MenuArea.Height - CurrentMenuPage->General_StartPoint.Y+OLED_UI_LineStep.TargetDistance-1) / (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->General_FontSize );
    }else if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        return (int16_t)(CurrentMenuPage->General_MenuArea.Width - CurrentMenuPage->General_StartPoint.X+OLED_UI_LineStep.TargetDistance-1) / (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->Tiles_TileWidth );
    }else{
			return 0;
		}
 }
 
 /**
  * @brief 获取菜单项结构体数组的最后一个元素的ID
  * @param 无
  * @return 菜单项结构体数组的最后一项的ID
  */
int16_t OLED_UI_GetMaxMenuItemNum(void){
    int16_t num = 0;
    while(CurrentMenuPage->General_MenuItems[num].General_item_text != NULL){
        num+=1;
    }
    return num - 1;
}
/**
 * @brief 菜单项滚动动画结束判断
 * @param 无
 * @note 判断菜单项滚动动画是否结束
 * @return 无
 */
bool OLED_UI_IfAnimationEnd(uint8_t AnimationType){
    // 如果当前正在运行窗口
    if(AnimationType == ANIM_WINDOW){
        return (OLED_UI_Window.CurrentArea.Width == OLED_UI_Window.TargetArea.Width && 
            OLED_UI_Window.CurrentArea.Height == OLED_UI_Window.TargetArea.Height && 
            OLED_UI_Window.CurrentArea.X == OLED_UI_Window.TargetArea.X && 
            OLED_UI_Window.CurrentArea.Y == OLED_UI_Window.TargetArea.Y);
    }else if(AnimationType == ANIM_MENU){

        return OLED_UI_MenuFrame.CurrentArea.Width == OLED_UI_MenuFrame.TargetArea.Width && 
            OLED_UI_MenuFrame.CurrentArea.Height == OLED_UI_MenuFrame.TargetArea.Height && 
            OLED_UI_MenuFrame.CurrentArea.X == OLED_UI_MenuFrame.TargetArea.X && 
            OLED_UI_MenuFrame.CurrentArea.Y == OLED_UI_MenuFrame.TargetArea.Y &&  
            OLED_UI_PageStartPoint.CurrentPoint.X == OLED_UI_PageStartPoint.TargetPoint.X;
    }else{
			return 0;
		}

}




/**
 * @brief 设置滚动条高度/宽度
 * @param 无
 * @note 该函数用于设置滚动条的高度/宽度
 * @return 无
 */
void OLED_UI_SetTargetScrollBarHeight(void)
{
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
		OLED_UI_ScrollBarHeight.TargetDistance = (float)OLED_UI_MenuFrame.CurrentArea.Height* (CurrentMenuPage->_ActiveMenuID + 1)/(OLED_UI_GetMaxMenuItemNum() + 1);
	}else if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        OLED_UI_ScrollBarHeight.TargetDistance = (float)OLED_UI_MenuFrame.CurrentArea.Width* (CurrentMenuPage->_ActiveMenuID + 1)/(OLED_UI_GetMaxMenuItemNum() + 1);
    }
}

/**
 * @brief 设置磁贴页面菜单项文字的起始点
 * @param 无
 * @note 该函数用于设置磁贴页面菜单项文字的起始点
 * @return 无
 */
void OLED_UI_SetTiles_FontTopDistance(void){
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        OLED_UI_Tiles_FontTopDistance.TargetDistance = OLED_UI_MenuFrame.TargetArea.Y + CurrentMenuPage->Tiles_FontTopDistance;
    }
}

/**
 * @brief 设置滚动条起始点
 * @param 无
 * @note 该函数用于设置滚动条的起始点
 */
void OLED_UI_SetTargetScrollBarStartPoint(void){

    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        OLED_UI_ScrollBarStartPoint.TargetPoint.X = OLED_UI_MenuFrame.CurrentArea.X;
        OLED_UI_ScrollBarStartPoint.TargetPoint.Y = CurrentMenuPage->Tiles_ScrollBarTopDistance + OLED_UI_MenuFrame.CurrentArea.Y ;
    }

}

/**
 * @brief 设置菜单区域高度/宽度
 * @note 该函数用于设置菜单区域的高度/宽度
 * @return 无
 */
void OLED_UI_SetTargetMenuArea(void)
{
    OLED_UI_MenuFrame.TargetArea.X = CurrentMenuPage->General_MenuArea.X;
    OLED_UI_MenuFrame.TargetArea.Y = CurrentMenuPage->General_MenuArea.Y;
	OLED_UI_MenuFrame.TargetArea.Height = CurrentMenuPage->General_MenuArea.Height;
	OLED_UI_MenuFrame.TargetArea.Width = CurrentMenuPage->General_MenuArea.Width;
	
}

/**
 * @brief 设置光标区域
 * @param 无
 * @note 该函数用于设置光标的目标显示区域
 * @return 无
 */
void OLED_UI_SetTargetCursorArea(void)
{
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST)
    {
        //目标光标x坐标等于 开始打印页面起始点的x坐标减1【减去1是为了确保光标覆盖到菜单项的文字】
		OLED_UI_Cursor.TargetArea.X = OLED_UI_PageStartPoint.TargetPoint.X -1;

        //目标光标y坐标等于 开始打印页面起始点的y坐标  加上  (字体高度 加 行距)乘 当前页面的ID号 减1【减去1是为了确保光标覆盖到菜单项的文字】
		OLED_UI_Cursor.TargetArea.Y = OLED_UI_PageStartPoint.TargetPoint.Y + CurrentMenuPage->_ActiveMenuID * ( CurrentMenuPage->General_LineSpace + CurrentMenuPage->General_FontSize) -1 ;

        //目标光标高度等于 字体高度 加2【加2是为了确保光标覆盖到菜单项的文字】
		OLED_UI_Cursor.TargetArea.Height = CurrentMenuPage->General_FontSize + 2;

        int8_t LinePerfixWidth = 0;
        int8_t RadioCompensationWidth = 0;
		if(CurrentMenuPage->List_IfDrawLinePerfix == true){
			LinePerfixWidth = OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_ASCII) + LINEPERFIX_DISTANCE;
		}else{
            LinePerfixWidth = 0;
        }
        //如果需要绘制单选框(即BoolRadioBox不为空)
        if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox != NULL){
            RadioCompensationWidth = (OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_CHINESE) + 2);
        }else{
            RadioCompensationWidth = 0;
        }

        OLED_UI_Cursor.TargetArea.Width =  fmin((float)
            OLED_UI_CalcStringWidth(CurrentMenuPage->General_FontSize,CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_item_text)
            + LinePerfixWidth + 2, 
			//当前页面的宽度 加 当前页面的起始坐标 减去开始打印页面起始点的坐标 减去6（是滚动条宽度加一）加上行前缀的宽度
			OLED_UI_MenuFrame.TargetArea.Width + OLED_UI_MenuFrame.TargetArea.X - OLED_UI_PageStartPoint.TargetPoint.X - 6 - RadioCompensationWidth ) ;
    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES)
    {
        //目标光标Y坐标等于 开始打印页面起始点的Y坐标减1【减去1是为了确保光标覆盖到菜单项的文字】
		OLED_UI_Cursor.TargetArea.Y = OLED_UI_PageStartPoint.TargetPoint.Y - 3;

        //目标光标y坐标等于 开始打印页面起始点的X坐标  加上  (磁贴高度 加 行距)乘 当前页面的ID号 减1【减去1是为了确保光标覆盖到菜单项的文字】
        OLED_UI_Cursor.TargetArea.X = OLED_UI_PageStartPoint.TargetPoint.X + CurrentMenuPage->_ActiveMenuID * ( CurrentMenuPage->General_LineSpace + CurrentMenuPage->Tiles_TileWidth) - 3 ;

        //目标光标高度等于 磁贴高度 加2【加2是为了确保光标覆盖到菜单项的文字】
		OLED_UI_Cursor.TargetArea.Height = CurrentMenuPage->Tiles_TileHeight + 6;

        //目标光标宽度等于 磁贴宽度 加2【加2是为了确保光标覆盖到菜单项的文字】
		OLED_UI_Cursor.TargetArea.Width = CurrentMenuPage->Tiles_TileWidth + 6;

    }

}

/**
 * @brief 将当前页面的LineSlip 设置为0
 * @param 无
 * @return 无
 */
void OLED_UI_SetLineSplitZero(void){

	for(int16_t i = 0; i<=OLED_UI_GetMaxMenuItemNum();i++){
		CurrentMenuPage->General_MenuItems[i]._LineSlip = 0;
	}
}

/**
 * @brief 将菜单整体向上或左移动一行
 * @param 无
 * @return 无
 */
void OLED_UI_MenuItemsMoveMinus(void){
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
	    OLED_UI_PageStartPoint.TargetPoint.Y -= (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->General_FontSize);
    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
	    OLED_UI_PageStartPoint.TargetPoint.X -= (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->Tiles_TileWidth);
    }
}
/**
 * @brief 将菜单整体向下或右移动一行
 * @param 无
 * @return 无
 */
void OLED_UI_MenuItemsMovePlus(void){
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
	    OLED_UI_PageStartPoint.TargetPoint.Y += (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->General_FontSize);
    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
	    OLED_UI_PageStartPoint.TargetPoint.X += (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->Tiles_TileWidth);
    }
}

/**
 * @brief 获取与处理输入
 * @param 无
 * @note 请保证OLED_UI_Config.h中定义的OLED_UI_INTERRUPT_TIME与实际情况相符。
 * @note 该函数在定时器中断中调用，用于处理输入。
 * @return 无
 */
void OLED_UI_HandleInput_Interrupt(void)
{
    //记录上一轮的按键状态
    OLED_UI_Enter._LastStatus = OLED_UI_Enter._CurrentStatus;
    OLED_UI_Back._LastStatus = OLED_UI_Back._CurrentStatus;
    OLED_UI_Up._LastStatus = OLED_UI_Up._CurrentStatus;
    OLED_UI_Down._LastStatus = OLED_UI_Down._CurrentStatus;
    OLED_UI_EncoderKey._LastStatus = OLED_UI_EncoderKey._CurrentStatus;


    //获取按键状态
    OLED_UI_Enter._CurrentStatus = Key_GetEnterStatus();
    OLED_UI_Back._CurrentStatus = Key_GetBackStatus();
    OLED_UI_Up._CurrentStatus = Key_GetUpStatus();
    OLED_UI_Down._CurrentStatus = Key_GetDownStatus();
    OLED_UI_EncoderKey._CurrentStatus = Key_GetEncoderStatus();

    //处理按键状态
    OLED_UI_KeyControlProcess(&OLED_UI_Enter);
    OLED_UI_KeyControlProcess(&OLED_UI_Back);
    OLED_UI_KeyControlProcess(&OLED_UI_Up);
    OLED_UI_KeyControlProcess(&OLED_UI_Down);
    OLED_UI_KeyControlProcess(&OLED_UI_EncoderKey);

    
}

/**
 * @brief 反转显示光标
 * @param X 光标X坐标
 * @param Y 光标Y坐标
 * @param Width 光标宽度
 * @param Height 光标高度
 * @param Style 光标样式
 * @note 该函数用于反转显示光标，使其显示为反色
 * @return 无
 */
void OLED_UI_ReverseCoordinate(int16_t X, int16_t Y, int16_t Width, int16_t Height,uint8_t Style){
    // 对光标进行限幅
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
        if(X + Width >= OLED_UI_MenuFrame.CurrentArea.X + OLED_UI_MenuFrame.CurrentArea.Width -6){
            Width = OLED_UI_MenuFrame.CurrentArea.X + OLED_UI_MenuFrame.CurrentArea.Width - 6 - X;
        }
        if(X  <= OLED_UI_MenuFrame.CurrentArea.X){
            
            Width = X + Width - OLED_UI_MenuFrame.CurrentArea.X;
            X = OLED_UI_MenuFrame.CurrentArea.X;
        }
        if(Y + Height >= OLED_UI_MenuFrame.CurrentArea.Y + OLED_UI_MenuFrame.CurrentArea.Height){
            Height = OLED_UI_MenuFrame.CurrentArea.Y + OLED_UI_MenuFrame.CurrentArea.Height - Y;
        }
        if(Y <= OLED_UI_MenuFrame.CurrentArea.Y){
            Height = Y + Height - OLED_UI_MenuFrame.CurrentArea.Y;
            Y = OLED_UI_MenuFrame.CurrentArea.Y;
          
        }
        
    }
	switch(Style){
		
		case CURSOR_REVERSE_RECTANGLE://矩形反色
			OLED_ReverseArea(X, Y, Width, Height);
			break;
		case CURSOR_REVERSE_ROUNDRECTANGLE://圆角矩形反色
			if(Width >= 3 && Height >= 3){
				OLED_ReverseArea(X, Y, 1, 1);
				OLED_ReverseArea(X+Width-1, Y, 1, 1);
				OLED_ReverseArea(X, Y+Height-1, 1, 1);
				OLED_ReverseArea(X+Width-1, Y+Height-1, 1, 1);
			}
			OLED_ReverseArea(X, Y, Width, Height);
			break;

		case CURSOR_HOLLOW_RECTANGLE://空心矩形
			OLED_ReverseArea(X, Y, Width, Height);
			OLED_ReverseArea(X+1, Y+1, Width-2, Height-2);
			break;
		case CURSOR_HOLLOW_ROUNDRECTANGLE://空心矩形
			OLED_ReverseArea(X, Y, Width, Height);
			OLED_ReverseArea(X+1, Y+1, Width-2, Height-2);
			if(Width >= 3){
				OLED_ReverseArea(X, Y, 1, 1);
				OLED_ReverseArea(X+Width-1, Y, 1, 1);
			}
			if(Height >= 3){
				OLED_ReverseArea(X, Y+Height-1, 1, 1);
				OLED_ReverseArea(X+Width-1, Y+Height-1, 1, 1);
			}
			break;
		case CURSOR_REVERSE_BLOCK://小方块
			
			OLED_ReverseArea(X, Y + 1, OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_ASCII), Height);
		
			break;
        case CURSOR_ARROW://箭头
            OLED_ShowImageArea(
                OLED_UI_MenuFrame.CurrentArea.X,
                OLED_UI_MenuFrame.CurrentArea.Y,
                OLED_UI_MenuFrame.CurrentArea.Width,
                OLED_UI_MenuFrame.CurrentArea.Height,
                X + Width/2 - 3,
                Y - 5,
                6,
                5,
                Arrow

            );

            break;
		case CURSOR_NOT_SHOW:
			break;
		default:
			OLED_ReverseArea(X, Y, Width, Height);
		break;
	}
}

/**
 * @brief 将CurrentMenuPage初始化
 * @param 无
 * @note 将CurrentMenuPage的属性初始化，与全局动画变量构建联系
 * @return 无
 */
void OLED_UI_CurrentMenuPageInit(void){
    // 初始化菜单的目标区域
    OLED_UI_MenuFrame.TargetArea.X = CurrentMenuPage->General_MenuArea.X;
    OLED_UI_MenuFrame.TargetArea.Y = CurrentMenuPage->General_MenuArea.Y;
    OLED_UI_MenuFrame.TargetArea.Width = CurrentMenuPage->General_MenuArea.Width;
    OLED_UI_MenuFrame.TargetArea.Height = CurrentMenuPage->General_MenuArea.Height;
    //初始化行距或磁贴间距
	OLED_UI_LineStep.TargetDistance = CurrentMenuPage->General_LineSpace;
    //初始化当前行间距或磁贴间距
    OLED_UI_LineStep.CurrentDistance = CurrentMenuPage->General_LineSpace;
    //初始化当前活跃给菜单项ID
	CurrentMenuPage->_ActiveMenuID = CurrentMenuPage->General_InitMenuID;
	//初始化当前槽位
	CurrentMenuPage->_Slot = CurrentMenuPage->General_InitSlot;
    // 标记为已经初始化
    CurrentMenuPage->_IfInit = true;

    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
        // 初始化菜单的开始区域目标值
        OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->General_StartPoint.X + CurrentMenuPage->General_MenuArea.X;
        OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->General_MenuArea.Y - (CurrentMenuPage->General_InitMenuID - CurrentMenuPage->General_InitSlot)*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);
        // 初始化菜单的当前值
		OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X+CurrentMenuPage->General_StartPoint.X + OLED_WIDTH;
		OLED_UI_PageStartPoint.CurrentPoint.Y = CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->General_MenuArea.Y - (CurrentMenuPage->General_InitMenuID - CurrentMenuPage->General_InitSlot)*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);
        // 初始化光标的当前区域
        OLED_UI_Cursor.CurrentArea.Width = 0;
        OLED_UI_Cursor.CurrentArea.Height = CurrentMenuPage->General_FontSize + 2;
        OLED_UI_Cursor.CurrentArea.X = CurrentMenuPage->General_MenuArea.X + CurrentMenuPage->General_MenuArea.Width;
        OLED_UI_Cursor.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y + CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->General_InitSlot*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);

    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        // 初始化菜单的目标值
        OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->General_StartPoint.X + CurrentMenuPage->General_MenuArea.X - (CurrentMenuPage->General_InitMenuID - CurrentMenuPage->General_InitSlot)*(CurrentMenuPage->Tiles_TileWidth + CurrentMenuPage->General_LineSpace);
        OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->General_MenuArea.Y;
        // 初始化菜单的当前值
		OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_PageStartPoint.TargetPoint.X + 2*CurrentMenuPage->Tiles_TileWidth;
		OLED_UI_PageStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y - CurrentMenuPage->Tiles_TileHeight;
        // 初始化菜单的当前区域（对于Tiles菜单，如果不设置当前区域，动画会有割裂感）
        OLED_UI_MenuFrame.CurrentArea.X = CurrentMenuPage->General_MenuArea.X;
        OLED_UI_MenuFrame.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y;
        OLED_UI_MenuFrame.CurrentArea.Width = CurrentMenuPage->General_MenuArea.Width;
        OLED_UI_MenuFrame.CurrentArea.Height = CurrentMenuPage->General_MenuArea.Height;
        // 初始化光标的当前区域
        OLED_UI_Cursor.CurrentArea.Width = 0;
        OLED_UI_Cursor.CurrentArea.Height = 0;
        OLED_UI_Cursor.CurrentArea.X = OLED_UI_MenuFrame.CurrentArea.X;
        OLED_UI_Cursor.CurrentArea.Y = OLED_UI_MenuFrame.CurrentArea.Y;
        // 初始化滚动条起始点（对于Tiles菜单，滚动条起始点是特有的属性）
        OLED_UI_ScrollBarStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X;
        OLED_UI_ScrollBarStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y + OLED_UI_MenuFrame.CurrentArea.Height;
        //设置字体到顶部的距离（对于Tiles菜单，字体到顶部的距离是特有的属性）
        OLED_UI_Tiles_FontTopDistance.CurrentDistance = OLED_UI_MenuFrame.TargetArea.Y + OLED_UI_MenuFrame.TargetArea.Height;
        
    }
    
}

/**
 * @brief 将CurrentMenuPage的备份还原
 * @param 无
 * @note 当返回的时候，还原CurrentMenuPage的备份
 * @return 无
 */
void OLED_UI_CurrentMenuPageBackUp(void){
    // 恢复菜单的目标值
    OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->_StartPoint.X;
    OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->_StartPoint.Y;
    //恢复当前行间距
	OLED_UI_LineStep.CurrentDistance = CurrentMenuPage->General_LineSpace;
	//恢复目标行间距
	OLED_UI_LineStep.TargetDistance = CurrentMenuPage->General_LineSpace;
    // 恢复菜单的目标区域
    OLED_UI_MenuFrame.TargetArea.X = CurrentMenuPage->General_MenuArea.X;
    OLED_UI_MenuFrame.TargetArea.Y = CurrentMenuPage->General_MenuArea.Y;
    OLED_UI_MenuFrame.TargetArea.Width = CurrentMenuPage->General_MenuArea.Width;
    OLED_UI_MenuFrame.TargetArea.Height = CurrentMenuPage->General_MenuArea.Height;

    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
        // 恢复菜单的当前值
        OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X + CurrentMenuPage->General_StartPoint.X - OLED_WIDTH;
		OLED_UI_PageStartPoint.CurrentPoint.Y = CurrentMenuPage->_StartPoint.Y;
        // 设置光标的目标区域
        OLED_UI_Cursor.CurrentArea.Width = 0;
        OLED_UI_Cursor.CurrentArea.Height = CurrentMenuPage->General_FontSize + 2;
        OLED_UI_Cursor.CurrentArea.X = CurrentMenuPage->General_MenuArea.X + CurrentMenuPage->General_StartPoint.X;
        OLED_UI_Cursor.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y + CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->_Slot*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);

    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        // 恢复菜单的当前值
        OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_PageStartPoint.TargetPoint.X - 2*CurrentMenuPage->Tiles_TileWidth;
		OLED_UI_PageStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y - CurrentMenuPage->Tiles_TileHeight;
        // 恢复光标的目标区域
        OLED_UI_Cursor.CurrentArea.Width = 0;
        OLED_UI_Cursor.CurrentArea.Height = CurrentMenuPage->General_FontSize + 2;
        OLED_UI_Cursor.CurrentArea.X = CurrentMenuPage->General_MenuArea.X + CurrentMenuPage->General_StartPoint.X;
        OLED_UI_Cursor.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y + CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->_Slot*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);
        // 初始化菜单的当前区域（对于Tiles菜单，如果不设置当前区域，动画会有割裂感）
        OLED_UI_MenuFrame.CurrentArea.X = CurrentMenuPage->General_MenuArea.X;
        OLED_UI_MenuFrame.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y;
        OLED_UI_MenuFrame.CurrentArea.Width = CurrentMenuPage->General_MenuArea.Width;
        OLED_UI_MenuFrame.CurrentArea.Height = CurrentMenuPage->General_MenuArea.Height;
        // 恢复滚动条起始点（对于Tiles菜单，滚动条起始点是特有的属性）
        OLED_UI_ScrollBarStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X;
        OLED_UI_ScrollBarStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y + OLED_UI_MenuFrame.CurrentArea.Height;
        //恢复字体到顶部的距离（对于Tiles菜单，字体到顶部的距离是特有的属性）
        OLED_UI_Tiles_FontTopDistance.CurrentDistance = OLED_UI_MenuFrame.TargetArea.Y + OLED_UI_MenuFrame.TargetArea.Height;

    }
}

/**
 * @brief 此函数处理确认任务
 * @param 无
 * @return 无
 */
void OLED_UI_EnterTask(void){
    // 确认任务类型,0为无操作，1为BoolRadioBox确认任务，2为回调函数确认任务，3为子菜单确认任务.其中子菜单优先级最高。
    int8_t EnterTaskType = 0;
    int8_t Offset = 0;
    int16_t X;
    int16_t Y;
    int16_t Width;
    int16_t Height;

    // 如果当前菜单项有BoolRadioBox，则确认任务类型为1
    if (CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox != NULL){
        EnterTaskType = 1;
    }
    if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_callback != NULL){
        EnterTaskType = 2;
    }
    if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage != NULL){
        EnterTaskType = 3;
    }

    switch(EnterTaskType)
    {
        case 1: // BoolRadioBox确认任务
            *CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox = !(*CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox);
            break;
        case 2: // 回调函数确认任务
            OLED_UI_EncoderDisable();
            CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_callback();
            OLED_UI_EncoderEnable();
            break;

        case 3: // 子菜单确认任务
            if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage->General_MenuType == MENU_TYPE_LIST && CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
                Offset = -5;
            }
            //如果父菜单没有边框并且当前菜单有边框
            if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage->General_MenuFrameStyle == MENU_FRAME_NONE && CurrentMenuPage->General_MenuFrameStyle != MENU_FRAME_NONE ){
                X = OLED_UI_MenuFrame.CurrentArea.X - 1;
                Y = OLED_UI_MenuFrame.CurrentArea.Y - 1;
                Width = OLED_UI_MenuFrame.CurrentArea.Width + 2;
                Height = OLED_UI_MenuFrame.CurrentArea.Height + 2;
            }else{
                X = OLED_UI_MenuFrame.CurrentArea.X;
                Y = OLED_UI_MenuFrame.CurrentArea.Y;
                Width = OLED_UI_MenuFrame.CurrentArea.Width + Offset;
                Height = OLED_UI_MenuFrame.CurrentArea.Height;
            }
            //将当前菜单的位置保存，以便返回时恢复
			CurrentMenuPage->_StartPoint.X = OLED_UI_PageStartPoint.TargetPoint.X;
			CurrentMenuPage->_StartPoint.Y = OLED_UI_PageStartPoint.TargetPoint.Y;
            //清除当前菜单页面
            //清除当前菜单页面
            OLED_UI_ClearAreaWithFadeOut(X,Y,Width,Height);
            // 跳转到子菜单
            CurrentMenuPage = CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage;
            // 将滚动偏移值归零
            OLED_UI_SetLineSplitZero();
            OLED_UI_CurrentMenuPageInit();
            break;
        default:
            break;
    }

    
}

/**
 * @brief 此函数处理取消任务
 * @param 无
 * @return 无
 */
void OLED_UI_BackTask(void){

    if(CurrentMenuPage->General_ParentMenuPage != NULL){
        int16_t X;
        int16_t Y;
        int8_t Offset = 0;
        int16_t Width;
        int16_t Height;
        
        if(CurrentMenuPage->General_ParentMenuPage->General_MenuType == MENU_TYPE_LIST && CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
            Offset = -5;
        }
        //如果父菜单没有边框并且当前菜单有边框
        if(CurrentMenuPage->General_ParentMenuPage->General_MenuFrameStyle == MENU_FRAME_NONE && CurrentMenuPage->General_MenuFrameStyle != MENU_FRAME_NONE ){
            X = OLED_UI_MenuFrame.CurrentArea.X - 1;
            Y = OLED_UI_MenuFrame.CurrentArea.Y - 1;
            Width = OLED_UI_MenuFrame.CurrentArea.Width + 2;
            Height = OLED_UI_MenuFrame.CurrentArea.Height + 2;
        }else{
            X = OLED_UI_MenuFrame.CurrentArea.X;
            Y = OLED_UI_MenuFrame.CurrentArea.Y;
            Width = OLED_UI_MenuFrame.CurrentArea.Width + Offset;
            Height = OLED_UI_MenuFrame.CurrentArea.Height;
        }

        
        
        //清除当前菜单页面
        OLED_UI_ClearAreaWithFadeOut(X,Y,Width,Height);
        // 跳转到父菜单
        CurrentMenuPage = CurrentMenuPage->General_ParentMenuPage;
        // 将滚动偏移值归零
        OLED_UI_SetLineSplitZero();
        if(CurrentMenuPage->_IfInit == false){
            OLED_UI_CurrentMenuPageInit();
        }else{
            OLED_UI_CurrentMenuPageBackUp();
        }
    }

}
/**
 * @brief 获取窗口数据情况
 * @param window
 * @return 浮点值为返回 WINDOW_DATA_STYLE_FLOAT ，int16_t值为返回 WINDOW_DATA_STYLE_INT ，空指针返回 WINDOW_DATA_STYLE_NONE
 */
int8_t OLED_UI_GetWindowProbDataStyle(MenuWindow* window){

	//保护避免访问非法内存
	if(window == NULL){
		return WINDOW_DATA_STYLE_NONE;
	}
	// 如果窗口数据类型为int8_t
	if(window->Prob_Data_Int_8_t != NULL){
		return WINDOW_DATA_STYLE_INT8;
		// 如果窗口数据类型为int16_t
	}else if(window->Prob_Data_Int_16_t != NULL){
		return WINDOW_DATA_STYLE_INT16;
		// 如果窗口数据类型为int32_t
	}else if(window->Prob_Data_Int_32_t != NULL){
		return WINDOW_DATA_STYLE_INT32;
		// 如果窗口数据类型为int64_t
	}else if(window->Prob_Data_Int_64_t != NULL){
		return WINDOW_DATA_STYLE_INT64;
		// 如果窗口数据类型为float
	}else if(window->Prob_Data_Float != NULL){
		return WINDOW_DATA_STYLE_FLOAT;
	}else{
		return WINDOW_DATA_STYLE_NONE;
	}
}
/**
 * @brief 主循环中处理输入
 * @param 无
 * @note 主循环中通过中断当中的按键状态处理结果来执行一些任务
 * @return 无
 */
void OLED_UI_HandleInput_MainLoop(void){
    // 禁止定时器中断,放置变量赋值的时候被定时器中断打断
    OLED_UI_TimerDisable();
    int8_t enter_states = OLED_UI_Enter.Status;
    int8_t back_states = OLED_UI_Back.Status;
    int8_t up_states = OLED_UI_Up.Status;
    int8_t down_states = OLED_UI_Down.Status;
    int8_t encoder_states = OLED_UI_EncoderKey.Status;
    // 获取长按上或下按键所得到的增量
    OLED_UI_DeltaMenuID+=OLED_UI_UpDownLongPress.DeltaData;
    OLED_UI_UpDownLongPress.DeltaData = 0;
    // 启用定时器中断
    OLED_UI_TimerEnable();
    int16_t MaxMenuID;

    
    
    
    // 获取编码器旋转的增量值
    OLED_UI_DeltaMenuID += OLED_UI_EncoderGet();

    


    // 确认按键任务处理
    switch(enter_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================单击按键处理逻辑==============================*/

        if(OLED_UI_WindowStatus.SustainFlag == false){
            OLED_UI_EnterTask();
        }else{
            OLED_UI_WindowStatus.Counter = 0;
        }

        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================双击按键处理逻辑==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================长按按键处理逻辑==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================无操作的处理逻辑==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }

    // 取消按键任务处理
    switch(back_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================单击按键处理逻辑==============================*/
        if(OLED_UI_WindowStatus.SustainFlag == false){
            OLED_UI_BackTask();
        }else{
            OLED_UI_WindowStatus.Counter = CurrentWindow->General_StayTime;
        }

        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================双击按键处理逻辑==============================*/


        

        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================长按按键处理逻辑==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================无操作的处理逻辑==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }

    // 上按键任务处理
    switch(up_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================单击按键处理逻辑==============================*/

        // 变化值自减1
        OLED_UI_DeltaMenuID -= 1;


        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================双击按键处理逻辑==============================*/

        


        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================长按按键处理逻辑==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================无操作的处理逻辑==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }

    // 下按键任务处理
    switch(down_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================单击按键处理逻辑==============================*/

        // 变化值自增1
        OLED_UI_DeltaMenuID += 1;


        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================双击按键处理逻辑==============================*/

        


        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================长按按键处理逻辑==============================*/

        


        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================无操作的处理逻辑==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }

    // 编码器按键任务处理
    switch(encoder_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================单击按键处理逻辑==============================*/
        if(OLED_UI_WindowStatus.SustainFlag == false){

            OLED_UI_EnterTask();
        }else{
            OLED_UI_WindowStatus.Counter = 0;
        }

        


        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================双击按键处理逻辑==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================长按按键处理逻辑==============================*/

        if(OLED_UI_WindowStatus.SustainFlag == false){
            OLED_UI_BackTask();
        }else{
            OLED_UI_WindowStatus.Counter = CurrentWindow->General_StayTime;
        }
        

        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================无操作的处理逻辑==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }
    
    // 如果当前有窗口事件发生，则将获取到的输入用于处理窗口事件
    if(OLED_UI_WindowStatus.SustainFlag != false){
        // 获取窗口数据类型
        
        int8_t DataStyle = OLED_UI_GetWindowProbDataStyle(CurrentWindow);
        if(DataStyle != WINDOW_DATA_STYLE_NONE)
        {
            switch (DataStyle) {
                case WINDOW_DATA_STYLE_FLOAT:
                    *CurrentWindow->Prob_Data_Float += (OLED_UI_DeltaMenuID * CurrentWindow->Prob_DataStep);
			        if(*CurrentWindow->Prob_Data_Float <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Float = CurrentWindow->Prob_MinData;}
			        if(*CurrentWindow->Prob_Data_Float >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Float = CurrentWindow->Prob_MaxData;}
                    break;
                case WINDOW_DATA_STYLE_INT8:
					*CurrentWindow->Prob_Data_Int_8_t += (OLED_UI_DeltaMenuID * CurrentWindow->Prob_DataStep);
					if(*CurrentWindow->Prob_Data_Int_8_t <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Int_8_t = CurrentWindow->Prob_MinData;}
					if(*CurrentWindow->Prob_Data_Int_8_t >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Int_8_t = CurrentWindow->Prob_MaxData;}
					break;
				case WINDOW_DATA_STYLE_INT16:
					
					*CurrentWindow->Prob_Data_Int_16_t += (OLED_UI_DeltaMenuID * CurrentWindow->Prob_DataStep);
					if(*CurrentWindow->Prob_Data_Int_16_t <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Int_16_t = CurrentWindow->Prob_MinData;}
					if(*CurrentWindow->Prob_Data_Int_16_t >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Int_16_t = CurrentWindow->Prob_MaxData;}
					break;
				case WINDOW_DATA_STYLE_INT32:
					*CurrentWindow->Prob_Data_Int_32_t += (OLED_UI_DeltaMenuID * CurrentWindow->Prob_DataStep);
					if(*CurrentWindow->Prob_Data_Int_32_t <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Int_32_t = CurrentWindow->Prob_MinData;}
					if(*CurrentWindow->Prob_Data_Int_32_t >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Int_32_t = CurrentWindow->Prob_MaxData;}
					break;
				case WINDOW_DATA_STYLE_INT64:
					*CurrentWindow->Prob_Data_Int_64_t += (OLED_UI_DeltaMenuID * CurrentWindow->Prob_DataStep);
					if(*CurrentWindow->Prob_Data_Int_64_t <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Int_64_t = CurrentWindow->Prob_MinData;}
					if(*CurrentWindow->Prob_Data_Int_64_t >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Int_64_t = CurrentWindow->Prob_MaxData;}
					break;
                

            }
            if(OLED_UI_DeltaMenuID != 0){
                OLED_UI_WindowStatus.Counter = 0;
            }
        }
        OLED_UI_DeltaMenuID = 0;
    }


    // 获取最大菜单id
    MaxMenuID = OLED_UI_GetMaxMenuItemNum();


    // 确保 ActiveMenuID 在合法范围内
    // 给DeltaMenuID限幅，防止超出菜单范围
    if(CurrentMenuPage->_ActiveMenuID + OLED_UI_DeltaMenuID < 0)
    {
        OLED_UI_DeltaMenuID = -CurrentMenuPage->_ActiveMenuID;
    }
    if(CurrentMenuPage->_ActiveMenuID + OLED_UI_DeltaMenuID > MaxMenuID)
    {
        OLED_UI_DeltaMenuID = MaxMenuID - CurrentMenuPage->_ActiveMenuID;
    }



    


    // 复位按键状态
    OLED_UI_Enter.Status = OLED_UI_KEY_NONE;
    OLED_UI_Back.Status = OLED_UI_KEY_NONE;
    OLED_UI_Up.Status = OLED_UI_KEY_NONE;
    OLED_UI_Down.Status = OLED_UI_KEY_NONE;
    OLED_UI_EncoderKey.Status = OLED_UI_KEY_NONE;

}

/**
 * @brief 设置菜单起始区域的目标值
 * @param 无
 * @note 设置菜单起始区域的目标值，用于动画效果
 * @return 无
 */
void OLED_UI_SetTargetMenuStartPoint(void){
    if(OLED_UI_DeltaMenuID == 0){
        return;
    }else{
        if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
            // 这句代码的作用是在磁贴菜单界面每次操作的时候滚动偏移置零
            OLED_UI_SetLineSplitZero();
        }
    }
    if(OLED_UI_DeltaMenuID < 0 ){
        
        for(int i = 0; i < -OLED_UI_DeltaMenuID; i++){
            if(CurrentMenuPage->_Slot == 0 && CurrentMenuPage->_ActiveMenuID !=0){
                OLED_UI_MenuItemsMovePlus();

            }
            //如果光标还没有到达最顶部的槽位，那么就向上移动槽位
			if(CurrentMenuPage->_Slot > 0){
				CurrentMenuPage->_Slot--;
			}
            CurrentMenuPage->_ActiveMenuID--;
        }
    }else if(OLED_UI_DeltaMenuID >= 0){
        for(int i = 0; i < OLED_UI_DeltaMenuID; i++){
            if(CurrentMenuPage->_Slot == OLED_UI_GetMaxSlotNum()-1 && CurrentMenuPage->_ActiveMenuID != OLED_UI_GetMaxMenuItemNum()){
                OLED_UI_MenuItemsMoveMinus();

            }
            //如果光标还没有到达最底部的槽位，那么就向下移动槽位
			if(CurrentMenuPage->_Slot < OLED_UI_GetMaxSlotNum()-1){
				CurrentMenuPage->_Slot++;
			}
            CurrentMenuPage->_ActiveMenuID++;
        }
    }
    OLED_UI_DeltaMenuID = 0;



}

/**
 * @brief 设置进度条宽度的目标值
 * @param 无
 * @note 设置进度条宽度的目标值，用于动画效果
 * @return 无
 */
void OLED_UI_SetTargetProbWidth(void){
    // 防止访问非法内存
    if(CurrentWindow != NULL){
        int8_t DataStyle = OLED_UI_GetWindowProbDataStyle(CurrentWindow);
        // 如果当前窗口有进度条，则设置进度条宽度的目标值
        if(DataStyle != WINDOW_DATA_STYLE_NONE){
            switch (DataStyle) {
				case WINDOW_DATA_STYLE_INT8:
				
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Int_8_t - CurrentWindow->Prob_MinData)/(CurrentWindow->Prob_MaxData - CurrentWindow->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4);
					break;
				case WINDOW_DATA_STYLE_INT16:
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Int_16_t - CurrentWindow->Prob_MinData)/(CurrentWindow->Prob_MaxData - CurrentWindow->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4);

					break;
				case WINDOW_DATA_STYLE_INT32:
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Int_32_t - CurrentWindow->Prob_MinData)/(CurrentWindow->Prob_MaxData - CurrentWindow->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4);
					break;
				case WINDOW_DATA_STYLE_INT64:
				
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Int_64_t - CurrentWindow->Prob_MinData)/(CurrentWindow->Prob_MaxData - CurrentWindow->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4);
					break;
                case WINDOW_DATA_STYLE_FLOAT:
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Float - CurrentWindow->Prob_MinData)/(CurrentWindow->Prob_MaxData - CurrentWindow->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4);
					break;
				default:
					break;
					
			}
        }
    }
}

/**
 * @brief 打印字符串，并在需要滚动的时候滚动显示
 * @note 如果字符串宽度超过了区域宽度，那么就滚动显示
 * @return 无
 */
void OLED_UI_PrintStringScroll(
    int16_t areaX,int16_t areaY,int16_t areaWidth,int16_t areaHeight,       //打印的区域
    int16_t X, int16_t Y,                                              //打印的起始位置
    int16_t compareWidth,                                               //比较的宽度
    int8_t font,
    const char* menuText,
    float * slip,bool ifAnimationEnd)
{

    // 区域宽度和高度必须大于0
    if(areaWidth <= 0 || areaHeight <= 0){return;}

    // 获取字符串宽度
    int16_t StringLength = OLED_UI_CalcStringWidth(font, menuText);

    // 设置文本间隔
    int16_t StrDis = compareWidth/3;

    // 只有当文本宽度超过区域宽度时才滚动
    if (StringLength > compareWidth)
    {

        // 重置滚动位置
        if (*slip < -StringLength - StrDis) {
            *slip = 0;
        }
        if(ifAnimationEnd){
            // 更新滚动位置
            *slip -= OLED_UI_LONG_MENU_SCROLL_PIXELS;
        }
        // 计算实际绘制位置
        int16_t pos1 = X + (int16_t)*slip;
        int16_t pos2 = X + (int16_t)*slip + StringLength + StrDis;

        // 在区域内绘制两个字符串
        OLED_PrintfArea(areaX, areaY, areaWidth, areaHeight, pos1, Y, font, menuText);
        OLED_PrintfArea(areaX, areaY, areaWidth, areaHeight, pos2, Y, font, menuText);

    } else {
        // 直接绘制静态文本
        OLED_PrintfArea(areaX, areaY, areaWidth, areaHeight, X, Y, font, menuText);
        * slip = 0;
    }

}

 
/**
 * @brief 在屏幕上打印菜单项
 * @param 无
 * @note 在屏幕上打印菜单项，包括菜单项名称、菜单项值等。
 * @return 无
 */
void OLED_UI_PrintMenuItems(void)
{
    // 获取菜单数量
    int16_t MenuNum = OLED_UI_GetMaxMenuItemNum();

    // 获取中文字体宽度
    uint8_t ChineseFontWidth = OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_CHINESE);

    // 获取ASCII字体宽度
    uint8_t ASCIIFontWidth = OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_ASCII);

    // 用于打印菜单的光标点为全局变量的当前点
    OLED_UI_Point CursorPoint = OLED_UI_PageStartPoint.CurrentPoint;



    // 如果当前菜单的类型是列表
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST)
    {
        //行前缀宽度，默认为0
		uint8_t LinePerfixWidth = 0;
		//bool选框宽度，默认为0
		uint8_t RadioCompensationWidth = 0;
        //如果需要绘制行前缀
		if(CurrentMenuPage->List_IfDrawLinePerfix == true){
			//将行前缀宽度设置为ASCII字体宽度加LINEPERFIX_DISTANCE（与后方字符串的距离）
			LinePerfixWidth = ASCIIFontWidth + LINEPERFIX_DISTANCE;
		}
        //打印菜单项
		for(int16_t i = 0; i<=MenuNum;i++)
        {
            // 如果光标小于起始区域的Y坐标或者大于起始区域的结束Y坐标，则跳过该项以节省资源
            if(CursorPoint.Y + CurrentMenuPage->General_FontSize < OLED_UI_MenuFrame.CurrentArea.Y || CursorPoint.Y > OLED_UI_MenuFrame.CurrentArea.Y + OLED_UI_MenuFrame.CurrentArea.Height){
				CursorPoint.Y += (CurrentMenuPage->General_FontSize + OLED_UI_LineStep.CurrentDistance);
				continue;
			}
            //记录此轮循环的字符串宽度
			int16_t StringLength = OLED_UI_CalcStringWidth(CurrentMenuPage->General_FontSize,CurrentMenuPage->General_MenuItems[i].General_item_text);
			//如果需要打印行前缀
	        if(CurrentMenuPage->List_IfDrawLinePerfix == true){
	        	const char* LinePerfixSymb = "";
	        	//如果当前的菜单项有子菜单且没有回调函数，那么打印相应的符号
	        	if(CurrentMenuPage->General_MenuItems[i].General_SubMenuPage != NULL && CurrentMenuPage->General_MenuItems[i].General_callback == NULL && CurrentMenuPage->General_MenuItems[i].List_BoolRadioBox == NULL){
	        		LinePerfixSymb = SUBMENU_PREFIX;
	        	}else//如果有回调函数
	        	if(CurrentMenuPage->General_MenuItems[i].General_SubMenuPage == NULL && CurrentMenuPage->General_MenuItems[i].General_callback != NULL && CurrentMenuPage->General_MenuItems[i].List_BoolRadioBox == NULL){
	        		LinePerfixSymb = FUNCTION_PREFIX;
	        	}else
	        	if(CurrentMenuPage->General_MenuItems[i].General_SubMenuPage == NULL && CurrentMenuPage->General_MenuItems[i].General_callback == NULL && CurrentMenuPage->General_MenuItems[i].List_BoolRadioBox != NULL)
	        	{
	        		LinePerfixSymb = RADIO_PREFIX;
	        	}else{
	        		LinePerfixSymb = NONE_PREFIX;
	        	}
	        	OLED_PrintfArea(OLED_UI_MenuFrame.CurrentArea.X,OLED_UI_MenuFrame.CurrentArea.Y,OLED_UI_MenuFrame.CurrentArea.Width - 6,OLED_UI_MenuFrame.CurrentArea.Height,CursorPoint.X,CursorPoint.Y,CurrentMenuPage->General_FontSize,LinePerfixSymb);
	        }else{
                LinePerfixWidth = 0;
            }
            //如果需要绘制单选框(即BoolRadioBox不为空)
            if(CurrentMenuPage->General_MenuItems[i].List_BoolRadioBox != NULL){
                RadioCompensationWidth = (ChineseFontWidth + 2);
                const char* RadioBoxSymb = "";
                if(*CurrentMenuPage->General_MenuItems[i].List_BoolRadioBox == true){
                    RadioBoxSymb = "■";
                }else{
                    RadioBoxSymb = "□";
                }
                OLED_PrintfArea(//在限制的区域内打印文字
                               //光标的起始x坐标加入行前缀宽度，这样可以自动留出打印行前缀的空间
                               OLED_UI_MenuFrame.CurrentArea.X ,
                               OLED_UI_MenuFrame.CurrentArea.Y,
                               OLED_UI_MenuFrame.CurrentArea.Width - 6,
                               OLED_UI_MenuFrame.CurrentArea.Height,
                
                               //打印文字的坐标
                               CurrentMenuPage->General_StartPoint.X + OLED_UI_MenuFrame.CurrentArea.X + OLED_UI_MenuFrame.CurrentArea.Width - RadioCompensationWidth - 8 ,
                               CursorPoint.Y,
                               //打印文字的大小
                               CurrentMenuPage->General_FontSize,
                               //打印文字的内容
                               RadioBoxSymb);
            }else{
                RadioCompensationWidth = 0;
            }

            // 限制显示区域的X坐标：如果开始打印的x坐标小于当前菜单区域的x坐标，那么从页面开始的x坐标开始打印，否则从开始打印的x坐标开始打印
            int16_t StringX = (CursorPoint.X + LinePerfixWidth < OLED_UI_MenuFrame.CurrentArea.X) ? OLED_UI_MenuFrame.CurrentArea.X : CursorPoint.X + LinePerfixWidth;

            int16_t StringY = OLED_UI_MenuFrame.CurrentArea.Y;

            // 显示区域的限制宽度：如果开始打印的x坐标小于当前菜单区域的x坐标，那么实际的宽度应该等于【开始打印的x坐标加【当前页面起始x坐坐标加上当前页面宽度减去5减去2】减去当前页面起始x坐标】
            // int16_t StringWidth = OLED_UI_MenuFrame.CurrentArea.Width + OLED_UI_MenuFrame.CurrentArea.X - StringX - RadioCompensationWidth - 8;
            int16_t StringWidth = (CursorPoint.X + LinePerfixWidth < OLED_UI_MenuFrame.CurrentArea.X) ? CursorPoint.X + LinePerfixWidth + OLED_UI_MenuFrame.CurrentArea.Width - 7 - RadioCompensationWidth - OLED_UI_MenuFrame.CurrentArea.X:OLED_UI_MenuFrame.CurrentArea.Width + OLED_UI_MenuFrame.CurrentArea.X - StringX - RadioCompensationWidth - 8;
            int16_t StringHeight = OLED_UI_MenuFrame.CurrentArea.Height;
            int16_t StringStartX = CursorPoint.X + LinePerfixWidth;
            int16_t StringStartY = CursorPoint.Y;
            int16_t CompareWidth = OLED_UI_MenuFrame.TargetArea.Width - 6 - LinePerfixWidth - CurrentMenuPage->General_StartPoint.X  -2 - RadioCompensationWidth;

            
            OLED_UI_PrintStringScroll(
                //限制的区域
                StringX,
                StringY,
                StringWidth,
                StringHeight,

                StringStartX,
                StringStartY,

                CompareWidth,

                CurrentMenuPage->General_FontSize,
                CurrentMenuPage->General_MenuItems[i].General_item_text,
                &CurrentMenuPage->General_MenuItems[i]._LineSlip,
                OLED_UI_IfAnimationEnd(ANIM_MENU));


            CursorPoint.Y += (CurrentMenuPage->General_FontSize + OLED_UI_LineStep.CurrentDistance);
        }

        

        
    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        //记录此轮循环的字符串宽度
		int16_t StringLength = OLED_UI_CalcStringWidth(CurrentMenuPage->General_FontSize,CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_item_text);

        //打印菜单项
		for(int16_t i = 0; i<=MenuNum;i++)
        {
           
            OLED_ShowImageArea(
                OLED_UI_MenuFrame.CurrentArea.X, 
                OLED_UI_MenuFrame.CurrentArea.Y,
                OLED_UI_MenuFrame.CurrentArea.Width, 
                OLED_UI_MenuFrame.CurrentArea.Height,
                ceil(CursorPoint.X),
                CursorPoint.Y,
                CurrentMenuPage->Tiles_TileWidth,
                CurrentMenuPage->Tiles_TileHeight,
                CurrentMenuPage->General_MenuItems[i].Tiles_Icon != NULL ? CurrentMenuPage->General_MenuItems[i].Tiles_Icon : UnKnown);

            CursorPoint.X += (CurrentMenuPage->Tiles_TileWidth + OLED_UI_LineStep.CurrentDistance);

            
            
        }
        
        OLED_UI_PrintStringScroll(
            //限制的区域
            OLED_UI_MenuFrame.CurrentArea.X, 
            OLED_UI_MenuFrame.CurrentArea.Y,
            OLED_UI_MenuFrame.CurrentArea.Width, 
            OLED_UI_MenuFrame.CurrentArea.Height,

            StringLength < OLED_UI_MenuFrame.TargetArea.Width ? OLED_UI_MenuFrame.CurrentArea.X + OLED_UI_MenuFrame.CurrentArea.Width/2 -StringLength/2 : OLED_UI_MenuFrame.CurrentArea.X + 5,
            // OLED_UI_MenuFrame.CurrentArea.Y + CurrentMenuPage->Tiles_FontTopDistance,
            OLED_UI_Tiles_FontTopDistance.CurrentDistance,
            OLED_UI_MenuFrame.CurrentArea.Width,

            CurrentMenuPage->General_FontSize,
            CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_item_text,
            &CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID]._LineSlip,
            OLED_UI_IfAnimationEnd(ANIM_MENU));
   
    }
    // 绘制菜单边框
    if(CurrentMenuPage->General_MenuFrameStyle == MENU_FRAME_RECTANGLE){
        OLED_DrawRectangle(OLED_UI_MenuFrame.CurrentArea.X-1, OLED_UI_MenuFrame.CurrentArea.Y-1,OLED_UI_MenuFrame.CurrentArea.Width+2, OLED_UI_MenuFrame.CurrentArea.Height+2, OLED_UNFILLED);
    }else if(CurrentMenuPage->General_MenuFrameStyle == MENU_FRAME_ROUNDRECTANGLE){
        OLED_DrawRoundedRectangle(OLED_UI_MenuFrame.CurrentArea.X-1, OLED_UI_MenuFrame.CurrentArea.Y-1, OLED_UI_MenuFrame.CurrentArea.Width+2, OLED_UI_MenuFrame.CurrentArea.Height+2,3, OLED_UNFILLED);
    }
    // 绘制光标
    OLED_UI_ReverseCoordinate(OLED_UI_Cursor.CurrentArea.X, OLED_UI_Cursor.CurrentArea.Y, OLED_UI_Cursor.CurrentArea.Width, OLED_UI_Cursor.CurrentArea.Height, CurrentMenuPage->General_CursorStyle);
    //绘制滚动条与其中心线
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){

        OLED_DrawRectangle(OLED_UI_MenuFrame.CurrentArea.X + OLED_UI_MenuFrame.CurrentArea.Width - 5,OLED_UI_MenuFrame.CurrentArea.Y,5,(OLED_UI_ScrollBarHeight.CurrentDistance > OLED_UI_MenuFrame.CurrentArea.Height) ?OLED_UI_MenuFrame.CurrentArea.Height:OLED_UI_ScrollBarHeight.CurrentDistance,OLED_FILLED);

        OLED_DrawLine(OLED_UI_MenuFrame.CurrentArea.X + OLED_UI_MenuFrame.CurrentArea.Width - 3,OLED_UI_MenuFrame.CurrentArea.Y,    OLED_UI_MenuFrame.CurrentArea.X + OLED_UI_MenuFrame.CurrentArea.Width-3,OLED_UI_MenuFrame.CurrentArea.Height + OLED_UI_MenuFrame.   CurrentArea.Y-1);
    }else if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        OLED_DrawRectangle(OLED_UI_ScrollBarStartPoint.CurrentPoint.X,OLED_UI_ScrollBarStartPoint.CurrentPoint.Y,(OLED_UI_ScrollBarHeight.    CurrentDistance > OLED_UI_MenuFrame.CurrentArea.Width) ? OLED_UI_MenuFrame.CurrentArea.Width:OLED_UI_ScrollBarHeight.  CurrentDistance,5,OLED_FILLED);
        OLED_DrawLine(
            OLED_UI_ScrollBarStartPoint.CurrentPoint.X,
            OLED_UI_ScrollBarStartPoint.CurrentPoint.Y + 2,
            OLED_UI_ScrollBarStartPoint.CurrentPoint.X + OLED_UI_MenuFrame.CurrentArea.Width - 1,
            OLED_UI_ScrollBarStartPoint.CurrentPoint.Y + 2
        );
    }
    if(CurrentMenuPage->General_ShowAuxiliaryFunction != NULL){
        CurrentMenuPage->General_ShowAuxiliaryFunction();
    }
    

}

/**
 * @brief 设置UI元素位置
 * @param 无
 * @note 设置UI元素位置，包括菜单、按钮、文本框、进度条等。
 * @return 无
 */
void OLED_UI_SetElementLocation(void)
{
    // 设置光标区域
    OLED_UI_SetTargetCursorArea();

    // 设置滚动条高度
    OLED_UI_SetTargetScrollBarHeight();

    // 设置字体顶部距离[磁贴专有]
    OLED_UI_SetTiles_FontTopDistance();

    // 设置滚动条起始位置
    OLED_UI_SetTargetScrollBarStartPoint();

    // 设置菜单区域
    OLED_UI_SetTargetMenuArea();

    // 设置菜单项开始位置
    OLED_UI_SetTargetMenuStartPoint();

    // 设置进度条长度
    OLED_UI_SetTargetProbWidth();

    // 计算动画：行间距或磁贴间距
    OLED_UI_ChangeDistance(&OLED_UI_LineStep);

    // 计算动画：字体顶部距离[磁贴专有]
    OLED_UI_ChangeDistance(&OLED_UI_Tiles_FontTopDistance);

    // 计算动画：滚动条高度
    OLED_UI_ChangeDistance(&OLED_UI_ScrollBarHeight);

    // 计算动画：滚动条起始位置
    OLED_UI_ChangePoint(&OLED_UI_ScrollBarStartPoint);

    // 计算动画：菜单项开始位置
    OLED_UI_ChangePoint(&OLED_UI_PageStartPoint);

    // 计算动画：菜单区域
    OLED_UI_ChangeArea(&OLED_UI_MenuFrame);

    // 计算动画：改变光标区域
    OLED_UI_ChangeArea(&OLED_UI_Cursor);

    // 计算动画：改变窗口区域
    OLED_UI_ChangeArea(&OLED_UI_Window);

    // 计算动画：改变进度条长度
    OLED_UI_ChangeDistance(&OLED_UI_ProbWidth);
}




/**
 * @brief 创建窗口
 * @param window 窗口结构体指针，用于存储用户创建的窗口信息
 * @note 创建窗口
 */
void OLED_UI_CreateWindow(MenuWindow* window){
    // 如果当前窗口为空
    if(CurrentWindow == NULL){
        // 将偏移值置零
        window->_LineSlip = 0;
    }
    //将窗口标志位设置为true，表示开始窗口效果
	OLED_UI_WindowStatus.SustainFlag = true;
    //重置窗口效果时间计数器
	OLED_UI_WindowStatus.Counter = 0;

    // 初始化窗口的目标大小与位置
    OLED_UI_Window.TargetArea.X = (OLED_WIDTH - window->General_Width)/2;
    OLED_UI_Window.TargetArea.Y = (OLED_HEIGHT - window->General_Height)/2;
    OLED_UI_Window.TargetArea.Width = window->General_Width;
    OLED_UI_Window.TargetArea.Height = window->General_Height;

    OLED_UI_Window.CurrentArea.X = (OLED_WIDTH - window->General_Width)/2;
    OLED_UI_Window.CurrentArea.Y = -window->General_Height - 1;
    OLED_UI_Window.CurrentArea.Width = window->General_Width;
    OLED_UI_Window.CurrentArea.Height = window->General_Height;
    //将当前窗口指针指向window
	CurrentWindow = window;
}



/**
 * @brief 绘制进度条
 * @param X 进度条起始x坐标
 * @param Y 进度条起始y坐标
 * @param Width 进度条宽度
 * @param Height 进度条高度
 * @param Percentage 进度百分比,0-1
 * @note 绘制进度条
 */
void OLED_UI_DrawProgressBar(int16_t X,int16_t Y,int16_t Width,int16_t Height,float Percentage)
{
    if(Percentage < 0){
        Percentage = 0;
    }else if(Percentage > 1){
        Percentage = 1;
    }
    OLED_DrawRoundedRectangle(X,Y,Width,Height,2,OLED_UNFILLED);
    OLED_DrawRectangle(X+2,Y+2,(Width-4)*Percentage,Height-4,OLED_FILLED);
}

/**
 * @brief 绘制窗口
 * @param 无
 * @note 绘制窗口
 */
void OLED_UI_DrawWindow(void)
{
    // 如果当前窗口为空，则直接返回
    if(CurrentWindow == NULL){
        // OLED_UI_WindowStatus.SustainFlag = false;
        return;
    }
    //如果窗口标志位没有被置位，则不进行窗口效果，将窗口的目标位置置位屏幕外部
	if(OLED_UI_WindowStatus.SustainFlag == false){
		// OLED_UI_Window.TargetArea.Height = OLED_UI_WINDOW_FADE_HEIGHT;
		// OLED_UI_Window.TargetArea.Width = OLED_UI_WINDOW_FADE_WIDTH;
		// OLED_UI_Window.TargetArea.X = OLED_UI_WINDOW_FADE_X;	
		// OLED_UI_Window.TargetArea.Y = OLED_UI_WINDOW_FADE_Y;
        OLED_UI_Window.TargetArea.X = (OLED_WIDTH - CurrentWindow->General_Width)/2;
        OLED_UI_Window.TargetArea.Y = -CurrentWindow->General_Height - 10;
        OLED_UI_Window.TargetArea.Width = CurrentWindow->General_Width;
        OLED_UI_Window.TargetArea.Height = CurrentWindow->General_Height;
	}
    
    // 如果计时时间到了，那么将标志位清零，计数器清零
    if(OLED_UI_WindowStatus.Counter > CurrentWindow->General_StayTime/OLED_UI_INTERRUPT_TIME)
    {
        OLED_UI_WindowStatus.SustainFlag = false;
        OLED_UI_WindowStatus.Counter = 0;
    }
    
    //如果窗口动画还没有退出屏幕，则绘制窗口
	// if(OLED_UI_Window.CurrentArea.Height != CurrentWindow->General_Height || OLED_UI_Window.CurrentArea.Width != CurrentWindow->General_Width || OLED_UI_Window.CurrentArea.Y != -CurrentWindow->General_Height - 10 || OLED_UI_Window.CurrentArea.X != (OLED_WIDTH - CurrentWindow->General_Width)/2){
    if(OLED_UI_Window.CurrentArea.Y + OLED_UI_Window.CurrentArea.Height > 0 || OLED_UI_WindowStatus.SustainFlag == true){
        // 判断指针的数据类型
        int8_t DataStyle = OLED_UI_GetWindowProbDataStyle(CurrentWindow);
        int16_t DataLength = 0;
        int16_t DataPerfixWidth = 0;
		//绘制边框
		if(CurrentWindow->General_FrameStyle != WINDOW_FRAME_ROUNDRECTANGLE){
			OLED_DrawRectangle(OLED_UI_Window.CurrentArea.X - 1,OLED_UI_Window.CurrentArea.Y - 1,OLED_UI_Window.CurrentArea.Width + 2,OLED_UI_Window.CurrentArea.Height + 2,OLED_UNFILLED);
			OLED_ClearArea(OLED_UI_Window.CurrentArea.X,OLED_UI_Window.CurrentArea.Y,OLED_UI_Window.CurrentArea.Width,OLED_UI_Window.CurrentArea.Height);
		}else{
			OLED_DrawRoundedRectangle(OLED_UI_Window.CurrentArea.X - 1,OLED_UI_Window.CurrentArea.Y - 1,OLED_UI_Window.CurrentArea.Width + 2,OLED_UI_Window.CurrentArea.Height + 2,2,OLED_UNFILLED);
			OLED_ClearArea(OLED_UI_Window.CurrentArea.X,OLED_UI_Window.CurrentArea.Y,OLED_UI_Window.CurrentArea.Width,OLED_UI_Window.CurrentArea.Height);
		}
        // 如果指针不为空
        if(DataStyle != WINDOW_DATA_STYLE_NONE ){
            DataPerfixWidth = OLED_UI_WINDOW_DATA_GAP;
            switch (DataStyle) {
                case WINDOW_DATA_STYLE_INT8:
                    DataLength = OLED_UI_CalcStringWidth(CurrentWindow->Prob_FontSize,"%d",*CurrentWindow->Prob_Data_Int_8_t);
                    OLED_PrintfArea(
                        //限制区域
                        OLED_UI_Window.CurrentArea.X,
						OLED_UI_Window.CurrentArea.Y,
						OLED_UI_Window.CurrentArea.Width,
						OLED_UI_Window.CurrentArea.Height,

                        OLED_UI_Window.CurrentArea.X + OLED_UI_Window.CurrentArea.Width - 1 - CurrentWindow->Title_FontSideDistance - DataLength,
						OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                        CurrentWindow->Prob_FontSize,
                        "%d",
                        *CurrentWindow->Prob_Data_Int_8_t

                    );
                    break;
                case WINDOW_DATA_STYLE_INT16:
                    DataLength = OLED_UI_CalcStringWidth(CurrentWindow->Prob_FontSize,"%d",*CurrentWindow->Prob_Data_Int_16_t);
                    OLED_PrintfArea(
                        //限制区域
                        OLED_UI_Window.CurrentArea.X,
						OLED_UI_Window.CurrentArea.Y,
						OLED_UI_Window.CurrentArea.Width,
						OLED_UI_Window.CurrentArea.Height,

                        OLED_UI_Window.CurrentArea.X + OLED_UI_Window.CurrentArea.Width - 1 - CurrentWindow->Title_FontSideDistance - DataLength,
						OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                        CurrentWindow->Prob_FontSize,
                        "%d",
                        *CurrentWindow->Prob_Data_Int_16_t

                    );
                    break;
                case WINDOW_DATA_STYLE_INT32:
                    DataLength = OLED_UI_CalcStringWidth(CurrentWindow->Prob_FontSize,"%d",*CurrentWindow->Prob_Data_Int_32_t);
                    OLED_PrintfArea(
                        //限制区域
                        OLED_UI_Window.CurrentArea.X,
                        OLED_UI_Window.CurrentArea.Y,
                        OLED_UI_Window.CurrentArea.Width,
                        OLED_UI_Window.CurrentArea.Height,

                        OLED_UI_Window.CurrentArea.X + OLED_UI_Window.CurrentArea.Width - 1 - CurrentWindow->Title_FontSideDistance - DataLength,
                        OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                        CurrentWindow->Prob_FontSize,
                        "%d",
                        *CurrentWindow->Prob_Data_Int_32_t

                    );
                    break;
                case WINDOW_DATA_STYLE_INT64:
                    DataLength = OLED_UI_CalcStringWidth(CurrentWindow->Prob_FontSize,"%d",*CurrentWindow->Prob_Data_Int_64_t);
                    OLED_PrintfArea(
                        //限制区域
                        OLED_UI_Window.CurrentArea.X,
                        OLED_UI_Window.CurrentArea.Y,
                        OLED_UI_Window.CurrentArea.Width,
                        OLED_UI_Window.CurrentArea.Height,

                        OLED_UI_Window.CurrentArea.X + OLED_UI_Window.CurrentArea.Width - 1 - CurrentWindow->Title_FontSideDistance - DataLength,
                        OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                        CurrentWindow->Prob_FontSize,
                        "%d",
                        *CurrentWindow->Prob_Data_Int_64_t

                    );
                    break;
                case WINDOW_DATA_STYLE_FLOAT:
                    DataLength = OLED_UI_CalcStringWidth(CurrentWindow->Prob_FontSize,"%.*f",CurrentWindow->Prob_FloatPointLength,*CurrentWindow->Prob_Data_Float);
                    OLED_PrintfArea(
                        //限制区域
                        OLED_UI_Window.CurrentArea.X,
                        OLED_UI_Window.CurrentArea.Y,
                        OLED_UI_Window.CurrentArea.Width,
                        OLED_UI_Window.CurrentArea.Height,

                        OLED_UI_Window.CurrentArea.X + OLED_UI_Window.CurrentArea.Width - 1 - CurrentWindow->Title_FontSideDistance - DataLength,
                        OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                        CurrentWindow->Prob_FontSize,
                        "%.*f",
                        CurrentWindow->Prob_FloatPointLength,
                        *CurrentWindow->Prob_Data_Float

                        );
                    break;

            }
            //显示进度条（外框）
            OLED_UI_DrawProgressBar(
                OLED_UI_Window.CurrentArea.X +CurrentWindow->Prob_SideDistance,
                OLED_UI_Window.CurrentArea.Y + OLED_UI_Window.CurrentArea.Height  - CurrentWindow->Prob_LineHeight - CurrentWindow->Prob_BottomDistance,
                OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance,
                CurrentWindow->Prob_LineHeight,
                OLED_UI_ProbWidth.CurrentDistance/(OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4)
            
            );
			

        }


        //打印窗口标题
        if(CurrentWindow->Title_text != NULL){
            OLED_UI_PrintStringScroll(
                // 限制区域
                OLED_UI_Window.CurrentArea.X + CurrentWindow->Title_FontSideDistance,
                OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                OLED_UI_Window.CurrentArea.Width - CurrentWindow->Title_FontSideDistance*2 - DataLength - DataPerfixWidth,
                CurrentWindow->Title_FontSize,
                // 打印起始位置
                OLED_UI_Window.CurrentArea.X + CurrentWindow->Title_FontSideDistance,
                OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                // 限制宽度
                OLED_UI_Window.TargetArea.Width - CurrentWindow->Title_FontSideDistance*2 - DataLength - DataPerfixWidth,
                CurrentWindow->Title_FontSize,
                CurrentWindow->Title_text,
                &CurrentWindow->_LineSlip,
                OLED_UI_IfAnimationEnd(ANIM_WINDOW)
            );
        }

    }else{
        CurrentWindow = NULL;
    }

}

/**
 * @brief 初始化检查
 * @param 无
 * @note 检查是否需正确传入了菜单结构体指针。
 * @return 无
 */
void OLED_UI_InitCheck(void)
{
    if(CurrentMenuPage == NULL){
        OLED_Init();
        while(1){
            //刷新屏幕
            OLED_Printf(0,0,OLED_FONT_8,"OLED UI\nError:\nWithout\nInit!");
            OLED_Update();
        }
    }

}
/**
 * @brief 打印UI元素
 * @param 无
 * @note 打印UI元素，包括菜单、按钮、文本框、进度条等。
 * @return 无
 */
void OLED_UI_PrintElement(void)
{
    //清除显存
    OLED_Clear();

    // 显示帧数
    OLED_UI_ShowFPS();

    // 显示菜单项
    OLED_UI_PrintMenuItems();

    // 显示窗口
    OLED_UI_DrawWindow();

    //刷屏
    OLED_Update();

    //如果屏幕发生变化，记录有效帧
    if(OLED_IfChangedScreen()){
        OLED_UI_FPS.Counter++;
    }
}
        /*===============================OLED-UI工具函数==============================*/
    /*===================================OLED-UI工具函数==================================*/
/*=======================================OLED-UI工具函数======================================*/


/*=======================================OLED-UI实现函数======================================*/
    /*===================================OLED-UI实现函数==================================*/
        /*===============================OLED-UI实现函数==============================*/




/**
 * @brief 主循环
 * @param 无
 * @note 主循环，用于显示UI。
 * @return 无
 */
void OLED_UI_MainLoop(void)
{
    //初始化检查
    OLED_UI_InitCheck();
    //处理输入
    OLED_UI_HandleInput_MainLoop();

    //设置UI元素位置
    OLED_UI_SetElementLocation();

    //打印UI元素（包含刷新屏幕）
    OLED_UI_PrintElement();

    //悬浮窗口显示


    
    
}



/**
 * @brief 初始化OLED UI
 * @param 无
 * @note 初始化OLED UI，包括初始化OLED、初始化定时器、初始化编码器、初始化按键。
 * @return 无
 */
void OLED_UI_Init(MenuPage* Page)
{
    OLED_Init();
    OLED_UI_TimerInit();
    OLED_UI_EncoderInit();
    OLED_UI_KeyInit();

    //设置当前页面的结构体指针
	CurrentMenuPage = Page;	//设置当前页面的结构体指针

    //初始化当前页面
    OLED_UI_CurrentMenuPageInit();
}

/**
 * @brief 对上/下按键的长时间按下事件
 * @param 无
 * @note 该函数用于判断上/下按键是否长时间按下。
 * @return 无
 */
void OLED_UI_UpAndDownLongPressEvent(void)
{
    static int16_t Key_UpPressCount = 0;
    static int16_t Key_DownPressCount = 0;
    // 如果上下按键同时被按下，则不处理
    if(OLED_UI_Up._CurrentStatus == 0 && OLED_UI_Down._CurrentStatus == 0){
        return;
    }
    // 如果当前上键正在被按下
    if(OLED_UI_Up._CurrentStatus == 0){
        Key_UpPressCount ++;
    }else{
        Key_UpPressCount = 0;
    }
    if(OLED_UI_Down._CurrentStatus == 0){
        Key_DownPressCount ++;
    }else{
        Key_DownPressCount = 0;
    }
    // 防止数据溢出
    if(Key_UpPressCount >= 1000){
        Key_UpPressCount = 1000;
    }
    if(Key_DownPressCount >= 1000){
        Key_DownPressCount = 1000;
    }
    if(Key_UpPressCount > OLED_UI_SHORT_HOLD_TIME_MS/OLED_UI_INTERRUPT_TIME || Key_DownPressCount > OLED_UI_SHORT_HOLD_TIME_MS/OLED_UI_INTERRUPT_TIME){
        OLED_UI_UpDownLongPress.Counter ++;
        if(OLED_UI_UpDownLongPress.Counter >= OLED_UI_PRESSED_SHORT){
            if(Key_DownPressCount > OLED_UI_SHORT_HOLD_TIME_MS/OLED_UI_INTERRUPT_TIME){
                OLED_UI_UpDownLongPress.DeltaData = 1;
            }else{
                OLED_UI_UpDownLongPress.DeltaData = -1;
            }
            OLED_UI_UpDownLongPress.Counter = 0;
        }
    }
    if(Key_DownPressCount > OLED_UI_LONG_HOLD_TIME_MS/OLED_UI_INTERRUPT_TIME || Key_UpPressCount > OLED_UI_LONG_HOLD_TIME_MS/OLED_UI_INTERRUPT_TIME){
        OLED_UI_UpDownLongPress.Counter ++;
        if(OLED_UI_UpDownLongPress.Counter >=OLED_UI_PRESSED_LONG){
            if(Key_DownPressCount > OLED_UI_LONG_HOLD_TIME_MS/OLED_UI_INTERRUPT_TIME){
                OLED_UI_UpDownLongPress.DeltaData = 1;
            }else{
                OLED_UI_UpDownLongPress.DeltaData = -1;
            }
            
            OLED_UI_UpDownLongPress.Counter = 0;
        }
    }
}

/**
 * @brief 需要在定时器中断中调用该函数
 * @param 无
 * @note 该函数用于接受编码器与按键的输入。
 * @return 无
 */
void OLED_UI_InterruptHandler(void)
{
    
    //帧数计数器
    OLED_UI_CountFPS_Interrupt();
    //处理输入
    OLED_UI_HandleInput_Interrupt();
    // 上下按键的短时间按下和长时间按下判定
    OLED_UI_UpAndDownLongPressEvent();

    // 如果当前不为空
    //如果当前正在运行窗口，那么计数
	if(OLED_UI_WindowStatus.SustainFlag == true){
		OLED_UI_WindowStatus.Counter++;
	}

}





        /*===============================OLED-UI实现函数==============================*/
    /*===================================OLED-UI实现函数==================================*/
/*=======================================OLED-UI实现函数======================================*/






