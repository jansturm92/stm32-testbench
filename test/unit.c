#include "myfunc.h"
#include "stm32_util.h"
#include <string.h>
#include <unity.h>

static void test_vec_acc_asm(void) {

    vec a, acc_ref, acc_asm;
    vec_random(a);
    vec_random(acc_ref);
    memcpy(acc_asm, acc_ref, sizeof(vec));
    vec_acc(acc_ref, a);
    vec_acc_asm(acc_asm, a);
    TEST_ASSERT_EQUAL(0,memcmp(acc_ref, acc_asm, sizeof(vec)));
    ++acc_asm[0];
    TEST_ASSERT_NOT_EQUAL(0,memcmp(acc_ref, acc_asm, sizeof(vec)));
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {

    stm32_setup(CLOCK_FAST);
    sleep(STM32_UNITTEST_INITIAL_SLEEP_DURATION);

    UNITY_BEGIN();

    //########################
    // put unit tests here
    RUN_TEST(test_vec_acc_asm);

    //########################

    UNITY_END();

    while (1) {
    }
}
