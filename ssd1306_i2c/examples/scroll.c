/**
 * @brief 滚动
 * 
 * @Copyright (C) 2022 IYATT-yx (Zhao Hongfei, 赵洪飞)，2514374431@qq.com
 */

#include "ssd1306_i2c.h"
#include "wiringPi.h"


int main(void)
{
    ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // 初始化
    ssd1306_clearDisplay();  // 清空缓存

    const char *str = "Hello Word!\n1234567890123456\n!@#$%^&*()_+\nabcdefghijklmn";
    ssd1306_drawString(str);  // 写字符串
    ssd1306_display();  // 刷新屏幕
    delay(2000);  // wiringPi 延时函数，单位为毫秒

    ssd1306_startscrollright(0, 0);  // 第一行右滚动
    delay(2000);
    ssd1306_stopscroll();  // 停止滚动

    ssd1306_clearDisplay();
    ssd1306_drawString(str);

    ssd1306_startscrollleft(0, 2); // 第 1～3 行左滚动
    delay(2000);
    ssd1306_stopscroll();

    ssd1306_clearDisplay();
    ssd1306_drawString(str);

    ssd1306_startscrolldiagright(0, 1);  // 第 1～2 行右滚动，其它行上滚动
    delay(2000);
    ssd1306_stopscroll();

    ssd1306_clearDisplay();
    ssd1306_drawString(str);

    ssd1306_startscrolldiagleft(0, 0);  // 第一行左滚动，其它行上滚动
}