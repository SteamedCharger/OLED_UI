/*========================================================================*/
/*========================================================================*/
// ���ͷ�ļ���OLED-Basic-Lib��� [Ӳ����] ʵ���ļ�����ֲ��ʱ����Ҫ��������ļ�������
// �汾v1.0.0
/*========================================================================*/
/*========================================================================*/

#include "OLED_driver.h"

/*========================================================================*/
/*================================[�����ú�]===============================*/
/*========================================================================*/
// ��Щ��������Ϊ�ӿ����ã����Բ���Ҫ��ͷ�ļ���������ֱ����Դ�ļ��ж��弴�ɡ�
// �Ƿ�����̬ˢ�¡����������OLED-Basic-Lib������Դ���µ�ʱ��ˢ���б仯���������Ч�ʡ�
// �ڶ�������£����鿪����̬ˢ�£��������ʾЧ�ʡ�
#define IF_ENABLE_DYNAMIC_REFRESH       (false)
#define DYNAMIC_REFRESH_LENGHT          (16)   // ��̬ˢ������ĳ��ȣ���λΪ���ء�

#define OLED_HEIGHT_DRIVER	        	(128)					//OLED���صĸ߶�
#define OLED_WIDTH_DRIVER		    	(128)					//OLED���صĿ��
#define OLED_PAGE_DRIVER				(OLED_HEIGHT_DRIVER/8)	//OLED��ҳ�����ɸ߶��Զ����㣩

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define OLED_CS_Clr()   (GPIOA->BSRR = GPIO_Pin_4 << 16)  // ���� CS
#define OLED_CS_Set()   (GPIOA->BSRR = GPIO_Pin_4)        // ���� CS

#define OLED_DC_Clr()   (GPIOA->BSRR = GPIO_Pin_3 << 16)  // ���� DC (����ģʽ)
#define OLED_DC_Set()   (GPIOA->BSRR = GPIO_Pin_3)        // ���� DC (����ģʽ)

#define OLED_RES_Clr()  (GPIOA->BSRR = GPIO_Pin_2 << 16)  // ��λ
#define OLED_RES_Set()  (GPIOA->BSRR = GPIO_Pin_2)        // �ͷŸ�λ

/*========================================================================*/
/*========================================================================*/
/*========================================================================*/

// �Դ����飬OLED-Basic-Lib�Ļ��ƺ������Ƕ����������в���
uint8_t OLED_DisplayBuf[OLED_HEIGHT_DRIVER/8][OLED_WIDTH_DRIVER];

bool OLED_ColorMode = true;			//OLED����ɫģʽ
bool OLED_IfUpdate=false;			//�Ƿ��Ѿ������Դ�

// ����û������˶�̬ˢ�£��򴴽���̬ˢ������У������
#if IF_ENABLE_DYNAMIC_REFRESH
// �˴�У��ֵ
uint16_t page_checksum[OLED_PAGE_DRIVER][OLED_WIDTH_DRIVER/DYNAMIC_REFRESH_LENGHT] = {0}; 
// ��һ��У��ֵ
uint16_t previous_page_checksum[OLED_PAGE_DRIVER][OLED_WIDTH_DRIVER/DYNAMIC_REFRESH_LENGHT] = {0}; 
#endif


/**
 * @brief ������ʾģʽ
 * @param colormode true: ��ɫģʽ��false: ��ɫģʽ
 * @return ��
 */
void OLED_SetColorMode(bool colormode){
	OLED_ColorMode = colormode;
}

/**
 * @brief �˴�ˢ���Ƿ��и����Դ棬���ڼ���֡��
 * @return true: �и��£�false: �޸���
 */
bool OLED_IfChangedScreen(void){
	return OLED_IfUpdate;
}

/**
  * @brief  ���뼶��ʱ
  * @param  xms ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void OLED_DelayMs(uint32_t xms)
{
	while(xms--)
	{
		SysTick->LOAD = 72 * 1000;				//���ö�ʱ����װֵ
		SysTick->VAL = 0x00;					//��յ�ǰ����ֵ
		SysTick->CTRL = 0x00000005;				//����ʱ��ԴΪHCLK��������ʱ��
		while(!(SysTick->CTRL & 0x00010000));	//�ȴ�������0
		SysTick->CTRL = 0x00000004;				//�رն�ʱ��
	}
}
/**
 * @brief  ��ʼ�� SPI1 ��Ϊ����
 */
void SPI1_Init(void)
{
    // ���� SPI1 �� GPIOA ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    // ���� SPI1 (SCK: PA5, MOSI: PA7)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ���� CS (PA4), DC (PA3), RES (PA2) ��Ϊ�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*SPI��ʼ��*/
	SPI_InitTypeDef SPI_InitStructure;						//����ṹ�����
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			//ģʽ��ѡ��ΪSPI��ģʽ
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//����ѡ��2��ȫ˫��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//���ݿ�ȣ�ѡ��Ϊ8λ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		//����λ��ѡ���λ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//�����ʷ�Ƶ��ѡ���Ƶ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				//SPI���ԣ�ѡ��ͼ���
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;			//SPI��λ��ѡ���һ��ʱ�ӱ��ز��������Ժ���λ����ѡ��SPIģʽ0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;				//NSS��ѡ�����������
	SPI_InitStructure.SPI_CRCPolynomial = 7;				//CRC����ʽ����ʱ�ò�������Ĭ��ֵ7
	SPI_Init(SPI1, &SPI_InitStructure);						//���ṹ���������SPI_Init������SPI1
	
    SPI_Cmd(SPI1, ENABLE);  // ʹ�� SPI1
}
/**
 * @brief SPI1 ����һ���ֽ�
 * @param data Ҫ���͵�����
 */
void SPI1_WriteByte(uint8_t data)
{
    while (!(SPI1->SR & SPI_I2S_FLAG_TXE));  // �ȴ� TXE ��־��λ�����ͻ�����Ϊ�գ�
    SPI_I2S_SendData(SPI1, data);           // ��������
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE)); // �ȴ� RXNE ��λ�����ջ������ǿգ�
    (void)SPI_I2S_ReceiveData(SPI1);        // ��ȡ������� RXNE ��־
}
/**
 * @brief OLEDд1�ֽ�����
 * @param data Ҫд�������
 * @return ��
 */
void OLED_Write_DATA(uint8_t data)
{
//    OLED_DC_Set();   // ����Ϊ����ģʽ
//    OLED_CS_Clr();   // ѡ�� OLED
    SPI1_WriteByte(data);  // ͨ��Ӳ�� SPI ��������
//    OLED_CS_Set();   // ȡ��ѡ�� OLED
}

/**
 * @brief OLEDд����
 * @param Data Ҫд�����������
 * @param Count Ҫд������ݸ���
 */
void OLED_WriteDataArr(uint8_t *Data, uint8_t Count)
{
	OLED_DC_Set();    // ��������������Ϊ����ģʽ
    OLED_CS_Clr();    // ѡ��OLED
	
	if (OLED_ColorMode) {
        for (uint8_t i = 0; i < Count; i++) {
            OLED_Write_DATA(Data[i]);
        }
    } else {
        for (uint8_t i = 0; i < Count; i++) {
            OLED_Write_DATA(~Data[i]);
        }
    }
	OLED_CS_Set();    // ȡ��ѡ��OLED
}


/**
 * @brief OLEDд����
 * @param data Ҫд�������
 * @return ��
 */
void  OLED_Write_CMD(uint8_t cmd)
{
    OLED_DC_Clr();   // ����Ϊ����ģʽ
    OLED_CS_Clr();   // ѡ�� OLED
    SPI1_WriteByte(cmd);  // ͨ��Ӳ�� SPI ��������
    OLED_CS_Set();   // ȡ��ѡ�� OLED
}




/**
 * @brief ������ʾ���λ��
 * @param Page ҳ��
 * @param X X������
 * @return ��
 */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*�����ڴ˵���X������ӦһЩоƬX�������ƫ��*/
	/*X += 2;*/
	/*ͨ��ָ������ҳ��ַ���е�ַ*/
	OLED_Write_CMD(0xB0 | Page);					//����ҳλ��
	OLED_Write_CMD(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	OLED_Write_CMD(0x00 | (X & 0x0F));			//����Xλ�õ�4λ
}



/**
 * @brief ���� CRC-16-CCITT У��ֵ
 * @param data Ҫ����У��ֵ���ֽ�����
 * @param length Ҫ����У��ֵ���ֽڳ���
 * @return У��ֵ
 */
uint16_t compute_crc16(const uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;
    const uint8_t *end = data + length;
    while (data < end) {
        crc ^= *data++ << 8;
        // չ����λ����
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
 * @brief ���㶯̬ˢ��У��ֵ
 * @note ���㶯̬ˢ��У��ֵ����������ʷУ��ֵ
 * @return ��
 */
void OLED_CalcVerify(void) {
    #if IF_ENABLE_DYNAMIC_REFRESH
    for (uint8_t page = 0; page < OLED_PAGE_DRIVER; page++) {
        // ����ÿ��ҳ��ֿ��У��ֵ
        for (uint8_t block = 0; block < (OLED_WIDTH_DRIVER / DYNAMIC_REFRESH_LENGHT); block++) {
            uint8_t xor_sum = 0;
            uint16_t crc_sum;
            uint16_t start_col = block * DYNAMIC_REFRESH_LENGHT;
            uint16_t end_col = start_col + DYNAMIC_REFRESH_LENGHT;

            // ������ʷУ��ֵ
            previous_page_checksum[page][block] = page_checksum[page][block];

            // ������кŻ����� XOR
            for (uint16_t col = start_col; col < end_col; col++) {
                xor_sum ^= OLED_DisplayBuf[page][col] ^ (uint8_t)col + OLED_ColorMode;
            }

            // ����Ľ��� CRC�������㵱ǰ������ݣ�
            crc_sum = compute_crc16(&OLED_DisplayBuf[page][start_col], DYNAMIC_REFRESH_LENGHT);

            // �ǶԳ����У��ֵ
            page_checksum[page][block] = crc_sum ^ (xor_sum << 8);
        }
    }
    #endif
}


/**
 * @brief ˢ����ʾ
 * @note ˢ����ʾ����������˶�̬ˢ�£����ˢ���б仯������
 * @return ��
 */
void OLED_Update(void)
{
    
    OLED_IfUpdate = false;
#if IF_ENABLE_DYNAMIC_REFRESH
	OLED_CalcVerify();
    /* ����ÿһҳ */
    for (uint8_t page = 0; page < OLED_PAGE_DRIVER; page++)
    {
        /* ����ÿ���� */
        for (uint8_t block = 0; block < (OLED_WIDTH_DRIVER / DYNAMIC_REFRESH_LENGHT); block++)
        {
            /* �����ÿ��б仯ʱ��ˢ�� */
            if (page_checksum[page][block] != previous_page_checksum[page][block])
            {
                /* ���㵱ǰ�����ʼ�� */
                uint16_t start_col = block * DYNAMIC_REFRESH_LENGHT;
                /* ���ù��λ��Ϊ��ҳ�Ŀ���ʼ�� */
                OLED_SetCursor(page, start_col);
                /* ���ÿ������д�� OLED */
                OLED_WriteDataArr(&OLED_DisplayBuf[page][start_col], DYNAMIC_REFRESH_LENGHT);
                OLED_IfUpdate = true;
            }
        }
    }
#else
    uint8_t page;
    for (page = 0; page < OLED_PAGE_DRIVER; page++)
    {
        /* ���ù��λ��Ϊÿһҳ�ĵ�һ�� */
        OLED_SetCursor(page, 0);
        /* ����д�� OLED_WIDTH_DRIVER �����ݣ����Դ����������д�뵽 OLED Ӳ�� */
        OLED_WriteDataArr(OLED_DisplayBuf[page], OLED_WIDTH_DRIVER);
    }
    OLED_IfUpdate = true;
#endif
}


/**
 * @brief ˢ��ָ������
 * @param X X������
 * @param Y Y������
 * @param Width ���
 * @param Height �߶�
 * @note �˺��������ٸ��²���ָ�������������������Y��ֻ��������ҳ����ͬһҳ��ʣ�ಿ�ֻ����һ�����
 * @return ��
 */
void OLED_UpdateArea(int16_t X, int16_t Y, int16_t Width, int16_t Height)
{
	int16_t j;
	
	/*������飬��ָ֤�����򲻻ᳬ����Ļ��Χ*/
	if (X > OLED_WIDTH_DRIVER-1) {return;}
	if (Y > OLED_HEIGHT_DRIVER-1) {return;}
	if (X < 0) {X = 0;}
	if (Y < 0) {Y = 0;}
	if (X + Width > OLED_WIDTH_DRIVER) {Width = OLED_WIDTH_DRIVER - X;}
	if (Y + Height > OLED_HEIGHT_DRIVER) {Height = OLED_HEIGHT_DRIVER - Y;}
	
	/*����ָ�������漰�����ҳ*/
	/*(Y + Height - 1) / 8 + 1��Ŀ����(Y + Height) / 8������ȡ��*/
	for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j ++)
	{
		/*���ù��λ��Ϊ���ҳ��ָ����*/
		OLED_SetCursor(j, X);
		/*����д��Width�����ݣ����Դ����������д�뵽OLEDӲ��*/
		OLED_WriteDataArr(&OLED_DisplayBuf[j][X], Width);
	}
	
}
//����һ��OLED_Clear�������������Ա���OLED_Init�����е���
extern void OLED_Clear(void);

/**
 * @brief OLED��ʼ��
 * @return ��
 */
void OLED_Init(void)
{
	SPI1_Init();  // ��ʼ�� SPI1

    OLED_RES_Clr();  // ��λ OLED
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
 * @brief ��������
 * @param Brightness ����ֵ��0-255
 * @note һЩ��ĻоƬ���ܻ������Ƚϵ͵�ʱ��ֱ�Ӻ�������Ҫע��һ�¡�
 * @return ��
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


