#include <iostream>
#include "EventHandler.h"

void EventHandler::processEvent() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_KEYDOWN:
                keyDown(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                keyUp(event.key.keysym.sym);
                break;
            default:break;
        }
    }
}

void EventHandler::keyUp(const SDL_Keycode keyCode) {
    if (keyMap.find(keyCode) != keyMap.end()) {
        chip8->releaseKey(keyMap.at(keyCode));
    } else if (keyCode == SDLK_ESCAPE) {
        exit(0);
    }
}

void EventHandler::keyDown(const SDL_Keycode keyCode) {
    if (keyMap.find(keyCode) != keyMap.end()) {
        chip8->pressKey(keyMap.at(keyCode));
    }
}

EventHandler::EventHandler(Chip8* chip8)
        :chip8(chip8) { }
