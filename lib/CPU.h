#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>

// Structure of the 6502
typedef struct {
    uint8_t AC;         // Accumulator
    uint8_t XR;         // 8-bit reg X
    uint8_t YR;         // 8-bit reg Y
    uint16_t PC;        // Program Counter
    uint8_t SP;         // Stack Pointer
    uint8_t SR;         // Status Register
}CPU;


// Status Register status
enum {
    SR_Carry        = 1 << 0,
    SR_Zero         = 1 << 1,
    SR_Interrupt    = 1 << 2,
    SR_Decimal      = 1 << 3,
    SR_Break        = 1 << 4,
    SR_Unused       = 1 << 5,      // This doesn't get used and is always 1
    SR_Overflow     = 1 << 6,
    SR_Negative     = 1 << 7
}Staus_Reg_State;

// Initialize the CPU 
void CPU_Init(CPU *cpu);



#endif
