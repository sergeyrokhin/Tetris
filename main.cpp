#include "battlefield.h"
#include "control.h"
#include <iostream>


int main(int argc, char const *argv[])
{
    /* code */
    
    std::cout << "hello" << std::endl;

    Coordinates size = {18, 12};
    // Tetris tetris(size);
    // BattleField field(size);
    std::shared_ptr<Tetris> tet ( new Tetris(size));
    std::shared_ptr<BattleField> fld ( new BattleField(size));

    ControlCenter cc(tet, fld);
    cc.StartProcess();
   
    std::cout << "Game over. Press any key" ;
    std::getchar();
    return 0;
}