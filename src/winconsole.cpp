#include "winconsole.h"
#include <iostream>


ConsoleFrame::ConsoleFrame(const Coordinates& position) : position_(position)
{
    if (!console_.is_initialized)
    {
        console_.is_initialized = true;
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        console_.hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(console_.hStdout, &csbiInfo))
        {
            std::cout << "GetConsoleScreenBufferInfo " << GetLastError() << std::endl;
            return;
        }
        console_.y_zero = csbiInfo.dwCursorPosition.Y;
    }
}

void ConsoleFrame::Print(const BattleField &battle_field) const
{
    auto count = battle_field.field_.size();
    for (int i = 0; i < count; i++)
    {
        //шкала глубины
        Print(static_cast<char>('0' + (count - 1 - i) % 10), { i, 0 });
        //само поле игры
        Print(battle_field.field_[i], {i, 1});
    }
}

ConsoleFrame::WinConsole ConsoleFrame::console_;

template <class T>
void ConsoleFrame::Print(const T& text, Coordinates position) const {

    //std::cout << "ConsoleFrame::Print " << position_.h_ << std::endl;
    COORD point = {static_cast<SHORT>(position_.w_ + position.w_), static_cast<SHORT>(position_.h_ + position.h_ + console_.y_zero)};
    {
        std::lock_guard m(console_.mutex_);
        SetConsoleCursorPosition(console_.hStdout, point);
        std::cout << text;
    }
}