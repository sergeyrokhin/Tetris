#include "../../console/winconsole.h"


int main(int argc, char const *argv[])
{
    BattleField bf({20, 40});
    ConsoleFrame cf({5,2});
    cf.Print(bf);
    return 0;
}
