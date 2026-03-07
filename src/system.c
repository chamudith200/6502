#include "../lib/CPU.h"
#include "../lib/bus.h"
#include "../lib/system.h"


// Initlaize the system
void SYS_Init(System *system) {
    CPU_Init(&system->CPU);
    Bus_Init(&system->bus);
}

// Load the rom into memory
int SYS_LoadRom(Bus *bus, char *rom_name) {
    // Open the programm
    FILE *rom = fopen(rom_name, "rb");
    if (!rom) {
        fprintf(stderr, "Failed to open rom: %s", rom_name);
        return 0;
    }

    // Read the data from 
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    fread(&bus->RAM[MEM_ProgOffset], 1, rom_size, rom);
    fclose(rom);
    return 1;
}

