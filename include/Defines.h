#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <windows.h>

// --- ANSI Color Codes ---
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define ORANGE  "\033[38;5;208m" 

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

/*
enableVirtualTerminal:
    make windows terminal able to read ansi coler code
*/
inline void enableVirtualTerminal()  
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);  //get curent console mode and write it in dwmode
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // add VT token prossesing activator to dwmode with OR
    SetConsoleMode(hOut, dwMode); //set the console mode to dwmode
}

#endif