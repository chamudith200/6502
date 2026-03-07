#ifndef SYSTEM_H
#define SYSTEM_H

#include "../lib/CPU.h"
#include "../lib/bus.h"

typedef struct system{
    CPU CPU;
    Bus bus;
}System;

// INitlaize the system
void SYS_Init(System *system);


// Load the rom into memory
int SYS_LoadRom(Bus *bus, char *rom_name);





#endif