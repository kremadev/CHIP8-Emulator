#include "cpu.h"

uint8_t buffer[32*64];
static uint8_t memory[4096];
static uint8_t V[16];
static uint16_t I;
static uint16_t stack[16];
static uint8_t  sp = 0;
static uint16_t pc = 0x200;
static uint16_t opcode;
static uint8_t delay_timer;
static uint8_t sound_timer;
uint8_t keypad[16];

static const uint8_t font[80] = {
	0xF0,0x90,0x90,0x90,0xF0, // 0
	0x20,0x60,0x20,0x20,0x70, // 1
	0xF0,0x10,0xF0,0x80,0xF0, // 2
	0xF0,0x10,0xF0,0x10,0xF0, // 3
	0x90,0x90,0xF0,0x10,0x10, // 4
	0xF0,0x80,0xF0,0x10,0xF0, // 5
	0xF0,0x80,0xF0,0x90,0xF0, // 6
	0xF0,0x10,0x20,0x40,0x40, // 7
	0xF0,0x90,0xF0,0x90,0xF0, // 8
	0xF0,0x90,0xF0,0x10,0xF0, // 9
	0xF0,0x90,0xF0,0x90,0x90, // A
	0xE0,0x90,0xE0,0x90,0xE0, // B
	0xF0,0x80,0x80,0x80,0xF0, // C
	0xE0,0x90,0x90,0x90,0xE0, // D
	0xF0,0x80,0xF0,0x80,0xF0, // E
	0xF0,0x80,0xF0,0x80,0x80  // F
};

int cpu_load_rom(const char *path) {
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		printf("Error file not found\n");
		return 1;
	}
	int n = fread(memory+0x200, 1, 3584, file);
	fclose(file);
	memcpy(memory+0x50, font, 80);
	if (n <= 1) {
		printf("Read error\n");
		return 1;
	}
	printf("Loaded ROM: %s\n", path);
	srand(time(NULL));
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
			switch (NNN) {
				case 0x0e0: 
					memset(buffer, 0, sizeof(buffer));
					break;
				case 0x0ee: 
					sp--;
					pc = stack[sp];
					break;
				default:
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
				if (y + r >= 32) break;
				uint8_t row = memory[I + r];
				uint8_t place = 128;
				for (int col = 0; col < 8; col++) {
					if (x + col >= 64) break;
					if ((row / place) % 2 == 1) {
						uint8_t *pixel = &buffer[(y + r) * 64 + x + col];
						if (*pixel == 1) V[0xF] = 1;
						*pixel ^= 1;
					}
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
		case 0xC:
			V[X] = rand() & NN;
			break;
		case 0xF:
			switch (NN) {
				case 0x7:
					V[X] = delay_timer;
					break;
				case 0x15:
					delay_timer = V[X];
					break;
				case 0x18:
					sound_timer = V[X];
					break;
				case 0x1E:
					I += V[X];
					break;
				case 0x55:
					for (int i = 0; i <= X; i++) memory[I + i] = V[i];
					break;
				case 0x65:
					for (int i = 0; i <= X; i++) V[i] = memory[I + i];
					break;
				case 0x33:
					memory[I] = V[X] / 100;
					memory[I + 1] = (V[X] / 10) % 10;
					memory[I + 2] = V[X] % 10;
					break;
				case 0x29:
					I = 0x50 + V[X] * 5;
					break;
				case 0xA:
					bit = 0;
					for (int i = 0; i < 16; i++) {
						if (keypad[i]) {
							V[X] = i;
							bit = 1;
						}
					}
					if (!bit) pc -= 2;  
					break;	
				default:
					printf("ERROR: Invalid instruction \"%04x\"\n", opcode);
					exit(1);
			} break;
		case 0xB:
			pc = NNN + V[0];
			break;
		case 0xE:
			switch (NN) {
				case 0x9E:
					if (keypad[V[X]]) pc += 2;
					break;
				case 0xA1:
					if (!keypad[V[X]]) pc += 2;
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

void cpu_tick_timers(void) {
	if (delay_timer > 0) delay_timer--;
	if (sound_timer > 0) sound_timer--;
}
