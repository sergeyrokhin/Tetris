#pragma once
#include "../common.h"
#include "../battlefield.h"

#include <mutex>
#include <memory>

namespace console
{

    int getch_my();
    int kbhit_my();

    class ConsoleFrame
    {
        struct WinConsole
        {
            static std::mutex mutex_;
        };

    private:
        WinConsole console_;
        Coordinates position_;

    private:
        template <class T>
        void Print(const T &text, Coordinates position) const;
        void InicializeWinConsole();

    public:
        ConsoleFrame(const Coordinates &position = {});
        ConsoleFrame(const ConsoleFrame &);
        ConsoleFrame &operator=(const ConsoleFrame &);
        ConsoleFrame();
        void Print(const BattleField &battle_field) const;
        void MoveCursorTo(Coordinates position) const;
        void CursorShow();
        void CursorHide();
    };
}