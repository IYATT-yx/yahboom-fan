/**
 * @file main.c
 * @author IYATT-yx
 * @brief RGB Cooling HAT 控制程序
 * @version 0.1
 * @date 2022-05-26
 * 
    Copyright (C) 2022 IYATT-yx iyatt@iyatt.com
    每个人都可以复制和分发本许可证文档的逐字副本，但不允许更改。
    这个程序是自由软件：你可以在自由软件基金会发布的GNU Affro通用公共许可证的条款下重新分发它和/或修改它，
    或者许可证的第3版，或者（在你的选择）任何其他版本。
    本程序的发布目的是希望它有用，但不提供任何担保；甚至没有对适销性或特定用途适用性的默示保证。
    有关更多详细信息，请参阅GNU Affero通用公共许可证。
    您应该已经收到GNU Affero通用公共许可证的副本以及本程序。如果没有，请参阅<https://www.gnu.org/licenses/>.
    Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.
    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "wiringPi.h"
#include "ssd1306_i2c.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/vfs.h>
#include <sys/sysinfo.h>

long g_pre_idle, g_pre_total, g_user, g_nice, g_sys, g_idle, g_iowait, g_irq, g_softirq, g_total;
char g_cpu_stat_string[64];
/**
 * @brief 获取 CPU 使用率
 * 
 * @return int CPU 使用率数值
 */
int get_cpu_usage()
{
    FILE *cpu_stat_file = fopen("/proc/stat", "r");
    memset(g_cpu_stat_string, '\0', sizeof(g_cpu_stat_string));
    fread(g_cpu_stat_string, sizeof(char), sizeof(g_cpu_stat_string), cpu_stat_file);
    fclose(cpu_stat_file);

    sscanf(g_cpu_stat_string, "%*s %ld %ld %ld %ld %ld %ld %ld", &g_user, &g_nice, &g_sys, &g_idle, &g_iowait, &g_irq, &g_softirq);
    g_total = g_user + g_nice + g_sys + g_idle + g_iowait + g_irq + g_softirq;
    int usage = (int)((double)(g_total - g_pre_total - (g_idle - g_pre_idle)) / (double)(g_total - g_pre_total) * 100);
    g_pre_total = g_total;
    g_pre_idle = g_idle;

    return usage;
}

char g_cpu_temperature_string[6];
/**
 * @brief 获取 CPU 温度
 * 
 * @return int CPU 温度值，摄氏度
 */
int get_cpu_temperature()
{
    FILE *cpu_temperature_file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	memset(g_cpu_temperature_string, '\0', sizeof(g_cpu_temperature_string));
    fread(g_cpu_temperature_string, sizeof(char), sizeof(g_cpu_temperature_string), cpu_temperature_file);
    fclose(cpu_temperature_file);

    long temperature;
    sscanf(g_cpu_temperature_string, "%ld", &temperature);
	temperature /= 1000;

    return (int)temperature;
}

struct ifaddrs *g_ifAddrStruct = NULL;
void *g_tmpAddrPtr = NULL;
char g_ip[24];
/**
 * @brief 获取 IP
 * 
 * @return char* eth0/wlan0:IP
 */
char *get_ip()
{
    getifaddrs(&g_ifAddrStruct);
    while (g_ifAddrStruct != NULL)    
    {
        if (g_ifAddrStruct->ifa_addr->sa_family == AF_INET)
        {
            g_tmpAddrPtr = &((struct sockaddr_in *)g_ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, g_tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

			memset(g_ip, '\0', sizeof(g_ip));
			if (strcmp(g_ifAddrStruct->ifa_name, "eth0") == 0)
			{
				sprintf(g_ip, "eth0:%s", addressBuffer);
				break;
			}
			else if (strcmp(g_ifAddrStruct->ifa_name, "wlan0") == 0)
			{
				sprintf(g_ip, "wlan0:%s", addressBuffer);
				break;
			}
        }
        g_ifAddrStruct = g_ifAddrStruct->ifa_next;
    }
	return g_ip;
}

char g_ram[32];
static struct sysinfo g_sys_info;
unsigned long g_total_ram, g_free_ram;
/**
 * @brief 获取内存可用大小
 * 
 * @return char* RAM:空闲/总MB
 */
char *get_ram()
{
	memset(g_ram, '\0', sizeof(g_ram));
	sysinfo(&g_sys_info);
	g_total_ram = g_sys_info.totalram >> 20;
	g_free_ram = g_sys_info.freeram >> 20;
	sprintf(g_ram, "RAM:%ld/%ldMB", g_free_ram, g_total_ram);
	return g_ram;
}

/**
 * @brief oled 按行显示，共四行
 * 
 * @param row 显示位置行数
 * @param s 显示内容字符串
 */
void write_line_string(int row, const char *s)
{
	int i = 0;
	while (s[i] != '\0')
	{
		ssd1306_drawChar(i * 6, row * 8, s[i], 1, 1);
		++i;
	}
}

int main()
{
    ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // oled 初始化
    int fd = wiringPiI2CSetup(0x0D);  // 风扇和 RGB 灯初始化，获取操作描述符
    int usage = 0;  // 使用率
    int temp = 0;  // 温度
    char row0[10];  // 第一行
    char row1[10];  // 第二行

    wiringPiI2CWriteReg8(fd, 0x04, 0x03);  // RGB 灯模式设定 

    while (1)
    {
        ssd1306_clearDisplay();  // 清空 oled 缓存

        // // 第一行显示 CPU 使用率
        usage = get_cpu_usage();
        memset(row0, '\0', sizeof(row0));
        sprintf(row0, "CPU: %d%%", usage);
        write_line_string(0, row0);
        // // 第二行显示 CPU 温度
        temp = get_cpu_temperature();
        memset(row1, '\0', sizeof(row1));
        sprintf(row1, "Temp: %dC", temp);
        write_line_string(1, row1);
        // 第三行显示内存可用大小
        write_line_string(2, get_ram());
        // 第四行显示 IP
        write_line_string(3, get_ip());

        // // 风扇控制
        if (usage > 50 || temp > 50)
        {
            wiringPiI2CWriteReg8(fd, 0x08, 0x01);
        }
        else
        {
            wiringPiI2CWriteReg8(fd, 0x08, 0x00);
        }


        ssd1306_display();  // 缓存刷新到屏幕显示
        delay(3000);  // 循环间隔时间
    }
    
}