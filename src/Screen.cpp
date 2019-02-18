#include "Screen.h"
#include "SDL_rect.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"

Screen::Screen(const int w, const int h):
		window(nullptr),
		renderer(nullptr),
		texture(nullptr),
		buffer(nullptr),
		w(w),
		h(h) {}

Screen::~Screen() {
	delete []buffer;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Screen::init() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		return false;
	}

	window = SDL_CreateWindow(
			"CHIP8",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			w,
			h,
			SDL_WINDOW_SHOWN
	);

	if (window == nullptr) {
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC,
			w,
			h
	);

	if (texture == nullptr) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	buffer = new Uint32[w * h];

	memset(buffer, 0, w * h * sizeof(Uint32));

	return true;
}

void Screen::update() {
	SDL_UpdateTexture(texture, nullptr, buffer, w * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

// We want to display each pixel from the chip8 memory as a square.
void Screen::renderSquare(const int x, const int y, const int squareWidth, const Uint8 red, const Uint8 green, const Uint8 blue) {
    Uint32 color = (red << 24) | (green << 16) | (blue << 8) | 0xFF;
	for (int yy = 0; yy < squareWidth; ++yy) {
		for (int xx = 0; xx < squareWidth; ++xx) {
			buffer[((x * squareWidth) + xx) + (((y * squareWidth) + yy) * w)] = color;
		}
	}
}

void Screen::renderText(int x, int y, const Uint8 red, const Uint8 green, const Uint8 blue)
{
    SDL_Color color = {red, green, blue};
    TTF_Font* sans = TTF_OpenFont("Sans.ttf", 24);

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, "put your text here", color);

    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect messageRect;
    messageRect.x = x;
    messageRect.y = y;
    messageRect.w = 100;
    messageRect.h = 100;

    SDL_RenderCopy(renderer, message, nullptr, &messageRect);
}
