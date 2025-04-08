#include "OLED_UI.h"


/*=======================================OLED-UIȫ�����ݽṹ======================================*/
/*=======================================OLED-UIȫ�����ݽṹ======================================*/
/*=======================================OLED-UIȫ�����ݽṹ======================================*/


// ȫ�ֱ�������������״̬���ƽṹ��.�������������룬����Ҫ����ǰ���ϴ�״̬����ʼ��Ϊ0
OLED_UI_KeyControl OLED_UI_Enter={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_ENTER_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_ENTER_LONG_PRESS_TIME_MS};
OLED_UI_KeyControl OLED_UI_Back={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_BACK_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_BACK_LONG_PRESS_TIME_MS};
OLED_UI_KeyControl OLED_UI_Up={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_UP_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_UP_LONG_PRESS_TIME_MS};
OLED_UI_KeyControl OLED_UI_Down={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_DOWN_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_DOWN_LONG_PRESS_TIME_MS};
OLED_UI_KeyControl OLED_UI_EncoderKey={._CurrentStatus = 1,._LastStatus = 1,.SetClickTime = OLED_UI_ENCODER_KEY_CLICK_TIME_MS,.SetLongPressTime = OLED_UI_ENCODER_KEY_LONG_PRESS_TIME_MS};

// ȫ�ֱ������洢֡��
OLED_UI_Counter OLED_UI_FPS = {.Counter = 0,.Time = 0,.Value = 0};

OLED_UI_WindowSustainCounter OLED_UI_WindowStatus = {.SustainFlag = false};

// ȫ�ֱ������洢�˵�IDƫ����
int16_t OLED_UI_DeltaMenuID = 0;

// ȫ�ֱ�����������־λ
OLED_UI_LongPress OLED_UI_UpDownLongPress = {.DeltaData = 0,.Counter = 0};



// ȫ�ֽṹ��ָ�룬ָ��ǰ�˵�ҳ��
MenuPage *CurrentMenuPage = NULL;

// ȫ�ֽṹ��ָ�룬ָ��ǰ�˵�����
MenuWindow *CurrentWindow = NULL;

/*=======================================OLED-UIȫ�����ݽṹ======================================*/
/*=======================================OLED-UIȫ�����ݽṹ======================================*/
/*=======================================OLED-UIȫ�����ݽṹ======================================*/

/*��Ҫ����Ч���Ľṹ��ע��ΪOLED_UI_xxxAnimationȫ�ֽṹ��*/

// OLED_UI_Cursor�����ڴ洢���Ľṹ�壬���ƹ����ƶ�
OLED_UI_AreaAnimation OLED_UI_Cursor;

// OLED_UI_MenuFrame�����ڴ洢�˵��߿�Ľṹ�壬���Ʋ˵��߿����ڲ�������ƶ�
OLED_UI_AreaAnimation OLED_UI_MenuFrame;

// OLED_UI_Window�����ڴ洢���ڵĽṹ�壬���ƴ��ڵ��ƶ�
OLED_UI_AreaAnimation OLED_UI_Window;

// OLED_UI_ScrollBarHeight�����ڴ洢�������߶ȵĽṹ�壬���ƹ������߶ȵı仯
OLED_UI_DistanceAnimation OLED_UI_ScrollBarHeight;

// OLED_UI_ScrollBarStartPoint�����ڴ洢��������ʼ��Ľṹ�壬���ƹ�����������ƶ�
OLED_UI_PointAnimation OLED_UI_ScrollBarStartPoint;

// OLED_UI_PageStartPoint�����ڴ洢ҳ����ʼ��Ľṹ�壬����ҳ��������ƶ�
OLED_UI_PointAnimation OLED_UI_PageStartPoint;

// OLED_UI_ProbWidth�����ڴ洢��������ȵĽṹ�壬���ƽ�������ȵı仯
OLED_UI_DistanceAnimation OLED_UI_ProbWidth;

// OLED_UI_LineStep�����ڴ洢�м��Ľṹ�壬�����м��ı仯
OLED_UI_DistanceAnimation OLED_UI_LineStep;

// OLED_UI_Tiles_FontTopDistance�����ڴ洢���嶥������Ľṹ�壬�������嶥������ı仯[������ר��]
OLED_UI_DistanceAnimation OLED_UI_Tiles_FontTopDistance;

/*=======================================OLED-UI���ߺ���======================================*/
    /*===================================OLED-UI���ߺ���==================================*/
        /*===============================OLED-UI���ߺ���==============================*/

/**
 * @brief �����ַ����Ŀ�ȣ���������Ӣ���ַ�����
 * @param Font ����߶�
 * @param format ��ʽ���ַ���
 * @return �ַ����Ŀ��
 */
int16_t OLED_UI_CalcStringWidth(int8_t Font, const char *format, ...) {
    int16_t StringLength = 0;
    char String[MAX_STRING_LENGTH]; // �̶���С������
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
        if ((unsigned char)*ptr & 0x80) { // ���������ַ�
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
 * @brief ���������Ļ
 * @param x ��ʼ������
 * @param y ��ʼ������
 * @param width ���
 * @param height �߶�
 * @note ���������Ļ��Ӧ�ý���Ч������ʾ���£���ʱ���ظ�5�Ρ�
 */
void OLED_UI_ClearAreaWithFadeOut(int16_t x, int16_t y, int16_t width, int16_t height){
    // ����ٶ�Ϊ0����ֱ�������Ļ
    if(CurrentMenuPage->General_MovingSpeed <= 0){
		OLED_Clear();
		return;
	}
    for(uint8_t i=1;i<=5;i++){
		// Ӧ�ý���Ч��
			
		OLED_FadeOut_Masking(x,y,width,height,i);
		// ��ʾ����
        OLED_Update();
		OLED_DelayMs(OLED_UI_FADE_TIME_MS);
    }
    OLED_DelayMs(OLED_UI_FADE_TIME_MS);
}



/**
 * @brief ��ȡ��ǰ��Ļˢ���ʣ�����洢��ȫ�ֱ���OLED_UI_FPS.value��
 * @param ��
 * @note ��ȷ��OLED_UI_Config.h�ж����OLED_UI_INTERRUPT_TIME��ʵ�����������⣬�˺�����Ҫ�����жϵ��С�
 * @return ��
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
 * @brief ��ʾFPS
 * @param ��
 * @note ����Ļ���Ͻ���ʾ��ǰ֡�ʡ�
 */
void OLED_UI_ShowFPS(void)
{
    int8_t StringLength = OLED_UI_CalcStringWidth(OLED_FONT_8, "FPS:%d", OLED_UI_FPS.Value);
    OLED_Printf(OLED_WIDTH - StringLength, 0, OLED_FONT_8, "FPS:%d", OLED_UI_FPS.Value);

}

/**
 * @brief ������״̬��ʹ�ô���ָ��ķ�ʽʵ�ָ���
 * @param key �������ƽṹ��ָ��
 * @note ������״̬������������˫�����������̰��������ͷŵȡ�
 */
void OLED_UI_KeyControlProcess(OLED_UI_KeyControl *key)
{

        //��ȷ�ϡ����������߼�
    if (key->_CurrentStatus != key->_LastStatus) {
        if (key->_CurrentStatus == 1) { 
            // �����ͷ��¼�
            /* ��������������ͷŴ��� */
            if (key->_ClickFrequency == 2) {
                // ������������ǲ�����״̬
                key->_ClickFrequency = 0;
                key->_ClickTime = 0;
                key->_ClickTimerFlag = 0;
                return; // ֱ���˳��������������߼�
            }

            if (key->Status != OLED_UI_KEY_LONG_PRESS) {
                // ��һ���ͷţ�����˫����ⴰ��
                if (key->_ClickFrequency == 0) {
                    key->_ClickFrequency = 1;  // ��¼��һ�ε��
                    key->_ClickTimerFlag = 1;  // ����˫����ʱ���
                    key->_ClickTime = 0;       // ���ü�ʱ��
                } 
                // �ڶ����ͷţ��ж�˫��
                else {
                    if (key->_ClickTime <= key->SetClickTime/OLED_UI_INTERRUPT_TIME) { // 500ms��
                        key->Status = OLED_UI_KEY_DOUBLE_CLICK;
                    }
                    // �����Ƿ�˫����������ɺ����ñ���
                    key->_ClickFrequency = 0;
                    key->_ClickTime = 0;
                    key->_ClickTimerFlag = 0;
                }
            }
        } else { 
            // ���������¼�
            // ��ʼ��������⣨�������õ��������
            key->_ClickTimerFlag = 1;  // ����������ʱ
            key->_ClickTime = 0;       // ���ü�ʱ��
            key->Status = OLED_UI_KEY_NONE; // �����״̬
        }
    }

    if (key->_ClickTimerFlag) {
        // �Ƚ��������ۼ�
        (key->_ClickTime) ++;

        // ������⣨���ڰ����Դ��ڰ���״̬ʱ��Ч��
        if (key->_CurrentStatus == 0) { 
            // ������ֵ������50*20ms=1000ms��
            if (key->_ClickTime  >= key->SetLongPressTime/OLED_UI_INTERRUPT_TIME) {
                key->Status = OLED_UI_KEY_LONG_PRESS;
                // ����������ǿ����������״̬������ǳ�������״̬
                key->_ClickFrequency = 2;  // �����������
                key->_ClickTime  = 0;
                key->_ClickTimerFlag = 0;
            }
        }
        // ˫����ʱ��⣨���ڰ������ͷ�ʱ��Ч��
        else { 
            // ��ʱ��ֵ��25*20ms=500ms��
            if (key->_ClickTime  > key->SetClickTime/OLED_UI_INTERRUPT_TIME) {
                // δ��������ʱ��ǵ���
                if (key->Status != OLED_UI_KEY_LONG_PRESS) {
                    key->Status = OLED_UI_KEY_CLICK;
                }
                // ����״̬
                key->_ClickFrequency = 0;
                key->_ClickTime  = 0;
                key->_ClickTimerFlag = 0;
            }
        }
    }

    // ���Ƴ���ʱ�䣨��ֹ�����
    if(key->_ClickTime >= key->SetLongPressTime/OLED_UI_INTERRUPT_TIME + 1){
        key->_ClickTime = key->SetLongPressTime/OLED_UI_INTERRUPT_TIME + 1;
    }

}

/**
 * @brief ���ݵ�ǰ��ѡ�Ķ�����ʽ���ı両��������
 * @param CurrentNum ��ǰֵ��ָ��
 * @param TargetNum Ŀ��ֵָ��
 * @param StartNum ��ʼֵָ��
 * @param CurrentStepNum ��ǰ����ָ��
 * @return ��
 */
void OLED_UI_ChangeFloat(float *CurrentNum, float *TargetNum, float *ErrorNum, float *LastErrorNum, float * IntegralNum, float *DerivativeNum)  {
	if(CurrentMenuPage->General_MoveStyle ==  UNLINEAR){
		if(*CurrentNum == *TargetNum){
			return;
		}
		//����û����ٶ�����Ϊ0����ô��ǰֱֵ�ӵ���Ŀ��ֵ
		if(CurrentMenuPage->General_MovingSpeed <= 0){
			*ErrorNum = 0;
			
		    *CurrentNum = *TargetNum;
			return;
		}
		
		//���㱾�����ֵ
		*ErrorNum = *TargetNum - *CurrentNum; 
		//���㵱ǰֵ
		*CurrentNum += 0.02*CurrentMenuPage->General_MovingSpeed * (*ErrorNum);
		//��Ŀ��ֵ�뵱ǰֵ���С���ٶ�ֵ��1/20ʱ����Ϊ�Ѿ�����Ŀ��ֵ
		if(fabs(*CurrentNum - *TargetNum) < fmaxf(CurrentMenuPage->General_MovingSpeed / 20.0f, 0.5f)) {

			*ErrorNum = 0;
			
		    *CurrentNum = *TargetNum;
			return;
		}
	}
	//�����ǰ�Ķ�����ʽ��PID_CURVE��ʽ
	if(CurrentMenuPage->General_MoveStyle ==  PID_CURVE){
		/*����һ�����صķ�������Ϊ����ǰֵ����Ŀ��ֵ��ʱ�������������ˣ����ǻ����û�б����㡣����ʵ������������Ч������õġ� */
		//����û����ٶ�����Ϊ0����ô��ǰֱֵ�ӵ���Ŀ��ֵ�����������м�ֵ����
		if(CurrentMenuPage->General_MovingSpeed <= 0){
			*ErrorNum = 0;
			*LastErrorNum = 0;
			*DerivativeNum = 0;
			*IntegralNum = 0;
		    *CurrentNum = *TargetNum;
			return;
		}
		// ����PID����
		//����PID����
		float Kp = OLED_UI_PID_ANIM_KP * CurrentMenuPage->General_MovingSpeed;
		float Ki = OLED_UI_PID_ANIM_KI * CurrentMenuPage->General_MovingSpeed;
		float Kd = OLED_UI_PID_ANIM_KD * CurrentMenuPage->General_MovingSpeed;
		//��¼��һ�����ֵ
		*LastErrorNum = *ErrorNum;
		//���㱾�����ֵ
		*ErrorNum = *TargetNum - *CurrentNum; 
		//�������ֵ
		*IntegralNum += *ErrorNum;
		//����΢��ֵ��������ʱ����Ϊ0.1s
		*DerivativeNum = (*ErrorNum - *LastErrorNum) / 0.1;
		//���㵱ǰֵ
		*CurrentNum += Kp * (*ErrorNum) + Ki *  (*IntegralNum) + Kd *(*DerivativeNum);
		//��Ŀ��ֵ�뵱ǰֵ���С��0.5ʱ����Ŀ��ֵǿ�Ƶ��ڵ�ǰֵ�����˻�������㣬���������м�ֵ����
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
 * @brief �ı�������
 * @param CurrentNum ��ǰֵ��ָ��
 * @param TargetNum Ŀ��ֵָ��
 * @param StepNum ����ָ��
 */
void OLED_UI_ChangeDistance(OLED_UI_DistanceAnimation *distance){
	OLED_UI_ChangeFloat(&distance->CurrentDistance,&distance->TargetDistance,&distance->_Error,&distance->_LastError,&distance->_Integral,&distance->_Derivative);
}

/**
 * @brief �ı���������
 * @param OLED_MovingArea �ṹ�������ָ��
 * @return ��
 */
void OLED_UI_ChangePoint(OLED_UI_PointAnimation *point){
	OLED_UI_ChangeFloat(&point->CurrentPoint.X,&point->TargetPoint.X,&point->_Error.X,&point->_LastError.X,&point->_Integral.X,&point->_Derivative.X);
	OLED_UI_ChangeFloat(&point->CurrentPoint.Y,&point->TargetPoint.Y,&point->_Error.Y,&point->_LastError.Y,&point->_Integral.Y,&point->_Derivative.Y);
}

/**
 * @brief �ı��������
 * @param OLED_MovingArea �ṹ�������ָ��
 * @return ��
 */
void OLED_UI_ChangeArea(OLED_UI_AreaAnimation *area)	{
	OLED_UI_ChangeFloat(&area->CurrentArea.X,&area->TargetArea.X,&area->_Error.X,&area->_LastError.X,&area->_Integral.X,&area->_Derivative.X);
	OLED_UI_ChangeFloat(&area->CurrentArea.Y,&area->TargetArea.Y,&area->_Error.Y,&area->_LastError.Y,&area->_Integral.Y,&area->_Derivative.Y);
	OLED_UI_ChangeFloat(&area->CurrentArea.Width,&area->TargetArea.Width,&area->_Error.Width,&area->_LastError.Width,&area->_Integral.Width,&area->_Derivative.Width);
	OLED_UI_ChangeFloat(&area->CurrentArea.Height,&area->TargetArea.Height,&area->_Error.Height,&area->_LastError.Height,&area->_Integral.Height,&area->_Derivative.Height);
}



/** 
* @brief ��ȡ��ǰ�˵�ҳ��������ɵĵĲ�λ����
* @param ��
* @return ��ǰ�˵�ҳ��Ĳ�λ����
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
  * @brief ��ȡ�˵���ṹ����������һ��Ԫ�ص�ID
  * @param ��
  * @return �˵���ṹ����������һ���ID
  */
int16_t OLED_UI_GetMaxMenuItemNum(void){
    int16_t num = 0;
    while(CurrentMenuPage->General_MenuItems[num].General_item_text != NULL){
        num+=1;
    }
    return num - 1;
}
/**
 * @brief �˵���������������ж�
 * @param ��
 * @note �жϲ˵�����������Ƿ����
 * @return ��
 */
bool OLED_UI_IfAnimationEnd(uint8_t AnimationType){
    // �����ǰ�������д���
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
 * @brief ���ù������߶�/���
 * @param ��
 * @note �ú����������ù������ĸ߶�/���
 * @return ��
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
 * @brief ���ô���ҳ��˵������ֵ���ʼ��
 * @param ��
 * @note �ú����������ô���ҳ��˵������ֵ���ʼ��
 * @return ��
 */
void OLED_UI_SetTiles_FontTopDistance(void){
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        OLED_UI_Tiles_FontTopDistance.TargetDistance = OLED_UI_MenuFrame.TargetArea.Y + CurrentMenuPage->Tiles_FontTopDistance;
    }
}

/**
 * @brief ���ù�������ʼ��
 * @param ��
 * @note �ú����������ù���������ʼ��
 */
void OLED_UI_SetTargetScrollBarStartPoint(void){

    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        OLED_UI_ScrollBarStartPoint.TargetPoint.X = OLED_UI_MenuFrame.CurrentArea.X;
        OLED_UI_ScrollBarStartPoint.TargetPoint.Y = CurrentMenuPage->Tiles_ScrollBarTopDistance + OLED_UI_MenuFrame.CurrentArea.Y ;
    }

}

/**
 * @brief ���ò˵�����߶�/���
 * @note �ú����������ò˵�����ĸ߶�/���
 * @return ��
 */
void OLED_UI_SetTargetMenuArea(void)
{
    OLED_UI_MenuFrame.TargetArea.X = CurrentMenuPage->General_MenuArea.X;
    OLED_UI_MenuFrame.TargetArea.Y = CurrentMenuPage->General_MenuArea.Y;
	OLED_UI_MenuFrame.TargetArea.Height = CurrentMenuPage->General_MenuArea.Height;
	OLED_UI_MenuFrame.TargetArea.Width = CurrentMenuPage->General_MenuArea.Width;
	
}

/**
 * @brief ���ù������
 * @param ��
 * @note �ú����������ù���Ŀ����ʾ����
 * @return ��
 */
void OLED_UI_SetTargetCursorArea(void)
{
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST)
    {
        //Ŀ����x������� ��ʼ��ӡҳ����ʼ���x�����1����ȥ1��Ϊ��ȷ����긲�ǵ��˵�������֡�
		OLED_UI_Cursor.TargetArea.X = OLED_UI_PageStartPoint.TargetPoint.X -1;

        //Ŀ����y������� ��ʼ��ӡҳ����ʼ���y����  ����  (����߶� �� �о�)�� ��ǰҳ���ID�� ��1����ȥ1��Ϊ��ȷ����긲�ǵ��˵�������֡�
		OLED_UI_Cursor.TargetArea.Y = OLED_UI_PageStartPoint.TargetPoint.Y + CurrentMenuPage->_ActiveMenuID * ( CurrentMenuPage->General_LineSpace + CurrentMenuPage->General_FontSize) -1 ;

        //Ŀ����߶ȵ��� ����߶� ��2����2��Ϊ��ȷ����긲�ǵ��˵�������֡�
		OLED_UI_Cursor.TargetArea.Height = CurrentMenuPage->General_FontSize + 2;

        int8_t LinePerfixWidth = 0;
        int8_t RadioCompensationWidth = 0;
		if(CurrentMenuPage->List_IfDrawLinePerfix == true){
			LinePerfixWidth = OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_ASCII) + LINEPERFIX_DISTANCE;
		}else{
            LinePerfixWidth = 0;
        }
        //�����Ҫ���Ƶ�ѡ��(��BoolRadioBox��Ϊ��)
        if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox != NULL){
            RadioCompensationWidth = (OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_CHINESE) + 2);
        }else{
            RadioCompensationWidth = 0;
        }

        OLED_UI_Cursor.TargetArea.Width =  fmin((float)
            OLED_UI_CalcStringWidth(CurrentMenuPage->General_FontSize,CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_item_text)
            + LinePerfixWidth + 2, 
			//��ǰҳ��Ŀ�� �� ��ǰҳ�����ʼ���� ��ȥ��ʼ��ӡҳ����ʼ������� ��ȥ6���ǹ�������ȼ�һ��������ǰ׺�Ŀ��
			OLED_UI_MenuFrame.TargetArea.Width + OLED_UI_MenuFrame.TargetArea.X - OLED_UI_PageStartPoint.TargetPoint.X - 6 - RadioCompensationWidth ) ;
    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES)
    {
        //Ŀ����Y������� ��ʼ��ӡҳ����ʼ���Y�����1����ȥ1��Ϊ��ȷ����긲�ǵ��˵�������֡�
		OLED_UI_Cursor.TargetArea.Y = OLED_UI_PageStartPoint.TargetPoint.Y - 3;

        //Ŀ����y������� ��ʼ��ӡҳ����ʼ���X����  ����  (�����߶� �� �о�)�� ��ǰҳ���ID�� ��1����ȥ1��Ϊ��ȷ����긲�ǵ��˵�������֡�
        OLED_UI_Cursor.TargetArea.X = OLED_UI_PageStartPoint.TargetPoint.X + CurrentMenuPage->_ActiveMenuID * ( CurrentMenuPage->General_LineSpace + CurrentMenuPage->Tiles_TileWidth) - 3 ;

        //Ŀ����߶ȵ��� �����߶� ��2����2��Ϊ��ȷ����긲�ǵ��˵�������֡�
		OLED_UI_Cursor.TargetArea.Height = CurrentMenuPage->Tiles_TileHeight + 6;

        //Ŀ�����ȵ��� ������� ��2����2��Ϊ��ȷ����긲�ǵ��˵�������֡�
		OLED_UI_Cursor.TargetArea.Width = CurrentMenuPage->Tiles_TileWidth + 6;

    }

}

/**
 * @brief ����ǰҳ���LineSlip ����Ϊ0
 * @param ��
 * @return ��
 */
void OLED_UI_SetLineSplitZero(void){

	for(int16_t i = 0; i<=OLED_UI_GetMaxMenuItemNum();i++){
		CurrentMenuPage->General_MenuItems[i]._LineSlip = 0;
	}
}

/**
 * @brief ���˵��������ϻ����ƶ�һ��
 * @param ��
 * @return ��
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
 * @brief ���˵��������»����ƶ�һ��
 * @param ��
 * @return ��
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
 * @brief ��ȡ�봦������
 * @param ��
 * @note �뱣֤OLED_UI_Config.h�ж����OLED_UI_INTERRUPT_TIME��ʵ����������
 * @note �ú����ڶ�ʱ���ж��е��ã����ڴ������롣
 * @return ��
 */
void OLED_UI_HandleInput_Interrupt(void)
{
    //��¼��һ�ֵİ���״̬
    OLED_UI_Enter._LastStatus = OLED_UI_Enter._CurrentStatus;
    OLED_UI_Back._LastStatus = OLED_UI_Back._CurrentStatus;
    OLED_UI_Up._LastStatus = OLED_UI_Up._CurrentStatus;
    OLED_UI_Down._LastStatus = OLED_UI_Down._CurrentStatus;
    OLED_UI_EncoderKey._LastStatus = OLED_UI_EncoderKey._CurrentStatus;


    //��ȡ����״̬
    OLED_UI_Enter._CurrentStatus = Key_GetEnterStatus();
    OLED_UI_Back._CurrentStatus = Key_GetBackStatus();
    OLED_UI_Up._CurrentStatus = Key_GetUpStatus();
    OLED_UI_Down._CurrentStatus = Key_GetDownStatus();
    OLED_UI_EncoderKey._CurrentStatus = Key_GetEncoderStatus();

    //������״̬
    OLED_UI_KeyControlProcess(&OLED_UI_Enter);
    OLED_UI_KeyControlProcess(&OLED_UI_Back);
    OLED_UI_KeyControlProcess(&OLED_UI_Up);
    OLED_UI_KeyControlProcess(&OLED_UI_Down);
    OLED_UI_KeyControlProcess(&OLED_UI_EncoderKey);

    
}

/**
 * @brief ��ת��ʾ���
 * @param X ���X����
 * @param Y ���Y����
 * @param Width �����
 * @param Height ���߶�
 * @param Style �����ʽ
 * @note �ú������ڷ�ת��ʾ��꣬ʹ����ʾΪ��ɫ
 * @return ��
 */
void OLED_UI_ReverseCoordinate(int16_t X, int16_t Y, int16_t Width, int16_t Height,uint8_t Style){
    // �Թ������޷�
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
		
		case CURSOR_REVERSE_RECTANGLE://���η�ɫ
			OLED_ReverseArea(X, Y, Width, Height);
			break;
		case CURSOR_REVERSE_ROUNDRECTANGLE://Բ�Ǿ��η�ɫ
			if(Width >= 3 && Height >= 3){
				OLED_ReverseArea(X, Y, 1, 1);
				OLED_ReverseArea(X+Width-1, Y, 1, 1);
				OLED_ReverseArea(X, Y+Height-1, 1, 1);
				OLED_ReverseArea(X+Width-1, Y+Height-1, 1, 1);
			}
			OLED_ReverseArea(X, Y, Width, Height);
			break;

		case CURSOR_HOLLOW_RECTANGLE://���ľ���
			OLED_ReverseArea(X, Y, Width, Height);
			OLED_ReverseArea(X+1, Y+1, Width-2, Height-2);
			break;
		case CURSOR_HOLLOW_ROUNDRECTANGLE://���ľ���
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
		case CURSOR_REVERSE_BLOCK://С����
			
			OLED_ReverseArea(X, Y + 1, OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_ASCII), Height);
		
			break;
        case CURSOR_ARROW://��ͷ
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
 * @brief ��CurrentMenuPage��ʼ��
 * @param ��
 * @note ��CurrentMenuPage�����Գ�ʼ������ȫ�ֶ�������������ϵ
 * @return ��
 */
void OLED_UI_CurrentMenuPageInit(void){
    // ��ʼ���˵���Ŀ������
    OLED_UI_MenuFrame.TargetArea.X = CurrentMenuPage->General_MenuArea.X;
    OLED_UI_MenuFrame.TargetArea.Y = CurrentMenuPage->General_MenuArea.Y;
    OLED_UI_MenuFrame.TargetArea.Width = CurrentMenuPage->General_MenuArea.Width;
    OLED_UI_MenuFrame.TargetArea.Height = CurrentMenuPage->General_MenuArea.Height;
    //��ʼ���о��������
	OLED_UI_LineStep.TargetDistance = CurrentMenuPage->General_LineSpace;
    //��ʼ����ǰ�м���������
    OLED_UI_LineStep.CurrentDistance = CurrentMenuPage->General_LineSpace;
    //��ʼ����ǰ��Ծ���˵���ID
	CurrentMenuPage->_ActiveMenuID = CurrentMenuPage->General_InitMenuID;
	//��ʼ����ǰ��λ
	CurrentMenuPage->_Slot = CurrentMenuPage->General_InitSlot;
    // ���Ϊ�Ѿ���ʼ��
    CurrentMenuPage->_IfInit = true;

    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
        // ��ʼ���˵��Ŀ�ʼ����Ŀ��ֵ
        OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->General_StartPoint.X + CurrentMenuPage->General_MenuArea.X;
        OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->General_MenuArea.Y - (CurrentMenuPage->General_InitMenuID - CurrentMenuPage->General_InitSlot)*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);
        // ��ʼ���˵��ĵ�ǰֵ
		OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X+CurrentMenuPage->General_StartPoint.X + OLED_WIDTH;
		OLED_UI_PageStartPoint.CurrentPoint.Y = CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->General_MenuArea.Y - (CurrentMenuPage->General_InitMenuID - CurrentMenuPage->General_InitSlot)*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);
        // ��ʼ�����ĵ�ǰ����
        OLED_UI_Cursor.CurrentArea.Width = 0;
        OLED_UI_Cursor.CurrentArea.Height = CurrentMenuPage->General_FontSize + 2;
        OLED_UI_Cursor.CurrentArea.X = CurrentMenuPage->General_MenuArea.X + CurrentMenuPage->General_MenuArea.Width;
        OLED_UI_Cursor.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y + CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->General_InitSlot*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);

    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        // ��ʼ���˵���Ŀ��ֵ
        OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->General_StartPoint.X + CurrentMenuPage->General_MenuArea.X - (CurrentMenuPage->General_InitMenuID - CurrentMenuPage->General_InitSlot)*(CurrentMenuPage->Tiles_TileWidth + CurrentMenuPage->General_LineSpace);
        OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->General_MenuArea.Y;
        // ��ʼ���˵��ĵ�ǰֵ
		OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_PageStartPoint.TargetPoint.X + 2*CurrentMenuPage->Tiles_TileWidth;
		OLED_UI_PageStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y - CurrentMenuPage->Tiles_TileHeight;
        // ��ʼ���˵��ĵ�ǰ���򣨶���Tiles�˵�����������õ�ǰ���򣬶������и��ѸУ�
        OLED_UI_MenuFrame.CurrentArea.X = CurrentMenuPage->General_MenuArea.X;
        OLED_UI_MenuFrame.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y;
        OLED_UI_MenuFrame.CurrentArea.Width = CurrentMenuPage->General_MenuArea.Width;
        OLED_UI_MenuFrame.CurrentArea.Height = CurrentMenuPage->General_MenuArea.Height;
        // ��ʼ�����ĵ�ǰ����
        OLED_UI_Cursor.CurrentArea.Width = 0;
        OLED_UI_Cursor.CurrentArea.Height = 0;
        OLED_UI_Cursor.CurrentArea.X = OLED_UI_MenuFrame.CurrentArea.X;
        OLED_UI_Cursor.CurrentArea.Y = OLED_UI_MenuFrame.CurrentArea.Y;
        // ��ʼ����������ʼ�㣨����Tiles�˵�����������ʼ�������е����ԣ�
        OLED_UI_ScrollBarStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X;
        OLED_UI_ScrollBarStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y + OLED_UI_MenuFrame.CurrentArea.Height;
        //�������嵽�����ľ��루����Tiles�˵������嵽�����ľ��������е����ԣ�
        OLED_UI_Tiles_FontTopDistance.CurrentDistance = OLED_UI_MenuFrame.TargetArea.Y + OLED_UI_MenuFrame.TargetArea.Height;
        
    }
    
}

/**
 * @brief ��CurrentMenuPage�ı��ݻ�ԭ
 * @param ��
 * @note �����ص�ʱ�򣬻�ԭCurrentMenuPage�ı���
 * @return ��
 */
void OLED_UI_CurrentMenuPageBackUp(void){
    // �ָ��˵���Ŀ��ֵ
    OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->_StartPoint.X;
    OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->_StartPoint.Y;
    //�ָ���ǰ�м��
	OLED_UI_LineStep.CurrentDistance = CurrentMenuPage->General_LineSpace;
	//�ָ�Ŀ���м��
	OLED_UI_LineStep.TargetDistance = CurrentMenuPage->General_LineSpace;
    // �ָ��˵���Ŀ������
    OLED_UI_MenuFrame.TargetArea.X = CurrentMenuPage->General_MenuArea.X;
    OLED_UI_MenuFrame.TargetArea.Y = CurrentMenuPage->General_MenuArea.Y;
    OLED_UI_MenuFrame.TargetArea.Width = CurrentMenuPage->General_MenuArea.Width;
    OLED_UI_MenuFrame.TargetArea.Height = CurrentMenuPage->General_MenuArea.Height;

    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
        // �ָ��˵��ĵ�ǰֵ
        OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X + CurrentMenuPage->General_StartPoint.X - OLED_WIDTH;
		OLED_UI_PageStartPoint.CurrentPoint.Y = CurrentMenuPage->_StartPoint.Y;
        // ���ù���Ŀ������
        OLED_UI_Cursor.CurrentArea.Width = 0;
        OLED_UI_Cursor.CurrentArea.Height = CurrentMenuPage->General_FontSize + 2;
        OLED_UI_Cursor.CurrentArea.X = CurrentMenuPage->General_MenuArea.X + CurrentMenuPage->General_StartPoint.X;
        OLED_UI_Cursor.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y + CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->_Slot*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);

    }
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
        // �ָ��˵��ĵ�ǰֵ
        OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_PageStartPoint.TargetPoint.X - 2*CurrentMenuPage->Tiles_TileWidth;
		OLED_UI_PageStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y - CurrentMenuPage->Tiles_TileHeight;
        // �ָ�����Ŀ������
        OLED_UI_Cursor.CurrentArea.Width = 0;
        OLED_UI_Cursor.CurrentArea.Height = CurrentMenuPage->General_FontSize + 2;
        OLED_UI_Cursor.CurrentArea.X = CurrentMenuPage->General_MenuArea.X + CurrentMenuPage->General_StartPoint.X;
        OLED_UI_Cursor.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y + CurrentMenuPage->General_StartPoint.Y + CurrentMenuPage->_Slot*(CurrentMenuPage->General_FontSize + CurrentMenuPage->General_LineSpace);
        // ��ʼ���˵��ĵ�ǰ���򣨶���Tiles�˵�����������õ�ǰ���򣬶������и��ѸУ�
        OLED_UI_MenuFrame.CurrentArea.X = CurrentMenuPage->General_MenuArea.X;
        OLED_UI_MenuFrame.CurrentArea.Y = CurrentMenuPage->General_MenuArea.Y;
        OLED_UI_MenuFrame.CurrentArea.Width = CurrentMenuPage->General_MenuArea.Width;
        OLED_UI_MenuFrame.CurrentArea.Height = CurrentMenuPage->General_MenuArea.Height;
        // �ָ���������ʼ�㣨����Tiles�˵�����������ʼ�������е����ԣ�
        OLED_UI_ScrollBarStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X;
        OLED_UI_ScrollBarStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y + OLED_UI_MenuFrame.CurrentArea.Height;
        //�ָ����嵽�����ľ��루����Tiles�˵������嵽�����ľ��������е����ԣ�
        OLED_UI_Tiles_FontTopDistance.CurrentDistance = OLED_UI_MenuFrame.TargetArea.Y + OLED_UI_MenuFrame.TargetArea.Height;

    }
}

/**
 * @brief �˺�������ȷ������
 * @param ��
 * @return ��
 */
void OLED_UI_EnterTask(void){
    // ȷ����������,0Ϊ�޲�����1ΪBoolRadioBoxȷ������2Ϊ�ص�����ȷ������3Ϊ�Ӳ˵�ȷ������.�����Ӳ˵����ȼ���ߡ�
    int8_t EnterTaskType = 0;
    int8_t Offset = 0;
    int16_t X;
    int16_t Y;
    int16_t Width;
    int16_t Height;

    // �����ǰ�˵�����BoolRadioBox����ȷ����������Ϊ1
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
        case 1: // BoolRadioBoxȷ������
            *CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox = !(*CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox);
            break;
        case 2: // �ص�����ȷ������
            OLED_UI_EncoderDisable();
            CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_callback();
            OLED_UI_EncoderEnable();
            break;

        case 3: // �Ӳ˵�ȷ������
            if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage->General_MenuType == MENU_TYPE_LIST && CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
                Offset = -5;
            }
            //������˵�û�б߿��ҵ�ǰ�˵��б߿�
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
            //����ǰ�˵���λ�ñ��棬�Ա㷵��ʱ�ָ�
			CurrentMenuPage->_StartPoint.X = OLED_UI_PageStartPoint.TargetPoint.X;
			CurrentMenuPage->_StartPoint.Y = OLED_UI_PageStartPoint.TargetPoint.Y;
            //�����ǰ�˵�ҳ��
            //�����ǰ�˵�ҳ��
            OLED_UI_ClearAreaWithFadeOut(X,Y,Width,Height);
            // ��ת���Ӳ˵�
            CurrentMenuPage = CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage;
            // ������ƫ��ֵ����
            OLED_UI_SetLineSplitZero();
            OLED_UI_CurrentMenuPageInit();
            break;
        default:
            break;
    }

    
}

/**
 * @brief �˺�������ȡ������
 * @param ��
 * @return ��
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
        //������˵�û�б߿��ҵ�ǰ�˵��б߿�
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

        
        
        //�����ǰ�˵�ҳ��
        OLED_UI_ClearAreaWithFadeOut(X,Y,Width,Height);
        // ��ת�����˵�
        CurrentMenuPage = CurrentMenuPage->General_ParentMenuPage;
        // ������ƫ��ֵ����
        OLED_UI_SetLineSplitZero();
        if(CurrentMenuPage->_IfInit == false){
            OLED_UI_CurrentMenuPageInit();
        }else{
            OLED_UI_CurrentMenuPageBackUp();
        }
    }

}
/**
 * @brief ��ȡ�����������
 * @param window
 * @return ����ֵΪ���� WINDOW_DATA_STYLE_FLOAT ��int16_tֵΪ���� WINDOW_DATA_STYLE_INT ����ָ�뷵�� WINDOW_DATA_STYLE_NONE
 */
int8_t OLED_UI_GetWindowProbDataStyle(MenuWindow* window){

	//����������ʷǷ��ڴ�
	if(window == NULL){
		return WINDOW_DATA_STYLE_NONE;
	}
	// ���������������Ϊint8_t
	if(window->Prob_Data_Int_8_t != NULL){
		return WINDOW_DATA_STYLE_INT8;
		// ���������������Ϊint16_t
	}else if(window->Prob_Data_Int_16_t != NULL){
		return WINDOW_DATA_STYLE_INT16;
		// ���������������Ϊint32_t
	}else if(window->Prob_Data_Int_32_t != NULL){
		return WINDOW_DATA_STYLE_INT32;
		// ���������������Ϊint64_t
	}else if(window->Prob_Data_Int_64_t != NULL){
		return WINDOW_DATA_STYLE_INT64;
		// ���������������Ϊfloat
	}else if(window->Prob_Data_Float != NULL){
		return WINDOW_DATA_STYLE_FLOAT;
	}else{
		return WINDOW_DATA_STYLE_NONE;
	}
}
/**
 * @brief ��ѭ���д�������
 * @param ��
 * @note ��ѭ����ͨ���жϵ��еİ���״̬��������ִ��һЩ����
 * @return ��
 */
void OLED_UI_HandleInput_MainLoop(void){
    // ��ֹ��ʱ���ж�,���ñ�����ֵ��ʱ�򱻶�ʱ���жϴ��
    OLED_UI_TimerDisable();
    int8_t enter_states = OLED_UI_Enter.Status;
    int8_t back_states = OLED_UI_Back.Status;
    int8_t up_states = OLED_UI_Up.Status;
    int8_t down_states = OLED_UI_Down.Status;
    int8_t encoder_states = OLED_UI_EncoderKey.Status;
    // ��ȡ�����ϻ��°������õ�������
    OLED_UI_DeltaMenuID+=OLED_UI_UpDownLongPress.DeltaData;
    OLED_UI_UpDownLongPress.DeltaData = 0;
    // ���ö�ʱ���ж�
    OLED_UI_TimerEnable();
    int16_t MaxMenuID;

    
    
    
    // ��ȡ��������ת������ֵ
    OLED_UI_DeltaMenuID += OLED_UI_EncoderGet();

    


    // ȷ�ϰ���������
    switch(enter_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================�������������߼�==============================*/

        if(OLED_UI_WindowStatus.SustainFlag == false){
            OLED_UI_EnterTask();
        }else{
            OLED_UI_WindowStatus.Counter = 0;
        }

        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================˫�����������߼�==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================�������������߼�==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================�޲����Ĵ����߼�==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }

    // ȡ������������
    switch(back_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================�������������߼�==============================*/
        if(OLED_UI_WindowStatus.SustainFlag == false){
            OLED_UI_BackTask();
        }else{
            OLED_UI_WindowStatus.Counter = CurrentWindow->General_StayTime;
        }

        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================˫�����������߼�==============================*/


        

        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================�������������߼�==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================�޲����Ĵ����߼�==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }

    // �ϰ���������
    switch(up_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================�������������߼�==============================*/

        // �仯ֵ�Լ�1
        OLED_UI_DeltaMenuID -= 1;


        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================˫�����������߼�==============================*/

        


        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================�������������߼�==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================�޲����Ĵ����߼�==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }

    // �°���������
    switch(down_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================�������������߼�==============================*/

        // �仯ֵ����1
        OLED_UI_DeltaMenuID += 1;


        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================˫�����������߼�==============================*/

        


        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================�������������߼�==============================*/

        


        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================�޲����Ĵ����߼�==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }

    // ����������������
    switch(encoder_states)
    {
        case OLED_UI_KEY_CLICK:
        /*============================�������������߼�==============================*/
        if(OLED_UI_WindowStatus.SustainFlag == false){

            OLED_UI_EnterTask();
        }else{
            OLED_UI_WindowStatus.Counter = 0;
        }

        


        /*========================================================================*/
            break;
        case OLED_UI_KEY_DOUBLE_CLICK:
        /*============================˫�����������߼�==============================*/




        /*========================================================================*/
            break;
        case OLED_UI_KEY_LONG_PRESS:
        /*============================�������������߼�==============================*/

        if(OLED_UI_WindowStatus.SustainFlag == false){
            OLED_UI_BackTask();
        }else{
            OLED_UI_WindowStatus.Counter = CurrentWindow->General_StayTime;
        }
        

        /*========================================================================*/
            break;
        case OLED_UI_KEY_NONE:
        /*============================�޲����Ĵ����߼�==============================*/




        /*========================================================================*/
            break;
        default:
            break;
    }
    
    // �����ǰ�д����¼��������򽫻�ȡ�����������ڴ������¼�
    if(OLED_UI_WindowStatus.SustainFlag != false){
        // ��ȡ������������
        
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


    // ��ȡ���˵�id
    MaxMenuID = OLED_UI_GetMaxMenuItemNum();


    // ȷ�� ActiveMenuID �ںϷ���Χ��
    // ��DeltaMenuID�޷�����ֹ�����˵���Χ
    if(CurrentMenuPage->_ActiveMenuID + OLED_UI_DeltaMenuID < 0)
    {
        OLED_UI_DeltaMenuID = -CurrentMenuPage->_ActiveMenuID;
    }
    if(CurrentMenuPage->_ActiveMenuID + OLED_UI_DeltaMenuID > MaxMenuID)
    {
        OLED_UI_DeltaMenuID = MaxMenuID - CurrentMenuPage->_ActiveMenuID;
    }



    


    // ��λ����״̬
    OLED_UI_Enter.Status = OLED_UI_KEY_NONE;
    OLED_UI_Back.Status = OLED_UI_KEY_NONE;
    OLED_UI_Up.Status = OLED_UI_KEY_NONE;
    OLED_UI_Down.Status = OLED_UI_KEY_NONE;
    OLED_UI_EncoderKey.Status = OLED_UI_KEY_NONE;

}

/**
 * @brief ���ò˵���ʼ�����Ŀ��ֵ
 * @param ��
 * @note ���ò˵���ʼ�����Ŀ��ֵ�����ڶ���Ч��
 * @return ��
 */
void OLED_UI_SetTargetMenuStartPoint(void){
    if(OLED_UI_DeltaMenuID == 0){
        return;
    }else{
        if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
            // ��������������ڴ����˵�����ÿ�β�����ʱ�����ƫ������
            OLED_UI_SetLineSplitZero();
        }
    }
    if(OLED_UI_DeltaMenuID < 0 ){
        
        for(int i = 0; i < -OLED_UI_DeltaMenuID; i++){
            if(CurrentMenuPage->_Slot == 0 && CurrentMenuPage->_ActiveMenuID !=0){
                OLED_UI_MenuItemsMovePlus();

            }
            //�����껹û�е�������Ĳ�λ����ô�������ƶ���λ
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
            //�����껹û�е�����ײ��Ĳ�λ����ô�������ƶ���λ
			if(CurrentMenuPage->_Slot < OLED_UI_GetMaxSlotNum()-1){
				CurrentMenuPage->_Slot++;
			}
            CurrentMenuPage->_ActiveMenuID++;
        }
    }
    OLED_UI_DeltaMenuID = 0;



}

/**
 * @brief ���ý�������ȵ�Ŀ��ֵ
 * @param ��
 * @note ���ý�������ȵ�Ŀ��ֵ�����ڶ���Ч��
 * @return ��
 */
void OLED_UI_SetTargetProbWidth(void){
    // ��ֹ���ʷǷ��ڴ�
    if(CurrentWindow != NULL){
        int8_t DataStyle = OLED_UI_GetWindowProbDataStyle(CurrentWindow);
        // �����ǰ�����н������������ý�������ȵ�Ŀ��ֵ
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
 * @brief ��ӡ�ַ�����������Ҫ������ʱ�������ʾ
 * @note ����ַ�����ȳ����������ȣ���ô�͹�����ʾ
 * @return ��
 */
void OLED_UI_PrintStringScroll(
    int16_t areaX,int16_t areaY,int16_t areaWidth,int16_t areaHeight,       //��ӡ������
    int16_t X, int16_t Y,                                              //��ӡ����ʼλ��
    int16_t compareWidth,                                               //�ȽϵĿ��
    int8_t font,
    const char* menuText,
    float * slip,bool ifAnimationEnd)
{

    // �����Ⱥ͸߶ȱ������0
    if(areaWidth <= 0 || areaHeight <= 0){return;}

    // ��ȡ�ַ������
    int16_t StringLength = OLED_UI_CalcStringWidth(font, menuText);

    // �����ı����
    int16_t StrDis = compareWidth/3;

    // ֻ�е��ı���ȳ���������ʱ�Ź���
    if (StringLength > compareWidth)
    {

        // ���ù���λ��
        if (*slip < -StringLength - StrDis) {
            *slip = 0;
        }
        if(ifAnimationEnd){
            // ���¹���λ��
            *slip -= OLED_UI_LONG_MENU_SCROLL_PIXELS;
        }
        // ����ʵ�ʻ���λ��
        int16_t pos1 = X + (int16_t)*slip;
        int16_t pos2 = X + (int16_t)*slip + StringLength + StrDis;

        // �������ڻ��������ַ���
        OLED_PrintfArea(areaX, areaY, areaWidth, areaHeight, pos1, Y, font, menuText);
        OLED_PrintfArea(areaX, areaY, areaWidth, areaHeight, pos2, Y, font, menuText);

    } else {
        // ֱ�ӻ��ƾ�̬�ı�
        OLED_PrintfArea(areaX, areaY, areaWidth, areaHeight, X, Y, font, menuText);
        * slip = 0;
    }

}

 
/**
 * @brief ����Ļ�ϴ�ӡ�˵���
 * @param ��
 * @note ����Ļ�ϴ�ӡ�˵�������˵������ơ��˵���ֵ�ȡ�
 * @return ��
 */
void OLED_UI_PrintMenuItems(void)
{
    // ��ȡ�˵�����
    int16_t MenuNum = OLED_UI_GetMaxMenuItemNum();

    // ��ȡ����������
    uint8_t ChineseFontWidth = OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_CHINESE);

    // ��ȡASCII������
    uint8_t ASCIIFontWidth = OLED_GetFontWidth(CurrentMenuPage->General_FontSize,OLED_ASCII);

    // ���ڴ�ӡ�˵��Ĺ���Ϊȫ�ֱ����ĵ�ǰ��
    OLED_UI_Point CursorPoint = OLED_UI_PageStartPoint.CurrentPoint;



    // �����ǰ�˵����������б�
    if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST)
    {
        //��ǰ׺��ȣ�Ĭ��Ϊ0
		uint8_t LinePerfixWidth = 0;
		//boolѡ���ȣ�Ĭ��Ϊ0
		uint8_t RadioCompensationWidth = 0;
        //�����Ҫ������ǰ׺
		if(CurrentMenuPage->List_IfDrawLinePerfix == true){
			//����ǰ׺�������ΪASCII�����ȼ�LINEPERFIX_DISTANCE������ַ����ľ��룩
			LinePerfixWidth = ASCIIFontWidth + LINEPERFIX_DISTANCE;
		}
        //��ӡ�˵���
		for(int16_t i = 0; i<=MenuNum;i++)
        {
            // ������С����ʼ�����Y������ߴ�����ʼ����Ľ���Y���꣬�����������Խ�ʡ��Դ
            if(CursorPoint.Y + CurrentMenuPage->General_FontSize < OLED_UI_MenuFrame.CurrentArea.Y || CursorPoint.Y > OLED_UI_MenuFrame.CurrentArea.Y + OLED_UI_MenuFrame.CurrentArea.Height){
				CursorPoint.Y += (CurrentMenuPage->General_FontSize + OLED_UI_LineStep.CurrentDistance);
				continue;
			}
            //��¼����ѭ�����ַ������
			int16_t StringLength = OLED_UI_CalcStringWidth(CurrentMenuPage->General_FontSize,CurrentMenuPage->General_MenuItems[i].General_item_text);
			//�����Ҫ��ӡ��ǰ׺
	        if(CurrentMenuPage->List_IfDrawLinePerfix == true){
	        	const char* LinePerfixSymb = "";
	        	//�����ǰ�Ĳ˵������Ӳ˵���û�лص���������ô��ӡ��Ӧ�ķ���
	        	if(CurrentMenuPage->General_MenuItems[i].General_SubMenuPage != NULL && CurrentMenuPage->General_MenuItems[i].General_callback == NULL && CurrentMenuPage->General_MenuItems[i].List_BoolRadioBox == NULL){
	        		LinePerfixSymb = SUBMENU_PREFIX;
	        	}else//����лص�����
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
            //�����Ҫ���Ƶ�ѡ��(��BoolRadioBox��Ϊ��)
            if(CurrentMenuPage->General_MenuItems[i].List_BoolRadioBox != NULL){
                RadioCompensationWidth = (ChineseFontWidth + 2);
                const char* RadioBoxSymb = "";
                if(*CurrentMenuPage->General_MenuItems[i].List_BoolRadioBox == true){
                    RadioBoxSymb = "��";
                }else{
                    RadioBoxSymb = "��";
                }
                OLED_PrintfArea(//�����Ƶ������ڴ�ӡ����
                               //������ʼx���������ǰ׺��ȣ����������Զ�������ӡ��ǰ׺�Ŀռ�
                               OLED_UI_MenuFrame.CurrentArea.X ,
                               OLED_UI_MenuFrame.CurrentArea.Y,
                               OLED_UI_MenuFrame.CurrentArea.Width - 6,
                               OLED_UI_MenuFrame.CurrentArea.Height,
                
                               //��ӡ���ֵ�����
                               CurrentMenuPage->General_StartPoint.X + OLED_UI_MenuFrame.CurrentArea.X + OLED_UI_MenuFrame.CurrentArea.Width - RadioCompensationWidth - 8 ,
                               CursorPoint.Y,
                               //��ӡ���ֵĴ�С
                               CurrentMenuPage->General_FontSize,
                               //��ӡ���ֵ�����
                               RadioBoxSymb);
            }else{
                RadioCompensationWidth = 0;
            }

            // ������ʾ�����X���꣺�����ʼ��ӡ��x����С�ڵ�ǰ�˵������x���꣬��ô��ҳ�濪ʼ��x���꿪ʼ��ӡ������ӿ�ʼ��ӡ��x���꿪ʼ��ӡ
            int16_t StringX = (CursorPoint.X + LinePerfixWidth < OLED_UI_MenuFrame.CurrentArea.X) ? OLED_UI_MenuFrame.CurrentArea.X : CursorPoint.X + LinePerfixWidth;

            int16_t StringY = OLED_UI_MenuFrame.CurrentArea.Y;

            // ��ʾ��������ƿ�ȣ������ʼ��ӡ��x����С�ڵ�ǰ�˵������x���꣬��ôʵ�ʵĿ��Ӧ�õ��ڡ���ʼ��ӡ��x����ӡ���ǰҳ����ʼx��������ϵ�ǰҳ���ȼ�ȥ5��ȥ2����ȥ��ǰҳ����ʼx���꡿
            // int16_t StringWidth = OLED_UI_MenuFrame.CurrentArea.Width + OLED_UI_MenuFrame.CurrentArea.X - StringX - RadioCompensationWidth - 8;
            int16_t StringWidth = (CursorPoint.X + LinePerfixWidth < OLED_UI_MenuFrame.CurrentArea.X) ? CursorPoint.X + LinePerfixWidth + OLED_UI_MenuFrame.CurrentArea.Width - 7 - RadioCompensationWidth - OLED_UI_MenuFrame.CurrentArea.X:OLED_UI_MenuFrame.CurrentArea.Width + OLED_UI_MenuFrame.CurrentArea.X - StringX - RadioCompensationWidth - 8;
            int16_t StringHeight = OLED_UI_MenuFrame.CurrentArea.Height;
            int16_t StringStartX = CursorPoint.X + LinePerfixWidth;
            int16_t StringStartY = CursorPoint.Y;
            int16_t CompareWidth = OLED_UI_MenuFrame.TargetArea.Width - 6 - LinePerfixWidth - CurrentMenuPage->General_StartPoint.X  -2 - RadioCompensationWidth;

            
            OLED_UI_PrintStringScroll(
                //���Ƶ�����
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
        //��¼����ѭ�����ַ������
		int16_t StringLength = OLED_UI_CalcStringWidth(CurrentMenuPage->General_FontSize,CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_item_text);

        //��ӡ�˵���
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
            //���Ƶ�����
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
    // ���Ʋ˵��߿�
    if(CurrentMenuPage->General_MenuFrameStyle == MENU_FRAME_RECTANGLE){
        OLED_DrawRectangle(OLED_UI_MenuFrame.CurrentArea.X-1, OLED_UI_MenuFrame.CurrentArea.Y-1,OLED_UI_MenuFrame.CurrentArea.Width+2, OLED_UI_MenuFrame.CurrentArea.Height+2, OLED_UNFILLED);
    }else if(CurrentMenuPage->General_MenuFrameStyle == MENU_FRAME_ROUNDRECTANGLE){
        OLED_DrawRoundedRectangle(OLED_UI_MenuFrame.CurrentArea.X-1, OLED_UI_MenuFrame.CurrentArea.Y-1, OLED_UI_MenuFrame.CurrentArea.Width+2, OLED_UI_MenuFrame.CurrentArea.Height+2,3, OLED_UNFILLED);
    }
    // ���ƹ��
    OLED_UI_ReverseCoordinate(OLED_UI_Cursor.CurrentArea.X, OLED_UI_Cursor.CurrentArea.Y, OLED_UI_Cursor.CurrentArea.Width, OLED_UI_Cursor.CurrentArea.Height, CurrentMenuPage->General_CursorStyle);
    //���ƹ���������������
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
 * @brief ����UIԪ��λ��
 * @param ��
 * @note ����UIԪ��λ�ã������˵�����ť���ı��򡢽������ȡ�
 * @return ��
 */
void OLED_UI_SetElementLocation(void)
{
    // ���ù������
    OLED_UI_SetTargetCursorArea();

    // ���ù������߶�
    OLED_UI_SetTargetScrollBarHeight();

    // �������嶥������[����ר��]
    OLED_UI_SetTiles_FontTopDistance();

    // ���ù�������ʼλ��
    OLED_UI_SetTargetScrollBarStartPoint();

    // ���ò˵�����
    OLED_UI_SetTargetMenuArea();

    // ���ò˵��ʼλ��
    OLED_UI_SetTargetMenuStartPoint();

    // ���ý���������
    OLED_UI_SetTargetProbWidth();

    // ���㶯�����м���������
    OLED_UI_ChangeDistance(&OLED_UI_LineStep);

    // ���㶯�������嶥������[����ר��]
    OLED_UI_ChangeDistance(&OLED_UI_Tiles_FontTopDistance);

    // ���㶯�����������߶�
    OLED_UI_ChangeDistance(&OLED_UI_ScrollBarHeight);

    // ���㶯������������ʼλ��
    OLED_UI_ChangePoint(&OLED_UI_ScrollBarStartPoint);

    // ���㶯�����˵��ʼλ��
    OLED_UI_ChangePoint(&OLED_UI_PageStartPoint);

    // ���㶯�����˵�����
    OLED_UI_ChangeArea(&OLED_UI_MenuFrame);

    // ���㶯�����ı�������
    OLED_UI_ChangeArea(&OLED_UI_Cursor);

    // ���㶯�����ı䴰������
    OLED_UI_ChangeArea(&OLED_UI_Window);

    // ���㶯�����ı����������
    OLED_UI_ChangeDistance(&OLED_UI_ProbWidth);
}




/**
 * @brief ��������
 * @param window ���ڽṹ��ָ�룬���ڴ洢�û������Ĵ�����Ϣ
 * @note ��������
 */
void OLED_UI_CreateWindow(MenuWindow* window){
    // �����ǰ����Ϊ��
    if(CurrentWindow == NULL){
        // ��ƫ��ֵ����
        window->_LineSlip = 0;
    }
    //�����ڱ�־λ����Ϊtrue����ʾ��ʼ����Ч��
	OLED_UI_WindowStatus.SustainFlag = true;
    //���ô���Ч��ʱ�������
	OLED_UI_WindowStatus.Counter = 0;

    // ��ʼ�����ڵ�Ŀ���С��λ��
    OLED_UI_Window.TargetArea.X = (OLED_WIDTH - window->General_Width)/2;
    OLED_UI_Window.TargetArea.Y = (OLED_HEIGHT - window->General_Height)/2;
    OLED_UI_Window.TargetArea.Width = window->General_Width;
    OLED_UI_Window.TargetArea.Height = window->General_Height;

    OLED_UI_Window.CurrentArea.X = (OLED_WIDTH - window->General_Width)/2;
    OLED_UI_Window.CurrentArea.Y = -window->General_Height - 1;
    OLED_UI_Window.CurrentArea.Width = window->General_Width;
    OLED_UI_Window.CurrentArea.Height = window->General_Height;
    //����ǰ����ָ��ָ��window
	CurrentWindow = window;
}



/**
 * @brief ���ƽ�����
 * @param X ��������ʼx����
 * @param Y ��������ʼy����
 * @param Width ���������
 * @param Height �������߶�
 * @param Percentage ���Ȱٷֱ�,0-1
 * @note ���ƽ�����
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
 * @brief ���ƴ���
 * @param ��
 * @note ���ƴ���
 */
void OLED_UI_DrawWindow(void)
{
    // �����ǰ����Ϊ�գ���ֱ�ӷ���
    if(CurrentWindow == NULL){
        // OLED_UI_WindowStatus.SustainFlag = false;
        return;
    }
    //������ڱ�־λû�б���λ���򲻽��д���Ч���������ڵ�Ŀ��λ����λ��Ļ�ⲿ
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
    
    // �����ʱʱ�䵽�ˣ���ô����־λ���㣬����������
    if(OLED_UI_WindowStatus.Counter > CurrentWindow->General_StayTime/OLED_UI_INTERRUPT_TIME)
    {
        OLED_UI_WindowStatus.SustainFlag = false;
        OLED_UI_WindowStatus.Counter = 0;
    }
    
    //������ڶ�����û���˳���Ļ������ƴ���
	// if(OLED_UI_Window.CurrentArea.Height != CurrentWindow->General_Height || OLED_UI_Window.CurrentArea.Width != CurrentWindow->General_Width || OLED_UI_Window.CurrentArea.Y != -CurrentWindow->General_Height - 10 || OLED_UI_Window.CurrentArea.X != (OLED_WIDTH - CurrentWindow->General_Width)/2){
    if(OLED_UI_Window.CurrentArea.Y + OLED_UI_Window.CurrentArea.Height > 0 || OLED_UI_WindowStatus.SustainFlag == true){
        // �ж�ָ�����������
        int8_t DataStyle = OLED_UI_GetWindowProbDataStyle(CurrentWindow);
        int16_t DataLength = 0;
        int16_t DataPerfixWidth = 0;
		//���Ʊ߿�
		if(CurrentWindow->General_FrameStyle != WINDOW_FRAME_ROUNDRECTANGLE){
			OLED_DrawRectangle(OLED_UI_Window.CurrentArea.X - 1,OLED_UI_Window.CurrentArea.Y - 1,OLED_UI_Window.CurrentArea.Width + 2,OLED_UI_Window.CurrentArea.Height + 2,OLED_UNFILLED);
			OLED_ClearArea(OLED_UI_Window.CurrentArea.X,OLED_UI_Window.CurrentArea.Y,OLED_UI_Window.CurrentArea.Width,OLED_UI_Window.CurrentArea.Height);
		}else{
			OLED_DrawRoundedRectangle(OLED_UI_Window.CurrentArea.X - 1,OLED_UI_Window.CurrentArea.Y - 1,OLED_UI_Window.CurrentArea.Width + 2,OLED_UI_Window.CurrentArea.Height + 2,2,OLED_UNFILLED);
			OLED_ClearArea(OLED_UI_Window.CurrentArea.X,OLED_UI_Window.CurrentArea.Y,OLED_UI_Window.CurrentArea.Width,OLED_UI_Window.CurrentArea.Height);
		}
        // ���ָ�벻Ϊ��
        if(DataStyle != WINDOW_DATA_STYLE_NONE ){
            DataPerfixWidth = OLED_UI_WINDOW_DATA_GAP;
            switch (DataStyle) {
                case WINDOW_DATA_STYLE_INT8:
                    DataLength = OLED_UI_CalcStringWidth(CurrentWindow->Prob_FontSize,"%d",*CurrentWindow->Prob_Data_Int_8_t);
                    OLED_PrintfArea(
                        //��������
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
                        //��������
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
                        //��������
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
                        //��������
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
                        //��������
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
            //��ʾ�����������
            OLED_UI_DrawProgressBar(
                OLED_UI_Window.CurrentArea.X +CurrentWindow->Prob_SideDistance,
                OLED_UI_Window.CurrentArea.Y + OLED_UI_Window.CurrentArea.Height  - CurrentWindow->Prob_LineHeight - CurrentWindow->Prob_BottomDistance,
                OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance,
                CurrentWindow->Prob_LineHeight,
                OLED_UI_ProbWidth.CurrentDistance/(OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4)
            
            );
			

        }


        //��ӡ���ڱ���
        if(CurrentWindow->Title_text != NULL){
            OLED_UI_PrintStringScroll(
                // ��������
                OLED_UI_Window.CurrentArea.X + CurrentWindow->Title_FontSideDistance,
                OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                OLED_UI_Window.CurrentArea.Width - CurrentWindow->Title_FontSideDistance*2 - DataLength - DataPerfixWidth,
                CurrentWindow->Title_FontSize,
                // ��ӡ��ʼλ��
                OLED_UI_Window.CurrentArea.X + CurrentWindow->Title_FontSideDistance,
                OLED_UI_Window.CurrentArea.Y + CurrentWindow->Title_FontTopDistance,
                // ���ƿ��
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
 * @brief ��ʼ�����
 * @param ��
 * @note ����Ƿ�����ȷ�����˲˵��ṹ��ָ�롣
 * @return ��
 */
void OLED_UI_InitCheck(void)
{
    if(CurrentMenuPage == NULL){
        OLED_Init();
        while(1){
            //ˢ����Ļ
            OLED_Printf(0,0,OLED_FONT_8,"OLED UI\nError:\nWithout\nInit!");
            OLED_Update();
        }
    }

}
/**
 * @brief ��ӡUIԪ��
 * @param ��
 * @note ��ӡUIԪ�أ������˵�����ť���ı��򡢽������ȡ�
 * @return ��
 */
void OLED_UI_PrintElement(void)
{
    //����Դ�
    OLED_Clear();

    // ��ʾ֡��
    OLED_UI_ShowFPS();

    // ��ʾ�˵���
    OLED_UI_PrintMenuItems();

    // ��ʾ����
    OLED_UI_DrawWindow();

    //ˢ��
    OLED_Update();

    //�����Ļ�����仯����¼��Ч֡
    if(OLED_IfChangedScreen()){
        OLED_UI_FPS.Counter++;
    }
}
        /*===============================OLED-UI���ߺ���==============================*/
    /*===================================OLED-UI���ߺ���==================================*/
/*=======================================OLED-UI���ߺ���======================================*/


/*=======================================OLED-UIʵ�ֺ���======================================*/
    /*===================================OLED-UIʵ�ֺ���==================================*/
        /*===============================OLED-UIʵ�ֺ���==============================*/




/**
 * @brief ��ѭ��
 * @param ��
 * @note ��ѭ����������ʾUI��
 * @return ��
 */
void OLED_UI_MainLoop(void)
{
    //��ʼ�����
    OLED_UI_InitCheck();
    //��������
    OLED_UI_HandleInput_MainLoop();

    //����UIԪ��λ��
    OLED_UI_SetElementLocation();

    //��ӡUIԪ�أ�����ˢ����Ļ��
    OLED_UI_PrintElement();

    //����������ʾ


    
    
}



/**
 * @brief ��ʼ��OLED UI
 * @param ��
 * @note ��ʼ��OLED UI��������ʼ��OLED����ʼ����ʱ������ʼ������������ʼ��������
 * @return ��
 */
void OLED_UI_Init(MenuPage* Page)
{
    OLED_Init();
    OLED_UI_TimerInit();
    OLED_UI_EncoderInit();
    OLED_UI_KeyInit();

    //���õ�ǰҳ��Ľṹ��ָ��
	CurrentMenuPage = Page;	//���õ�ǰҳ��Ľṹ��ָ��

    //��ʼ����ǰҳ��
    OLED_UI_CurrentMenuPageInit();
}

/**
 * @brief ����/�°����ĳ�ʱ�䰴���¼�
 * @param ��
 * @note �ú��������ж���/�°����Ƿ�ʱ�䰴�¡�
 * @return ��
 */
void OLED_UI_UpAndDownLongPressEvent(void)
{
    static int16_t Key_UpPressCount = 0;
    static int16_t Key_DownPressCount = 0;
    // ������°���ͬʱ�����£��򲻴���
    if(OLED_UI_Up._CurrentStatus == 0 && OLED_UI_Down._CurrentStatus == 0){
        return;
    }
    // �����ǰ�ϼ����ڱ�����
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
    // ��ֹ�������
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
 * @brief ��Ҫ�ڶ�ʱ���ж��е��øú���
 * @param ��
 * @note �ú������ڽ��ܱ������밴�������롣
 * @return ��
 */
void OLED_UI_InterruptHandler(void)
{
    
    //֡��������
    OLED_UI_CountFPS_Interrupt();
    //��������
    OLED_UI_HandleInput_Interrupt();
    // ���°����Ķ�ʱ�䰴�ºͳ�ʱ�䰴���ж�
    OLED_UI_UpAndDownLongPressEvent();

    // �����ǰ��Ϊ��
    //�����ǰ�������д��ڣ���ô����
	if(OLED_UI_WindowStatus.SustainFlag == true){
		OLED_UI_WindowStatus.Counter++;
	}

}





        /*===============================OLED-UIʵ�ֺ���==============================*/
    /*===================================OLED-UIʵ�ֺ���==================================*/
/*=======================================OLED-UIʵ�ֺ���======================================*/






