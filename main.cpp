#include <iostream>
#include "src/Chip8.h"

int main() {
	Chip8 chip8{};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
	for (;;) {
		chip8.cycle();
	}
#pragma clang diagnostic pop
}