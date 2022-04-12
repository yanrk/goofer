/********************************************************
 * Description : interface of console color print
 * Date        : 2022-04-11 19:28:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2022 - 2022
 ********************************************************/

#ifndef GOOFER_CONSOLE_COLOR_H
#define GOOFER_CONSOLE_COLOR_H


#include <cstddef>
#include "common/common.h"

enum GOOFER_CONSOLE_FONT_COLOR
{
    GCFC_NONE,
    GCFC_BLACK,
    GCFC_DARK_RED,
    GCFC_DARK_GREEN,
    GCFC_DARK_YELLOW,
    GCFC_DARK_BLUE,
    GCFC_DARK_MAGENTA,
    GCFC_DARK_CYAN,
    GCFC_DARK_GRAY,
    GCFC_BRIGHT_GRAY,
    GCFC_BRIGHT_RED,
    GCFC_BRIGHT_GREEN,
    GCFC_BRIGHT_YELLOW,
    GCFC_BRIGHT_BLUE,
    GCFC_BRIGHT_MAGENTA,
    GCFC_BRIGHT_CYAN,
    GCFC_WHITE
};

NAMESPACE_GOOFER_BEGIN

GOOFER_CXX_API(void) goofer_console_color_enable();
GOOFER_CXX_API(void) goofer_console_color_disable();
GOOFER_CXX_API(size_t) goofer_console_color_stdout_print(GOOFER_CONSOLE_FONT_COLOR foreground_color, GOOFER_CONSOLE_FONT_COLOR background_color, const char * format, ...);
GOOFER_CXX_API(size_t) goofer_console_color_stderr_print(GOOFER_CONSOLE_FONT_COLOR foreground_color, GOOFER_CONSOLE_FONT_COLOR background_color, const char * format, ...);

NAMESPACE_GOOFER_END


#endif // GOOFER_CONSOLE_COLOR_H
