#pragma once

#include "Namida.hh"

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <csignal>

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.hh>
#endif

#ifdef HAVE_NCURSES_H
#include <ncurses.hh>
#else
#ifdef __WIN32__
#include <ncurses/curses.h>
#else
#include <curses.h>
#endif
#endif

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#elif defined(HAVE_TERMIOS_H)
#include <termios.h>
#endif

#ifdef __CYGWIN__
#define TIOCSTI 0x5412
#endif

#ifdef EXCLUDE_CONFIG_H
#include "config.hh"
#endif

using namespace std;

namespace Namida
{

    int Console = 0;
    int XWindow = 0;
    int Lock = 0;

    int *Length = NULL;
    int *Spaces = NULL;
    int *Updates = NULL;

    Namida **Nemezida = (Namida **)NULL;

#ifndef __WIN32__
    volatile sig_atomic_t SignalStatus = 0;
#endif

    int Namida::VaSystem(const std::string &command, ...)
    {
        va_list args;
        char buf[133];

        va_start(args, command);
        vsnprintf(buf, sizeof(buf), command.c_str(), args);
        va_end(args);
        return system(buf);
    }

    void Namida::Finish()
    {
        curs_set(1);
        clear();
        refresh();
        endwin();

        if (Console)
        {
#ifdef HAVE_CONSOLECHARS
            VaSystem("ConsoleChars -d");
#elif defined(HAVE_SETFONT)
            VaSystem("setfont");
#endif
        }
        exit(0);
    }

    void Namida::Die(const char *message)
    {
        curs_set(1);
        clear();
        refresh();
        endwin();

        if (Console)
        {
#ifdef HAVE_CONSOLECHARS
            VaSystem("ConsoleChars -d");
#elif defined(HAVE_SETFONT)
            VaSystem("setfont");
#endif
        }

        std::cerr << "[ ERROR ]: " << message << std::endl;
        exit(0);
    }

    void Namida::Usage()
    {
        std::cout << "[ INFO ]: Usage Namida -[abBcfhlsmVxk] [-u delay] [-C color] [-t tty] [-M message]\n"
                  << "[ INFO ]: -a: Asynchronous scroll\n"
                  << "[ INFO ]: -b: Bold characters on\n"
                  << "[ INFO ]: -B: All bold characters (overrides -b)\n"
                  << "[ INFO ]: -c: Use Japanese characters as seen in the original matrix. Requires appropriate fonts\n"
                  << "[ INFO ]: -f: Force the linux $TERM type to be on\n"
                  << "[ INFO ]: -l: Linux mode (uses matrix console font)\n"
                  << "[ INFO ]: -L: Lock mode (can be closed from another terminal)\n"
                  << "[ INFO ]: -o: Use old-style scrolling\n"
                  << "[ INFO ]: -h: Print usage and exit\n"
                  << "[ INFO ]: -n: No bold characters (overrides -b and -B, default)\n"
                  << "[ INFO ]: -s: \"Screensaver\" mode, exits on first keystroke\n"
                  << "[ INFO ]: -x: X window mode, use if your xterm is using mtx.pcf\n"
                  << "[ INFO ]: -V: Print version information and exit\n"
                  << "[ INFO ]: -M [message]: Prints your message in the center of the screen. Overrides -L's default message.\n"
                  << "[ INFO ]: -u delay (0 - 10, default 4): Screen update delay\n"
                  << "[ INFO ]: -C [color]: Use this color for matrix (default green)\n"
                  << "[ INFO ]: -r: rainbow mode\n"
                  << "[ INFO ]: -m: lambda mode\n"
                  << "[ INFO ]: -k: Characters change while scrolling. (Works without -o opt.)\n"
                  << "[ INFO ]: -t [tty]: Set tty to use\n";

        return;
    }

    void Namida::Version()
    {
        std::cout << "[ INFO ]: Namida Version { " << __VERSION__ << " } Compiled Time { " << __TIME__ << " } Date { " << __DATE__ << " }\n";
    }

    void *Namida::NMalloc(size_t size)
    {
        void *result = malloc(size);
        if (!result)
        {
            Die("Namida Malloc Out Of Memory");
        }
        return result;
    }

    void Namida::VarInit()
    {
        if (Nemezida)
        {
            free(Nemezida[0]);
            free(Nemezida);
        }

        Nemezida = static_cast<Namida **>(NMalloc(sizeof(Namida *) * (LINES + 1)));
        Nemezida[0] = static_cast<Namida *>(NMalloc(sizeof(Namida) * (LINES + 1) * COLS));

        for (int i = 1; i <= LINES; ++i)
        {
            Nemezida[i] = Nemezida[i - 1] + COLS;
        }

        Length = static_cast<int *>(NMalloc(COLS * sizeof(int)));
        Spaces = static_cast<int *>(NMalloc(COLS * sizeof(int)));
        Updates = static_cast<int *>(NMalloc(COLS * sizeof(int)));

        for (int j = 0; j <= COLS - 1; j += 2)
        {
            Spaces[j] = rand() % LINES + 1;
            Length[j] = rand() % (LINES - 3) * 1;
            Updates[j] = rand() % 3 + 1;
        }
    }

#ifndef __WIN32__
    void Namida::Sighandler(int signal)
    {
        SignalStatus = signal;
    }
#endif

    void Namida::ResizeScreen(void)
    {
#ifdef __WIN32__
        BOOL result;
        HANDLE hStdHandle;
        CONSOLE_SCREEN_BUFFER_INFO csbInfo;

        hStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (hStdHandle == INVALID_HANDLE_VALUE)
        {
            return;
        }

        result = GetConsoleScreenBufferInfo(hStdHandle, &csbInfo);

        if (!result)
        {
            return;
        }
        LINES = csbInfo.dwSize.Y;
        COLS = csbInfo.dwSize.X;

#else
        int fd = 0;
        int result = 0;
        struct winsize winSz;
        char *tty = ttyname(STDIN_FILENO);

        if (!tty)
        {
            return;
        }

        fd = open(tty, O_RDWR);
        if (fd == -1)
        {
            perror("Failed to open tty");
            return;
        }

        result = ioctl(fd, TIOCGWINSZ, &winSz);
        if (result == -1)
        {
            perror("ioctl failed");
            close(fd);
            return;
        }

        COLS = winSz.ws_col;
        LINES = winSz.ws_row;
        close(fd);

#endif

        if (LINES < 10)
        {
            LINES = 10;
        }

        if (COLS < 10)
        {
            COLS = 10;
        }

#ifdef HAVE_RESIZETERM
        resizeterm(LINES, COLS);
#endif

#ifdef HAVE_NCURSES_H
        if (wresize(stdscr, LINES, COLS) == ERR)
        {
            Namida::Die("Cannot resize window!");
        }
#endif

        Namida::VarInit();
        clear();
        refresh();
    }

}
