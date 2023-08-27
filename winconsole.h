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
	unsigned y_zero;
    void Print(const std::string& text, Coordinates position) const;
   public:
	ConsoleFrame();
    void Process(const BattleField &field) const;
    void Print(const BattleField &field) const;
    bool is_run_ = true;
};

