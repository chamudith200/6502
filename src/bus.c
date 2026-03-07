#include "../lib/BUS.h"

#include <stdio.h>
#include <string.h>

// Intialize the BUS
void BUS_Init(Bus *bus) {
    memset(bus->RAM, 0, 65536);
}

// Read bytes from memory
uint8_t BUS_ReadAddr(Bus *bus, uint16_t addr) {
    return bus->RAM[addr];
}


// Write byte into memory
void BUS_WriteAddr(Bus *bus, uint16_t addr, uint8_t val) {
    bus->RAM[addr] = val;
}