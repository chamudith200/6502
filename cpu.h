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

// helper vars
uint8_t opcode;


#define fetch() {\
    opcode = memory[pc++];\
}

static void execute(uint8_t opcode) {
    switch (opcode) {
        case 0xA9:  // Load immidiate
            a = memory[pc++];
            break;

        case 0xA5:  // Load zero page
            uint8_t ldz_addr = memory[pc++];
            a = memory[ldz_addr];
            break;

        case 0xB5:  // Load zero page, X
            uint8_t ldzp_addr = memory[pc++];
            a = memory[(ldzp_addr + x) & 0xFF];
            break;

        case 0xAD:  // Load absolute
            uint16_t addr = memory[pc++];
            addr |= memory[pc++] << 8;
            a = memory[addr];
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

    memory[0x6] = 0xAD;
    memory[0x7] = 0x34;
    memory[0x8] = 0x12;

}


#endif

