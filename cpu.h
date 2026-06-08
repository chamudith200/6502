#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>

// Define this for debug information
#define DEBUG_INFO
#ifdef DEBUG_INFO
#define dbgInfo() printf("A: 0x%02X  X: 0x%02X  Y: 0x%02X  0xPC: 0x%04X  STAT: 0x%02X\n", a, x, y, pc, status)
#else
#define dbgInfo()
#endif


// cpu architecture
uint16_t pc;
uint8_t sp, x, y, a, status;
uint8_t memory[65536];  // 64KB

#define FLAG_CARRY  0x1
#define FLAG_ZERO   0x2
#define FLAG_ITRD   0x4
#define FLAG_DECM   0x8
#define FLAG_BREAK  0x10
#define FLAG_OVRF   0x40
#define FLAG_NEG    0x80

#define SET_Z(val) {if (val == 0) status |= FLAG_ZERO; else status &= ~FLAG_ZERO;}
#define SET_N(val) {if (val & 0x80) status |= FLAG_NEG; else status &= ~FLAG_NEG;}
#define SET_C(a, opr) {if (((a) + (opr)) & 0x100) status|= FLAG_CARRY; else status &= !FLAG_CARRY;}
#define SET_V(a, opr, result) {if (((a) ^ (result) & (opr) ^ (result)) & 0x80) status |= FLAG_OVRF; else status &= ~FLAG_OVRF;}
#define SET_ZN(val) {SET_Z(val) ; SET_N(val);}


// helper vars
uint8_t opcode;
uint16_t addr;
uint16_t tmpX, tmpY;

#define fetch() {\
    opcode = memory[pc++];\
}

#define clr_addr() {\
    addr &= 0x00;\
}


static void execute(uint8_t opcode) {
    switch (opcode) {
        // LDA
        case 0xA9:  // Load immidiate
            a = memory[pc++];
            SET_ZN(a);
            break;

        case 0xA5:  // Load zero page
            uint8_t ldz_addr = memory[pc++];
            a = memory[ldz_addr];
            SET_ZN(a);
            break;

        case 0xB5:  // Load zero page, X
            uint8_t ldzp_addr = memory[pc++];
            a = memory[(ldzp_addr + x) & 0xFF];
            SET_ZN(a);
            break;

        case 0xAD:  // Load absolute
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            a = memory[addr];
            SET_ZN(a);
            break;

        case 0xBD:  // Load absolute, X
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            addr += x;
            a = memory[addr];
            SET_ZN(a);
            break;

        case 0xB9:  // Load absolute, Y
            clr_addr();
            addr = memory[pc++];
            addr  |= memory[pc++] << 8;
            addr += y;
            a = memory[addr];
            SET_ZN(a);
            break;

        case 0xA1:  // Load indirect, X
            clr_addr();
            uint8_t tmpAddr = memory[pc++];
            addr = memory[(tmpAddr + x) & 0xFF];
            addr |= memory[((tmpAddr + x) & 0xFF) + 1] << 8;
            a = memory[addr];
            SET_ZN(a);
            break;

        case 0xB1:  // Load indirect, y
            clr_addr();
            tmpAddr = memory[pc++];
            addr = memory[tmpAddr];
            addr |= memory[(tmpAddr + 1) & 0xFF] << 8;
            addr += y;
            a = memory[addr];
            SET_ZN(a);
            break;

        // STA
        case 0x85:  // Store zero page
            memory[memory[pc++]] = a;
            break;
        
        case 0x95:  // Store zero page, x
            uint8_t szx_adr = memory[pc++];
            memory[(szx_adr + x) & 0xFF] = a;
            break;

        case 0x8D:  // Store absolute
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            memory[addr] = a;
            break;

        case 0x9D:  // Store absolute, x
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            addr += x;
            memory[addr] = a;
            break;

        case 0x99:  // Store absolute, y
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            addr += y;
            memory[addr] = a;
            break;

        case 0x81:  // Store indirect, x
            clr_addr();
            tmpAddr = memory[pc++] + x;
            addr = memory[tmpAddr];
            addr = memory[(tmpAddr + 1) & 0xFF] << 8;
            memory[addr] = a;
            break;

        case 0x91:  // Store indirect, y
            clr_addr();
            tmpAddr = memory[pc++];
            addr = memory[tmpAddr];
            addr = memory[(tmpAddr + 1) & 0xFF] << 8;
            addr += y;
            memory[addr] = a;
            break;

        // LDX
        case 0xA2:  // Load x, immediate
            x = memory[pc++];
            SET_ZN(x);
            break;

        case 0xA6:  // Load x, zero page
            x = memory[memory[pc++]];
            SET_ZN(x);
            break;

        case 0xB6:  // Load x, zero page, y
            x = memory[(memory[pc++] + y) & 0xFF];
            SET_ZN(x);
            break;

        case 0xAE:  // load x, absolute
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            x = memory[addr];
            SET_ZN(x);
            break;

        case 0xBE:  // Load x, absolute, y
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            addr = (addr + y) & 0xFF;
            x = memory[addr];
            SET_ZN(x);
            break;

        // LDY
        case 0xA0:  // Load y, immediate
            y = memory[pc++];
            SET_ZN(y);
            break;

        case 0xA4:  // Load y, zero page
            y = memory[memory[pc++]];
            SET_ZN(y);
            break;

        case 0xB4:  // Load y, zero page, x
            y = memory[(memory[pc++] + x) & 0xFF];
            SET_ZN(y);
            break;

        case 0xAC:  // load y, absolute
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            y = memory[addr];
            SET_ZN(y);
            break;

        case 0xBC:  // Load y, absolute, x
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            addr = (addr + y) & 0xFF;
            y = memory[addr];
            SET_ZN(y);
            break;

        case 0x69:  // AddC immediate
            SET_C(a, memory[pc + 1]);
            a += memory[pc++];
            SET_ZN(a);

            break;

        case 0x65:  // AddC zero page
            a += memory[memory[pc++]];
            SET_ZN(a);
            break;

        case 0x75:  // AddC zero page, x
            a += memory[(memory[pc++] + x) & 0xFF];
            SET_ZN(a);
            break;


    }
}


void step() {
    fetch();
    execute(opcode);
}



// test funcs
void tst_LDA() {
    memory[0x0] = 0xA9;
    memory[0x1] = 0x5D;

    memory[0x2] = 0xA5;
    memory[0x3] = 0x00;

    memory[0x4] = 0xB5;
    memory[0x5] = 0x02;

    memory[0x1234] = 0xAA;
    memory[0x1237] = 0xBB;
    memory[0x1236] = 0xCC;

    memory[0x6] = 0xAD;
    memory[0x7] = 0x34;
    memory[0x8] = 0x12;

    memory[0x9] = 0xBD;
    memory[0xA] = 0x34;
    memory[0xB] = 0x12;

    memory[0x02B5] = 0xCC;
    memory[0xC] = 0xA1;
    memory[0xD] = 0x02;
}


// Load a ROM into the memory
int loadROM(uint8_t *memory, const char *RomName) {
    FILE *file = fopen(RomName, "rb");
    if (!file) {
        printf("Failed to open %s\n", RomName);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long romLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    // if (romLength > 256) {
    //     printf("Program is too long (max size = 256 Bytes)\n");
    //     return 2;
    // }

    fread(memory, romLength, 1, file);
    fclose(file);
    return 0;
}


#endif

