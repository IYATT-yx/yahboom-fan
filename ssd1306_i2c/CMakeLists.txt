#[[
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
]]

cmake_minimum_required(VERSION 3.16.3)
project(SSD1306_I2C
        VERSION 0.1
        HOMEPAGE_URL github.com/IYATT-yx/ssd1306_i2c)

if (${CMAKE_C_COMPILER_VERSION} VERSION_LESS "8.3.0")
    message(FATAL_ERROR "GNU gcc 版本至少需要 8.3.0")
endif ()

set(CMAKE_C_FLAGS_RELEASE "-std=c17 -O3")
set(CMAKE_C_FLAGS_DEBUG "-std=c17 -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Werror=format-security -Wextra -pedantic -Wimplicit-fallthrough -Wsequence-point -Wswitch-unreachable -Wswitch-enum -Wstringop-truncation -Wbool-compare -Wtautological-compare -Wfloat-equal -Wshadow=global -Wpointer-arith -Wpointer-compare -Wcast-align -Wcast-qual -Wwrite-strings -Wdangling-else -Wlogical-op -Wconversion -g -O0 -DDEBUG")

# 查找 wiringPi 库
find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)
add_library(ssd1306_i2c STATIC ${SRC})  # 编译库
target_link_libraries(ssd1306_i2c ${WIRINGPI_LIBRARIES}) # 链接 wiringPi

# 安装
set(INSTALL_LIBRARY_PATH "/usr/local/lib")
set(INSTALL_INCLUDE_PATH "/usr/local/include")
# 库文件
install(TARGETS ssd1306_i2c
        LIBRARY DESTINATION ${INSTALL_LIBRARY_PATH})
# 头文件
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/oled_fonts.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ssd1306_i2c.h
        DESTINATION ${INSTALL_INCLUDE_PATH})
# pkg-config 信息文件
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ssd1306_i2c.pc
        DESTINATION "/usr/local/lib/pkgconfig")
# cmake 信息文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake
        DESTINATION "/usr/local/lib")


# 样例
#######
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)  # 编译的可执行文件输出到 bin 目录
# 字符和字符串显示
add_executable(word examples/word.c)
target_link_libraries(word ssd1306_i2c)
# 简单图形
add_executable(figure examples/figure.c)
target_link_libraries(figure ssd1306_i2c)
# 滚动
add_executable(scroll examples/scroll.c)
target_link_libraries(scroll ssd1306_i2c)