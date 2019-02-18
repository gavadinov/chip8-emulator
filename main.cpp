#include <iostream>
#include <thread>
#include <chrono>
#include "src/Chip8.h"
#include "src/Screen.h"
#include "src/EventHandler.h"

int main(int argc, char** argv) {
    const int modifier = 15; // Factor each pixel by this value
    const int window_w = modifier * Chip8::SCREEN_W + Debugger::SCREEN_W;
    const int window_h = modifier * Chip8::SCREEN_H;

    if (argc < 2) {
        std::cout << "Missing rom file. Usage: chip8 APPLICATION.c8" << std::endl;
        return 1;
    }

    Debugger debugger;
    Chip8 chip8(&debugger);
    Screen screen(window_w, window_h);
    EventHandler eventHandler(&chip8);

    if (!screen.init()) {
        std::cout << SDL_GetError() << std::endl;
    }

    chip8.loadApplication(argv[1]);

    for (;;) {
        chip8.cycle();

        if (chip8.getDrawFlag()) {
            for (int y = 0; y < Chip8::SCREEN_H; ++y) {
                for (int x = 0; x < Chip8::SCREEN_W; ++x) {
                    if (chip8.getScreen()[x + (y * Chip8::SCREEN_W)] == 1) {
                        screen.renderSquare(x, y, modifier, 0xFF, 0xFF, 0xFF);
                    } else {
                        screen.renderSquare(x, y, modifier, 0, 0, 0);
                    }
                }
            }
            screen.update();
        }

        eventHandler.processEvent();

        screen.renderText(modifier * Chip8::SCREEN_W + 20, 10, 255, 255, 255);

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Very stupid way to simulate 60Hz
    }
}