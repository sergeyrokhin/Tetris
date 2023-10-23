#include <iostream>

#include "control.h"
#include "console/winconsole.h"



int main(int argc, char const *argv[])
{
    /* code */
    
    std::cout << "Control key: Space - Exit" << std::endl;
    std::cout << "4: Left; 6: Right; 9: Down" << std::endl;
    std::cout << "7:RotateLeft; 9: RotateRight; 5: Drop" << std::endl;

    //Размер стакана 16 на 15
	ControlCenter cc({ 16, 15 });

    //Два поля для отрисовки стакана с соответствующими координатами.
    cc.AddObserver(ConsoleFrame({8, 1}));
    cc.AddObserver(ConsoleFrame({10, 30}));
    cc.StartProceses();
   
    std::cout<< std::endl << "Game over." ;
    return 0;
}