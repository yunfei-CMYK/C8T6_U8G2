#include "oled.h"
#include "oledfont.h"

uint8_t OLED_GRAM[144][8];

//反显函数
void OLED_ColorTurn(uint8_t i) {
    if (i == 0) {
        OLED_WR_Byte(0xA6, OLED_CMD);//正常显示
    }
    if (i == 1) {
        OLED_WR_Byte(0xA7, OLED_CMD);//反色显示
    }
}

//屏幕旋转180度
void OLED_DisplayTurn(uint8_t i) {
    if (i == 0) {
        OLED_WR_Byte(0xC8, OLED_CMD);//正常显示
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1) {
        OLED_WR_Byte(0xC0, OLED_CMD);//反转显示
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

void OLED_WR_Byte(uint8_t dat, uint8_t cmd) {
//    if (cmd) {
//        OLED_DC_Set();
//    } else {
//        OLED_DC_Clr();
//        OLED_CS_Clr();
//        HAL_SPI_Transmit(&hspi1, &dat, 1, 1000);
//        OLED_CS_Set();
//        OLED_DC_Set();
//    }
    uint8_t buffer[1];
    buffer[0]=dat;
    if(cmd==OLED_CMD)
    {
        OLED_DC_Clr();
        HAL_SPI_Transmit(&hspi1,buffer,1,0xffff);
    }
    else if(cmd==OLED_DATA)
    {
        OLED_DC_Set();
        HAL_SPI_Transmit(&hspi1,buffer,1,0xffff);
    }

}

//开启OLED显示
void OLED_DisPlay_On(void) {
    OLED_WR_Byte(0x8D, OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x14, OLED_CMD);//开启电荷泵
    OLED_WR_Byte(0xAF, OLED_CMD);//点亮屏幕
}

//关闭OLED显示
void OLED_DisPlay_Off(void) {
    OLED_WR_Byte(0x8D, OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x10, OLED_CMD);//关闭电荷泵
    OLED_WR_Byte(0xAE, OLED_CMD);//关闭屏幕
}

//更新显存到OLED
void OLED_Refresh(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00, OLED_CMD);   //设置低列起始地址
        OLED_WR_Byte(0x10, OLED_CMD);   //设置高列起始地址
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
    }
}

//清屏函数
void OLED_Clear(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        for (n = 0; n < 128; n++) {
            OLED_GRAM[n][i] = 0;//清除所有数据
        }
    }
    OLED_Refresh();//更新显示
}

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t) {
    uint8_t i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t) { OLED_GRAM[x][i] |= n; }
    else {
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
    OLED_Refresh();
}

//画线
//x1,y1:起点坐标
//x2,y2:结束坐标
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode) {
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;//画线起点坐标
    uCol = y1;
    if (delta_x > 0)incx = 1; //设置单步方向
    else if (delta_x == 0)incx = 0;//垂直线
    else {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0;//水平线
    else {
        incy = -1;
        delta_y = -delta_x;
    }
    if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
    else distance = delta_y;
    for (t = 0; t < distance + 1; t++) {
        OLED_DrawPoint(uRow, uCol, mode);//画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
    OLED_Refresh();
}

//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r) {
    int a, b, num;
    a = 0;
    b = r;
    while (2 * b * b >= r * r) {
        OLED_DrawPoint(x + a, y - b, 1);
        OLED_DrawPoint(x - a, y - b, 1);
        OLED_DrawPoint(x - a, y + b, 1);
        OLED_DrawPoint(x + a, y + b, 1);

        OLED_DrawPoint(x + b, y + a, 1);
        OLED_DrawPoint(x + b, y - a, 1);
        OLED_DrawPoint(x - b, y - a, 1);
        OLED_DrawPoint(x - b, y + a, 1);

        a++;
        num = (a * a + b * b) - r * r;//计算画的点离圆心的距离
        if (num > 0) {
            b--;
            a--;
        }
    }
    OLED_Refresh();
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode) {
    uint8_t i, m, temp, size2, chr1;
    uint8_t x0 = x, y0 = y;
    if (size1 == 8)size2 = 6;
    else size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);  //得到字体一个字符对应点阵集所占的字节数
    chr1 = chr - ' ';  //计算偏移后的值
    for (i = 0; i < size2; i++) {
        if (size1 == 8) { temp = asc2_0806[chr1][i]; } //调用0806字体
        else if (size1 == 12) { temp = asc2_1206[chr1][i]; } //调用1206字体
        else if (size1 == 16) { temp = asc2_1608[chr1][i]; } //调用1608字体
        else if (size1 == 24) { temp = asc2_2412[chr1][i]; } //调用2412字体
        else return;
        for (m = 0; m < 8; m++) {
            if (temp & 0x01)OLED_DrawPoint(x, y, mode);
            else OLED_DrawPoint(x, y, !mode);
            temp >>= 1;
            y++;
        }
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2)) {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
    OLED_Refresh();
}


//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
//mode:0,反色显示;1,正常显示
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1, uint8_t mode) {
    while ((*chr >= ' ') && (*chr <= '~'))//判断是不是非法字符!
    {
        OLED_ShowChar(x, y, *chr, size1, mode);
        if (size1 == 8)x += 6;
        else x += size1 / 2;
        chr++;
    }
    OLED_Refresh();
}

//m^n
uint32_t OLED_Pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--) {
        result *= m;
    }
    return result;
    OLED_Refresh();
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1, uint8_t mode) {
    uint8_t t, temp, m = 0;
    if (size1 == 8)m = 2;
    for (t = 0; t < len; t++) {
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if (temp == 0) {
            OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
        } else {
            OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
        }
    }
    OLED_Refresh();
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//mode:0,反色显示;1,正常显示
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1, uint8_t mode) {
    uint8_t m, temp;
    uint8_t x0 = x, y0 = y;
    uint16_t i, size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1;  //得到字体一个字符对应点阵集所占的字节数
    for (i = 0; i < size3; i++) {
        if (size1 == 16) { temp = Hzk1[num][i]; }//调用16*16字体
        else if (size1 == 24) { temp = Hzk2[num][i]; }//调用24*24字体
        else if (size1 == 32) { temp = Hzk3[num][i]; }//调用32*32字体
        else if (size1 == 64) { temp = Hzk4[num][i]; }//调用64*64字体
        else return;
        for (m = 0; m < 8; m++) {
            if (temp & 0x01)OLED_DrawPoint(x, y, mode);
            else OLED_DrawPoint(x, y, !mode);
            temp >>= 1;
            y++;
        }
        x++;
        if ((x - x0) == size1) {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
    OLED_Refresh();
}

//num 显示汉字的个数
//space 每一遍显示的间隔
//mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(uint8_t num, uint8_t space, uint8_t mode) {
    uint8_t i, n, t = 0, m = 0, r;
    while (1) {
        if (m == 0) {
            OLED_ShowChinese(128, 24, t, 16, mode); //写入一个汉字保存在OLED_GRAM[][]数组中
            t++;
        }
        if (t == num) {
            for (r = 0; r < 16 * space; r++)      //显示间隔
            {
                for (i = 1; i < 144; i++) {
                    for (n = 0; n < 8; n++) {
                        OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
                    }
                }
                OLED_Refresh();
            }
            t = 0;
        }
        m++;
        if (m == 16) { m = 0; }
        for (i = 1; i < 144; i++)   //实现左移
        {
            for (n = 0; n < 8; n++) {
                OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
            }
        }
        OLED_Refresh();
    }
    OLED_Refresh();
}

//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode) {
    uint16_t j = 0;
    uint8_t i, n, temp, m;
    uint8_t x0 = x, y0 = y;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (n = 0; n < sizey; n++) {
        for (i = 0; i < sizex; i++) {
            temp = BMP[j];
            j++;
            for (m = 0; m < 8; m++) {
                if (temp & 0x01)OLED_DrawPoint(x, y, mode);
                else OLED_DrawPoint(x, y, !mode);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == sizex) {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
    OLED_Refresh();
}

//OLED的初始化
void OLED_Init(void) {
    OLED_RES_Clr();
    HAL_Delay(200);
    OLED_RES_Set();

    OLED_WR_Byte(0xAE, OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00, OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10, OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40, OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD);// Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1, OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8, OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6, OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40, OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20, OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD);//
    OLED_WR_Byte(0x8D, OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14, OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4, OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_Clear();
    OLED_WR_Byte(0xAF,OLED_CMD);
}

/*----------------------u8g2----------------------*/
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND: /*通过SPI发送arg_int个字节数据*/
//           HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)arg_ptr, arg_int);while(hspi1.TxXferCount);
            /*配置了DMA取消上一行注释即可*/
            HAL_SPI_Transmit(&hspi1,(uint8_t *)arg_ptr,arg_int,200);
            /*这是CubeMX生成的初始化*/
            break;
        case U8X8_MSG_BYTE_INIT: /*初始化函数*/
            break;
        case U8X8_MSG_BYTE_SET_DC: /*设置DC引脚,表明发送的是数据还是命令*/
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,arg_int);
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            break;
        default:
            return 0;
    }
    return 1;
}

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: /*delay和GPIO的初始化，在main中已经初始化完成了*/
            break;
        case U8X8_MSG_DELAY_MILLI: /*延时函数*/
            HAL_Delay(arg_int);     //调用谁stm32系统延时函数
            break;
        case U8X8_MSG_GPIO_CS: /*片选信号*/ //由于只有一个SPI设备，所以片选信号在初始化时已经设置为常有效
            HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, arg_int);
            break;
        case U8X8_MSG_GPIO_DC: /*设置DC引脚,表明发送的是数据还是命令*/
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,arg_int);
            break;
        case U8X8_MSG_GPIO_RESET:
            break;
    }
    return 1;
}

void u8g2Init(u8g2_t *u8g2)
{
/********************************************
U8G2_R0     //不旋转，不镜像
U8G2_R1     //旋转90度
U8G2_R2     //旋转180度
U8G2_R3     //旋转270度
U8G2_MIRROR   //没有旋转，横向显示左右镜像
U8G2_MIRROR_VERTICAL    //没有旋转，竖向显示镜像
********************************************/
    OLED_RES_Set();
//    u8g2_Setup_sh1106_128x64_noname_2(u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);  // 初始化1.3寸OLED u8g2 结构体
    u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);  // 初始化0.96寸OLED u8g2 结构体
    u8g2_InitDisplay(u8g2);     //初始化显示
    u8g2_SetPowerSave(u8g2, 0); //开启显示
}
/*官方logo的Demo*/
void draw(u8g2_t *u8g2)
{
    u8g2_SetFontMode(u8g2, 1); /*字体模式选择*/
    u8g2_SetFontDirection(u8g2, 0); /*字体方向选择*/
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*字库选择*/
    u8g2_DrawStr(u8g2, 0, 20, "U");

    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");

    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");

    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);

    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
}
/********************************* end_of_file **********************************/


