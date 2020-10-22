#ifdef CM4
.syntax unified
.cpu cortex-m4
.thumb

//*************************************************
// void vec_acc(vec r, const vec a);
// - r0: pointer to vec r
// - r1: pointer to vec a
//*************************************************
.align 4
.global vec_acc_asm
.type vec_acc_asm, %function
vec_acc_asm:
  push    {r4 - r10, lr}

  mov r14, #16
  .equ mask, 0x7f7f7f7f
  movw r10, #:lower16:mask
  movt r10, #:upper16:mask 
1:
  ldm r0, {r2-r5}
  ldm r1!, {r6-r9}
  uadd8 r2, r2, r6
  uadd8 r3, r3, r7
  uadd8 r4, r4, r8
  uadd8 r5, r5, r9
  .irp reg, r2,r3,r4,r5
    and \reg, r10
  .endr 
  stm r0!, {r2-r5}
  subs r14, #1
  bne 1b

  pop    {r4 - r10, pc}

#endif // CM4

#ifdef CM0
.syntax unified
.cpu cortex-m0
.thumb

//*************************************************
// void vec_acc(vec r, const vec a);
// - r0: pointer to vec r
// - r1: pointer to vec a
//*************************************************
mask:
.align 4
.word 0x7f7f7f7f

.global vec_acc_asm
.type vec_acc_asm, %function
vec_acc_asm:
 push    {r4 - r7, lr}

  movs r7, #32
  ldr r6, =mask
  ldr r6, [r6]
1:
  ldm r0!, {r2,r3}
  ldm r1!, {r4,r5}
  subs r0, #8
  adds r2, r4
  adds r3, r5
  ands r2, r6
  ands r3, r6
 
  stm r0!, {r2,r3}
  subs r7, #1
  bne 1b

  pop    {r4 - r7, pc}

#endif // CM0