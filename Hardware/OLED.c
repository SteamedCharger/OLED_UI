/*========================================================================*/
/*========================================================================*/
// ���ͷ�ļ���OLED-Basic-Lib��� [�����] ʵ���ļ�����ֲ��ʱ��ֻ��Ҫ�޸�ͷ�ļ�����Ļ�����߶�
// �汾v1.0.0
/*========================================================================*/
/*========================================================================*/
#include "OLED.h"
/**
  * ����OLED�Դ����飬�������Ѿ���OLED_Driver.c�ж���
  * ���е���ʾ��������ֻ�ǶԴ��Դ�������ж�д
  * ������OLED_Update������OLED_UpdateArea����
  * �ŻὫ�Դ���������ݷ��͵�OLEDӲ����������ʾ
  */
extern uint8_t OLED_DisplayBuf[OLED_HEIGHT/8][OLED_WIDTH];





/**�����Ƿ��ڻ���ͼ���������֮ǰ��ǰ������������Դ�ĺ� */
#define IF_CLEAR_AREA        (false)


/*********************���ߺ�����********************/

/**
 * @brief ������ת��Ϊ�ַ���
 * @param num Ҫת��������
 * @param str ���������
 * @return ����ַ���
 */
char* itoa(int num, char* str, int base) {
    char* ptr = str;
    char temp_buf[33]; // ���ӻ�������С��ȷ����ȫ
    int temp_ptr = 0;
    int is_negative = 0;
    unsigned int value;
    
    // ������
    if (num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }
    
    // ������
    if (num < 0 && base == 10) {
        is_negative = 1;
        value = -num;
    } else {
        // ��ʮ���ƻ�������ֱ��תΪ�޷��Ŵ���
        value = (unsigned int)num;
    }
    
    // ת�����ֵ��ַ���������ʱ������
    while (value > 0) {
        int digit = value % base;
        temp_buf[temp_ptr++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        value /= base;
    }
    
    // ����Ǹ�������Ӹ���
    if (is_negative) {
        *ptr++ = '-';
    }
    
    // ��������ʱ������������ַ���
    while (temp_ptr > 0) {
        *ptr++ = temp_buf[--temp_ptr];
    }
    
    *ptr = '\0';
    return str;
}

/**
 * @brief ��������ת��Ϊ�ַ���
 * @param num Ҫת���ĸ�����
 * @param str ���������
 * @param precision ���ȣ�С�����λ
 * @return ����ַ���
 */
char* ftoa(float num, char* str, int precision) {
    char* ptr = str;
    int int_part;
    float float_part;
    int i;
    char temp_buf[32]; // ������ʱ�洢��������
    int temp_ptr = 0;
    
    // ������ - �ȼ�¼���ţ���������
    int is_negative = 0;
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    // �����������ֺ�С������
    int_part = (int)num;
    float_part = num - int_part;
    
    // ����Ǹ���������Ӹ���
    if (is_negative) {
        *ptr++ = '-';
    }
    
    // ת���������� - �ȴ�����ʱ������
    if (int_part == 0) {
        *ptr++ = '0';
    } else {
        while (int_part > 0) {
            temp_buf[temp_ptr++] = '0' + (int_part % 10);
            int_part /= 10;
        }
        
        // ��ת�����Ƶ�����ַ���
        for (i = temp_ptr - 1; i >= 0; i--) {
            *ptr++ = temp_buf[i];
        }
    }
    
    // ����С������
    if (precision > 0) {
        *ptr++ = '.';
        
        // ����С������
        for (i = 0; i < precision; i++) {
            float_part *= 10;
            int digit = (int)float_part;
            *ptr++ = '0' + digit;
            float_part -= digit;
        }
    }
    
    *ptr = '\0';
    return str;
}

/**
 * @brief һ���򵥵�sprintf���������ڽ���ʽ���ַ��������������
 * @param buf ���������
 * @param format ��ʽ���ַ���
 * @param... ��ʽ������
 * @return д����ַ���
 */
/**
 * @brief һ���򵥵�sprintf���������ڽ���ʽ���ַ��������������
 * @param buf ���������
 * @param format ��ʽ���ַ���
 * @param... ��ʽ������
 * @return д����ַ���
 */
int OLED_Simple_vsprintf(char* buf, const char* format, va_list args) {
    char* str_ptr = buf;           // ����������ĵ�ǰָ��
    const char* fmt_ptr = format;  // ��ʽ�ַ����ĵ�ǰָ��
    char temp_buf[32];             // ��ʱ���������㹻���ɴ��������ת��
    char padding_char;             // ����ַ���Ĭ�Ͽո��0��
    int padding_width, precision;  // ����Ⱥ;���
    int left_align;                // ������־
    int len, i;

    while (*fmt_ptr) {
        // �Ǹ�ʽ���ַ�ֱ�Ӹ���
        if (*fmt_ptr != '%') {
            *str_ptr++ = *fmt_ptr++;
            continue;
        }

        fmt_ptr++;  // ���� '%'

        // ���� '%%' ת��
        if (*fmt_ptr == '%') {
            *str_ptr++ = '%';
            fmt_ptr++;
            continue;
        }

        // ����������Ϳ��
        padding_char = ' ';
        padding_width = 0;
        precision = 6;  // Ĭ�ϸ���������
        left_align = 0; // Ĭ���Ҷ���

        // ���������־
        if (*fmt_ptr == '-') {
            left_align = 1;
            fmt_ptr++;
        }

        if (*fmt_ptr == '0' && !left_align) { // 0�����ڷ������ʱ��Ч
            padding_char = '0';
            fmt_ptr++;
        }

        // �������
        while (*fmt_ptr >= '0' && *fmt_ptr <= '9') {
            padding_width = padding_width * 10 + (*fmt_ptr - '0');
            fmt_ptr++;
        }

        // �������ȣ�֧�� %.*f ��ʽ
        if (*fmt_ptr == '.') {
            fmt_ptr++;
            if (*fmt_ptr == '*') { // ����̬����
                precision = va_arg(args, int); // �Ӳ����б��ȡ����
                fmt_ptr++;
            } else { // ��̬����
                precision = 0;
                while (*fmt_ptr >= '0' && *fmt_ptr <= '9') {
                    precision = precision * 10 + (*fmt_ptr - '0');
                    fmt_ptr++;
                }
            }
        }

        // ���ݸ�ʽ˵�����������
        switch (*fmt_ptr) {
            case 'd':
            case 'i': {
                int val = va_arg(args, int);
                itoa(val, temp_buf, 10);
                len = 0;
                while (temp_buf[len]) len++;

                // �������
                if (!left_align) {
                    for (i = 0; i < padding_width - len; i++) {
                        *str_ptr++ = padding_char;
                    }
                }

                // ��������
                for (i = 0; i < len; i++) {
                    *str_ptr++ = temp_buf[i];
                }

                // �����ʱ���Ҳ����
                if (left_align) {
                    for (i = 0; i < padding_width - len; i++) {
                        *str_ptr++ = ' '; // �����ʱ�Ҳ�ʼ���ÿո����
                    }
                }
                break;
            }
            case 'u': {
                unsigned int val = va_arg(args, unsigned int);
                itoa(val, temp_buf, 10);
                len = 0;
                while (temp_buf[len]) len++;

                // �����������
                if (!left_align) {
                    for (i = 0; i < padding_width - len; i++) {
                        *str_ptr++ = padding_char;
                    }
                }

                for (i = 0; i < len; i++) {
                    *str_ptr++ = temp_buf[i];
                }

                if (left_align) {
                    for (i = 0; i < padding_width - len; i++) {
                        *str_ptr++ = ' ';
                    }
                }
                break;
            }
            case 'f': {
                float val = (float)va_arg(args, double); // va_arg �� float ����ʹ�� double
                ftoa(val, temp_buf, precision);          // ʹ�ý����õ��ľ���
                len = 0;
                while (temp_buf[len]) len++;

                // �������
                if (!left_align) {
                    for (i = 0; i < padding_width - len; i++) {
                        *str_ptr++ = padding_char;
                    }
                }

                // ��������
                for (i = 0; i < len; i++) {
                    *str_ptr++ = temp_buf[i];
                }

                if (left_align) {
                    for (i = 0; i < padding_width - len; i++) {
                        *str_ptr++ = ' ';
                    }
                }
                break;
            }
            case 'c': {
                char val = (char)va_arg(args, int); // char �� va_arg ������Ϊ int

                // �������
                if (!left_align) {
                    for (i = 0; i < padding_width - 1; i++) {
                        *str_ptr++ = padding_char;
                    }
                }

                *str_ptr++ = val;

                if (left_align) {
                    for (i = 0; i < padding_width - 1; i++) {
                        *str_ptr++ = ' ';
                    }
                }
                break;
            }
            case 's': {
                char* val = va_arg(args, char*);
                len = 0;
                while (val[len]) len++;

                // �������
                if (!left_align) {
                    for (i = 0; i < padding_width - len; i++) {
                        *str_ptr++ = padding_char;
                    }
                }

                // �����ַ���
                for (i = 0; i < len; i++) {
                    *str_ptr++ = val[i];
                }

                if (left_align) {
                    for (i = 0; i < padding_width - len; i++) {
                        *str_ptr++ = ' ';
                    }
                }
                break;
            }
            default:
                // δ֪��ʽ������
                break;
        }

        fmt_ptr++;
    }

    *str_ptr = '\0';  // ȷ���ַ����� null ��β
    return str_ptr - buf;  // ����д����ַ���
}








/**
 * @brief ����X��Y�η�
 * @param X ����
 * @param Y ָ��
 * @return X��Y�η�
 */
inline uint32_t OLED_Pow(uint32_t X, uint32_t Y) {
    uint32_t result = 1;
    while (Y--) {
        result *= X;
    }
    return result;
}


/**
 * @brief ��ȡ�ַ����
 * @param oledfont ��������
 * @param ascii_or_chinese �궨�壬��ʾ��ASCII�ַ����������ַ�
 * @return �ַ����
 */
uint8_t OLED_GetFontWidth(uint8_t oledfont, uint8_t ascii_or_chinese) {
    switch (oledfont) {
#ifdef OLED_FONT_8
        case OLED_FONT_8:  return (ascii_or_chinese == OLED_ASCII) ? 6 : 8;
#endif

#ifdef OLED_FONT_12
        case OLED_FONT_12: return (ascii_or_chinese == OLED_ASCII) ? 7 : 12;
#endif

#ifdef OLED_FONT_16
        case OLED_FONT_16: return (ascii_or_chinese == OLED_ASCII) ? 8 : 16;
#endif

#ifdef OLED_FONT_20
        case OLED_FONT_20: return (ascii_or_chinese == OLED_ASCII) ? 10 : 20;
#endif		
    }
	return 0;
}


/**
 * @brief �ж�ָ�����Ƿ���ָ��������ڲ�
 * @param nvert ����εĶ�����
 * @param vertx verty ��������ζ����x��y���������
 * @param testx testy ���Ե��X��y����
 * @return ָ�����Ƿ���ָ��������ڲ���1�����ڲ���0�������ڲ�
 */
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy)
{
	int16_t i = 0, j = 0;
	uint8_t c = 0;
	/*���㷨��W. Randolph Franklin���*/
	/*�ο����ӣ�https://wrfranklin.org/Research/Short_Notes/pnpoly.html*/
	for (i = 0, j = nvert - 1; i < nvert; j = i++)
	{
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
		{
			c = !c;
		}
	}
	return c;
}

/**
 * @brief �ȴ�С����
 * @param a ��һ��ֵ
 * @param b �ڶ���ֵ
 * @param c ������ֵ
 * @param d ���ĸ�ֵ
 * @return �ĸ�����ֵ���е����ֵ
 */
int16_t max(int16_t a, int16_t b, int16_t c, int16_t d) {
    int16_t max_val = a; // ����a������

    if (b > max_val) {
        max_val = b; // ���b���ڵ�ǰ���ֵ����������ֵΪb
    }
    if (c > max_val) {
        max_val = c; // ���c���ڵ�ǰ���ֵ����������ֵΪc
    }
    if (d > max_val) {
        max_val = d; // ���d���ڵ�ǰ���ֵ����������ֵΪd
    }

    return max_val; // �������ֵ
}


/**
 * @brief ����ֵ����
 * @param num ����ֵ
 * @return num�ľ���ֵ
 */
float numabs(float num){
	if(num>0)
		return num;
	if(num<0)
		return -num;
	return 0;
}




/**
 * @brief �ж�ָ�����Ƿ���ָ���Ƕ��ڲ�
 * @param X ָ�����X����
 * @param Y ָ�����Y����
 * @param StartAngle ��ʼ�Ƕ�
 * @param EndAngle ��ֹ�Ƕ�
 * @return ָ�����Ƿ���ָ���Ƕ��ڲ���1�����ڲ���0�������ڲ�
 */
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
	int16_t PointAngle;
	PointAngle = atan2(Y, X) / 3.14 * 180;	//����ָ����Ļ��ȣ���ת��Ϊ�Ƕȱ�ʾ
	if (StartAngle < EndAngle)	//��ʼ�Ƕ�С����ֹ�Ƕȵ����
	{
		/*���ָ���Ƕ�����ʼ��ֹ�Ƕ�֮�䣬���ж�ָ������ָ���Ƕ�*/
		if (PointAngle >= StartAngle && PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	else			//��ʼ�Ƕȴ�������ֹ�Ƕȵ����
	{
		/*���ָ���Ƕȴ�����ʼ�ǶȻ���С����ֹ�Ƕȣ����ж�ָ������ָ���Ƕ�*/
		if (PointAngle >= StartAngle || PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	return 0;		//�������������������ж��ж�ָ���㲻��ָ���Ƕ�
}

/*********************���ߺ�����********************/

/*********************���ܺ�����*********************/

/**
 * @brief ����Դ�����
 * @param ��
 * @return ��
 */
void OLED_Clear(void) {
    memset(OLED_DisplayBuf, 0, sizeof(OLED_DisplayBuf));
}


/**
 * @brief ���ָ������
 * @param X ָ���������Ͻǵĺ����꣬��Χ�����⣬��Ч�Զ�����
 * @param Y ָ���������Ͻǵ������꣬��Χ�����⣬��Ч�Զ�����
 * @param Width ָ������Ŀ�ȣ���Χ�����⣬��Ч�Զ�����
 * @param Height ָ������ĸ߶ȣ���Χ����������Ч�Զ�����
 * @return ��
 */
void OLED_ClearArea(int16_t X, int16_t Y, int16_t Width, int16_t Height) {
    if (Width <= 0 || Height <= 0) return;

    int16_t x_start = (X < 0) ? 0 : X;
    int16_t y_start = (Y < 0) ? 0 : Y;
    int16_t x_end = (X + Width > OLED_WIDTH) ? OLED_WIDTH : (X + Width);
    int16_t y_end = (Y + Height > OLED_HEIGHT) ? OLED_HEIGHT : (Y + Height);

    if (x_start >= x_end || y_start >= y_end) return;

    int16_t start_page = y_start >> 3;
    int16_t end_page = (y_end - 1) >> 3;
    uint8_t start_mask = 0xFF << (y_start & 7);
    uint8_t end_mask = 0xFF >> (7 - ((y_end - 1) & 7));

    for (int16_t page = start_page; page <= end_page; page++) {
        uint8_t mask = 0xFF;
        if (page == start_page) mask &= start_mask;
        if (page == end_page) mask &= end_mask;
        for (int16_t x = x_start; x < x_end; x++) {
            OLED_DisplayBuf[page][x] &= ~mask;
        }
    }
}



/**
 * @brief ��ɫ�Դ�����
 * @param ��
 * @return ��
 */
void OLED_Reverse(void)
{
	uint16_t i, j;
	for (j = 0; j < OLED_HEIGHT/8; j ++)				//����ҳ
	{
		for (i = 0; i < OLED_WIDTH; i ++)			//����OLED_WIDTH��
		{
			OLED_DisplayBuf[j][i] ^= 0xFF;	//���Դ���������ȫ��ȡ��
		}
	}
}



/**
 * @brief ��ɫָ������
 * @param X ָ���������Ͻǵĺ����꣬��Χ�����⣬��Ч�Զ�����
 * @param Y ָ���������Ͻǵ������꣬��Χ�����⣬��Ч�Զ�����
 * @param Width ָ������Ŀ�ȣ���Χ�����⣬��Ч�Զ�����
 * @param Height ָ������ĸ߶ȣ���Χ����������Ч�Զ�����
 * @return ��
 */
void OLED_ReverseArea(int16_t X, int16_t Y, int16_t Width, int16_t Height)
{
	int16_t i, j, x, y;
	if(Width <= 0 || Height <= 0) {return; }
	/*������飬��ָ֤�����򲻻ᳬ����Ļ��Χ*/
	if (X > OLED_WIDTH-1) {return;}
	if (Y > OLED_HEIGHT-1) {return;}
	if (X + Width > OLED_WIDTH) {Width = OLED_WIDTH - X;}
	if (Y + Height > OLED_HEIGHT) {Height = OLED_HEIGHT - Y;}
	if (X + Width < 0) {return;}
	if (Y + Height < 0) {return;}
	if (X < 0) { x = 0;} else { x = X;}
	if (Y < 0) { y = 0;} else { y = Y;}
	
	for (j = y; j < Y + Height; j ++)		//����ָ��ҳ
	{
		for (i = x; i < X + Width; i ++)	//����ָ����
		{
			OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8);	//���Դ�����ָ������ȡ��
		}
	}
}
/**
  * @brief ��ָ����������ʾͼƬ
  * @param X_Pic ͼƬ���Ͻǵĺ�����
  * @param Y_Pic ͼƬ���Ͻǵ�������
  * @param PictureWidth ͼƬ���
  * @param PictureHeight ͼƬ�߶�
  * @param X_Area ��ʾ��������Ͻǵĺ�����
  * @param Y_Area ��ʾ��������Ͻǵ�������
  * @param AreaWidth ��ʾ����Ŀ��
  * @param AreaHeight ��ʾ����ĸ߶�
  * @param Image ͼƬȡģ����
  * @note �˺���������Ҫ�������Խ�һ��ͼƬ��ʾ��ָ���������ڣ�ʵ�ָ��ӵ���ʾЧ����ΪOLED_UI����๦���ṩ������
  * @retval ��
  */
 void OLED_ShowImageArea(int16_t X_Area, int16_t Y_Area, int16_t AreaWidth, int16_t AreaHeight, int16_t X_Pic, int16_t Y_Pic, int16_t PictureWidth, int16_t PictureHeight, const uint8_t *Image)
 {
	 if (PictureWidth == 0 || PictureHeight == 0 || AreaWidth == 0 || AreaHeight == 0 || X_Pic > OLED_WIDTH-1 || X_Area > OLED_WIDTH-1 || Y_Pic > OLED_HEIGHT-1 || Y_Area > OLED_HEIGHT-1) {return; }
		  int16_t startX = (X_Pic < X_Area) ? X_Area : X_Pic;
	 int16_t endX = ((X_Area + AreaWidth - 1) < (X_Pic + PictureWidth - 1)) ? (X_Area + AreaWidth - 1) : (X_Pic + PictureWidth - 1);
	 int16_t startY = (Y_Pic < Y_Area) ? Y_Area : Y_Pic;
	 int16_t endY = ((Y_Area + AreaHeight - 1) < (Y_Pic + PictureHeight - 1)) ? (Y_Area + AreaHeight - 1) : (Y_Pic + PictureHeight - 1);
	 endX = (endX > OLED_WIDTH-1) ? OLED_WIDTH-1 : endX;
	 endY = (endY > OLED_HEIGHT-1) ? OLED_HEIGHT-1 : endY;
		 if(startX > endX || startY > endY){return;}
#if IF_CLEAR_AREA
		 OLED_ClearArea(startX, startY, endX - startX + 1, endY - startY + 1);
#endif
		 for (uint8_t j = 0; j <= (PictureHeight - 1) / 8; j++) {
		 for (uint8_t i = 0; i < PictureWidth; i++) {
			 uint8_t currX = X_Pic + i;
			 if (currX < startX || currX > endX) {continue;};
			 for (uint8_t bit = 0; bit < 8; bit++) {
				 uint8_t currY = Y_Pic + j * 8 + bit;
				 if (currY < startY || currY > endY) {continue;};
				 uint8_t page = currY / 8;
				 uint8_t bit_pos = currY % 8;
				 uint8_t data = Image[j * PictureWidth + i];
				 if (data & (1 << bit)) {OLED_DisplayBuf[page][currX] |= (1 << bit_pos); }
			 }
		 }
	 }
 }
 
 
 
/**
 * @brief ��ʾͼ��
 * @param X ָ��ͼ�����Ͻǵĺ����꣬��Χ�����⣬��Ч�Զ�����
 * @param Y ָ��ͼ�����Ͻǵ������꣬��Χ����Χ�����⣬��Ч�Զ�����
 * @param Width ָ��ͼ��Ŀ�ȣ���Χ������
 * @param Height ָ��ͼ��ĸ߶ȣ���Χ������
 * @param Image ָ��Ҫ��ʾ��ͼ��
 * @return ��
 */
void OLED_ShowImage(int16_t X, int16_t Y, uint16_t Width, uint16_t Height, const uint8_t *Image)
{
    OLED_ShowImageArea(0, 0, OLED_WIDTH, OLED_HEIGHT, X, Y, Width, Height, Image);
}

/**
  * @brief ��ָ����Χ����ʾһ���ַ�
  * @param RangeX ָ���ַ�������ʾ��Χ�����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * @param RangeY ָ���ַ�������ʾ��Χ�����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * @param RangeWidth ָ����Χ���
  * @param RangeHeight ָ����Χ�߶�
  * @param X ָ���ַ����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * @param Y ָ���ַ����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * @param Char ָ��Ҫ��ʾ���ַ�����Χ��ASCII��ɼ��ַ�
  * @param FontSize ָ�������С,OLED_6X8_HALF,OLED_7X12_HALF,OLED_8X16_HALF,OLED_10X20_HALF
  * @retval ��
  */
void OLED_ShowCharArea(int16_t RangeX, int16_t RangeY, int16_t RangeWidth, int16_t RangeHeight, 
                       int16_t X, int16_t Y, char Char, uint8_t FontSize) {
    if (Char < ' ' || Char > '~') {
        // ���������ַ����ڿɴ�ӡ��Χ�ڣ�ֱ�ӷ���
        return;
    }

    int16_t width, height;
    const uint8_t *font_data;

    switch (FontSize) {
		#ifdef OLED_FONT_8
        case OLED_FONT_8:
            width = 6;
            height = 8;
            font_data = OLED_F6x8[Char - ' '];
            break;

		#endif

		#ifdef OLED_FONT_12
        case OLED_FONT_12:
            width = 7;
            height = 12;
            font_data = OLED_F7x12[Char - ' '];
            break;
		#endif

		#ifdef OLED_FONT_16
        case OLED_FONT_16:
            width = 8;
            height = 16;
            font_data = OLED_F8x16[Char - ' '];
            break;
		#endif

		#ifdef OLED_FONT_20
        case OLED_FONT_20:
            width = 10;
            height = 20;
            font_data = OLED_F10x20[Char - ' '];
            break;
		#endif
			
    }

    OLED_ShowImageArea(RangeX, RangeY, RangeWidth, RangeHeight, X, Y, width, height, font_data);
}


 

/**
  * @brief ��ָ������Χ��OLED��ʾ���ִ�
  * @param RangeX ָ���ַ�������ʾ��Χ�����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * @param RangeY ָ���ַ�������ʾ��Χ�����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * @param RangeWidth ָ����Χ���
  * @param RangeHeight ָ����Χ�߶�
  * @param X ָ���ַ����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * @param Y ָ���ַ����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * @param Chinese ָ��Ҫ��ʾ�ĺ��ִ�����Χ������ȫ��Ϊ���ֻ���ȫ���ַ�����Ҫ�����κΰ���ַ�
  *           ��ʾ�ĺ�����Ҫ��OLED_Data.c���OLED_CF16x16��OLED_CF12x12���鶨��
  *           δ�ҵ�ָ������ʱ������ʾĬ��ͼ�Σ�һ�������ڲ�һ���ʺţ�
  * @param FontSize ָ���������ִ�С����OLED_8X8_FULL,OLED_12X12_FULL,OLED_16X16_FULL,OLED_20X20_FULL
  * @retval ��
  */


void OLED_ShowChineseArea(int16_t RangeX,int16_t RangeY,int16_t RangeWidth,int16_t RangeHeight, int16_t X, int16_t Y, char *Chinese, uint8_t FontSize)
{
    uint8_t pChinese = 0;
    uint8_t pIndex;
    uint8_t i;
    char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};
    for (i = 0; Chinese[i] != '\0'; i ++)    // �������ִ�
    {
        SingleChinese[pChinese] = Chinese[i];    // ��ȡ���ִ����ݵ�������������
        pChinese ++;                            // �ƴ�����
        
        if (pChinese >= OLED_CHN_CHAR_WIDTH)    // ��ȡ����һ�������ĺ���
        {
            pChinese = 0;    // �ƴι���
            const void* fontArray;
			switch (FontSize) {
				#ifdef OLED_FONT_8
				case OLED_FONT_8:
					fontArray = (const void*) OLED_CF8x8;
					for (pIndex = 0; strcmp(((const ChineseCell8x8_t*)fontArray)[pIndex].Index, "") != 0; pIndex++) {
						if (strcmp(((const ChineseCell8x8_t*)fontArray)[pIndex].Index, SingleChinese) == 0) break;
					}
					OLED_ShowImageArea(RangeX, RangeY, RangeWidth, RangeHeight, 
									  X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 8, Y, 8, 8, 
									  ((const ChineseCell8x8_t*)fontArray)[pIndex].Data);
					break;
				#endif
			
				#ifdef OLED_FONT_12
				case OLED_FONT_12:
					fontArray = (const void*) OLED_CF12x12;
					for (pIndex = 0; strcmp(((const ChineseCell12x12_t*)fontArray)[pIndex].Index, "") != 0; pIndex++) {
						if (strcmp(((const ChineseCell12x12_t*)fontArray)[pIndex].Index, SingleChinese) == 0) break;
					}
					OLED_ShowImageArea(RangeX, RangeY, RangeWidth, RangeHeight, 
									  X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 12, Y, 12, 12, 
									  ((const ChineseCell12x12_t*)fontArray)[pIndex].Data);
					break;
				#endif
			
				#ifdef OLED_FONT_16
				case OLED_FONT_16:
					fontArray = (const void*) OLED_CF16x16;
					for (pIndex = 0; strcmp(((const ChineseCell16x16_t*)fontArray)[pIndex].Index, "") != 0; pIndex++) {
						if (strcmp(((const ChineseCell16x16_t*)fontArray)[pIndex].Index, SingleChinese) == 0) break;
					}
					OLED_ShowImageArea(RangeX, RangeY, RangeWidth, RangeHeight, 
									  X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 16, Y, 16, 16, 
									  ((const ChineseCell16x16_t*)fontArray)[pIndex].Data);
					break;
				#endif
			
				#ifdef OLED_FONT_20
				case OLED_FONT_20:
					fontArray = (const void*) OLED_CF20x20;
					for (pIndex = 0; strcmp(((const ChineseCell20x20_t*)fontArray)[pIndex].Index, "") != 0; pIndex++) {
						if (strcmp(((const ChineseCell20x20_t*)fontArray)[pIndex].Index, SingleChinese) == 0) break;
					}
					OLED_ShowImageArea(RangeX, RangeY, RangeWidth, RangeHeight, 
									  X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 20, Y, 20, 20, 
									  ((const ChineseCell20x20_t*)fontArray)[pIndex].Data);
					break;
				#endif
			}
			
			}
		}
}



/**
  * @brief ��ָ������Χ��OLED��ʾ����ַ�����������ASCII��
  * @param RangeX ָ���ַ�������ʾ��Χ�����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * @param RangeY ָ���ַ�������ʾ��Χ�����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * @param RangeWidth ָ����Χ���
  * @param RangeHeight ָ����Χ�߶�
  * @param X ָ���ַ����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * @param Y ָ���ַ����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * @param String ָ��Ҫ��ʾ�Ļ���ַ�������Χ��ȫ���ַ������ַ�������
  *           ��ʾ�ĺ�����Ҫ��OLED_Data.c���OLED_CF16x16��OLED_CF12x12���鶨��
  *           δ�ҵ�ָ������ʱ������ʾĬ��ͼ�Σ�һ�������ڲ�һ���ʺţ�
  * @param ChineseFontSize ָ���������ִ�С��OLED_12X12��OLED_16X16��OLED_8X8
  * @param ASCIIFontSize  ָ��ASCII���ִ�С,OLED_6X8��OLED_7X12��OLED_8X16
  * @retval ��
  */

void OLED_ShowMixStringArea(int16_t RangeX, int16_t RangeY, int16_t RangeWidth, int16_t RangeHeight, 
                           int16_t X, int16_t Y, char *String, uint8_t Font)
{
    int16_t originX = X;  // ��¼����X����
    
    while (*String != '\0') {
        // �����з�
        if (*String == '\n') {
            X = originX;  // ��������
            Y += Font + OLED_LINE_SPACING;  // �ƶ�����һ��
            String++;
            
            continue;
        }

        if (*String & 0x80) {  // �����ַ�����
            char Chinese[OLED_CHN_CHAR_WIDTH + 1];
            for (uint8_t i = 0; i < OLED_CHN_CHAR_WIDTH; i++) {
                Chinese[i] = *(String + i);
            }
            Chinese[OLED_CHN_CHAR_WIDTH] = '\0';
            OLED_ShowChineseArea(RangeX, RangeY, RangeWidth, RangeHeight, X, Y, Chinese, Font);
            X += OLED_GetFontWidth(Font, OLED_CHINESE) + OLED_CHAR_SPACING;
            String += OLED_CHN_CHAR_WIDTH;
        } else {  // ASCII�ַ�����
            OLED_ShowCharArea(RangeX, RangeY, RangeWidth, RangeHeight, X, Y, *String, Font);
            X += OLED_GetFontWidth(Font, OLED_ASCII) + OLED_CHAR_SPACING;
            String++;
        }
    }
}




/**
  * ��    ����OLEDʹ��printf������ӡ��ʽ���ַ���,��������Ӣ�Ļ��ӵ��ַ������˺�����bilibili@��nm������ ���
  * ��    ����X ָ�����ִ����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * ��    ����Y ָ�����ִ����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * ��    ����ChineseFontSize ָ�����������С��OLED_8X8_FULL,OLED_12X12_FULL,OLED_16X16_FULL,OLED_20X20_FULL
  * ��    ����ASCIIFontSize ָ��ASCII�����С,OLED_6X8_HALF,OLED_7X12_HALF,OLED_8X16_HALF,OLED_10X20_HALF
  * ��    ����format ָ��Ҫ��ʾ�ĸ�ʽ���ַ�������Χ��ASCII��ɼ��ַ���ɵ��ַ���
  * ��    ����... ��ʽ���ַ��������б�
  * �� �� ֵ����
  * ˵    �������ô˺�����Ҫ�������س�������Ļ�ϣ�������ø��º���
  */

void OLED_Printf(int16_t X, int16_t Y, uint8_t Font,const char *format, ...)
{
	char String[MAX_STRING_LENGTH];						//�����ַ�����
	va_list arg;							//����ɱ�����б��������͵ı���arg
	va_start(arg, format);					//��format��ʼ�����ղ����б�arg����
	#if USE_SIMPLE_VSPRINTF
	OLED_Simple_vsprintf(String, format, arg);			//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	#else
	vsprintf(String, format, arg);			//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	#endif
	va_end(arg);							//��������arg
	OLED_ShowMixStringArea(0, 0, OLED_WIDTH, OLED_HEIGHT, X, Y, String, Font);//OLED��ʾ�ַ����飨�ַ�����
}















/**
  * @brief OLEDʹ��printf������ָ�������ڴ�ӡ��ʽ���ַ������˺�����bilibili@��nm������ ���
  * @param RangeX ָ���ַ�������ʾ��Χ�����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * @param RangeY ָ���ַ�������ʾ��Χ�����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * @param RangeWidth ָ����Χ���
  * @param RangeHeight ָ����Χ�߶�
  * @param X ָ���ַ����Ͻǵĺ����꣬��Χ����ֵ~OLED_WIDTH-1
  * @param Y ָ���ַ����Ͻǵ������꣬��Χ����ֵ~OLED_HEIGHT-1
  * @param ChineseFontSize ָ�����������С,OLED_8X8_FULL,OLED_12X12_FULL,OLED_16X16_FULL,OLED_20X20_FULL
  * @param ASCIIFontSize ָ��ASCII�����С,OLED_6X8_HALF,OLED_7X12_HALF,OLED_8X16_HALF,OLED_10X20_HALF
			format ָ��Ҫ��ʾ�ĸ�ʽ���ַ�������Χ��ASCII��ɼ��ַ���ɵ��ַ���
  * @param ... ��ʽ���ַ��������б�
  * @return ��
  */

void OLED_PrintfArea(int16_t RangeX,int16_t RangeY,int16_t RangeWidth,int16_t RangeHeight,int16_t X, int16_t Y, uint8_t Font,const char *format, ...)
{
	//�����п�����ʾ�������ַ���������128
	char String[MAX_STRING_LENGTH];						//�����ַ�����
	va_list arg;							//����ɱ�����б��������͵ı���arg
	va_start(arg, format);					//��format��ʼ�����ղ����б�arg����
	#if USE_SIMPLE_VSPRINTF
	OLED_Simple_vsprintf(String, format, arg);			//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	#else
	vsprintf(String, format, arg);			//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	#endif
	va_end(arg);							//��������arg
	OLED_ShowMixStringArea(RangeX,RangeY,RangeWidth,RangeHeight,X, Y, String, Font);//OLED��ʾ�ַ����飨�ַ�����
}






/**
 * @brief ����
 * @param X ��ĺ�����
 * @param Y ���������
 * @return ��
 */
inline void OLED_DrawPoint(int16_t X, int16_t Y) {
    if (X < 0 || Y < 0 || X >= OLED_WIDTH || Y >= OLED_HEIGHT) return;
    OLED_DisplayBuf[Y >> 3][X] |= (1 << (Y & 7));
}


/**
 * @brief ��ȡָ��λ�õ��ֵ
 * @param X ��ĺ�����
 * @param Y ���������
 * @return 1��������0��Ϩ��
 */
inline uint8_t OLED_GetPoint(int16_t X, int16_t Y) {
    if (X < 0 || Y < 0 || X >= OLED_WIDTH || Y >= OLED_HEIGHT) return 0;
    return (OLED_DisplayBuf[Y >> 3][X] >> (Y & 7)) & 1;
}




/**
 * @brief ����
 * @param X0 �ߵ���������
 * @param Y0 �ߵ����������
 * @param X1 �ߵ��յ������
 * @param Y1 �ߵ��յ�������
 * @return ��
 */
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
	int16_t x, y, dx, dy, d, incrE, incrNE, temp;
	int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
	uint8_t yflag = 0, xyflag = 0;
	
	if (y0 == y1)		//���ߵ�������
	{
		/*0�ŵ�X�������1�ŵ�X���꣬�򽻻�����X����*/
		if (x0 > x1) {temp = x0; x0 = x1; x1 = temp;}
		
		/*����X����*/
		for (x = x0; x <= x1; x ++)
		{
			OLED_DrawPoint(x, y0);	//���λ���
		}
	}
	else if (x0 == x1)	//���ߵ�������
	{
		/*0�ŵ�Y�������1�ŵ�Y���꣬�򽻻�����Y����*/
		if (y0 > y1) {temp = y0; y0 = y1; y1 = temp;}
		
		/*����Y����*/
		for (y = y0; y <= y1; y ++)
		{
			OLED_DrawPoint(x0, y);	//���λ���
		}
	}
	else				//б��
	{
		/*ʹ��Bresenham�㷨��ֱ�ߣ����Ա����ʱ�ĸ������㣬Ч�ʸ���*/
		/*�ο��ĵ���https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
		/*�ο��̳̣�https://www.bilibili.com/video/BV1364y1d7Lo*/
		
		if (x0 > x1)	//0�ŵ�X�������1�ŵ�X����
		{
			/*������������*/
			/*������Ӱ�컭�ߣ����ǻ��߷����ɵ�һ���������������ޱ�Ϊ��һ��������*/
			temp = x0; x0 = x1; x1 = temp;
			temp = y0; y0 = y1; y1 = temp;
		}
		
		if (y0 > y1)	//0�ŵ�Y�������1�ŵ�Y����
		{
			/*��Y����ȡ��*/
			/*ȡ����Ӱ�컭�ߣ����ǻ��߷����ɵ�һ�������ޱ�Ϊ��һ����*/
			y0 = -y0;
			y1 = -y1;
			
			/*�ñ�־λyflag����ס��ǰ�任���ں���ʵ�ʻ���ʱ���ٽ����껻����*/
			yflag = 1;
		}
		
		if (y1 - y0 > x1 - x0)	//����б�ʴ���1
		{
			/*��X������Y���껥��*/
			/*������Ӱ�컭�ߣ����ǻ��߷����ɵ�һ����0~90�ȷ�Χ��Ϊ��һ����0~45�ȷ�Χ*/
			temp = x0; x0 = y0; y0 = temp;
			temp = x1; x1 = y1; y1 = temp;
			
			/*�ñ�־λxyflag����ס��ǰ�任���ں���ʵ�ʻ���ʱ���ٽ����껻����*/
			xyflag = 1;
		}
		
		/*����ΪBresenham�㷨��ֱ��*/
		/*�㷨Ҫ�󣬻��߷������Ϊ��һ����0~45�ȷ�Χ*/
		dx = x1 - x0;
		dy = y1 - y0;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		d = 2 * dy - dx;
		x = x0;
		y = y0;
		
		/*����ʼ�㣬ͬʱ�жϱ�־λ�������껻����*/
		if (yflag && xyflag){OLED_DrawPoint(y, -x);}
		else if (yflag)		{OLED_DrawPoint(x, -y);}
		else if (xyflag)	{OLED_DrawPoint(y, x);}
		else				{OLED_DrawPoint(x, y);}
		
		while (x < x1)		//����X���ÿ����
		{
			x ++;
			if (d < 0)		//��һ�����ڵ�ǰ�㶫��
			{
				d += incrE;
			}
			else			//��һ�����ڵ�ǰ�㶫����
			{
				y ++;
				d += incrNE;
			}
			
			/*��ÿһ���㣬ͬʱ�жϱ�־λ�������껻����*/
			if (yflag && xyflag){OLED_DrawPoint(y, -x);}
			else if (yflag)		{OLED_DrawPoint(x, -y);}
			else if (xyflag)	{OLED_DrawPoint(y, x);}
			else				{OLED_DrawPoint(x, y);}
		}	
	}
}


/**
 * @brief ������
 * @param X �������Ͻǵĺ�����
 * @param Y �������Ͻǵ�������
 * @param Width ���εĿ��
 * @param Height ���εĸ߶�
 * @param IsFilled �Ƿ����
 * @return ��
 */

void OLED_DrawRectangle(int16_t X, int16_t Y, int16_t Width, int16_t Height, uint8_t IsFilled)
{
    if (Width == 0 || Height == 0) { return; } // ��Ȼ�߶�Ϊ0��������

    int16_t X_start = X;
    int16_t X_end = X + Width - 1;
    int16_t Y_start = Y;
    int16_t Y_end = Y + Height - 1;

    // ������������Ļ��Χ��
    if (X_start < 0) X_start = 0;
    if (X_end >= OLED_WIDTH) X_end = OLED_WIDTH - 1;
    if (Y_start < 0) Y_start = 0;
    if (Y_end >= OLED_HEIGHT) Y_end = OLED_HEIGHT - 1;

    // ������Ч��Ⱥ͸߶�
    int16_t validWidth = X_end - X_start + 1;
    int16_t validHeight = Y_end - Y_start + 1;
    if (validWidth <= 0 || validHeight <= 0) { return; }

    
    // ������ʼ�ͽ���ҳ
    int16_t start_page = Y_start / 8;
    int16_t end_page = Y_end / 8;

    // ����ÿҳ������
    uint8_t start_mask = 0xFF << (Y_start % 8);
    uint8_t end_mask = 0xFF >> (7 - (Y_end % 8));

    
		if(IsFilled){
			// ����ÿһ�У�Ӧ������
			for (int16_t x = X_start; x <= X_end; x++) {
					for (int16_t page = start_page; page <= end_page; page++) {
							uint8_t mask = 0xFF;
							if (page == start_page) mask &= start_mask;
							if (page == end_page) mask &= end_mask;
							if (page >= 0 && page < OLED_HEIGHT / 8) { // ȷ��ҳ����Ч
									OLED_DisplayBuf[page][x] |= mask;
							}
					}
			}
		}else{
			// ���ƾ��α߿�
        for (int16_t i = X; i <= X + Width - 1; i++) {
            OLED_DrawPoint(i, Y);
            OLED_DrawPoint(i, Y + Height - 1);
        }
        for (int16_t i = Y; i <= Y + Height - 1; i++) {
            OLED_DrawPoint(X, i);
            OLED_DrawPoint(X + Width - 1, i);
        }
		}
		
		
   
}

/**
 * @brief ��������
 * @param X0 ��һ���˵�ĺ�����
 * @param Y0 ��һ���˵��������
 * @param X1 �ڶ����˵�ĺ�����
 * @param Y1 �ڶ����˵��������
 * @param X2 �������˵�ĺ�����
 * @param Y2 �������˵��������
 * @param IsFilled �Ƿ����
 * @return ��
 */
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled)
{
	int16_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
	int16_t i, j;
	int16_t vx[] = {X0, X1, X2};
	int16_t vy[] = {Y0, Y1, Y2};
	
	if (!IsFilled)			//ָ�������β����
	{
		/*���û��ߺ���������������ֱ������*/
		OLED_DrawLine(X0, Y0, X1, Y1);
		OLED_DrawLine(X0, Y0, X2, Y2);
		OLED_DrawLine(X1, Y1, X2, Y2);
	}
	else					//ָ�����������
	{
		/*�ҵ���������С��X��Y����*/
		if (X1 < minx) {minx = X1;}
		if (X2 < minx) {minx = X2;}
		if (Y1 < miny) {miny = Y1;}
		if (Y2 < miny) {miny = Y2;}
		
		/*�ҵ�����������X��Y����*/
		if (X1 > maxx) {maxx = X1;}
		if (X2 > maxx) {maxx = X2;}
		if (Y1 > maxy) {maxy = Y1;}
		if (Y2 > maxy) {maxy = Y2;}
		
		/*��С�������֮��ľ���Ϊ������Ҫ��������*/
		/*���������������еĵ�*/
		/*����X����*/
		for (i = minx; i <= maxx; i ++)
		{
			/*����Y����*/
			for (j = miny; j <= maxy; j ++)
			{
				/*����OLED_pnpoly���ж�ָ�����Ƿ���ָ��������֮��*/
				/*����ڣ��򻭵㣬������ڣ���������*/
				if (OLED_pnpoly(3, vx, vy, i, j)) {OLED_DrawPoint(i, j);}
			}
		}
	}
}

/**
 * @brief ��Բ
 * @param X Բ�ĵĺ�����
 * @param Y Բ�ĵ�������
 * @param Radius Բ�İ뾶
 * @param IsFilled �Ƿ����
 * @return ��
 */
void OLED_DrawCircle(int16_t X, int16_t Y, int16_t Radius, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	/*ʹ��Bresenham�㷨��Բ�����Ա����ʱ�ĸ������㣬Ч�ʸ���*/
	/*�ο��ĵ���https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
	/*�ο��̳̣�https://www.bilibili.com/video/BV1VM4y1u7wJ*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*��ÿ���˷�֮һԲ������ʼ��*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X + y, Y + x);
	OLED_DrawPoint(X - y, Y - x);
	
	if (IsFilled)		//ָ��Բ���
	{
		/*������ʼ��Y����*/
		for (j = -y; j < y; j ++)
		{
			/*��ָ�����򻭵㣬��䲿��Բ*/
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	while (x < y)		//����X���ÿ����
	{
		x ++;
		if (d < 0)		//��һ�����ڵ�ǰ�㶫��
		{
			d += 2 * x + 1;
		}
		else			//��һ�����ڵ�ǰ�㶫�Ϸ�
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*��ÿ���˷�֮һԲ���ĵ�*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X + y, Y + x);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - y, Y - x);
		OLED_DrawPoint(X + x, Y - y);
		OLED_DrawPoint(X + y, Y - x);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X - y, Y + x);
		
		if (IsFilled)	//ָ��Բ���
		{
			/*�����м䲿��*/
			for (j = -y; j < y; j ++)
			{
				/*��ָ�����򻭵㣬��䲿��Բ*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
			
			/*�������ಿ��*/
			for (j = -x; j < x; j ++)
			{
				/*��ָ�����򻭵㣬��䲿��Բ*/
				OLED_DrawPoint(X - y, Y + j);
				OLED_DrawPoint(X + y, Y + j);
			}
		}
	}
}


/**
 * @brief ����Բ
 * @param X Բ�ĵĺ�����
 * @param Y Բ�ĵ�������
 * @param A ��Բ�ĺ�����᳤��	
 * @param B ��Բ��������᳤��
 * @param IsFilled �Ƿ����
 * @return ��
 */
void OLED_DrawEllipse(int16_t X, int16_t Y, int16_t A, int16_t B, uint8_t IsFilled)
{
	int16_t x, y, j;
	int16_t a = A, b = B;
	float d1, d2;
	
	/*ʹ��Bresenham�㷨����Բ�����Ա��ⲿ�ֺ�ʱ�ĸ������㣬Ч�ʸ���*/
	/*�ο����ӣ�https://blog.csdn.net/myf_666/article/details/128167392*/
	
	x = 0;
	y = b;
	d1 = b * b + a * a * (-b + 0.5);
	
	if (IsFilled)	//ָ����Բ���
	{
		/*������ʼ��Y����*/
		for (j = -y; j < y; j ++)
		{
			/*��ָ�����򻭵㣬��䲿����Բ*/
			OLED_DrawPoint(X, Y + j);
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	/*����Բ������ʼ��*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X - x, Y + y);
	OLED_DrawPoint(X + x, Y - y);
	
	/*����Բ�м䲿��*/
	while (b * b * (x + 1) < a * a * (y - 0.5))
	{
		if (d1 <= 0)		//��һ�����ڵ�ǰ�㶫��
		{
			d1 += b * b * (2 * x + 3);
		}
		else				//��һ�����ڵ�ǰ�㶫�Ϸ�
		{
			d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
			y --;
		}
		x ++;
		
		if (IsFilled)	//ָ����Բ���
		{
			/*�����м䲿��*/
			for (j = -y; j < y; j ++)
			{
				/*��ָ�����򻭵㣬��䲿����Բ*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*����Բ�м䲿��Բ��*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
	
	/*����Բ���ಿ��*/
	d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
	
	while (y > 0)
	{
		if (d2 <= 0)		//��һ�����ڵ�ǰ�㶫��
		{
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x ++;
			
		}
		else				//��һ�����ڵ�ǰ�㶫�Ϸ�
		{
			d2 += a * a * (-2 * y + 3);
		}
		y --;
		
		if (IsFilled)	//ָ����Բ���
		{
			/*�������ಿ��*/
			for (j = -y; j < y; j ++)
			{
				/*��ָ�����򻭵㣬��䲿����Բ*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*����Բ���ಿ��Բ��*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
}


/**
 * @brief ��Բ��
 * @param X Բ�ĵĺ�����
 * @param Y Բ�ĵ�������
 * @param Radius Բ���İ뾶
 * @param StartAngle Բ������ʼ�Ƕ�
 * @param EndAngle Բ������ֹ�Ƕ�
 * @param IsFilled �Ƿ����
 * @return ��
 */
void OLED_DrawArc(int16_t X, int16_t Y, int16_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	if(Radius <=0){return;} //�뾶Ϊ0��ֱ�ӷ���
	/*�˺�������Bresenham�㷨��Բ�ķ���*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*�ڻ�Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
	if (OLED_IsInAngle(x, y, StartAngle, EndAngle))	{OLED_DrawPoint(X + x, Y + y);}
	if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
	if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
	if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
	
	if (IsFilled)	//ָ��Բ�����
	{
		/*������ʼ��Y����*/
		for (j = -y; j < y; j ++)
		{
			/*�����Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
			if (OLED_IsInAngle(0, j, StartAngle, EndAngle)) {OLED_DrawPoint(X, Y + j);}
		}
	}
	
	while (x < y)		//����X���ÿ����
	{
		x ++;
		if (d < 0)		//��һ�����ڵ�ǰ�㶫��
		{
			d += 2 * x + 1;
		}
		else			//��һ�����ڵ�ǰ�㶫�Ϸ�
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*�ڻ�Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
		if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + y);}
		if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
		if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
		if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
		if (OLED_IsInAngle(x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y - y);}
		if (OLED_IsInAngle(y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y - x);}
		if (OLED_IsInAngle(-x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + y);}
		if (OLED_IsInAngle(-y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + x);}
		
		if (IsFilled)	//ָ��Բ�����
		{
			/*�����м䲿��*/
			for (j = -y; j < y; j ++)
			{
				/*�����Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
				if (OLED_IsInAngle(x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + j);}
				if (OLED_IsInAngle(-x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + j);}
			}
			
			/*�������ಿ��*/
			for (j = -x; j < x; j ++)
			{
				/*�����Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
				if (OLED_IsInAngle(-y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + j);}
				if (OLED_IsInAngle(y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + j);}
			}
		}
	}
}



/**
 * @brief ��Բ�Ǿ���
 * @param X �������Ͻǵĺ�����
 * @param Y �������Ͻǵ�������
 * @param Width ���εĿ��
 * @param Height ���εĸ߶�
 * @param Radius Բ�ǰ뾶
 * @param IsFilled �Ƿ����
 * @return ��
 */
void OLED_DrawRoundedRectangle(int16_t X, int16_t Y, int16_t Width, int16_t Height, int16_t Radius, uint8_t IsFilled)
{
    // ��ȫ���
    if (Width == 0 || Height == 0) return;  // ��Ȼ�߶�Ϊ0,ֱ�ӷ���
    if (Radius > Width / 2 || Radius > Height / 2) {
        Radius = (Width < Height ? Width : Height) / 2;  // ����Բ�ǰ뾶��������ߵ�һ��
    }

    if (Radius <= 0) {
        OLED_DrawRectangle(X, Y, Width, Height, IsFilled);  // ����뾶Ϊ0,�˻�Ϊ��ͨ����
        return;
    }
    else if(Radius == 1 || Radius == 2) {	// ����뾶Ϊ1��2��Բ�ǿ��Լ�Ϊ�����㣬�Ż�����
    	OLED_DrawPoint(X + 1, Y);
    	OLED_DrawPoint(X + Width - 2, Y);

    	OLED_DrawPoint(X + 1, Y + Height - 1);
    	OLED_DrawPoint(X + Width - 2, Y + Height - 1);

    	OLED_DrawPoint(X, Y + 1);
    	OLED_DrawPoint(X, Y + Height - 2);

    	OLED_DrawPoint(X + Width - 1, Y + 1);
    	OLED_DrawPoint(X + Width - 1, Y + Height - 2);
    }
    else {
		// �����ĸ�Բ��
		OLED_DrawArc(X + Radius, Y + Radius, Radius,180,  -90, IsFilled);
		OLED_DrawArc(X + Width - Radius - 1, Y + Radius, Radius,-90,  0, IsFilled);
		OLED_DrawArc(X + Radius, Y + Height - Radius - 1, Radius,90,  180, IsFilled);
		OLED_DrawArc(X + Width - Radius - 1, Y + Height - Radius - 1, Radius,0,  90, IsFilled);
    }

    // ������ƾ�������
    if (IsFilled) {
        OLED_DrawRectangle(X + Radius, Y, Width - 2 * Radius, Height, OLED_FILLED);
        OLED_DrawRectangle(X, Y + Radius, Width, Height - 2 * Radius, OLED_FILLED);
    } else {
        // ���ƶ����͵ײ���ֱ��
        OLED_DrawLine(X + Radius, Y, X + Width - Radius - 1, Y);
        OLED_DrawLine(X + Radius, Y + Height - 1, X + Width - Radius - 1, Y + Height - 1);
        // ���������Ҳ��ֱ��
        OLED_DrawLine(X, Y + Radius, X, Y + Height - Radius - 1);
        OLED_DrawLine(X + Width - 1, Y + Radius, X + Width - 1, Y + Height - Radius - 1);
    }
}


/** 
 * @brief����ָ������Ӧ��ģʽ������Ч��(�ɰ������)
 * @param x0 ������ʼX����
 * @param y0 ������ʼY����
 * @param width ������
 * @param height ����߶�
 * @param fadeLevel ������λ��1��5֮���ֵ����ʾ��ͬ������Ϩ��ģʽ
 * @note �ú���������ָ������Ӧ��ģʽ������Ч����ʹ�ø�����������𽥱䰵��Ч�������ɰ��������
*/
void OLED_FadeOut_Masking(int16_t x0, int16_t y0, int16_t width, int16_t height, int8_t fadeLevel) {
    // ��鲢��������Χ
    if (x0 < 0) {
        width += x0;
        x0 = 0;
    }
    if (y0 < 0) {
        height += y0;
        y0 = 0;
    }
    if (x0 + width > OLED_WIDTH) {
        width = OLED_WIDTH - x0;
    }
    if (y0 + height > OLED_HEIGHT) {
        height = OLED_HEIGHT - y0;
    }
    if (width <= 0 || height <= 0) {
        return;
    }

    // ȷ��������λ����Ч��Χ��
    if (fadeLevel < 1 || fadeLevel > 5) {
        return;
    }

    // ����2x2����Ľ���ģʽ
    // ÿ�������ʾһ��2x2��������Щ������ҪϨ��
    // 0��ʾ��������1��ʾϨ��
    const uint8_t patterns[5][2][2] = {
        {{0, 0},  // Level 1: ȫ��
         {0, 0}},
        
        {{1, 0},  // Level 2: ���Ͻ�Ϩ��
         {0, 0}},
        
        {{1, 0},  // Level 3: ���ϽǺ����½�Ϩ��
         {0, 1}},
        
        {{1, 0},  // Level 4: ֻ�������Ͻ�
         {1, 1}},
        
        {{1, 1},  // Level 5: ȫ��
         {1, 1}}
    };

    // ����߽�
    int16_t xEnd = x0 + width;
    int16_t yEnd = y0 + height;

    // Ӧ�ý���Ч��
    for (int16_t y = y0; y < yEnd; y++) {
        int page = y / 8;
        int bit_pos = y % 8;
        uint8_t pixel_mask = 1 << bit_pos;
        
        for (int16_t x = x0; x < xEnd; x++) {
            // ������2x2�����е����λ��
            int grid_x = (x - x0) % 2;
            int grid_y = (y - y0) % 2;
            
            // ���ݽ������������λ�þ����Ƿ�Ϩ������
            if (patterns[fadeLevel - 1][grid_y][grid_x]) {
                OLED_DisplayBuf[page][x] &= ~pixel_mask;
            }
        }
    }
}




