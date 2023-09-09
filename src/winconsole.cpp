#include "winconsole.h"
#include <iostream>

ConsoleFrame::ConsoleFrame() : y_zero(0)
{
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
    {
        std::cout << "GetConsoleScreenBufferInfo " << GetLastError() << std::endl;
        return;
    }
    y_zero = csbiInfo.dwCursorPosition.Y;
}

void ConsoleFrame::ConsoleProcessThread(const BattleField &field)
{
    game_is_run_ = true;
    while (game_is_run_)
    {
        Print(field);
    }
}

void ConsoleFrame::Print(const BattleField &field) const
{
    auto count = field.field_.size();
    for (int i = 0; i < count; i++)
    {
        Print(static_cast<char>('0' + (count - 1 - i) % 10), { i, 0 });
        Print(field.field_[i], {i, 1});
    }
}

//void ConsoleFrame::Print(const std::string& text, Coordinates position) const {
template <class T>
void ConsoleFrame::Print(const T& text, Coordinates position) const {

    COORD point = {static_cast<SHORT>(position.w_), static_cast<SHORT>(position.h_ + y_zero)};
    SetConsoleCursorPosition(hStdout, point);
    std::cout << text;
}