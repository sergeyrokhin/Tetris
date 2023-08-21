#include "battlefield.h"
#include "winconsole.h"
#include <iostream>


int main(int argc, char const *argv[])
{
    /* code */
    
    std::cout << "hello" << std::endl;

    Coordinates size = {18, 8};
    ConsoleFrame console;
    Tetris tetris(size);
    BattleField field(size);

    tetris.Start();
    tetris.NewTetramino();

    //(TetraminoType i = TetraminoType::I; i <= TetraminoType::T; i = static_cast<TetraminoType>((int)i + 1))
    while (tetris.GameIsStart())
    {
    draw(field, tetris);
    //std::cout << field << std::endl;
     console.Print(field);

    tetris.Move(MoveType::Left);
    draw(field, tetris);
     console.Print(field);

    tetris.Move(MoveType::Drop);
    draw(field, tetris);
     console.Print(field);

     std::getchar();

    }
    
    std::getchar();
    return 0;
}