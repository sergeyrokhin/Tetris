#include "control.h"
#include "consoleframe.h"
#include "nixcursor.h"

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace console
{

    void clear_screen()
    {
        CLEAR();
    }

    void hide_cursor()
    {
        HIDE_CURSOR();
    }

    void enable_cursor()
    {
        RESET_CURSOR();
    }

    int kbhit_my()
    {
        static const int STDIN = 0;
        static bool initialized = false;

        if (!initialized)
        {
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

    void ConsoleFrame::MoveCursorTo(Coordinates position) const
    {
        console::MOVETO(position_.w_ + position.w_, position_.h_ + position.h_);
    }

    void ConsoleFrame::OutToConsole(std::string_view buf, Coordinates position) const
    {
        console::MOVETO(position_.w_ + position.w_, position_.h_ + position.h_);
        printf("%s", buf.data());
        fflush(stdout);
    }

    void ConsoleFrame::InicializeWinConsole()
    {
    }
}

void ControlCenter::InitScreen([[maybe_unused]] Coordinates size)
{
}
