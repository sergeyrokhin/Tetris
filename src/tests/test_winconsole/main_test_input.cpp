#include "console/winconsole.h"
#include "common.h"

int main(int argc, char const *argv[])
{
std::cout << "Type console. ESC - exit" << std::endl;
   while (true)
   {
    //    while( !console::kbhit() ) { 
    //        wait();
    //    }
       auto ch = console::getch_my();
       switch (ch)
       {
       case 27:
        return 0;
        break;
       
       case 52:
        break;

       case 53:
        break;

       case 57:
        break;

       case 54:
        break;

       case 55:
        break;

       case 56:
           break;

       default:
        break;
       }
       std::cout << ch << std::endl;
   }
    
    return 0;
}
