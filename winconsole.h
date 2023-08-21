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
    void Print(const std::string& text, Coordinates position);
   public:
	ConsoleFrame();
    void Print(const BattleField &field);
};

