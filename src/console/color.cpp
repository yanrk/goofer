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

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <unistd.h>
#endif // _MSC_VER

#include <cstdio>
#include <cstdarg>
#include "console/color.h"

#ifdef _MSC_VER
    #define vfprintf_safe vfprintf_s
    #define fprintf_safe  fprintf_s
#else
    #define vfprintf_safe vfprintf
    #define fprintf_safe  fprintf
#endif // _MSC_VER

static unsigned short get_psx_foreground_color(GOOFER_CONSOLE_FONT_COLOR console_color)
{
    unsigned short color = 0;
    switch (console_color)
    {
        case GCFC_BLACK:
        {
            color = 30;
            break;
        }
        case GCFC_DARK_RED:
        {
            color = 31;
            break;
        }
        case GCFC_DARK_GREEN:
        {
            color = 32;
            break;
        }
        case GCFC_DARK_YELLOW:
        {
            color = 33;
            break;
        }
        case GCFC_DARK_BLUE:
        {
            color = 34;
            break;
        }
        case GCFC_DARK_MAGENTA:
        {
            color = 35;
            break;
        }
        case GCFC_DARK_CYAN:
        {
            color = 36;
            break;
        }
        case GCFC_DARK_GRAY:
        {
            color = 90;
            break;
        }
        case GCFC_BRIGHT_GRAY:
        {
            color = 37;
            break;
        }
        case GCFC_BRIGHT_RED:
        {
            color = 91;
            break;
        }
        case GCFC_BRIGHT_GREEN:
        {
            color = 92;
            break;
        }
        case GCFC_BRIGHT_YELLOW:
        {
            color = 93;
            break;
        }
        case GCFC_BRIGHT_BLUE:
        {
            color = 94;
            break;
        }
        case GCFC_BRIGHT_MAGENTA:
        {
            color = 95;
            break;
        }
        case GCFC_BRIGHT_CYAN:
        {
            color = 96;
            break;
        }
        case GCFC_WHITE:
        {
            color = 97;
            break;
        }
        default:
        {
            color = 39;
            break;
        }
    }
    return (color);
}

static unsigned short get_psx_background_color(GOOFER_CONSOLE_FONT_COLOR console_color)
{
    unsigned short color = 0;
    switch (console_color)
    {
        case GCFC_BLACK:
        {
            color = 40;
            break;
        }
        case GCFC_DARK_RED:
        {
            color = 41;
            break;
        }
        case GCFC_DARK_GREEN:
        {
            color = 42;
            break;
        }
        case GCFC_DARK_YELLOW:
        {
            color = 43;
            break;
        }
        case GCFC_DARK_BLUE:
        {
            color = 44;
            break;
        }
        case GCFC_DARK_MAGENTA:
        {
            color = 45;
            break;
        }
        case GCFC_DARK_CYAN:
        {
            color = 46;
            break;
        }
        case GCFC_DARK_GRAY:
        {
            color = 100;
            break;
        }
        case GCFC_BRIGHT_GRAY:
        {
            color = 47;
            break;
        }
        case GCFC_BRIGHT_RED:
        {
            color = 101;
            break;
        }
        case GCFC_BRIGHT_GREEN:
        {
            color = 102;
            break;
        }
        case GCFC_BRIGHT_YELLOW:
        {
            color = 103;
            break;
        }
        case GCFC_BRIGHT_BLUE:
        {
            color = 104;
            break;
        }
        case GCFC_BRIGHT_MAGENTA:
        {
            color = 105;
            break;
        }
        case GCFC_BRIGHT_CYAN:
        {
            color = 106;
            break;
        }
        case GCFC_WHITE:
        {
            color = 107;
            break;
        }
        default:
        {
            color = 49;
            break;
        }
    }
    return (color);
}

#ifdef _MSC_VER
static unsigned short get_win_foreground_color(GOOFER_CONSOLE_FONT_COLOR console_color)
{
    unsigned short color = 0;
    switch (console_color)
    {
        case GCFC_BLACK:
        {
            color = 0;
            break;
        }
        case GCFC_DARK_RED:
        {
            color = FOREGROUND_RED;
            break;
        }
        case GCFC_DARK_GREEN:
        {
            color = FOREGROUND_GREEN;
            break;
        }
        case GCFC_DARK_YELLOW:
        {
            color = FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        }
        case GCFC_DARK_BLUE:
        {
            color = FOREGROUND_BLUE;
            break;
        }
        case GCFC_DARK_MAGENTA:
        {
            color = FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        }
        case GCFC_DARK_CYAN:
        {
            color = FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }
        case GCFC_DARK_GRAY:
        {
            color = FOREGROUND_INTENSITY;
            break;
        }
        case GCFC_BRIGHT_GRAY:
        {
            color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }
        case GCFC_BRIGHT_RED:
        {
            color = FOREGROUND_INTENSITY | FOREGROUND_RED;
            break;
        }
        case GCFC_BRIGHT_GREEN:
        {
            color = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
            break;
        }
        case GCFC_BRIGHT_YELLOW:
        {
            color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        }
        case GCFC_BRIGHT_BLUE:
        {
            color = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
            break;
        }
        case GCFC_BRIGHT_MAGENTA:
        {
            color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        }
        case GCFC_BRIGHT_CYAN:
        {
            color = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }
        case GCFC_WHITE:
        {
            color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }
        default:
        {
            color = 0;
            break;
        }
    }
    return (color);
}

static unsigned short get_win_background_color(GOOFER_CONSOLE_FONT_COLOR console_color)
{
    unsigned short color = 0;
    switch (console_color)
    {
        case GCFC_BLACK:
        {
            color = 0;
            break;
        }
        case GCFC_DARK_RED:
        {
            color = BACKGROUND_RED;
            break;
        }
        case GCFC_DARK_GREEN:
        {
            color = BACKGROUND_GREEN;
            break;
        }
        case GCFC_DARK_YELLOW:
        {
            color = BACKGROUND_RED | BACKGROUND_GREEN;
            break;
        }
        case GCFC_DARK_BLUE:
        {
            color = BACKGROUND_BLUE;
            break;
        }
        case GCFC_DARK_MAGENTA:
        {
            color = BACKGROUND_RED | BACKGROUND_BLUE;
            break;
        }
        case GCFC_DARK_CYAN:
        {
            color = BACKGROUND_GREEN | BACKGROUND_BLUE;
            break;
        }
        case GCFC_DARK_GRAY:
        {
            color = BACKGROUND_INTENSITY;
            break;
        }
        case GCFC_BRIGHT_GRAY:
        {
            color = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
            break;
        }
        case GCFC_BRIGHT_RED:
        {
            color = BACKGROUND_INTENSITY | BACKGROUND_RED;
            break;
        }
        case GCFC_BRIGHT_GREEN:
        {
            color = BACKGROUND_INTENSITY | BACKGROUND_GREEN;
            break;
        }
        case GCFC_BRIGHT_YELLOW:
        {
            color = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN;
            break;
        }
        case GCFC_BRIGHT_BLUE:
        {
            color = BACKGROUND_INTENSITY | BACKGROUND_BLUE;
            break;
        }
        case GCFC_BRIGHT_MAGENTA:
        {
            color = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE;
            break;
        }
        case GCFC_BRIGHT_CYAN:
        {
            color = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE;
            break;
        }
        case GCFC_WHITE:
        {
            color = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
            break;
        }
        default:
        {
            color = 0;
            break;
        }
    }
    return (color);
}
#endif // _MSC_VER

static bool s_console_color_posix = false;
static bool s_console_color_enable = false;

static size_t goofer_console_color_print(GOOFER_CONSOLE_FONT_COLOR foreground_color, GOOFER_CONSOLE_FONT_COLOR background_color, FILE * stream, const char * format, va_list args)
{
    if (nullptr == stream || nullptr == format || nullptr == args)
    {
        return (0);
    }

    if (stream != stdout && stream != stderr)
    {
        return (0);
    }

    int ret_siz = 0;

    if (s_console_color_enable)
    {
        if (s_console_color_posix)
        {
            if (GCFC_NONE != foreground_color)
            {
                if (GCFC_NONE != background_color)
                {
                    fprintf_safe(stream, "\x1b[%d;%dm", get_psx_foreground_color(foreground_color), get_psx_background_color(background_color));
                }
                else
                {
                    fprintf_safe(stream, "\x1b[%dm", get_psx_foreground_color(foreground_color));
                }
            }
            else
            {
                if (GCFC_NONE != background_color)
                {
                    fprintf_safe(stream, "\x1b[%dm", get_psx_background_color(background_color));
                }
            }

            ret_siz = vfprintf_safe(stream, format, args);

            if (GCFC_NONE != foreground_color || GCFC_NONE != background_color)
            {
                fprintf_safe(stream, "\x1b[39;49m\x1b[K");
            }
        }
        else
        {
#ifdef _MSC_VER
            HANDLE console_handle = GetStdHandle(stream == stdout ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info = { 0x0 };
            if (INVALID_HANDLE_VALUE != console_handle && !!GetConsoleScreenBufferInfo(console_handle, &console_screen_buffer_info))
            {
                if (GCFC_NONE != foreground_color)
                {
                    if (GCFC_NONE != background_color)
                    {
                        SetConsoleTextAttribute(console_handle, (console_screen_buffer_info.wAttributes & 0xff00) | get_win_foreground_color(foreground_color) | get_win_background_color(background_color));
                    }
                    else
                    {
                        SetConsoleTextAttribute(console_handle, (console_screen_buffer_info.wAttributes & 0xfff0) | get_win_foreground_color(foreground_color));
                    }
                }
                else
                {
                    if (GCFC_NONE != background_color)
                    {
                        SetConsoleTextAttribute(console_handle, (console_screen_buffer_info.wAttributes & 0xff0f) | get_win_background_color(background_color));
                    }
                }

                ret_siz = vfprintf_safe(stream, format, args);

                if (GCFC_NONE != foreground_color || GCFC_NONE != background_color)
                {
                    SetConsoleTextAttribute(console_handle, console_screen_buffer_info.wAttributes);
                }
            }
            else
            {
                ret_siz = vfprintf_safe(stream, format, args);
            }
#else
            ret_siz = vfprintf_safe(stream, format, args);
#endif // _MSC_VER
        }
    }
    else
    {
        ret_siz = vfprintf_safe(stream, format, args);
    }

    return (ret_siz < 0 ? 0 : ret_siz);
}

NAMESPACE_GOOFER_BEGIN

void goofer_console_color_enable()
{
    s_console_color_posix = false;
#ifdef _MSC_VER
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (INVALID_HANDLE_VALUE != console_handle)
    {
        DWORD console_mode = 0;
        if (!!GetConsoleMode(console_handle, &console_mode))
        {
            console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if (!!SetConsoleMode(console_handle, console_mode))
            {
                s_console_color_posix = true;
            }
        }
    }
#else
    s_console_color_posix = true;
#endif // _MSC_VER
    s_console_color_enable = true;
}

void goofer_console_color_disable()
{
    s_console_color_enable = false;
}

size_t goofer_console_color_stdout_print(GOOFER_CONSOLE_FONT_COLOR foreground_color, GOOFER_CONSOLE_FONT_COLOR background_color, const char * format, ...)
{
    va_list args;

    va_start(args, format);

    size_t ret_siz = goofer_console_color_print(foreground_color, background_color, stdout, format, args);

    va_end(args);

    return (ret_siz);
}

size_t goofer_console_color_stderr_print(GOOFER_CONSOLE_FONT_COLOR foreground_color, GOOFER_CONSOLE_FONT_COLOR background_color, const char * format, ...)
{
    va_list args;

    va_start(args, format);

    size_t ret_siz = goofer_console_color_print(foreground_color, background_color, stderr, format, args);

    va_end(args);

    return (ret_siz);
}

NAMESPACE_GOOFER_END
