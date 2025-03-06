# **OLED UI** 一个丝滑的ui框架
> ***既然来了，点个star吧！万分感谢！***
---
> 2025.3.3留：由于作者在esp32上测试时发现程序框架有stm32上没有出现的bug，大概率是指针使用不当造成的，所以作者打算重构OLED UI。在此前，OLED UI已经重构了2次，耗费了作者许多的精力，但好在取得了一些小小的成果。此次重构将解决现有阶段的bug，优化程序设计，将OLED UI在 [OLED-Basic-Lib](https://github.com/bdth-7777777/OLED-Basic-Lib)的基础上开发，优化了底层代码，支持全系屏幕自适应刷新，并增加一些功能，敬请期待！作者正在熬夜撸码中···
---
**写在最前面**：
* 由于并本项目并没有使用U8G2库，所以占有的程序空间并没有特别多。
* 由c编写，所以可以部署在仅支持c的平台上。
* 移植不困难，基本只需要有控制GPIO的能力即可移植。
* 字符的长度不再受到屏幕宽度的限制，字符长度超过规定的显示区域的时候会自动滚动显示。
* 尽管使用了软件iic或是spi进行通信，但是您无需担心屏幕的刷新率不够支撑丝滑的动画效果。例如在主频为72MHz的STM32F103C8t6单片机上使用软件SPI，128X128的OLED屏幕仍然可以在多数时间当中维持在80Hz以上的刷新率，这显然足够了。
---
> 目前项目仍然在整理优化当中
---
## 目录
- [总览与介绍](#1-总览与介绍)
- [移植部署方式](#2-移植部署方式)
- [创建自己的第一个列表类菜单](##3-创建自己的第一个列表类菜单)
***
## 1. 总览与介绍
* **关于编程语言：** 此项目使用了c语言，在C99标准下编写。经过验证，此项目也可以在c++的arduino框架下编译运行。
* **关于硬件组成：** 
    1. **显示部分：** OLED显示屏，以下是(将要)支持的显示屏型号：

        | 屏幕主控芯片  | 分辨率 | 通讯方式 | 是否已经推出例程 |
        |-------|-----|------|------|
        | SSD1306  | 128x32  | IIC  |   ✘  |
        | SSD1305  | 128x32  | SPI  |   ✘  |
        | SSD1306  | 128x64  | IIC  |   ✔  |
        | SH1106   | 128x64  | SPI  |   ✘  |
        | SH1106   | 128x64  | SPI  |   ✘  |
        | SH1107   | 128x128 | IIC  |   ✔  |
        | SH1107   | 128x128 | IIC  |   ✘  |
         

    2. **按键部分：** 目前最基本的按键支持是四个按键，分别是 **确认键** **返回键** **上键** **下键** ，这四个按键可以实现所有最基本的功能。
    3. **编码器部分：** 编码器的加入可以使操作更加便捷优雅，但是不使用编码器也可以实现所有功能。所以移植的时候可以先不使用编码器，待按键功能测试完毕后再考虑移植编码器。
    4. **定时器部分：** 定时器提供20ms的中断，应该是属于状态机(我看别人会把这种在定时器中断当中读取按键的方式叫做状态机)，可以实现一些ui的精确停留延时的效果，并可以计算当前刷屏的帧数。【当然，没有定时器中断也可以运行OLED UI，只是这样不能计算帧数和进行精确的停留延时。】
* **关于例程提供：**
    我会尽可能多地提供一些例程，下面是一些已经完成或是将会完成的例程：
    
    |  芯片生产商  | MCU  | 是否已经完成例程 | IDE/开发环境 |
    |-------|-------|-----| ----- |
    |  STM  | STM32F103C8t6  | ✔  | Keil5|
    |  STC  | STC8051U  | ✘  |  Keil5|
    |  乐鑫  | ESP32 WOORM32  | ✔  | VsCode PlatformIO Arduino|
    |  英飞凌  | TC264   | ✘  | ADS|
    |  TI    | MSPm0g3507g   | ✘  | CCS|
 

## 2. 移植部署方式
> **如果您要移植此项目到别的平台，请耐心阅读以下内容。**
---
* ### 第一步 点亮您的OLED屏幕
    * **【复制OLED基础库并进行简单移植】** 首先，您需要将OLED屏幕的基础库复制到您的工程目录下， **也就是说，您需要保证您的工程当中包含了表格当中的这些文件。** 我将他们列在了下方，您需要根据您的OLED屏幕型号选择适合文件。
        | 文件名  | 描述 | 移植的时候是否需要修改 |
        |-------|-----|------|
        | `OLED_Driver.c`  | 底层驱动文件         |   需要    |
        | `OLED_Driver.h`  | 底层驱动文件的头文件  |   需要    |
        | `OLED_Fonts.c`   | 字库文件          |   不需要  |
        | `OLED_Fonts.h`   | 字库文件的头文件  |   不需要  |
        | `OLED.c`         | 应用层文件        |   不需要  |
        | `OLED.h`         | 应用层文件的头文件 |   不需要  |

        ----
        > 此OLED的底层驱动、字库和应用层的结构取自 [ 【 江协科技的OLED教程 】 ](https://www.bilibili.com/video/BV1EN41177Pc/?spm_id_from=333.1387.homepage.video_card.click&vd_source=1183a399479e248598095c5e770be232),这个教程非常不错，我在他的文件基础上修改增添了不少东西，在此表衷心感谢。
        ----
        **好了，您的工程文件夹中应该包含了这些文件了。** 现在，请打开 `OLED_Driver.c` 和 `OLED_Driver.h` 文件，找到 `OLED_Init(void)` 函数，将GPIO的初始化部分修改为您的实际GPIO初始化代码。此外，您还需要修改将GPIO置高电平或置低电平的函数，将他们修改成适合您的GPIO驱动的函数。到现在为止，操作都很简单，就是初始化GPIO，然后修改GPIO驱动函数，即使是初学者应该也能轻松搞定，毕竟步操作的难度堪比点灯。

        接着，在您的主函数所在的文件内包含 `"OLED.h"` 这个头文件，然后初始化OLED屏幕，进行简单的测试。代码如下：
        ```c
        #include "OLED.h"

        int main(){
            // 初始化OLED屏幕
            OLED_Init();
            // 进行简单的测试
            while(1){
                // 清除屏幕
                OLED_Clear();
                // 使用可以显示中英文混合字符串的函数，中文使用12X12的字体，英文使用7X12的字体
                OLED_PrintfMix(0, 0,OLED_12X12FULL,OLED_7X12_HALF,"你好,HI.");
                // 将显存数组的内容刷新到屏幕上
                OLED_Update();
            }
        }
        ```
        编译运行一气呵成，如果没有报错，恭喜您，您的OLED屏幕已经工作正常了。此时，您成功地移植了一个好用的OLED基础库。这个库有很多功能函数，后面我会详细介绍，您也可以参考 [ 【 江协科技的OLED教程 】 ](https://www.bilibili.com/video/BV1EN41177Pc/?spm_id_from=333.1387.homepage.video_card.click&vd_source=1183a399479e248598095c5e770be232)，这个教程有其中一些函数的使用方法。

    * **注意事项与可能遇到的问题**
        1. 如果您成功编译并且运行了，ASCII字符能够正常显示但是中文字符无法显示，请检查您的代码文件的编码格式。如果您使用的是UTF-8编码，请打开 `OLED_Fonts.h` 文件，在前几行找到如下宏定义代码：
            ```c
            /*中文字符字节宽度*/
            #define OLED_CHN_CHAR_WIDTH			(2)		//UTF-8编码格式给3，GB2312编码格式给2
            ```
            并将括号内部的2改成3，这是因为UTF-8编码格式给中文字符分配了3个字节，而GB2312编码格式给中文字符分配了2个字节。（这在江协科技的教程当中是有的）
        2. 您最好打开 `OLED.h` 文件,检查一下以下代码
            ```c
            //使用宏定义的方式确定oled的横向像素与竖向像素
            #define OLED_WIDTH						(128)					
            #define OLED_HEIGHT 					(128)
            ```
            确认一下OLED屏幕的实际分辨率，如果分辨率与上述代码不符，请修改这两个宏定义。
        3. 有关IIC通讯的问题：如果您的OLED屏幕使用的是IIC通讯，并且恰好您的单片机主频非常高，那么有必要在置高低电平的函数中加入延时，保证屏幕能够正确地接受数据。如果您的屏幕使用的是SPI通讯，那么您可以忽略这一点。当然，您完全可以自己修改OLED驱动文件，使用硬件IIC。
---
* ### 第二步 尝试运行示例程序        
    * **【复制OLED_UI实现文件并进行按钮驱动的编写】** 现在，您需要将OLED_UI的实现文件复制到您的工程目录下， **也就是说，您需要保证您的工程当中包含了表格当中的这些文件。** 我将他们列在了下方。
        | 文件名  | 描述 | 移植的时候是否需要修改 |
        |-------|-----|------|
        | `OLED_UI_Driver.c`  | 按钮、编码器等驱动文件         |   需要    |
        | `OLED_UI_Driver.h`  | 按钮、编码器等驱动文件的头文件  |   需要    |
        | `OLED_UI.c`  | 实现文件         |   暂不需要    |
        | `OLED_UI.h`  | 实现文件的头文件  |   暂不需要    |
        | `OLED_UI_MenuData.c`  | 菜单数据文件（其实就是示例程序的菜单数据）| 暂不需要    |
        | `OLED_UI_MenuData.h`  | 菜单数据文件的头文件  |   暂不需要    |
        ---
        **好了，您的工程文件夹中应该包含了这些文件了。**  现在，请打开 `OLED_UI_Driver.c` 和 `OLED_UI_Driver.h` 。这两个文件主要是用来驱动按键、编码器等硬件的，同时也包含一些需要根据实际移植情况修改的函数。在 `OLED_UI_Driver.h` 当中，有这些函数的声明，您需要根据您的实际情况实现这些函数。下方是对它们的说明：
        
        **读取按键状态的函数（此处使用宏来代替，目的是提高效率，减少代码量）**
        ```c
        //获取确认，取消，上，下按键状态的函数(【Q：为什么使用宏定义而不是函数？A：因为这样可以提高效率，减少代码量】)
        #define Key_GetEnterStatus()    digitalRead(BUTTON_PIN_1)  //确认键
        #define Key_GetBackStatus()     digitalRead(BUTTON_PIN_2)  //返回键
        #define Key_GetUpStatus()       digitalRead(BUTTON_PIN_3)  //上键
        #define Key_GetDownStatus()     digitalRead(BUTTON_PIN_4)  //下键
        ```
        这些宏定义是用来获取按键的状态的，您需要根据实际情况修改它们，使它们能够获取到按键的状态。

        **初始化函数**
        ```c
        //定时器中断初始化函数，初始化一个每20ms运行一次的定时器中断
        void Timer_Init(void);

        //按键初始化函数
        void Key_Init(void);

        //编码器初始化函数
        void Encoder_Init(void);
        ```
        这些函数是用来初始化硬件的，您需要根据实际情况修改它们，使它们能够正确地初始化硬件。
        
        **编码器使能、失能、读取增量值函数**
        ``` c
        // 编码器使能函数
        void Encoder_Enable(void);

        //编码器失能函数
        void Encoder_Disable(void);

        //读取编码器的增量值
        int16_t Encoder_Get(void);
        ```
        这些函数是用来控制编码器的，您需要根据实际情况修改它们，使它们能够正确地控制编码器。
        **延时函数**
        ```c
        //延时函数
        void Delay_us(uint32_t xus);
        void Delay_ms(uint32_t xms);
        void Delay_s(uint32_t xs);
        ```
        这些函数是用来延时的，您需要根据实际情况修改它们，使它们能够正确地延时。

        以上这些函数的修改，如果有困难，请参考 **例程中** 的实现。此外，编码器**并不是必须的**，如果您不需要编码器，那么您可以不实现这些函数。【将函数内容清空，如果有return值的函数，则返回0即可。】

    * **【编写主函数并运行示例程序】** 现在，您需要编写主函数，并在其中调用OLED_UI的实现函数。代码如下：
        ```c
        // 包含OLED_UI.h头文件
        #include "OLED_UI.h"
        // 包含例程菜单数据头文件
        #include "OLED_UI_MenuData.h"

        int main() {

          OLED_UI_Init(&MainMenuPage);
          while(1) {
            //OLED_UI的主循环函数
            OLED_UI_MainLoop();
          }
        }
        
        // 定时器中断服务函数
        void Timer() {
          // OLED_UI的定时器中断服务函数
          OLED_UI_InterruptHandler();
        }
        ```
        如果之前的操作都没问题，那一般来说就可以成功运行OLED UI的示例程序了。如果有问题，请仔细检查您的代码。请注意，`void Timer()` 是您的定时器中断服务函数，您需要根据实际情况修改函数名。

        如果定时器中断函数对您来说有困难，请将 `OLED_UI_InterruptHandler()` 这个函数也放置到主循环内，那样也可以运行OLED_UI，如下。只不过有关精确时间的计算就不再准确。

        ```c
        // 包含OLED_UI.h头文件
        #include "OLED_UI.h"
        // 包含例程菜单数据头文件
        #include "OLED_UI_MenuData.h"

        int main() {

          OLED_UI_Init(&MainMenuPage);
          while(1) {
            //OLED_UI的主循环函数
            OLED_UI_MainLoop();
            OLED_UI_InterruptHandler();
          }
        }
        ```

## 3. 创建自己的第一个菜单
> **如果您已经成功运行了示例程序，那么您已经具备了创建自己的菜单的条件，下面将介绍如何创建自己的菜单。**
---
**如果从头开始从原理讲起，反而还会让大家觉得麻烦，所以我们直接实战，上代码。**

未完待续...
