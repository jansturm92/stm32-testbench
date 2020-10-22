#ifndef MYFUNC_H
#define MYFUNC_H

#include <stddef.h>
#include <stdint.h>

#define Q (1U << 7U)
#define VECLEN 256

typedef uint8_t vec[VECLEN]; // elements in Z_q = [0, q-1] with q=2^7

void vec_random(vec v);
void vec_acc(vec r, const vec a);
void vec_acc_asm(vec r, const vec a);

#endif // MYFUNC_H