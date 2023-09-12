#pragma once
#include <mutex>

#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "common.h"

#include "battlefield.h"
#include "observer.h"

class ConsoleFrame
{
    struct WinConsole {
        int y_zero = 0;
        HANDLE hStdout;
        std::mutex mutex_;
        bool is_initialized = false;
    };
    
    static WinConsole console_;
    Coordinates position_;
    template <class T>
    void Print(const T& text, Coordinates position) const;
public:
    ConsoleFrame(const Coordinates& position = {});
    ~ConsoleFrame() = default;
    void Print(const BattleField& battle_field) const;
};

