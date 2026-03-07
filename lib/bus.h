#ifndef BUS_H
#define BUS_H

#include <stdint.h>

// Bus
typedef struct 
{    uint8_t RAM[65536]; // RAM
}Bus;

enum {
    MEM_Zero        = 0x0000,
    MEM_Reset       = 0xFFFC,
    MEM_Stack       = 0x1000,
    MEM_NIMI        = 0xFFFA,
    MEM_BRK         = 0xFFFE,    
    MEM_ProgOffset  = 0x8000
}Important_mem_addr;

// Intialize the BUS 
void Bus_Init(Bus *bus);

// Read bytes from memory
uint8_t BUS_ReadAddr(Bus *bus, uint16_t addr);

// Write byte into memory
void BUS_WriteAddr(Bus *bus, uint16_t addr, uint8_t val);

#endif