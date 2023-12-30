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

    void ConsoleFrame::Print(const BattleField &battle_field) const
    {
        int count = static_cast<int>(battle_field.field_.size());
        std::string buf_string;

        std::lock_guard m(mutex_console);
        for (int i = 0; i < count; i++)
        {
            buf_string.resize(1);
            // шкала глубины
            buf_string[0] = static_cast<char>('0' + (count - 1 - i) % 10);
            // само поле игры
            buf_string += battle_field.field_[i];
            OutToConsole(buf_string, {i, 0});
        }
    }

    std::mutex ConsoleFrame::mutex_console;

}
