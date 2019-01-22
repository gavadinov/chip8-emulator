#include "Screen.h"

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
void Screen::setSquare(const int x, const int y, const int modifier, const Uint32 color) {
	for (int yy = 0; yy < modifier; ++yy) {
		for (int xx = 0; xx < modifier; ++xx) {
			buffer[((x * modifier) + xx) + (((y * modifier) + yy) * w)] = color;
		}
	}
}

