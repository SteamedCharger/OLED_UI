/*========================================================================*/
/*========================================================================*/
// 这个头文件是OLED-Basic-Lib库的 [硬件层] 实现文件，移植的时候需要更改这个文件的内容
// 版本v1.0.0
/*========================================================================*/
/*========================================================================*/

#include "OLED_driver.h"

/*========================================================================*/
/*================================[可配置宏]===============================*/
/*========================================================================*/
// 这些宏无需作为接口配置，所以不需要在头文件中声明，直接在源文件中定义即可。
// 是否开启动态刷新。如果开启，OLED-Basic-Lib会仅在显存更新的时候刷新有变化的区域，提高效率。
// 在多数情况下，建议开启动态刷新，以提高显示效率。
#define IF_ENABLE_DYNAMIC_REFRESH       (false)
#define DYNAMIC_REFRESH_LENGHT          (16)   // 动态刷新区块的长度，单位为像素。

#define OLED_HEIGHT_DRIVER	        	(128)					//OLED像素的高度
#define OLED_WIDTH_DRIVER		    	(128)					//OLED像素的宽度
#define OLED_PAGE_DRIVER				(OLED_HEIGHT_DRIVER/8)	//OLED的页数（由高度自动计算）

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define OLED_CS_Clr()   (GPIOA->BSRR = GPIO_Pin_4 << 16)  // 拉低 CS
#define OLED_CS_Set()   (GPIOA->BSRR = GPIO_Pin_4)        // 拉高 CS

#define OLED_DC_Clr()   (GPIOA->BSRR = GPIO_Pin_3 << 16)  // 拉低 DC (命令模式)
#define OLED_DC_Set()   (GPIOA->BSRR = GPIO_Pin_3)        // 拉高 DC (数据模式)

#define OLED_RES_Clr()  (GPIOA->BSRR = GPIO_Pin_2 << 16)  // 复位
#define OLED_RES_Set()  (GPIOA->BSRR = GPIO_Pin_2)        // 释放复位

/*========================================================================*/
/*========================================================================*/
/*========================================================================*/

// 显存数组，OLED-Basic-Lib的绘制函数都是对这个数组进行操作
uint8_t OLED_DisplayBuf[OLED_HEIGHT_DRIVER/8][OLED_WIDTH_DRIVER];

bool OLED_ColorMode = true;			//OLED的颜色模式
bool OLED_IfUpdate=false;			//是否已经更新显存

// 如果用户定义了动态刷新，则创建动态刷新区块校验数组
#if IF_ENABLE_DYNAMIC_REFRESH
// 此次校验值
uint16_t page_checksum[OLED_PAGE_DRIVER][OLED_WIDTH_DRIVER/DYNAMIC_REFRESH_LENGHT] = {0}; 
// 上一次校验值
uint16_t previous_page_checksum[OLED_PAGE_DRIVER][OLED_WIDTH_DRIVER/DYNAMIC_REFRESH_LENGHT] = {0}; 
#endif


/**
 * @brief 设置显示模式
 * @param colormode true: 黑色模式，false: 白色模式
 * @return 无
 */
void OLED_SetColorMode(bool colormode){
	OLED_ColorMode = colormode;
}

/**
 * @brief 此次刷新是否有更新显存，用于计算帧率
 * @return true: 有更新，false: 无更新
 */
bool OLED_IfChangedScreen(void){
	return OLED_IfUpdate;
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void OLED_DelayMs(uint32_t xms)
{
	while(xms--)
	{
		SysTick->LOAD = 72 * 1000;				//设置定时器重装值
		SysTick->VAL = 0x00;					//清空当前计数值
		SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
		while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
		SysTick->CTRL = 0x00000004;				//关闭定时器
	}
}
/**
 * @brief  初始化 SPI1 作为主机
 */
void SPI1_Init(void)
{
    // 开启 SPI1 和 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 SPI1 (SCK: PA5, MOSI: PA7)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 CS (PA4), DC (PA3), RES (PA2) 作为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*SPI初始化*/
	SPI_InitTypeDef SPI_InitStructure;						//定义结构体变量
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			//模式，选择为SPI主模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//方向，选择2线全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//数据宽度，选择为8位
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		//先行位，选择高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//波特率分频，选择分频
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				//SPI极性，选择低极性
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;			//SPI相位，选择第一个时钟边沿采样，极性和相位决定选择SPI模式0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;				//NSS，选择由软件控制
	SPI_InitStructure.SPI_CRCPolynomial = 7;				//CRC多项式，暂时用不到，给默认值7
	SPI_Init(SPI1, &SPI_InitStructure);						//将结构体变量交给SPI_Init，配置SPI1
	
    SPI_Cmd(SPI1, ENABLE);  // 使能 SPI1
}
/**
 * @brief SPI1 发送一个字节
 * @param data 要发送的数据
 */
void SPI1_WriteByte(uint8_t data)
{
    while (!(SPI1->SR & SPI_I2S_FLAG_TXE));  // 等待 TXE 标志置位（发送缓冲区为空）
    SPI_I2S_SendData(SPI1, data);           // 发送数据
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE)); // 等待 RXNE 置位（接收缓冲区非空）
    (void)SPI_I2S_ReceiveData(SPI1);        // 读取数据清除 RXNE 标志
}
/**
 * @brief OLED写1字节数据
 * @param data 要写入的数据
 * @return 无
 */
void OLED_Write_DATA(uint8_t data)
{
//    OLED_DC_Set();   // 设置为数据模式
//    OLED_CS_Clr();   // 选中 OLED
    SPI1_WriteByte(data);  // 通过硬件 SPI 发送数据
//    OLED_CS_Set();   // 取消选中 OLED
}

/**
 * @brief OLED写数据
 * @param Data 要写入的数据数组
 * @param Count 要写入的数据个数
 */
void OLED_WriteDataArr(uint8_t *Data, uint8_t Count)
{
	OLED_DC_Set();    // 设置数据命令线为数据模式
    OLED_CS_Clr();    // 选中OLED
	
	if (OLED_ColorMode) {
        for (uint8_t i = 0; i < Count; i++) {
            OLED_Write_DATA(Data[i]);
        }
    } else {
        for (uint8_t i = 0; i < Count; i++) {
            OLED_Write_DATA(~Data[i]);
        }
    }
	OLED_CS_Set();    // 取消选中OLED
}


/**
 * @brief OLED写命令
 * @param data 要写入的命令
 * @return 无
 */
void  OLED_Write_CMD(uint8_t cmd)
{
    OLED_DC_Clr();   // 设置为命令模式
    OLED_CS_Clr();   // 选中 OLED
    SPI1_WriteByte(cmd);  // 通过硬件 SPI 发送数据
    OLED_CS_Set();   // 取消选中 OLED
}




/**
 * @brief 设置显示光标位置
 * @param Page 页号
 * @param X X轴坐标
 * @return 无
 */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*可以在此调整X，以适应一些芯片X轴坐标的偏移*/
	/*X += 2;*/
	/*通过指令设置页地址和列地址*/
	OLED_Write_CMD(0xB0 | Page);					//设置页位置
	OLED_Write_CMD(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_Write_CMD(0x00 | (X & 0x0F));			//设置X位置低4位
}



/**
 * @brief 计算 CRC-16-CCITT 校验值
 * @param data 要计算校验值的字节数组
 * @param length 要计算校验值的字节长度
 * @return 校验值
 */
uint16_t compute_crc16(const uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;
    const uint8_t *end = data + length;
    while (data < end) {
        crc ^= *data++ << 8;
        // 展开的位计算
        crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}


/**
 * @brief 计算动态刷新校验值
 * @note 计算动态刷新校验值，并更新历史校验值
 * @return 无
 */
void OLED_CalcVerify(void) {
    #if IF_ENABLE_DYNAMIC_REFRESH
    for (uint8_t page = 0; page < OLED_PAGE_DRIVER; page++) {
        // 计算每个页面分块的校验值
        for (uint8_t block = 0; block < (OLED_WIDTH_DRIVER / DYNAMIC_REFRESH_LENGHT); block++) {
            uint8_t xor_sum = 0;
            uint16_t crc_sum;
            uint16_t start_col = block * DYNAMIC_REFRESH_LENGHT;
            uint16_t end_col = start_col + DYNAMIC_REFRESH_LENGHT;

            // 更新历史校验值
            previous_page_checksum[page][block] = page_checksum[page][block];

            // 计算带列号混淆的 XOR
            for (uint16_t col = start_col; col < end_col; col++) {
                xor_sum ^= OLED_DisplayBuf[page][col] ^ (uint8_t)col + OLED_ColorMode;
            }

            // 计算改进版 CRC（仅计算当前块的数据）
            crc_sum = compute_crc16(&OLED_DisplayBuf[page][start_col], DYNAMIC_REFRESH_LENGHT);

            // 非对称组合校验值
            page_checksum[page][block] = crc_sum ^ (xor_sum << 8);
        }
    }
    #endif
}


/**
 * @brief 刷新显示
 * @note 刷新显示，如果开启了动态刷新，则仅刷新有变化的区域。
 * @return 无
 */
void OLED_Update(void)
{
    
    OLED_IfUpdate = false;
#if IF_ENABLE_DYNAMIC_REFRESH
	OLED_CalcVerify();
    /* 遍历每一页 */
    for (uint8_t page = 0; page < OLED_PAGE_DRIVER; page++)
    {
        /* 遍历每个块 */
        for (uint8_t block = 0; block < (OLED_WIDTH_DRIVER / DYNAMIC_REFRESH_LENGHT); block++)
        {
            /* 仅当该块有变化时才刷新 */
            if (page_checksum[page][block] != previous_page_checksum[page][block])
            {
                /* 计算当前块的起始列 */
                uint16_t start_col = block * DYNAMIC_REFRESH_LENGHT;
                /* 设置光标位置为该页的块起始列 */
                OLED_SetCursor(page, start_col);
                /* 将该块的数据写入 OLED */
                OLED_WriteDataArr(&OLED_DisplayBuf[page][start_col], DYNAMIC_REFRESH_LENGHT);
                OLED_IfUpdate = true;
            }
        }
    }
#else
    uint8_t page;
    for (page = 0; page < OLED_PAGE_DRIVER; page++)
    {
        /* 设置光标位置为每一页的第一列 */
        OLED_SetCursor(page, 0);
        /* 连续写入 OLED_WIDTH_DRIVER 个数据，将显存数组的数据写入到 OLED 硬件 */
        OLED_WriteDataArr(OLED_DisplayBuf[page], OLED_WIDTH_DRIVER);
    }
    OLED_IfUpdate = true;
#endif
}


/**
 * @brief 刷新指定区域
 * @param X X轴坐标
 * @param Y Y轴坐标
 * @param Width 宽度
 * @param Height 高度
 * @note 此函数会至少更新参数指定的区域。如果更新区域Y轴只包含部分页，则同一页的剩余部分会跟随一起更新
 * @return 无
 */
void OLED_UpdateArea(int16_t X, int16_t Y, int16_t Width, int16_t Height)
{
	int16_t j;
	
	/*参数检查，保证指定区域不会超出屏幕范围*/
	if (X > OLED_WIDTH_DRIVER-1) {return;}
	if (Y > OLED_HEIGHT_DRIVER-1) {return;}
	if (X < 0) {X = 0;}
	if (Y < 0) {Y = 0;}
	if (X + Width > OLED_WIDTH_DRIVER) {Width = OLED_WIDTH_DRIVER - X;}
	if (Y + Height > OLED_HEIGHT_DRIVER) {Height = OLED_HEIGHT_DRIVER - Y;}
	
	/*遍历指定区域涉及的相关页*/
	/*(Y + Height - 1) / 8 + 1的目的是(Y + Height) / 8并向上取整*/
	for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j ++)
	{
		/*设置光标位置为相关页的指定列*/
		OLED_SetCursor(j, X);
		/*连续写入Width个数据，将显存数组的数据写入到OLED硬件*/
		OLED_WriteDataArr(&OLED_DisplayBuf[j][X], Width);
	}
	
}
//声明一下OLED_Clear的清屏函数，以便在OLED_Init函数中调用
extern void OLED_Clear(void);

/**
 * @brief OLED初始化
 * @return 无
 */
void OLED_Init(void)
{
	SPI1_Init();  // 初始化 SPI1

    OLED_RES_Clr();  // 复位 OLED
    OLED_DelayMs(50);
    OLED_RES_Set();
	OLED_Write_CMD(0xAE);//--turn off oled panel
	OLED_Write_CMD(0xd5); // Set Frame Frequency
	OLED_Write_CMD(0x50); // 104Hz
	OLED_Write_CMD(0x20); // Set Memory Addressing Mode
	OLED_Write_CMD(0x81); // Set Contrast Control
	OLED_Write_CMD(0x4f);
	OLED_Write_CMD(0xad); // Set DC/DC off
	OLED_Write_CMD(0x8a);

	OLED_Write_CMD(0xC0);
	OLED_Write_CMD(0xA0);

	OLED_Write_CMD(0xdc); // Set Display Start Line
	OLED_Write_CMD(0x00);
	OLED_Write_CMD(0xd3); // Set Display Offset
	OLED_Write_CMD(0x00);
	OLED_Write_CMD(0xd9); // Set Discharge / Pre-Charge Period
	OLED_Write_CMD(0x22);
	OLED_Write_CMD(0xdb); // Set Vcomh voltage
	OLED_Write_CMD(0x35);
	
	OLED_Write_CMD(0xa8); // Set Multiplex Ration
	OLED_Write_CMD(0x7f);
	
	OLED_Write_CMD(0xa4); // Set Entire Display OFF/ON
	OLED_Write_CMD(0xa6); // Set Normal/Reverse Display

	OLED_Clear();
	OLED_Update();
	OLED_Write_CMD(0xAF);//Display ON
}



/**
 * @brief 设置亮度
 * @param Brightness 亮度值，0-255
 * @note 一些屏幕芯片可能会在亮度较低的时候直接黑屏，需要注意一下。
 * @return 无
 */
void OLED_SetBrightness(int16_t Brightness){
	if(Brightness>255){
		Brightness=255;
	}
	if(Brightness<0){
		Brightness=0;
	}
	OLED_Write_CMD(0x81);
	OLED_Write_CMD(Brightness);
 }


