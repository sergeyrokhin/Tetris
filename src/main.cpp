﻿#include "control.h"
#include "console/consoleframe.h"

#include <iostream>

int main()
{
    //Размер стакана 16 на 15
	ControlCenter cc({ 16, 15 });

    //Два поля для отрисовки стакана с соответствующими координатами.
    cc.AddObserver(console::ConsoleFrame({8, 1}));
    cc.AddObserver(console::ConsoleFrame({10, 30}));
    cc.StartProceses();
   
    std::cout<< std::endl << "Game over.\n" ;
    return 0;
}