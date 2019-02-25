#ifndef CHIP8_DEBUGGER_H
#define CHIP8_DEBUGGER_H

#include "types.h"

class Debugger {
public:
    void opcode(WORD opcode);

    void keyUp(BYTE key);
    void keyDown(BYTE key);

    void checkKeyPressed(BYTE key);
    void checkKeyNotPressed(BYTE key);
private:

};

#endif //CHIP8_DEBUGGER_H
