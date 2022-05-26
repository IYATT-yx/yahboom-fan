/**
 * 简单图形
 * 
 * @Copyright (C) 2022 IYATT-yx (Zhao Hongfei, 赵洪飞)，2514374431@qq.com
 * 
 */

#include "ssd1306_i2c.h"


int main(void)
{
    ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // 初始化
    ssd1306_clearDisplay();  // 清空缓存

    ssd1306_drawFastHLine(10, 10, 50, 1);  // 水平线绘制

    ssd1306_drawFastVLine(20, 0, 24, 1);  // 纵向线绘制

    ssd1306_fillRect(30, 12, 60, 15, 1);  // 矩形绘制

    ssd1306_display();  // 将缓存刷写到屏幕
}