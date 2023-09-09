#include "battlefield.h"
#include "control.h"
#include <iostream>


int main(int argc, char const *argv[])
{
    /* code */
    
    std::cout << "hello" << std::endl;

    ControlCenter cc({ 10, 12 });
    cc.StartProceses();
   
    std::cout<< std::endl << "Game over." ;
    return 0;
}