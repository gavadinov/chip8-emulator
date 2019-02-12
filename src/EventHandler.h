#ifndef CHIP8_EVENTHANDLER_H
#define CHIP8_EVENTHANDLER_H

#include <SDL_events.h>
#include <map>
#include "Chip8.h"
#include "types.h"

class EventHandler {
public:
    explicit EventHandler(Chip8* chip8);
    void processEvent();

private:
    Chip8* chip8;
    const std::map<SDL_Keycode, BYTE> keyMap = {
            {SDLK_1, 0x1},
            {SDLK_2, 0x2},
            {SDLK_3, 0x3},
            {SDLK_4, 0xC},

            {SDLK_q, 0x4},
            {SDLK_w, 0x5},
            {SDLK_e, 0x6},
            {SDLK_r, 0xD},

            {SDLK_a, 0x7},
            {SDLK_s, 0x8},
            {SDLK_d, 0x9},
            {SDLK_f, 0xE},

            {SDLK_z, 0xA},
            {SDLK_x, 0x0},
            {SDLK_c, 0xB},
            {SDLK_v, 0xF}
    };

    SDL_Event event;

    void keyUp(SDL_Keycode keyCode);
    void keyDown(SDL_Keycode keyCode);
};

#endif //CHIP8_EVENTHANDLER_H
