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
	if (opcode / 0x1000 == 0x1) {
		printf("jmp\n");
		pc = opcode % 0x1000;
	}
}
