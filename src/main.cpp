#include <iostream>

#include "battlefield.h"
#include "control.h"



int main(int argc, char const *argv[])
{
    /* code */
    
    std::cout << "Control key: Esc - Game over" << std::endl;
    std::cout << "4: Left; 6: Right; 9: Down" << std::endl;
    std::cout << "7:RotateLeft; 9: RotateRight; 5: Drop" << std::endl;

    ControlCenter cc({ 15, 14 });
    cc.StartProceses();
   
    std::cout<< std::endl << "Game over." ;
    return 0;
}