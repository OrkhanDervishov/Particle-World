#ifndef PW_CONSOLE_H_
#define PW_CONSOLE_H_

#include <stdio.h>

#define PW_ANSI_COLOR_BLACK   "\x1b[30m"
#define PW_ANSI_COLOR_RED     "\x1b[31m"
#define PW_ANSI_COLOR_GREEN   "\x1b[32m"
#define PW_ANSI_COLOR_YELLOW  "\x1b[33m"
#define PW_ANSI_COLOR_BLUE    "\x1b[34m"
#define PW_ANSI_COLOR_MAGENTA "\x1b[35m"
#define PW_ANSI_COLOR_CYAN    "\x1b[36m"
#define PW_ANSI_COLOR_WHITE   "\x1b[37m"

#define PW_ANSI_BACK_BLACK   "\x1b[40m"
#define PW_ANSI_BACK_RED     "\x1b[41m"
#define PW_ANSI_BACK_GREEN   "\x1b[42m"
#define PW_ANSI_BACK_YELLOW  "\x1b[43m"
#define PW_ANSI_BACK_BLUE    "\x1b[44m"
#define PW_ANSI_BACK_MAGENTA "\x1b[45m"
#define PW_ANSI_BACK_CYAN    "\x1b[46m"
#define PW_ANSI_BACK_WHITE   "\x1b[47m"

#define PW_ANSI_RESET   "\x1b[0m" 

#define PW_CONSOLE_STR(str, color) printf("%s%s" PW_ANSI_RESET, color, str) 
#define PW_CONSOLE_NL printf("\n");

#endif //PW_CONSOLE_H_