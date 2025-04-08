#ifndef __OLED_UI_H
#define __OLED_UI_H
#include "OLED.h"
#include "OLED_UI_Driver.h"
#include "OLED_UI_Config.h"

/*******************�йذ���״̬�ĺ궨��***************************** */
#define OLED_UI_KEY_NONE                            (0)            //�ް�������
#define OLED_UI_KEY_CLICK                           (1)            //��������
#define OLED_UI_KEY_DOUBLE_CLICK                    (2)            //˫������
#define OLED_UI_KEY_LONG_PRESS                      (3)            //��������


/*********************���ڲ˵����͵ĺ�************************/

#define MENU_TYPE_LIST				                (0)			//�б�����
#define MENU_TYPE_TILES				                (1)			//��������

/***********�йع�����͵ĺ�***********/
#define CURSOR_REVERSE_RECTANGLE 				    (0)			//���η�ɫ
#define CURSOR_REVERSE_ROUNDRECTANGLE 		        (1)			//Բ�Ǿ��η�ɫ
#define CURSOR_HOLLOW_RECTANGLE 					(2)			//���ľ���
#define CURSOR_HOLLOW_ROUNDRECTANGLE 			    (3)			//����Բ�Ǿ���
#define CURSOR_REVERSE_BLOCK						(4)			//�»���
#define CURSOR_ARROW								(5)			//��ͷ
#define CURSOR_NOT_SHOW							    (6)			//����ʾ���

/***********�йز˵��߿����͵ĺ�***********/
#define MENU_FRAME_RECTANGLE						(0)			//���α߿�
#define MENU_FRAME_ROUNDRECTANGLE					(1)			//Բ�Ǿ��α߿�
#define MENU_FRAME_NONE							    (2)			//�ޱ߿�

/***************�йش������͵ĺ�***********/
#define WINDOW_FRAME_RECTANGLE					    (0)			//���α߿�
#define WINDOW_FRAME_ROUNDRECTANGLE				    (1)			//Բ�Ǿ��α߿�

/*********************���ڴ����������͵ĺ�* ***************/
#define WINDOW_DATA_STYLE_NONE	                    (0)			//��
#define WINDOW_DATA_STYLE_FLOAT		                (1)			//����������
#define WINDOW_DATA_STYLE_INT8		                (2)			//����8λ����
#define WINDOW_DATA_STYLE_INT16		                (3)			//����16λ����
#define WINDOW_DATA_STYLE_INT32		                (4)			//����32λ����
#define WINDOW_DATA_STYLE_INT64		                (5)			//����64λ����

/*********************���ڶ������͵ĺ�***********************/
#define UNLINEAR 			                        (0)
#define PID_CURVE     	 	                        (1)

/************���ڲ˵���ǰ׺���ŵĺ�**********************/

#define FUNCTION_PREFIX			                    "~"			//����ǰ׺����
#define SUBMENU_PREFIX				                ">"			//�˵�ǰ׺����
#define RADIO_PREFIX				                "*"			//��ѡ��ǰ׺����
#define NONE_PREFIX					                "-"			//�޲���ǰ׺����

#define LINEPERFIX_DISTANCE			                (2)			//��ǰ׺����������ֵľ���

/************���ڳ�������״̬�ĺ�**********************/
#define KEY_UP_LONGPRESS_NONE                       (0)            //��û�а���
#define KEY_UP_LONGPRESS_SHORT                      (1)            //�϶�ʱ�䰴��
#define KEY_UP_LONGPRESS_LONG                       (2)            //�ϳ�ʱ�䰴��
#define KEY_DOWN_LONGPRESS_NONE                     (3)            //��û�а���
#define KEY_DOWN_LONGPRESS_SHORT                    (4)            //�¶�ʱ�䰴��
#define KEY_DOWN_LONGPRESS_LONG                     (5)            //�³�ʱ�䰴��

/************���ڶ������͵ĺ�**********************/
#define ANIM_MENU                                   (0)
#define ANIM_WINDOW                                 (1)

/*��ʾ�˵����͵����ݽṹ*/
typedef int8_t OLED_UI_MenuStyle;

/*��ʾ�˵�ID�����ݽṹ*/
typedef int16_t OLED_UI_MenuID;

/*��ʾ�˵��ƶ������ٶȵ����ݽṹ*/
typedef float OLED_UI_MenuMovingSpeed;

/*��ʾ�˵�ID�����ݽṹ*/
typedef int8_t OLED_UI_Font;

/*��ʾ���������־λ�����ݽṹD*/
typedef int8_t OLED_UI_MutexFlag;

/*�������ṹ�� */
typedef struct OLED_UI_Counter{
    int16_t Counter;
    int16_t Time;
    int16_t Value;
} OLED_UI_Counter;


/**OLED_UI��������ʵ�ִ���ͣ���Ľṹ��
 * ֵ��ע����ǣ���SustainFlagΪfalse��ʱ�򣬵�ǰ����ָ�벢��һ����NULL�������п��������˳���
 */
typedef struct OLED_UI_WindowSustainCounter{
	int16_t Counter;		//������
	bool SustainFlag;	//��־λ�����ڱ�ʾ�Ƿ���ͣ��״̬
}OLED_UI_WindowSustainCounter;

/*��ʱ�䰴ס���ŵĽṹ��*/
typedef struct OLED_UI_LongPress{
    int16_t DeltaData;      // ��������
    int16_t Counter;        // ������
}OLED_UI_LongPress;


// /*OLED_UI�����йذ������ƵĽṹ��*/

typedef struct  OLED_UI_KeyControl{
    int8_t Status;	    //ȷ�ϼ�״̬
    int16_t SetClickTime;	//���õ�����ʱʱ��
    int16_t SetLongPressTime;	//���ó���ʱ��
    
    int8_t _CurrentStatus;     //��ǰ����״̬
    int8_t _LastStatus;        //��һ�ΰ���״̬
    int8_t _ClickTimerFlag;	//ȷ�ϼ���ʱ����־λ
    int8_t _ClickFrequency;	//ȷ�ϼ��������
    int16_t _ClickTime;	//ȷ�ϼ�����ʱ��
} OLED_UI_KeyControl;




/*OLED_UI�����е�����Ľṹ��*/
typedef struct OLED_UI_Point{
	float X; 
	float Y;
} OLED_UI_Point;

/*OLED_UI�����й�λ�����С�Ľṹ��*/
typedef struct  OLED_UI_Area{
    float X;
    float Y;
    float Width;
    float Height;
} OLED_UI_Area;

/*OLED_UI���д�Ź����Ϣ�Ľṹ�塾�����������ƶ�Ч���� */
typedef struct OLED_UI_AreaAnimation{
	OLED_UI_Area CurrentArea;	//��ǰ�������
	OLED_UI_Area TargetArea;		//Ŀ��������
	OLED_UI_Area _Error;			//���ֵ
	OLED_UI_Area _LastError;		//��һ�ε����ֵ
	OLED_UI_Area _Integral;		//����ֵ
	OLED_UI_Area _Derivative;		//΢��ֵ

}OLED_UI_AreaAnimation;

typedef struct OLED_UI_PointAnimation{
	OLED_UI_Point CurrentPoint;	//��ǰ������
	OLED_UI_Point TargetPoint;		//Ŀ�������
	OLED_UI_Point _Error;			//���ֵ
	OLED_UI_Point _LastError;		//��һ�ε����ֵ
	OLED_UI_Point _Integral;		//����ֵ
	OLED_UI_Point _Derivative;		//΢��ֵ

}OLED_UI_PointAnimation;


typedef struct OLED_UI_DistanceAnimation{
	float CurrentDistance;		//��ǰֵ
	float TargetDistance;		//Ŀ��ֵ
	float _Error;					//���ֵ
	float _LastError;				//��һ�ε����ֵ
	float _Integral;				//����ֵ
	float _Derivative;				//΢��ֵ

}OLED_UI_DistanceAnimation;



typedef struct MenuPage {
    OLED_UI_MenuStyle       General_MenuType;                                    //�˵�����
    OLED_UI_MenuMovingSpeed General_MovingSpeed;                                //�˵��ƶ��ٶ�
    uint8_t                 General_CursorStyle;                                //�����ʽ
    uint8_t                 General_MoveStyle;                                    //�ƶ���ʽ
    OLED_UI_Font            General_FontSize;                                    //�����С
    struct MenuPage*        General_ParentMenuPage;                            //���˵�ҳ��
    struct MenuItem*        General_MenuItems;                                  //�˵���
    int16_t                 General_LineSpace;                                    //�м����м��
    OLED_UI_Area            General_MenuArea;                                //�б�˵�����
    uint8_t                 General_MenuFrameStyle;                           //���������������
    int16_t                 General_InitMenuID;                             //��ʼ�˵���ID
    int16_t                 General_InitSlot;                              //��ʼ��λ
    OLED_UI_Point           General_StartPoint;                            //�б�����ʼ���꡾����ڲ˵�������ʼλ�õ����꡿
    void                    (*General_ShowAuxiliaryFunction)(void);             //������ʾ����

    bool                    List_IfDrawLinePerfix;                          //�Ƿ�����б���ǰ׺��

    int16_t Tiles_TileWidth;                          //�������
    int16_t Tiles_TileHeight;                         //�����߶�
    int16_t Tiles_FontTopDistance;                    //���־����ҳ���붥������
    int16_t Tiles_ScrollBarTopDistance;               //����������ҳ���붥������


    OLED_UI_MenuID _ActiveMenuID;                    //��ǰ����˵�ID
    OLED_UI_MenuID _Slot;                            //��λ
    OLED_UI_Point _StartPoint;                      //��ʼ����
	bool _IfInit;                                     //�Ƿ��ʼ��
} MenuPage;

typedef struct MenuItem {
    const char* General_item_text;                        //�˵����ı�
    void (*General_callback)(void);                  //�˵���ص�����
    MenuPage* General_SubMenuPage;                  //�Ӳ˵�ҳ��

    bool* List_BoolRadioBox;                         //������ѡ��
    const uint8_t* Tiles_Icon;                      //����ͼ��

    float _LineSlip;                                 //����λ
    // float _LineSlip2;                                 //����λ
} MenuItem;

typedef struct MenuWindow{
    int16_t General_Width;                            //[ͨ������]���ڿ��
    int16_t General_Height;                           //[ͨ������]���ڸ߶�
    float   General_StayTime;                             //[ͨ������]����ͣ��ʱ��
    uint8_t General_FrameStyle;                             //[ͨ������]��������

    const char* Title_text;                                //[��������]�����ı�
    uint8_t Title_FontSize;                            //[��������]�����С
    int16_t Title_FontSideDistance;						//[��������]����������ľ���
	int16_t Title_FontTopDistance;						//[��������]������붥���ľ���

    uint8_t Prob_FontSize;                                //[����������]�����С
    uint8_t Prob_FloatPointLength;                        //[����������]���������ݵ�С���㳤��
    float* Prob_Data_Float;                                //[������ʾ����]���������ݡ���ѡһ��
    int8_t* Prob_Data_Int_8_t;                                //[������ʾ����]8λ�������ݡ���ѡһ��
    int16_t* Prob_Data_Int_16_t;                                //[������ʾ����]16λ�������ݡ���ѡһ��
    int32_t* Prob_Data_Int_32_t;                                //[������ʾ����]32λ�������ݡ���ѡһ��
    int64_t* Prob_Data_Int_64_t;                                //[������ʾ����]64λ�������ݡ���ѡһ��
    float Prob_DataStep;									//[����������]�������ݲ���
	float Prob_MinData;									//[����������]�������ݵ���Сֵ
	float Prob_MaxData;									//[����������]�������ݵ����ֵ
    int16_t Prob_SideDistance;								//[����������]��������Ե���봰�ڱ�Ե�ľ���
	int16_t Prob_LineHeight;								//[����������]�����������߶�
    int16_t Prob_BottomDistance;							//[����������]�������ײ����봰�ڵײ��ľ���

    float _LineSlip;                                        //[˽������]���ڹ�����ʾ���ı�

} MenuWindow;


#ifdef __cplusplus
extern "C" {
#endif

// ���غ���
void OLED_UI_BackTask(void);

// OLED_UI��ѭ��
void OLED_UI_MainLoop(void);

// ��������
void OLED_UI_CreateWindow(MenuWindow* window);

// ��ʼ��OLED_UI
void OLED_UI_Init(MenuPage* Page);

// �жϴ�����
void OLED_UI_InterruptHandler(void);



#ifdef __cplusplus
}  // extern "C"
#endif

#endif
