#ifndef SYSTEM_H
#define SYSTEM_H

#include "../lib/CPU.h"
#include "../lib/BUS.h"

typedef struct system{
    CPU CPU;
    Bus bus;
}System;

// INitlaize the system
void SYS_Init(System *system);


// Load the rom into memory
int SYS_LoadRom(System *sys, char *rom_name);

// Main system loop
int SYS_MainLoop(System *system);



#endif