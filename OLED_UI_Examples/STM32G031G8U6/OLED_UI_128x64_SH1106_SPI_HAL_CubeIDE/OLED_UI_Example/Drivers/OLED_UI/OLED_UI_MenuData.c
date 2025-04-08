#include "OLED_UI_MenuData.h"

#define SPEED 3.0f
#define ANIMATION  UNLINEAR
extern MenuPage SubMenuPage1, SubMenuPage2, MainMenuPage;
bool test1;

// void auxfunc(void){
//     OLED_Printf(0,100,OLED_FONT_8,"Main slot%d mid:%d\nsub1 slot%d mid:%d\nsub2 slot%d mid:%d",MainMenuPage._Slot,MainMenuPage._ActiveMenuID,SubMenuPage1._Slot,SubMenuPage1._ActiveMenuID,SubMenuPage2._Slot,SubMenuPage2._ActiveMenuID);
// }

void callback1(){
    OLED_Clear();
    // OLED_ShowImage(2,2,124,123,Image_alipay_QR_Code);
    OLED_Update();
    OLED_DelayMs(1000);
}

MenuPage SubMenuPage1 = {

    // 通用属性
    .General_MenuArea = {40, 40, 50, 34},
    .General_MenuFrameStyle = MENU_FRAME_ROUNDRECTANGLE,
    .General_InitMenuID = 1,
    .General_InitSlot = 0,
    .General_MenuType = MENU_TYPE_LIST,
    .General_CursorStyle = CURSOR_REVERSE_ROUNDRECTANGLE,
    .General_FontSize = OLED_FONT_8,
    .General_ParentMenuPage = &MainMenuPage,
    .General_LineSpace = 4,
    .General_MoveStyle = ANIMATION,
    .General_MovingSpeed = SPEED,
    .General_ShowAuxiliaryFunction = NULL,
    // 特殊属性
    .General_StartPoint = {3,3},
    .List_IfDrawLinePerfix = true,

    // 菜单项内容
    .General_MenuItems = (MenuItem[]){
        {.General_item_text = "back", .General_callback = OLED_UI_BackTask, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "SubMenuPage1", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "1231231234567890", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123abcdefghijklmnopqrstuvwxyz", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123,.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},

        {.General_item_text = NULL}, // 最后一项表示分割线
    },

};

MenuPage SubMenuPage2 = {

    // 通用属性
    .General_MenuArea = {10, 10, 100, 98},
    .General_MenuFrameStyle = MENU_FRAME_ROUNDRECTANGLE,
    .General_InitMenuID = 1,
    .General_InitSlot = 0,
    .General_MenuType = MENU_TYPE_LIST,
    .General_CursorStyle = CURSOR_REVERSE_ROUNDRECTANGLE,
    .General_FontSize = OLED_FONT_8,
    .General_ParentMenuPage = &MainMenuPage,
    .General_LineSpace = 4,
    .General_MoveStyle = ANIMATION,
    .General_MovingSpeed = SPEED,
    .General_ShowAuxiliaryFunction = NULL,
    // 特殊属性
    .General_StartPoint = {3,3},
    .List_IfDrawLinePerfix = true,

    // 菜单项内容
    .General_MenuItems = (MenuItem[]){
        {.General_item_text = "back", .General_callback = OLED_UI_BackTask, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "SubMenuPage2", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "1231231234567890", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123abcdefghijklmnopqrstuvwxyz", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123,.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},

        {.General_item_text = NULL}, // 最后一项表示分割线
    },

};
MenuPage SubMenuPage3 = {

    // 通用属性
    .General_MenuArea = {10, 10, 100, 98},
    .General_MenuFrameStyle = MENU_FRAME_ROUNDRECTANGLE,
    .General_InitMenuID = 1,
    .General_InitSlot = 0,
    .General_MenuType = MENU_TYPE_LIST,
    .General_CursorStyle = CURSOR_REVERSE_ROUNDRECTANGLE,
    .General_FontSize = OLED_FONT_8,
    .General_ParentMenuPage = &MainMenuPage,
    .General_LineSpace = 4,
    .General_MoveStyle = ANIMATION,
    .General_MovingSpeed = SPEED,
    .General_ShowAuxiliaryFunction = NULL,
    // 特殊属性
    .General_StartPoint = {3,3},
    .List_IfDrawLinePerfix = true,

    // 菜单项内容
    .General_MenuItems = (MenuItem[]){
        {.General_item_text = "back", .General_callback = OLED_UI_BackTask, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "SubMenuPage2", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "1231231234567890", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123abcdefghijklmnopqrstuvwxyz", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123,.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},

        {.General_item_text = NULL}, // 最后一项表示分割线
    },

};
MenuPage SubMenuPage4 = {

    // 通用属性
    .General_MenuArea = {10, 10, 100, 98},
    .General_MenuFrameStyle = MENU_FRAME_ROUNDRECTANGLE,
    .General_InitMenuID = 1,
    .General_InitSlot = 0,
    .General_MenuType = MENU_TYPE_LIST,
    .General_CursorStyle = CURSOR_REVERSE_ROUNDRECTANGLE,
    .General_FontSize = OLED_FONT_8,
    .General_ParentMenuPage = &SubMenuPage5,
    .General_LineSpace = 4,
    .General_MoveStyle = ANIMATION,
    .General_MovingSpeed = SPEED,
    .General_ShowAuxiliaryFunction = NULL,
    // 特殊属性
    .General_StartPoint = {3,3},
    .List_IfDrawLinePerfix = true,

    // 菜单项内容
    .General_MenuItems = (MenuItem[]){
        {.General_item_text = "back", .General_callback = OLED_UI_BackTask, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "SubMenuPage2", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "1231231234567890", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123abcdefghijklmnopqrstuvwxyz", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = "123123,.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},

        {.General_item_text = NULL}, // 最后一项表示分割线
    },
};
MenuPage SubMenuPage5 = {

    // 通用属性
    .General_MenuArea = {1, 1, 126, 126},
    .General_MenuFrameStyle = MENU_FRAME_NONE,
    .General_InitMenuID = 0,
    .General_InitSlot = 0,
    .General_MenuType = MENU_TYPE_TILES,
    .General_CursorStyle = CURSOR_ARROW,
    .General_FontSize = OLED_FONT_8,
    .General_ParentMenuPage = &MainMenuPage,
    .General_LineSpace = 10,
    .General_MoveStyle = ANIMATION,
    .General_MovingSpeed = SPEED,
    .General_ShowAuxiliaryFunction = NULL,
    .General_StartPoint = {128/2-64/2,10},
    // 特殊属性
    .Tiles_TileHeight = 64,
    .Tiles_TileWidth = 64,
    .Tiles_FontTopDistance = 100,
    .Tiles_ScrollBarTopDistance = 87,

    // 菜单项内容
    .General_MenuItems = (MenuItem[]){
        {.General_item_text = "back", .General_callback = OLED_UI_BackTask, .General_SubMenuPage = NULL},
        {.General_item_text = "SubMenuPage3", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage4, .Tiles_Icon = Image_calc_64},
        {.General_item_text = "123123English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .Tiles_Icon = Image_wechat_64},
        {.General_item_text = "1231231234567890", .General_callback = NULL, .General_SubMenuPage = NULL, .Tiles_Icon = Image_settings_64},
        {.General_item_text = "123123ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = NULL, .General_SubMenuPage = NULL, .Tiles_Icon = Image_alipay_64},
        {.General_item_text = "123123abcdefghijklmnopqrstuvwxyz", .General_callback = NULL, .General_SubMenuPage = NULL, .Tiles_Icon = Image_night_64},
        {.General_item_text = "123123,.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .Tiles_Icon =  Image_more_64},

        {.General_item_text = NULL}, // 最后一项表示分割线
    },

};
int8_t testnum = 20;
int16_t testnum2 = 20;
int32_t testnum3 = 20;
int64_t testnum4 = 20;
uint64_t testnum5 = 20;
float testnum6 = 20.00;
MenuWindow windowTest1 = {
    .General_FrameStyle = WINDOW_FRAME_ROUNDRECTANGLE,
    .General_Height = 40,
    .General_Width = 80,
    .General_StayTime = 3000,

    .Title_text = "Test1",
    .Title_FontSize = OLED_FONT_8,
    .Title_FontSideDistance = 3,
    .Title_FontTopDistance = 3,

    .Prob_FontSize = OLED_FONT_8,
    // .Prob_Data_Int_8_t = &testnum,
    .Prob_Data_Int_64_t = &testnum4,
    .Prob_FloatPointLength = 2,
    .Prob_DataStep = 5,
    .Prob_BottomDistance = 3,
    .Prob_SideDistance = 3,
    .Prob_MaxData = 100,
    .Prob_MinData = -100,
    .Prob_LineHeight = 10,
};

void OLED_WindowTest1(void){
    OLED_UI_CreateWindow(&windowTest1);
}
extern MenuWindow *CurrentWindow ;
extern OLED_UI_WindowSustainCounter OLED_UI_WindowStatus;
extern OLED_UI_AreaAnimation OLED_UI_Window;
void OLED_MAIN_AUX(void){
    if(CurrentWindow == NULL){
        OLED_Printf(0,0,OLED_FONT_8,"NULL");
    }else{
        OLED_Printf(0,0,OLED_FONT_8,"NOT NULL");
    }
    // OLED_Printf(0,10,OLED_FONT_8,"%.2f,%d\n %.1f,%.1f,%.1f,%.1f\n%.1f,%.1f,%.1f,%.1f",*windowTest1.Prob_Data_Float,OLED_UI_WindowStatus.SustainFlag,OLED_UI_Window.CurrentArea.X,OLED_UI_Window.CurrentArea.Y,OLED_UI_Window.CurrentArea.Width,OLED_UI_Window.CurrentArea.Height,OLED_UI_Window.TargetArea.X,OLED_UI_Window.TargetArea.Y,OLED_UI_Window.TargetArea.Width,OLED_UI_Window.TargetArea.Height);
}
MenuPage MainMenuPage = {

    // 通用属性
    .General_MenuArea = {10, 70, 100, 50},
    .General_MenuFrameStyle = MENU_FRAME_ROUNDRECTANGLE,
    .General_InitMenuID = 0,
    .General_InitSlot = 0,
    .General_MenuType = MENU_TYPE_LIST,
    .General_CursorStyle = CURSOR_REVERSE_ROUNDRECTANGLE,
    .General_FontSize = OLED_FONT_8,
    .General_ParentMenuPage = NULL,
    .General_LineSpace = 4,
    .General_MoveStyle = ANIMATION,
    .General_MovingSpeed = SPEED,
    .General_ShowAuxiliaryFunction = OLED_MAIN_AUX,
    // 特殊属性
    .General_StartPoint = {3,3},
    .List_IfDrawLinePerfix = true,

    // 菜单项内容
    .General_MenuItems = (MenuItem[]){
        {.General_item_text = "", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "中文文本", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage5, .List_BoolRadioBox = NULL},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = callback1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},

        {.General_item_text = "windowTest1", .General_callback = OLED_WindowTest1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = ",.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},  
        {.General_item_text = "SUb1", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage1, .List_BoolRadioBox = NULL},
        {.General_item_text = "sub2", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "中文文本", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage5, .List_BoolRadioBox = NULL},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = callback1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},

        {.General_item_text = "windowTest1", .General_callback = OLED_WindowTest1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = ",.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},  
        {.General_item_text = "SUb1", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage1, .List_BoolRadioBox = NULL},
        {.General_item_text = "sub2", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "中文文本", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage5, .List_BoolRadioBox = NULL},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = callback1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},

        {.General_item_text = "windowTest1", .General_callback = OLED_WindowTest1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = ",.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},  
        {.General_item_text = "SUb1", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage1, .List_BoolRadioBox = NULL},
        {.General_item_text = "sub2", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "中文文本", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage5, .List_BoolRadioBox = NULL},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = callback1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},

        {.General_item_text = "windowTest1", .General_callback = OLED_WindowTest1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = ",.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},  
        {.General_item_text = "SUb1", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage1, .List_BoolRadioBox = NULL},
        {.General_item_text = "sub2", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        {.General_item_text = "中文文本", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage5, .List_BoolRadioBox = NULL},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", .General_callback = callback1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},

        {.General_item_text = "windowTest1", .General_callback = OLED_WindowTest1, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},
        {.General_item_text = ",.[]!@#$+-/^&*()", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = &test1},
        {.General_item_text = "English Text", .General_callback = NULL, .General_SubMenuPage = NULL, .List_BoolRadioBox = NULL},  
        {.General_item_text = "SUb1", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage1, .List_BoolRadioBox = NULL},
        {.General_item_text = "sub2", .General_callback = NULL, .General_SubMenuPage = &SubMenuPage2, .List_BoolRadioBox = NULL},
        
        {.General_item_text = NULL}, // 最后一项表示分割线
    },

};


