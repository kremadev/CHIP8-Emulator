#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern uint8_t buffer[32*64];

int cpu_load_rom(const char *path); 
void cpu_cycle(void);

#endif
