#pragma once
#include <stdio.h>
//Только в консоли linux
namespace console {

// clear the screen
inline void CLEAR() {printf("\033[2J");}
 
// Move the cursor up
inline void MOVEUP(int x) {printf("\033[%dA", (x));}
 
// move the cursor down
inline void MOVEDOWN(int x) {printf("\033[%dB", (x));}
 
// move the cursor left
inline void MOVELEFT(int y) {printf("\033[%dD", (y));}
 
// right cursor
inline void MOVERIGHT(int y) {printf("\033[%dC",(y));}
 
// positioning cursor
inline void MOVETO(int x, int y) {printf("\033[%d;%dH", (y), (x));}
 
// cursor reset
inline void RESET_CURSOR() {printf("\033[H");}
 
// hide the cursor
inline void HIDE_CURSOR() {printf("\033[?25l");}
 
// show cursor
inline void SHOW_CURSOR() {printf("\033[?25h");}
 
//Reverse display
inline void HIGHT_LIGHT() {printf("\033[7m");}
inline void UN_HIGHT_LIGHT() {printf("\033[27m");}

} //namespace console
