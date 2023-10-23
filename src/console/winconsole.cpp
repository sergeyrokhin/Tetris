#include <iostream>
#include <stdio.h>
#include <string>
#include "winconsole.h"
#include "cursor.h"

namespace console
{
#ifdef _WIN32
#include <conio.h>
    int getch_my()
    {
        return _getch();
    }
    int kbhit_my()
    {
        return _kbhit();
    }
#else

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
//#include <stropts.h>

int kbhit_my() {
    static const int STDIN = 0;
    static bool initialized = false;
 
    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }
 
    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

    /* reads from keypress, doesn't echo */
    int getch_my(void)
    {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }

    /* reads from keypress, echoes */
    int getche(void)
    {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
#endif

} // namespace console {

void ConsoleFrame::InicializeWinConsole()
{
    if (!console_.is_initialized)
    {
        console_.is_initialized = true;
        console::HIDE_CURSOR();
    }
}

ConsoleFrame::ConsoleFrame(const Coordinates &position) : position_(position)
{
    InicializeWinConsole();
}

ConsoleFrame::ConsoleFrame(const ConsoleFrame & other)  : position_(other.position_)
{
    InicializeWinConsole();
}

ConsoleFrame &ConsoleFrame::operator=(const ConsoleFrame & other)
{
    position_ = other.position_;
    return *this;
}

ConsoleFrame::ConsoleFrame()
{
    console::SHOW_CURSOR();
}

void ConsoleFrame::Print(const BattleField &battle_field) const
{
    auto count = battle_field.field_.size();
    for (int i = 0; i < count; i++)
    {
        // шкала глубины
        Print(static_cast<char>('0' + (count - 1 - i) % 10), {i, 0});
        // само поле игры
        Print(battle_field.field_[i], {i, 1});
    }
    fflush(stdout);
}

ConsoleFrame::WinConsole ConsoleFrame::console_;

template <class T>
void ConsoleFrame::Print(const T &text, Coordinates position) const
{

    {
        std::lock_guard m(console_.mutex_);
        console::MOVETO(position_.w_ + position.w_, position_.h_ + position.h_);
        //std::cout << text;
        if constexpr (std::is_same_v<T, std::string>)
        {
            printf("%s", text.c_str());
        } else if constexpr (std::is_same_v<T, int>) {
            printf("%d", text);
        } else if constexpr (std::is_same_v<T, char>) {
            printf("%c", text);
        }

        
    }
}

std::mutex ConsoleFrame::WinConsole::mutex_;