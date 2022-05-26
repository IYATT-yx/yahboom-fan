/**
 * @brief 字符和字符串显示
 * 
 * @Copyright (C) 2022 IYATT-yx (Zhao Hongfei, 赵洪飞)，2514374431@qq.com
 */

#include "ssd1306_i2c.h"
#include "wiringPi.h"


int main(void)
{
    ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // 初始化
    ssd1306_clearDisplay();  // 清空缓存
    
    ssd1306_drawString("Hello!");  // 向缓存中写字符
    ssd1306_display();  // 刷写缓存到屏幕
    delay(1000);  // wiringPi 库中的延时函数，单位为毫秒

    ssd1306_setTextSize(2);  // 修改字体大小

    ssd1306_drawString("(^-^)");
    ssd1306_display();
    delay(1000);

    ssd1306_clearDisplay();
    ssd1306_drawChar(50, 20, 'A', 1, 2);
    ssd1306_display();
    delay(1000);

    ssd1306_dim(1);  // 息屏
    delay(1000);
    ssd1306_dim(0);  // 亮屏

    ssd1306_invertDisplay(1);  // 亮底黑显
    delay(1000);
    ssd1306_invertDisplay(0);  // 黑底亮显
}