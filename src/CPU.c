#include "../lib/CPU.h"
#include "../lib/BUS.h"

#include <stdint.h>
#include <string.h>

// Initialize the CPU 
void CPU_Init(CPU *cpu) {
    cpu->SR = SR_Interrupt;  
}


// Set the pc according to the reset vector
void CPU_reset(CPU *cpu, Bus *bus) {
    cpu->SP = 0xFF;
    uint16_t prog = BUS_ReadAddr(bus, 0xFFFD) << 8;
    prog |= BUS_ReadAddr(bus, 0xFFFC);
    cpu->PC = prog;
}


// Fetch bytes from memory
uint8_t CPU_FetchByte(CPU *cpu) {
    uint8_t byte = BUS_ReadAddr(cpu->bus, cpu->PC);
    cpu->PC++;
    return byte;
}