#pragma once

#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "common.h"

#include "battlefield.h"
#include "observer.h"

class ConsoleFrame
{
	HANDLE hStdout;
	int y_zero;
//    void Print(const std::string& text, Coordinates position) const;
    template <class T>
    void Print(const T& text, Coordinates position) const;
public:
	ConsoleFrame();
    //void ConsoleProcessThread(const std::shared_ptr<BattleField> field_ptr);
    void Print(const std::shared_ptr<BattleField> field_ptr) const;
};

//template <typename T>
//void Observer::ObserverObject<T>::Draw(){
//    ::Draw();
//}

