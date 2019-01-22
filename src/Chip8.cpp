#include <fstream>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <SDL2/SDL_keycode.h>
#include "Chip8.h"

static void unknownOpcode(WORD opcode) {
    std::cout << "Unknown opcode: 0x" << std::hex << opcode << std::endl;
}

BYTE fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, //0
        0x20, 0x60, 0x20, 0x20, 0x70, //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
        0x90, 0x90, 0xF0, 0x10, 0x10, //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
        0xF0, 0x10, 0x20, 0x40, 0x40, //7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
        0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void Chip8::init() {
    pc = START_ADDRESS;
    sp = 0;
    opcode = 0;
    I = 0;
    delay_timer = 0;
    sound_timer = 0;

    memset(V, 0, 16);
    memset(stack, 0, 16);
    memset(memory, 0, 4096);
    memset(keys, 0, 16);
    memset(screen, 0, SCREEN_SIZE);

    memcpy(memory, fontset, 80);
}

bool Chip8::loadApplication(const std::string filename) {
    init();

    std::ifstream file;
    file.open(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size > 4096) {
        std::cout << "File is too big" << std::endl;
        return false;
    }
    file.read((char*) (memory + START_ADDRESS), size);
    file.close();

    return true;
}

void Chip8::cycle() {
    drawFlag = false;
    // The size of the next opcode is 2 bytes so we merge pc and pc+1
    opcode = (memory[pc] << 8) | memory[pc + 1];
    std::cout << "Executing opcode: 0x" << std::hex << opcode << std::endl;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000: // 0x00E0 - Clears the screen.
                    memset(screen, 0, SCREEN_SIZE);
                    drawFlag = true;
                    pc += 2;
                    break;
                case 0x000E: // 0x00EE - Returns from a subroutine.
                    pc = stack[--sp];
                    pc += 2;
                    break;
                default:
                    unknownOpcode(opcode);
            }
            break;
        case 0x1000: // 0x1NNN - Jumps to address NNN.
            pc = static_cast<WORD>(opcode & 0x0FFF);
            break;
        case 0x2000: // 0x2NNN - Calls subroutine at NNN.
            stack[sp++] = pc;
            pc = static_cast<WORD>(opcode & 0x0FFF);
            break;
        case 0x3000: // 0x3XNN - Skips the next instruction if VX equals NN.
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x4000: // 0x4XNN - Skips the next instruction if VX doesn't equal NN.
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x5000: // 0x5XY0 - Skips the next instruction if VX equals VY.
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x6000: // 0x6XNN - Sets VX to NN.
            V[(opcode & 0x0F00) >> 8] = static_cast<BYTE>(opcode & 0x00FF);
            pc += 2;
            break;
        case 0x7000: // 0x7XNN - Adds NN to VX. (Carry flag is not changed)
            V[(opcode & 0x0F00) >> 8] += static_cast<BYTE>(opcode & 0x00FF);
            pc += 2;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000: // 0x8XY0 - Sets VX to the value of VY.
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0001: // 0x8XY1 - Sets VX to VX | VY.
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0002: // 0x8XY2 - Sets VX to VX & VY.
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0003: // 0x8XY3 - Sets VX to VX ^ VY.
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0004: // 0x8XY4 - Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
                    if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) { // overflow check
                        V[0xF] = 1; // carry
                    } else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0005: // 0x8XY5 - VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    if (V[(opcode & 0x00F0) >> 4] > (V[(opcode & 0x0F00) >> 8])) { // borrow check
                        V[0xF] = 0; // borrow
                    } else {
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0006: // 0x8XY6 - Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
                    V[0xF] = static_cast<BYTE>(V[(opcode & 0x0F00) >> 8] & 0x1);
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;
                case 0x0007: // 0x8XY7 - Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    if (V[(opcode & 0x0F00) >> 8] > (V[(opcode & 0x00F0) >> 4])) { // borrow check
                        V[0xF] = 0; // borrow
                    } else {
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x000E: // 0x8XYE - Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
                    V[0xF] = static_cast<BYTE>(V[(opcode & 0x0F00) >> 8] >> 7);
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                    break;
                default:
                    unknownOpcode(opcode);
            }
            break;
        case 0x9000: // 0x9XY0 - Skips the next instruction if VX doesn't equal VY.
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0xA000: // 0xANNN - Sets I to the address NNN.
            I = static_cast<WORD>(opcode & 0x0FFF);
            pc += 2;
            break;
        case 0xB000: // 0xBNNN - Jumps to the address NNN plus V0.
            pc = static_cast<WORD>((opcode & 0x0FFF) + V[0]);
            break;
        case 0xC000: // 0xCXNN - Sets VX to the result of a random number & NN.
            V[(opcode & 0x0F00) >> 8] = static_cast<BYTE>((rand() % 255) & (opcode & 0x00FF));
            pc += 2;
            break;
        case 0xD000: // 0xDXYN - Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
            // Each row of 8 pixels is read as bit-coded starting from memory location I;
            // I value doesn't change after the execution of this instruction.
            // VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
            // and to 0 if that doesn't happen.
        {
            auto x = V[(opcode & 0x0F00) >> 8];
            auto y = V[(opcode & 0x00F0) >> 4];
            auto height = opcode & 0x000F;
            BYTE pixel;
            int screenIndex;

            V[0xF] = 0;
            for (int row = 0; row < height; ++row) {
                pixel = memory[I + row];
                for (int col = 0; col < 8; ++col) {
                    if ((pixel & (0x80 >> col)) != 0) {
                        screenIndex = x + col + ((y + row) * SCREEN_W);
                        if (screen[screenIndex] == 1) {
                            V[0xF] = 1; // collision
                        }
                        screen[screenIndex] ^= 1;
                    }
                }
            }

            drawFlag = true;
            pc += 2;
            break;
        }
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x009E: // 0xEX9E - Skips the next instruction if the key stored in VX is pressed.
//                {
//                    auto key = (opcode & 0x0F00) >> 8;
//                    std::cout << "Check if IS pressed: " << (char)keyMap.at(key) << std::endl;
//                }
                    if (keys[(opcode & 0x0F00) >> 8] != 0) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                case 0x00A1: // 0xEXA1 - Skips the next instruction if the key stored in VX isn't pressed.
//                {
//                    auto key = (opcode & 0x0F00) >> 8;
//                    std::cout << "Check if NOT pressed: " << (char)keyMap.at(key) << std::endl;
//                }
                    if (keys[(opcode & 0x0F00) >> 8] == 0) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                default:
                    unknownOpcode(opcode);
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007: // 0xFX07 - Sets VX to the value of the delay timer.
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                    break;
                case 0x000A: // 0xFX0A - A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
                {
                    bool keyPressed = false;
                    for (BYTE key : keys) {
                        if (key != 0) {
                            V[(opcode & 0x0F00) >> 8] = key;
                            keyPressed = true;
                            break;
                        }
                    }

                    if (!keyPressed) {
                        return; // Skip this cycle and try again
                    }

                    pc += 2;
                    break;
                }
                case 0x0015: // 0xFX15 - Sets the delay timer to VX.
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x0018: // 0xFX18 - Sets the sound timer to VX.
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x001E: // 0xFX1E - Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.
                    if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x0029: // 0xFX29 - Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    I = static_cast<WORD>(V[(opcode & 0x0F00) >> 8] * 0x5);
                    pc += 2;
                    break;
                case 0x0033: // 0xFX33 - Stores the binary-coded decimal representation of VX,
                    // with the most significant of three digits at the address in I,
                    // the middle digit at I plus 1, and the least significant digit at I plus 2.
                    memory[I] = static_cast<BYTE>(V[(opcode & 0x0F00) >> 8] / 100);
                    memory[I + 1] = static_cast<BYTE>((V[(opcode & 0x0F00) >> 8] / 10) % 10);
                    memory[I + 2] = static_cast<BYTE>((V[(opcode & 0x0F00) >> 8] % 100) % 10);
                    pc += 2;
                    break;
                case 0x0055: // 0xFX55 - Stores V0 to VX (including VX) in memory starting at address I.
                    // The offset from I is increased by 1 for each value written, but I itself is left unmodified.
                    for (int ii = 0; ii <= (opcode & 0x0F00) >> 8; ++ii) {
                        memory[I + ii] = V[ii];
                    }
                    pc += 2;
                    break;
                case 0x0065: // 0xFX65 - Fills V0 to VX (including VX) with values from memory starting at address I.
                    // The offset from I is increased by 1 for each value written, but I itself is left unmodified.
                    for (int ii = 0; ii <= (opcode & 0x0F00) >> 8; ++ii) {
                        V[ii] = memory[I + ii];
                    }
                    pc += 2;
                    break;
                default:
                    unknownOpcode(opcode);
            }
            break;
        default:
            unknownOpcode(opcode);
    }

    // Update timers
    if (delay_timer > 0) {
        --delay_timer;
    }

    if (sound_timer > 0) {
        if (sound_timer == 1) {
            printf("BEEP!\n");
        }
        --sound_timer;
    }
}

bool Chip8::getDrawFlag() {
    return drawFlag;
}

BYTE* Chip8::getScreen() {
    return screen;
}

void Chip8::pressKey(BYTE key) {
    std::cout << "Key DOWN: " << (int)key << std::endl;
    keys[key] = 1;
}

void Chip8::releaseKey(BYTE key) {
    std::cout << "Key UP: " << (int)key << std::endl;
    keys[key] = 0;
}
