#ifndef UNIT_TEST

#include "myfunc.h"
#include "stm32_util.h"
#include <stdio.h>

uint32_t cyc;

int main(void) {

    stm32_setup(CLOCK_BENCHMARK);
    sleep(1);

    printf("\n***************************************************\n"
           "\t\tSTM32 Testbench\n\n");
    stm32_print_boardinfo();
    printf("\n***************************************************\n\n");
    printf("Running benchmarks ...\n");

    //########################
    // put benchmarks here
    

    vec a, b;
    MEASURE("vec_random", vec_random(a));
    MEASURE("vec_acc", vec_acc(a, b));
    MEASURE("vec_acc_asm", vec_acc_asm(a, b));

    //########################
    printf("\ndone\n");
    while (1) {
        stm32_leds_toggle();
        sleep(1);
    }
}

#endif