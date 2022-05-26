# 简介
这是一个为树梅派编写的 ssd1306 i2c oled 库。

# 测试环境
树莓派 cm4  
官方系统 64 位（Debian 10）  
官方系统 64 位（Debian 11）    
[wiringPi C 库](https://github.com/WiringPi/WiringPi)  

128x32 oled  
128x64 oled
# 使用

安装依赖
```bash
sudo apt update && sudo apt install build-essential cmake git

git clone https://github.com/WiringPi/WiringPi.git --depth=1

sudo bash WiringPi/INSTALL
```

安装
```bash
git clone https://github.com/IYATT-yx/ssd1306_i2c.git --depth=1

mkdir ssd1306_i2c/build && cd ssd1306_i2c/build

cmake -DCMAKE_BUILD_TYPE=release ..

sudo make install

sudo su

echo "export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig" >> /etc/bash.bashrc

source /etc/bash.bashrc

exit
```

卸载
```bash
# 进入 ssd1306_i2c 目录，再执行
sudo xargs rm -rf < build/install_manifest.txt
```

开发

```c
// 包含头文件
#include "ssd1306_i2c.h"
```

```bash
# 命令行编译模板
# 编译源码 demo.c 生成可执行文件 demo
gcc demo.c -o demo `pkg-config --cflags --libs ssd1306_i2c` -no-pie
```

```cmake
# cmake 链接模板
find_package(ssd1306_i2c REQUIRED)

# 链接部分 - demo为可执行文件
target_link_libraries(demo ${ssd1306_i2c_LIBRARIES} ${WIRINGPI_LIBRARIES})
```

注：本项目依赖 wiringPi C 库，链接本库同时可链接 wiringPi

了解 API 接口如何使用，可阅读 ssd1306_i2c.h 尾部的注解以及 examples 中的样例代码。

# 分辨率设置

默认分辨率设置为  128x64  
如果要修改，打开文件 [ssd1306_i2.h](include/ssd1306_i2c.h)，搜索关键词`分辨率设置`，注释原定义的分辨率，去掉要打开的分辨率定义的注释符。  
然后重新进行上面的安装操作，完成修改。  

# 声明
 本项目基于 https://github.com/iliapenev/ssd1306_i2c  
 并进一步完善功能和修复bug，增加了 cmake 脚本实现本库的安装，添加了对 pkg-config 和 cmake 链接本库的支持，编写了样例程序，以及补充了API接口注解。

 Copyright (C) 2022 IYATT-yx (Zhao Hongfei, 赵洪飞)，iyatt@iyatt.com
