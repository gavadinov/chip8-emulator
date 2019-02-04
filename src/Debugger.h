#ifndef CHIP8_DEBUGGER_H
#define CHIP8_DEBUGGER_H

#include <SDL2/SDL_keycode.h>
#include <map>
#include <iostream>
#include "types.h"

class Debugger {
public:
    static void opcode(WORD opcode);

    static void keyUp(BYTE key);
    static void keyDown(BYTE key);

    static void checkKeyPressed(BYTE key);
    static void checkKeyNotPressed(BYTE key);
private:

};

#endif //CHIP8_DEBUGGER_H
