/*********************************************************************
SSD1306 I2C Library for Raspberry Pi.
Based on Adafruit SSD1306 Arduino library. Some functions came from Adafruit GFX lib. 

Modified by Ilia Penev
Tested on Raspberry Pi 2 with 0.96 Yellow/Blue OLED
*********************************************************************/

/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#ifndef SSD1306_I2C_H_
#define SSD1306_I2C_H_

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1306_I2C_ADDRESS   0x3C	// 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

/*=========================================================================
    SSD1306 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1306_128_64  128x64 pixel display

    SSD1306_128_32  128x32 pixel display

    SSD1306_96_16

    -----------------------------------------------------------------------*/
// 分辨率设置
// #define SSD1306_128_64
  #define SSD1306_128_32
//   #define SSD1306_96_16
/*=========================================================================*/

#if defined SSD1306_128_32
        #define WIDTH 128
        #define HEIGHT 32
#endif

#if defined SSD1306_128_64
        #define WIDTH 128
        #define HEIGHT 64
#endif

#if defined SSD1306_96_16
        #define WIDTH 96
        #define HEIGHT 16
#endif

#if defined SSD1306_128_64 && defined SSD1306_128_32
        #error "Only one SSD1306 display can be specified at once in SSD1306.h"
#endif
#if !defined SSD1306_128_64 && !defined SSD1306_128_32 && !defined SSD1306_96_16
        #error "At least one SSD1306 display must be specified in SSD1306.h"
#endif

#if defined SSD1306_128_64
        #define SSD1306_LCDWIDTH                  128
        #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
        #define SSD1306_LCDWIDTH                  128
        #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
        #define SSD1306_LCDWIDTH                  96
        #define SSD1306_LCDHEIGHT                 16
#endif

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

/**
 * 使用需要引用本头文件 
 * 
 * 首先需要初始化 ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
 * 所有对屏幕的操作实际上是对一个缓存数组的操作，操作后需要使用 ssd1306_display(); 刷新到屏幕
 * 
 * 屏幕坐标以左上角为原点，从左往右为 x 方向，从上往下为 y 方向
 * 
 */

/**
 * @brief 初始化
 * 
 * @param switchvcc 填 SSD1306_SWITCHCAPVCC
 * @param i2caddr 填 SSD1306_I2C_ADDRESS
 */
void ssd1306_begin(unsigned int switchvcc, unsigned int i2caddr);


void ssd1306_command(unsigned int c);


/**
 * @brief 清空缓存
 * 
 */
void ssd1306_clearDisplay(void);


/**
 * @brief 背景反转
 * 
 * @param i 置为 1 变成亮背景，黑色字符; 置为 0 变成黑背景，亮字符。默认为后者。
 */
void ssd1306_invertDisplay(unsigned int i);


/**
 * @brief 将缓存刷写到屏幕
 * 
 */
void ssd1306_display();


/**
 * @brief 右滚动，开始行和结束行之间的行数滚动，闭区间
 * 
 * @param start 开始行，第一行为 0
 * @param stop 结束行
 */
void ssd1306_startscrollright(unsigned int start, unsigned int stop);


/**
 * @brief 左滚动，开始行和结束行之间的行数滚动，闭区间
 * 
 * @param start 开始行，第一行为 0
 * @param stop 结束行
 */
void ssd1306_startscrollleft(unsigned int start, unsigned int stop);


/**
 * @brief 指定行区间右滚动（闭区间），其它行上滚动
 * 
 * @param start 开始行，第一行为 0
 * @param stop 结束行
 */
void ssd1306_startscrolldiagright(unsigned int start, unsigned int stop);


/**
 * @brief 指定行区间左滚动（闭区间），其它行上滚动
 * 
 * @param start 开始行，第一行为 0
 * @param stop 结束行
 */
void ssd1306_startscrolldiagleft(unsigned int start, unsigned int stop);


/**
 * @brief 停止滚动
 * 
 */
void ssd1306_stopscroll(void);


/**
 * @brief 息屏
 * 
 * @param dim 1 息屏; 0 恢复亮屏
 */
void ssd1306_dim(unsigned int dim);

/**
 * @brief 对缓存点位显示的操作 - 最底层屏幕显示实现
 * 
 * @param x 
 * @param y 
 * @param color 亮点与暗点
 */
void ssd1306_drawPixel(int x, int y, unsigned int color);


/**
 * @brief 纵向线绘制
 * 
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param h 直线长度
 * @param color 置 1 显示
 */
void ssd1306_drawFastVLine(int x, int y, int h, unsigned int color);


/**
 * @brief 水平线绘制
 * 
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param w 直线长度
 * @param color 置 1 显示
 */
void ssd1306_drawFastHLine(int x, int y, int w, unsigned int color);


/**
 * @brief 矩形绘制
 * 
 * @param x 左上角横坐标
 * @param y 左上角纵坐标
 * @param w 宽度
 * @param h 长度
 * @param fillcolor 置 1 可见
 */
void ssd1306_fillRect(int x, int y, int w, int h, int fillcolor);


/**
 * @brief 修改字体大小
 * 
 * @param s 字体大小值
 */
void ssd1306_setTextSize(int s);


/**
 * @brief 写字符串 - 基于“写字符”的封装
 * 
 * @param str 要写入的内容（不支持中文）
 */
void ssd1306_drawString(const char *str);


/**
 * @brief 写字符
 * 
 * @param x 左上角横坐标
 * @param y 左上角纵坐标
 * @param c 要写入的字符（不支持中文）
 * @param color 置 1 显示
 * @param size 字体大小
 */
void ssd1306_drawChar(int x, int y, unsigned char c, int color, int size);

#endif				/* _SSD1306_I2C_H_ */
