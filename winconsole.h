#pragma once

#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "common.h"

#include "battlefield.h"

class ConsoleFrame
{
	HANDLE hStdout;
	int y_zero;
//    void Print(const std::string& text, Coordinates position) const;
    template <class T>
    void Print(const T& text, Coordinates position) const;
public:
	ConsoleFrame();
    void Process(const BattleField &field);
    void Print(const BattleField &field) const;
    bool game_is_run_ = false;
};

