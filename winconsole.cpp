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

void ConsoleFrame::Print(const BattleField &field)
{
    auto count = field.field_.size();
    for (int i = 0; i < count; i++)
    {
        Print(field.field_[i], {i, 0});
    }
}

void ConsoleFrame::Print(const std::string& text, Coordinates position) {
    COORD point = {position.w_, position.h_ + y_zero};
    SetConsoleCursorPosition(hStdout, point);
    std::cout << text;
}