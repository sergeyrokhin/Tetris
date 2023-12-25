#include "consoleframe.h"

namespace console
{
    ConsoleFrame::ConsoleFrame(const Coordinates &position) : position_(position)
    {
        InicializeWinConsole();
    }

    ConsoleFrame::ConsoleFrame(const ConsoleFrame &other) : position_(other.position_)
    {
        InicializeWinConsole();
    }

    ConsoleFrame &ConsoleFrame::operator=(const ConsoleFrame &other)
    {
        position_ = other.position_;
        return *this;
    }

    ConsoleFrame::ConsoleFrame()
    {
        // CursorShow();
    }

    void ConsoleFrame::Print(const BattleField &battle_field) const
    {
        int count = static_cast<int>(battle_field.field_.size());
        for (int i = 0; i < count; i++)
        {
            // шкала глубины
            Print(static_cast<char>('0' + (count - 1 - i) % 10), {i, 0});
            // само поле игры
            Print(battle_field.field_[i], {i, 1});
        }
        fflush(stdout);
    }

    template <class T>
    void ConsoleFrame::Print(const T &text, Coordinates position) const
    {

        {
            std::lock_guard m(console_.mutex_);
            MoveCursorTo(position);
            // std::cout << text;
            if constexpr (std::is_same_v<T, std::string>)
            {
                printf("%s", text.c_str());
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                printf("%d", text);
            }
            else if constexpr (std::is_same_v<T, char>)
            {
                printf("%c", text);
            }
        }
    }

    std::mutex ConsoleFrame::WinConsole::mutex_;

}
