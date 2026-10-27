#include "cpu.h"

uint8_t buffer[32*64];
static uint8_t memory[4096];
static uint8_t V[16];
static uint16_t I;
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
	printf("%04x\n", opcode);
	pc += 2;
	pc %= 4096;
	switch (opcode / 0x1000) {
		case 0x1:
			pc = opcode % 0x1000;
			break;
		case 0xA:
			printf("ANNN\n");
			I = opcode % 0x1000;
			break;
		case 0x0:
			if (opcode == 0x00e0) {
				memset(buffer, 0, sizeof(buffer));
			}
			break;
		case 0x6:
			uint8_t X = (opcode / 0x100) % 0x10;
			uint8_t NN = opcode % 0x100;
			V[X] = NN;
			break;
	}
}
