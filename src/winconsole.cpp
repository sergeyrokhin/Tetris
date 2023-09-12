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

//void ConsoleFrame::ConsoleProcessThread(const std::shared_ptr<BattleField> field_ptr)
//{
//    game_is_run_ = true;
//    while (game_is_run_)
//    {
//        Print(field_ptr);
//    }
//}

void ConsoleFrame::Print(const std::shared_ptr<BattleField> field_ptr) const
{
    auto count = field_ptr.get()->field_.size();
    for (int i = 0; i < count; i++)
    {
        //шкала глубины
        Print(static_cast<char>('0' + (count - 1 - i) % 10), { i, 0 });
        //само поле игры
        Print(field_ptr.get()->field_[i], {i, 1});
    }
}

template <class T>
void ConsoleFrame::Print(const T& text, Coordinates position) const {

    COORD point = {static_cast<SHORT>(position.w_), static_cast<SHORT>(position.h_ + y_zero)};
    SetConsoleCursorPosition(hStdout, point);
    std::cout << text;
}