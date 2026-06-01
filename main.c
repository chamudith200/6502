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

    DO_STEPS(4);


    // while (running) {
    //     step();
    //     dbgInfo();
    //     getchar();
    // }
}