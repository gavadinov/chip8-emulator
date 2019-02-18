#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include <string>
#include "types.h"
#include "Debugger.h"

class Chip8 {
public:
    const static auto SCREEN_W = 64;
    const static auto SCREEN_H = 32;
    const static size_t SCREEN_SIZE = SCREEN_W * SCREEN_H;

    Chip8(Debugger* debugger);

    bool getDrawFlag();

    BYTE* getScreen();

    void pressKey(BYTE key);
    void releaseKey(BYTE key);

    void cycle();

    bool loadApplication(std::string filename);

private:
    const static auto START_ADDRESS = 512;

    Debugger* debugger;

    WORD pc;                // Program counter
    WORD opcode;            // Current opcode
    WORD I;                // Index register
    WORD sp;                // Stack pointer

    BYTE V[16];            // V-registers (V0-VF)
    WORD stack[16];        // Stack (16 levels)
    BYTE memory[4096];    // Memory (size = 4k)

    BYTE delay_timer;        // Delay timer
    BYTE sound_timer;        // Sound timer

    BYTE screen[SCREEN_SIZE];    // Total amount of pixels: 2048
    BYTE keys[16];

    bool drawFlag;

    void init();
};

#endif //CHIP8_CHIP8_H
