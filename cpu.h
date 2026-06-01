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
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            a = memory[addr];
            break;

        case 0xBD:  // Load absolute, X
            clr_addr();
            addr = memory[pc++];
            addr |= memory[pc++] << 8;
            addr += x;
            a = memory[addr];
            break;

        case 0xB9:  // Load absolute, Y
            clr_addr();
            addr = memory[pc++];
            addr  |= memory[pc++] << 8;
            addr += y;
            a = memory[addr];
            break;

        case 0xA1:  // Load indirect, X
            clr_addr();
            uint8_t tmpAddr = memory[pc++];
            addr = memory[(tmpAddr + x) & 0xFF];
            addr |= memory[((tmpAddr + x) & 0xFF) + 1] << 8;
            a = memory[addr];
            break;

        case 0xB1:  // Load indirect, y
            clr_addr();
            tmpAddr = memory[pc++];
            addr = memory[tmpAddr];
            addr |= memory[(tmpAddr + 1) & 0xFF] << 8;
            addr += y;
            a = memory[addr];
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


#endif

