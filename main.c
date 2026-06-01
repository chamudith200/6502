#include "cpu.h"
#include <stdio.h>

#define DO_STEPS(n) {\
    for (int i = 0; i < n; i++) {\
        step(); dbgInfo();\
    }\
}

int main() {

    int running = 1;

    tst_LDA();

    step(); dbgInfo(); 
    step(); dbgInfo();
    step(); dbgInfo();
    step(); dbgInfo();
    x = 0x3;
    step(); dbgInfo();
    x = 0x2;
    step(); dbgInfo();


    // while (running) {
    //     step();
    //     dbgInfo();
    //     getchar();
    // }
}