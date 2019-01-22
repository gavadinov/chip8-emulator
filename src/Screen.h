#ifndef SCREEN_H_
#define SCREEN_H_

#include "SDL2/SDL.h"

class Screen {
public:
    Screen(int h, int w);
    ~Screen();

    bool init();

    void update();

    void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
    void setSquare(int x, int y, int modifier, Uint32 color);
private:
    const int w;
    const int h;

    SDL_Window* window;

    SDL_Renderer* renderer;

    SDL_Texture* texture;

    Uint32* buffer;
};

#endif /* SCREEN_H_ */
