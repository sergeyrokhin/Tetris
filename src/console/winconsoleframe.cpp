#include "control.h"
#include "consoleframe.h"
#include <conio.h>
#include <Windows.h>

#include <strsafe.h>

void ErrorExit(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                                      (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
                    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                    TEXT("%s failed with error %d: %s"),
                    lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

namespace console
{
    int y_zero = 0;
    HANDLE hStdOut;
    bool is_initialized = false;
    COORD dwSize; //  SHORT X;  SHORT Y; X The horizontal coordinate or column value. Y The vertical coordinate or row value.
    Coordinates minScreanSize;

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
        SetConsoleCursorPosition(hStdOut, point);
    }

    void ConsoleFrame::OutToConsole(std::string_view buf, Coordinates position) const
    {

        COORD point = {static_cast<SHORT>(position_.w_ + position.w_), static_cast<SHORT>(position_.h_ + position.h_ + y_zero)};
        if (!SetConsoleCursorPosition(hStdOut, point))
        {
            std::cerr << '>' << point.X << 'x' << dwSize.X << '-' << point.Y << 'y' << dwSize.Y << '<';
        }

        DWORD lpNumberOfCharsWritten;
        LPVOID lpReserved = nullptr;
        WriteConsole(hStdOut, buf.data(), buf.size(), &lpNumberOfCharsWritten, lpReserved);
    }

    void ConsoleFrame::InicializeWinConsole()
    {
        Coordinates size_frame = {minScreanSize.h_ + position_.h_ + HEADER_SCREAN_H, minScreanSize.w_ + position_.w_ + HEADER_SCREAN_W};
        if (size_frame.h_ > dwSize.Y || size_frame.w_ > dwSize.X)
        {
            using namespace std::literals;

            dwSize.Y = size_frame.h_ > dwSize.Y ? size_frame.h_ : dwSize.Y;
            dwSize.X = size_frame.w_ > dwSize.X ? size_frame.w_ : dwSize.X;

            if (!SetConsoleScreenBufferSize(hStdOut, dwSize))
            {
                std::string buf("SetConsoleScreenBufferSize ");
                buf += " X "s + std::to_string(dwSize.X) + " Y "s + std::to_string(dwSize.Y);
                ErrorExit(TEXT(const_cast<char *>(buf.c_str())));
            }
        }
    }

    void clear_screen()
    {
        // HANDLE hStdOut;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = {0, 0};

        // hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        //  if (hStdOut == INVALID_HANDLE_VALUE)
        //      return;

        /* Get the number of cells in the current buffer */
        if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
            return;
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;

        /* Fill the entire buffer with spaces */
        if (!FillConsoleOutputCharacter(
                hStdOut,
                (TCHAR)' ',
                cellCount,
                homeCoords,
                &count))
            return;

        /* Fill the entire buffer with the current colors and attributes */
        if (!FillConsoleOutputAttribute(
                hStdOut,
                csbi.wAttributes,
                cellCount,
                homeCoords,
                &count))
            return;

        /* Move the cursor home */
        SetConsoleCursorPosition(hStdOut, homeCoords);
    }

    void hide_cursor()
    {
        // HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(hStdOut, &info);
    }

    void enable_cursor()
    {
        // HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = TRUE;
        SetConsoleCursorInfo(hStdOut, &info);
    }
}

void ControlCenter::InitScreen(Coordinates size)
{
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    console::hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(console::hStdOut, &csbiInfo))
    {
        ErrorExit(TEXT(const_cast<char *>("GetConsoleScreenBufferInfo")));
        return;
    }
    console::dwSize = csbiInfo.dwSize;
    console::y_zero = csbiInfo.dwCursorPosition.Y;
    console::minScreanSize = {size.h_ + console::y_zero, size.w_};
}
