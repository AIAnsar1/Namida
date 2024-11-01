// Namida.hh
#pragma once

#include <string>

namespace Namida
{
    class Namida
    {
    public:
        int Val;
        bool IsHead;

    public:
        int VaSystem(const std::string &command, ...);
        void Finish();
        void Die(const char *message);
        void Usage();
        void Version();
        void *NMalloc(size_t size);
        void VarInit();
        void Sighandler(int signal);
        void ResizeScreen();
    };
}