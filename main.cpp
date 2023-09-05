#include "battlefield.h"
#include "control.h"
#include <iostream>


int main(int argc, char const *argv[])
{
    /* code */
    
    std::cout << "hello" << std::endl;

    Coordinates size = {10, 12};
    // Tetris tetris(size);
    // BattleField field(size);
    std::shared_ptr<Tetris> tet ( new Tetris(size));
    std::shared_ptr<BattleField> fld ( new BattleField(size));

    ControlCenter cc(tet, fld);
    cc.StartProcess();
   
    std::cout<< std::endl << "Game over." ;
    return 0;
}