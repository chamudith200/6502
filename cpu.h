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

// Helper funcs
// addressing

uint8_t addr_zero_page() {
    return memory[pc++];
}

uint8_t addr_zero_page_x() {
    return (memory[pc++] + x) & 0xFF;
}

uint8_t addr_zero_page_y() {
    return (memory[pc++] + y) & 0xFF;
}

uint16_t addr_absolute() {
    uint16_t tmp_addr = memory[pc++];
    tmp_addr |= memory[pc++] << 8;
    return tmp_addr;
}

uint16_t addr_absolute_x() {
    uint16_t tmp_addr = memory[pc++];
    tmp_addr |= memory[pc++] << 8;
    return tmp_addr + x;
}

uint16_t addr_absolute_y() {
    uint16_t tmp_addr = memory[pc++];
    tmp_addr |= memory[pc++] << 8;
    return tmp_addr + y;
}

uint16_t addr_indirect_x() {
    uint8_t tmp_8_addr = memory[pc++] + x;
    uint16_t tmp_addr = memory[tmp_8_addr];
    tmp_addr |= memory[(tmp_8_addr + 1) & 0xFF] << 8;
    return tmp_addr;
}

uint16_t addr_indirect_y() {
    uint8_t tmp_8_addr = memory[pc++];
    uint16_t tmp_addr = memory[tmp_8_addr];
    tmp_addr |= memory[(tmp_8_addr + 1) & 0xFF] << 8;
    return tmp_addr + y;
}



static void execute(uint8_t opcode) {
    switch (opcode) {
        // LDA
        case 0xA9:  // Load immidiate
            a = memory[pc++];
            SET_ZN(a);
            break;

        case 0xA5:  // Load zero page
            a = memory[addr_zero_page()];
            SET_ZN(a);
            break;

        case 0xB5:  // Load zero page, X
            a = memory[addr_zero_page_x()];
            SET_ZN(a);
            break;

        case 0xAD:  // Load absolute
            a = memory[addr_absolute()];
            SET_ZN(a);
            break;

        case 0xBD:  // Load absolute, X
            a = memory[addr_absolute_x()];
            SET_ZN(a);
            break;

        case 0xB9:  // Load absolute, Y
            a = memory[addr_absolute_y()];
            SET_ZN(a);
            break;

        case 0xA1:  // Load indirect, X
            a = memory[addr_indirect_x()];
            SET_ZN(a);
            break;

        case 0xB1:  // Load indirect, y
            a = memory[addr_indirect_y()];
            SET_ZN(a);
            break;

        // STA
        case 0x85:  // Store zero page
            memory[addr_zero_page()] = a;
            break;
        
        case 0x95:  // Store zero page, x
            memory[addr_zero_page_x()] = a;
            break;

        case 0x8D:  // Store absolute
            memory[addr_absolute()] = a;
            break;

        case 0x9D:  // Store absolute, x
            memory[addr_absolute_x()] = a;
            break;

        case 0x99:  // Store absolute, y
            memory[addr_absolute_y()] = a;
            break;

        case 0x81:  // Store indirect, x
            memory[addr_indirect_x()] = a;
            break;

        case 0x91:  // Store indirect, y
            memory[addr_indirect_y()] = a;
            break;

        // LDX
        case 0xA2:  // Load x, immediate
            x = memory[pc++];
            SET_ZN(x);
            break;

        case 0xA6:  // Load x, zero page
            x = memory[addr_zero_page()];
            SET_ZN(x);
            break;

        case 0xB6:  // Load x, zero page, y
            x = memory[addr_zero_page_y()];
            SET_ZN(x);
            break;

        case 0xAE:  // load x, absolute
            x = memory[addr_absolute()];
            SET_ZN(x);
            break;

        case 0xBE:  // Load x, absolute, y
            x = memory[addr_absolute_y()];
            SET_ZN(x);
            break;

        // LDY
        case 0xA0:  // Load y, immediate
            y = memory[pc++];
            SET_ZN(y);
            break;

        case 0xA4:  // Load y, zero page
            y = memory[addr_zero_page()];
            SET_ZN(y);
            break;

        case 0xB4:  // Load y, zero page, x
            y = memory[addr_zero_page_x()];
            SET_ZN(y);
            break;

        case 0xAC:  // load y, absolute
            y = memory[addr_absolute()];
            SET_ZN(y);
            break;

        case 0xBC:  // Load y, absolute, x
            y = memory[addr_absolute_x()];
            SET_ZN(y);
            break;

        case 0x69:  // AddC immediate
            uint8_t A = a;
            a += memory[pc++];
            SET_ZN(a);
            SET_C(A, memory[pc]);
            SET_V(A, memory[pc], a);
            break;

        case 0x65:  // AddC zero page
            A = a;
            a += memory[addr_zero_page()];
            SET_ZN(a);
            SET_C(A, memory[pc]);
            SET_V(A, memory[pc], a);
            break;

        case 0x75:  // AddC zero page, x
            A = a;
            a += memory[addr_zero_page_x()];
            SET_ZN(a);
            SET_C(A, memory[addr]);
            SET_V(A, memory[addr], a);
            break;

        case 0x6D:  // Addc absolute
            A = a;
            a += memory[addr_absolute()];
            SET_ZN(a);
            SET_C(A, memory[addr]);
            SET_V(A, memory[addr], a);
            break;

        case 0x7D:  // AddC absolute, x
            A = a;
            a += memory[addr_absolute_x()];
            SET_ZN(a);
            SET_C(A, memory[addr]);
            SET_V(A, memory[addr], a);
            break;

        case 0x79:  // AddC absolute, y
            A = a;
            a += memory[addr_absolute_y()];
            SET_ZN(a);
            SET_C(A, memory[addr]);
            SET_V(A, memory[addr], a);
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
    memory[0x3] = 0x05;

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

