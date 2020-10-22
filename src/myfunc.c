#include "myfunc.h"
#include "stm32_util.h"

void vec_random(vec v) {

    randombytes(v, VECLEN);
    for (int i = 0; i < VECLEN; ++i) {
        v[i] &= Q - 1;
    }
}

void vec_acc(vec r, const vec a) {

    for (int i = 0; i < VECLEN; ++i) {
        r[i] = (r[i] + a[i]) & (Q - 1);
    }
}