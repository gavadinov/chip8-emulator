#include <fstream>
#include <cstring>
#include <iostream>
#include "Chip8.h"

static void unknownOpcode(WORD opcode) {
	std::cout << "Unknown opcode: 0x" << std::hex << opcode << std::endl;
}

void Chip8::init() {
	pc = START_ADDRESS;
	sp = 0;
	opcode = 0;
	I = 0;
	delay_timer = 0;
	sound_timer = 0;

	memset(V, 0, sizeof(V));
	memset(stack, 0, sizeof(stack));
	memset(memory, 0, sizeof(memory));
	memset(screen, 0, sizeof(screen));
	memset(key, 0, sizeof(key));
}

bool Chip8::loadApplication(const std::string filename) {
	init();

	std::ifstream file;
	file.open(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	if (size > sizeof(memory)) {
		std::cout << "File is too big" << std::endl;
		return false;
	}
	file.read((char*) (memory + START_ADDRESS), size);
	file.close();

	return true;
}

void Chip8::cycle() {
	// The size of the next opcode is 2 bytes so we merge pc and pc+1
	opcode = (memory[pc] << BYTE_SIZE) | memory[pc + 1];

	switch (opcode & 0xF000) {
		case 0x0000:
			switch (opcode & 0x000F) {
				case 0x0000: // 0x00E0 - Clears the screen.
					memset(screen, 0, sizeof(screen));
					drawFlag = true;
					pc += 2;
					break;
				case 0x000E: // 0x00EE - Returns from a subroutine.
					pc = stack[-- sp];
					pc += 2;
					break;
				default:
					unknownOpcode(opcode);
			}
			break;
		case 0x1000: // 0x1NNN - Jumps to address NNN.
			pc = static_cast<WORD>(opcode & 0x0FFF);
			break;
		case 0x2000: // 0x2NNN - Calls subroutine at NNN.
			break;
		case 0x3000: // 0x3XNN - Skips the next instruction if VX equals NN.
			break;
		case 0x4000: // 0x4XNN - Skips the next instruction if VX doesn't equal NN.
			break;
		case 0x5000: // 0x5XY0 - Skips the next instruction if VX equals VY.
			break;
		case 0x6000: // 0x6XNN - Sets VX to NN.
			break;
		case 0x7000: // 0x7XNN - Adds NN to VX. (Carry flag is not changed)
			break;
		case 0x8000:
			switch (opcode & 0x000F) {
				case 0x0000: // 0x8XY0 - Sets VX to the value of VY.
					V[(opcode & 0x0F00) >> (BYTE_SIZE * 2)] = V[(opcode & 0x00F0) >> BYTE_SIZE];
					pc += 2;
					break;
				case 0x0001: // 0x8XY1 - Sets VX to VX | VY.
					break;
				case 0x0002: // 0x8XY2 - Sets VX to VX & VY.
					break;
				case 0x0003: // 0x8XY3 - Sets VX to VX ^ VY.
					break;
				case 0x0004: // 0x8XY4 - Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
					break;
				case 0x0005: // 0x8XY5 - VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					break;
				case 0x0006: // 0x8XY6 - Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
					break;
				case 0x0007: // 0x8XY7 - Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					break;
				case 0x000E: // 0x8XYE - Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
					break;
				default:
					unknownOpcode(opcode);
			}
			break;
		case 0x9000: // 0x9XY0 - Skips the next instruction if VX doesn't equal VY.
			break;
		case 0xA000: // 0xANNN - Sets I to the address NNN.
			I = static_cast<WORD>(opcode & 0x0FFF);
			pc += 2;
			break;
		case 0xB000: // 0xBNNN - Jumps to the address NNN plus V0.
			break;
		case 0xC000: // 0xCXNN - Sets VX to the result of a random number & NN.
			break;
		case 0xD000: // 0xDXYN - Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
			// Each row of 8 pixels is read as bit-coded starting from memory location I;
			// I value doesn't change after the execution of this instruction.
			// VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
			// and to 0 if that doesn't happen
			break;
		case 0xE000:
			switch (opcode & 0x000F) {
				case 0x000E: // 0xEX9E - Skips the next instruction if the key stored in VX is pressed.
					break;
				case 0x0001: // 0xEXA1 - Skips the next instruction if the key stored in VX isn't pressed.
					break;
				default:
					unknownOpcode(opcode);
			}
			break;
		case 0xF000:
			switch (opcode & 0x00FF) {
				case 0x0007: // 0xFX07 - Sets VX to the value of the delay timer.
					break;
				case 0x000A: // 0xFX0A - A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
					break;
				case 0x0015: // 0xFX15 - Sets the delay timer to VX.
					break;
				case 0x0018: // 0xFX18 - Sets the sound timer to VX.
					break;
				case 0x001E: // 0xFX1E - Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.
					break;
				case 0x0029: // 0xFX29 - Sets I to the location of the sprite for the character in VX.
					break;
				case 0x0033: // 0xFX33 - Stores the binary-coded decimal representation of VX,
					// with the most significant of three digits at the address in I,
					// the middle digit at I plus 1, and the least significant digit at I plus 2.
					break;
				case 0x0055: // 0xFX55 - Stores V0 to VX (including VX) in memory starting at address I.
					// The offset from I is increased by 1 for each value written, but I itself is left unmodified.
					break;
				case 0x0065: // 0xFX65 - Fills V0 to VX (including VX) with values from memory starting at address I.
					// The offset from I is increased by 1 for each value written, but I itself is left unmodified.
					break;
				default:
					unknownOpcode(opcode);
			}
			break;
		default:
			unknownOpcode(opcode);
	}
}