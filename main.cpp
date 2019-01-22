#include <iostream>
#include "src/Chip8.h"
#include "src/Screen.h"
#include "src/EventHandler.h"

int main(int argc, char** argv) {
    const int modifier = 20;
    const int window_w = modifier * Chip8::SCREEN_W;
    const int window_h = modifier * Chip8::SCREEN_H;

    if (argc < 2) {
        std::cout << "Missing rom file. Usage: chip8 APPLICATION.c8" << std::endl;
        return 1;
    }

    Chip8 chip8{};
    Screen screen(window_w, window_h);
    EventHandler eventHandler(&chip8);
    if (!screen.init()) {
        std::cout << SDL_GetError() << std::endl;
    }

    chip8.loadApplication(argv[1]);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (;;) {
        chip8.cycle();

        if (chip8.getDrawFlag()) {
            for (int y = 0; y < Chip8::SCREEN_H; ++y) {
                for (int x = 0; x < Chip8::SCREEN_W; ++x) {
                    if (chip8.getScreen()[x + (y * Chip8::SCREEN_W)] == 1) {
                        screen.setSquare(x, y, modifier, 0xFFFFFFFF);
                    } else {
                        screen.setSquare(x, y, modifier, 0);
                    }
                }
            }
            screen.update();
        }

        eventHandler.processEvent();
    }
#pragma clang diagnostic pop
}