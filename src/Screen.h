#ifndef SCREEN_H_
#define SCREEN_H_

#include "SDL.h"

class Screen {
public:
    Screen(int h, int w);
    ~Screen();

    bool init();

    void update();

    void renderSquare(const int x, const int y, const int modifier, const Uint8 red, const Uint8 green, const Uint8 blue);
private:
    const int w;
    const int h;

    SDL_Window* window;

    SDL_Renderer* renderer;

    SDL_Texture* texture;

    Uint32* buffer;
};

#endif /* SCREEN_H_ */
