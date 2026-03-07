#include "../lib/system.h"
#include "../lib/CPU.h"
#include "../lib/BUS.h"


// Initlaize the system
void SYS_Init(System *system) {
    CPU_Init(&system->CPU);
    BUS_Init(&system->bus);
}

// Load the rom into memory
int SYS_LoadRom(System *sys, char *rom_name) {
    // Open the programm
    FILE *rom = fopen(rom_name, "rb");
    if (!rom) {
        fprintf(stderr, "Failed to open rom: %s", rom_name);
        return 0;
    }
    fprintf(stderr, "FIle opened succesfully: %s\n", rom_name);

    // Read the data from 
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    fread(&sys->bus.RAM[MEM_ProgOffset], 1, rom_size, rom);
    fclose(rom);
    return 1;
}

// Main system loop
int SYS_MainLoop(System *system) {
    while (1) {

    }
}
