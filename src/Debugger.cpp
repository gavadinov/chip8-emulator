#include <map>
#include <iostream>
#include <SDL_keycode.h>
#include "Debugger.h"

const static std::map<BYTE, char> keyMap = {
        {0x1, SDLK_1},
        {0x2, SDLK_2},
        {0x3, SDLK_3},
        {0xC, SDLK_4},

        {0x4, SDLK_q},
        {0x5, SDLK_w},
        {0x6, SDLK_e},
        {0xD, SDLK_r},

        {0x7, SDLK_a},
        {0x8, SDLK_s},
        {0x9, SDLK_d},
        {0xE, SDLK_f},

        {0xA, SDLK_z},
        {0x0, SDLK_x},
        {0xB, SDLK_c},
        {0xF, SDLK_v}
};

void Debugger::opcode(WORD opcode) {
    //    std::cout << "Executing opcode: 0x" << std::hex << opcode << std::endl;
}

void Debugger::keyDown(BYTE key) {
    std::cout << "Key DOWN: " << keyMap.at(key) << std::endl;
}

void Debugger::keyUp(BYTE key) {
    std::cout << "Key UP: " << keyMap.at(key) << std::endl;
}

void Debugger::checkKeyPressed(BYTE key) {
    std::cout << "Expect Key Pressed: " << keyMap.at(key) << std::endl;
}

void Debugger::checkKeyNotPressed(BYTE key) {
    std::cout << "Expect Key NOT Pressed: " << keyMap.at(key) << std::endl;
}
