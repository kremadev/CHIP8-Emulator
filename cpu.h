#ifndef CPU_H
#define CPU_H

#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern uint8_t buffer[32*64];
extern uint8_t keypad[16];

int cpu_load_rom(const char *path); 
void cpu_cycle(void);
void cpu_tick_timers(void);

#endif
