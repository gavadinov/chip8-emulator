#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include <string>

typedef unsigned char BYTE;

typedef unsigned short WORD;

class Chip8 {
public:
    Chip8() = default;

    ~Chip8() = default;

    bool getDrawFlag();

    void cycle();

    bool loadApplication(std::string filename);

private:
    const static auto START_ADDRESS = 512;
    const static auto SCREEN_W = 64;
    const static auto SCREEN_H = 32;

    WORD pc;                // Program counter
    WORD opcode;            // Current opcode
    WORD I;                // Index register
    WORD sp;                // Stack pointer

    BYTE V[16];            // V-registers (V0-VF)
    WORD stack[16];        // Stack (16 levels)
    BYTE memory[4096];    // Memory (size = 4k)

    BYTE delay_timer;        // Delay timer
    BYTE sound_timer;        // Sound timer

    BYTE screen[SCREEN_W * SCREEN_H];    // Total amount of pixels: 2048
    BYTE key[16];

    bool drawFlag;

    void init();

};

#endif //CHIP8_CHIP8_H
