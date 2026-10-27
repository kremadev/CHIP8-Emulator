#include "cpu.h"
#include <stdlib.h>

uint8_t buffer[32*64];
static uint8_t memory[4096];
static uint8_t V[16];
static uint16_t I;
static uint16_t stack[16];
static uint8_t  sp = 0;
static uint16_t pc = 0x200;
static uint16_t opcode;

int cpu_load_rom(const char *path) {
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		printf("Error file not found\n");
		return 1;
	}
	int n = fread(memory+0x200, 1, 3584, file);
	fclose(file);
	if (n <= 1) {
		printf("Read error\n");
		return 1;
	}
	printf("Loaded ROM: %s\n", path);
	return 0;
}
void cpu_cycle(void) {
	opcode = memory[pc] * 256 + memory[pc + 1];
	uint8_t X  = (opcode / 0x100) % 0x10;
	uint8_t NN = opcode % 0x100;
	uint16_t NNN = opcode % 0x1000;
	uint8_t Y = (opcode / 0x10) % 0x10;
	uint8_t N = opcode % 0x10;
	uint8_t bit = 0;

	pc += 2;
	pc %= 4096;
	switch (opcode / 0x1000) {
		case 0x1:
			pc = NNN;
			break;
		case 0xA:
			I = NNN;
			break;
		case 0x0:
			if (opcode == 0x00e0) {
				memset(buffer, 0, sizeof(buffer));
			} else if (opcode == 0x00ee) {
				sp--;
				pc = stack[sp];

			} else {
				printf("ERROR: Invalid instruction \"%04x\"\n", opcode);
				exit(1);
			} break;
		case 0x6:
			V[X] = NN;
			break;
		case 0x7:
			V[X] += NN;
			break;
		case 0xD: {
			uint8_t x = V[X] % 64;
			uint8_t y = V[Y] % 32;
			V[0xF] = 0;
			for (int r = 0; r < N; r++) {
				uint8_t row = memory[I + r];
				uint8_t place = 128;
				for (int col = 0; col < 8; col++) {
					if ((row / place) % 2 == 1)
						buffer[(y + r) * 64 + x + col] ^= 1;
					place /= 2;
				}
			} break;
		}
		case 0x2:
			stack[sp] = pc;
			sp++;
			pc  = NNN;
			break;
		case 0x3:
			if (V[X] == NN) {
				pc += 2;
			} break;
		case 0x4:
			if (V[X] != NN) {
				pc += 2;
			} break;
		case 0x5:
			if (V[X] == V[Y]) {
				pc += 2;
			} break;
		case 0x9:
			if (V[X] != V[Y]) {
				pc += 2;
			} break;
		case 0x8:
			switch (N) {
				case 0x0:
					V[X] = V[Y];
					break;
				case 0x1:
					V[X] |= V[Y];
					break;
				case 0x2:
					V[X] &= V[Y];
					break;
				case 0x3:
					V[X] ^= V[Y];
					break;
				case 0x4: {
						uint16_t sum = V[X] + V[Y];
						V[X] = sum;
						V[0xF] = (sum > 255);
					} break;
				case 0x5: {
						int diff = V[X] - V[Y];
						V[X] = diff;
						V[0xF] = !(diff < 0);
					} break;
				case 0x7: {
						int diff = V[Y] - V[X];
						V[X] = diff;
						V[0xF] = !(diff < 0);
					} break;
				case 0x6:
					bit = V[X] & 1;
					V[X] = V[X] >> 1;
					V[0xF] = bit;
					break;
				case 0xE:
					bit = V[X] >> 7;
					V[X] = V[X] << 1;
					V[0xF] = bit;
					break;	
				default:
					printf("ERROR: Invalid instruction \"%04x\"\n", opcode);
					exit(1);
			} break;	
		default:
			printf("ERROR: Invalid instruction \"%04x\"\n", opcode);
			exit(1);
	}
}

