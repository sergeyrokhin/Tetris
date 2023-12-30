#pragma once
#include "../common.h"
#include "../battlefield.h"

#include <mutex>
#include <memory>

namespace console
{
inline int HEADER_SCREAN_H = 1;
inline int HEADER_SCREAN_W = 20;
    int getch_my();
    int kbhit_my();
    void clear_screen();
    void hide_cursor();
    void enable_cursor();

    class ConsoleFrame
    {
    private:
        static std::mutex mutex_console;
        Coordinates position_;

    private:
        template <class T>
        void Print(const T &text, Coordinates position) const;
        void InicializeWinConsole();

    public:
        ConsoleFrame(const Coordinates &position = {});
        ConsoleFrame(const ConsoleFrame &);
        ConsoleFrame &operator=(const ConsoleFrame &);
        ConsoleFrame() = delete;
        void Print(const BattleField &battle_field) const;
        void MoveCursorTo(Coordinates position) const;
        void OutToConsole(std::string_view buf, Coordinates position) const;
    };
}