#include "consoleframe.h"
#include <conio.h>
#include <Windows.h>

namespace console
{
            int y_zero = 0;
            HANDLE hStdout;
            bool is_initialized = false;

    int getch_my()
    {
        return _getch();
    }
    int kbhit_my()
    {
        return _kbhit();
    }

    void ConsoleFrame::MoveCursorTo(Coordinates position) const
    {
        COORD point = {static_cast<SHORT>(position_.w_ + position.w_), static_cast<SHORT>(position_.h_ + position.h_ + y_zero)};
        SetConsoleCursorPosition(hStdout, point);
    }
    void ConsoleFrame::InicializeWinConsole()
    {
        if (!is_initialized)
        {
            is_initialized = true;

            CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
            hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
            if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
            {
                std::cout << "GetConsoleScreenBufferInfo " << GetLastError() << std::endl;
                return;
            }
            y_zero = csbiInfo.dwCursorPosition.Y;
        }
    }

}
