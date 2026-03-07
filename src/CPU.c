#include "../lib/CPU.h"
#include "../lib/bus.h"

#include <stdint.h>
#include <string.h>

// Initialize the CPU 
void CPU_Init(CPU *cpu) {
    cpu->SR = SR_Interrupt;  
}

// Intialize the BUS
void CPU_BusInti(Bus *bus) {
    memset(bus->RAM, 0, 65536);
}


// Set the pc according to the reset vector
void CPU_reset(CPU *cpu, Bus *bus) {
    cpu->SP = 0xFF;
    uint16_t prog = BUS_ReadAddr(bus, 0xFFFD) << 8;
    prog |= BUS_ReadAddr(bus, 0xFFFC);
    cpu->PC = prog;
}