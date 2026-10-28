/* func_L03_003D20F8 (252 bytes, 1 place) -- NEAR MISS, not integrated.
   state: 252 bytes, 62 of 63 instructions identical; the fault is one instruction's
   position: the D_001395C0 store and the call's first argument are swapped.
   check.py (03_endako), with the source below in the level's own unit
   (src/levels/03_endako/L03_00399230.c, built at -O2 -G0 -ffunction-sections):
     003D2134  lui    $v1, 0x0       |  lui   $v1, 0x14
     003D2138  sb    $v0, 0x0($v1)  |  addiu $a0, $s0, 0x10
     003D213C  addiu $a0, $s0, 0x10 |  sb    $v0, -0x6A40($v1)
   Everything else matches, word for word, from the prologue to the epilogue.

   What is measured and right:
   - the six bytes at 0x1A7AFA, 0x1A7B32, 0x1A7AFB, 0x1A7B33, 0x1A7AFC and 0x1A7B34 are
     stored with the constant-address form `lui $at,0x1A; sb $v0,0x7AFA($at)`. A named
     symbol does not give that form at -G0 (one %hi register per store, all hoisted) and
     at -G8 it gives it, but then the last of the six is placed in the call's delay slot
     as small data (`sb $v0,0x0($gp)`), which retail does not have. The literal
     addresses in the source below are what produce retail's form; the project's naming
     rule (D_XXXXXXXX for the boot's resident data) is broken by them, and the row in
     nonmatching/README.md records that.
   - the source order of the six constant stores decides their order in the output: the
     one written last is scheduled first. The order written below is the one that
     reproduces retail's order (0x1A7AFA, 0x1A7B32, 0x1A7AFB, 0x1A7B33, 0x1A7AFC,
     0x1A7B34).
   - D_001395C0 keeps the named-symbol form: its %hi is hoisted into $v1 before the
     call's argument, exactly as in retail, and the byte is written with the constant 1.
   - the actor's +0x68 block is cleared with a zero halfword at +0x24FC of the resident
     block, the state word at +0x2C decides whether the first func_311758 call happens,
     the second one takes the table entry, and 0x80468CDC is stored at the actor's +0x78.

   Tried and not enough: the six stores as named symbols at -G8 (with and without
     __attribute__((nosda)) on D_001395C0); the D_001395C0 store written before and
     after the six and last in the source; the call written before the stores; the
     call's arguments computed into variables before the stores.
   Next idea: a lever that keeps the D_001395C0 store after the func_2BEE78 argument
   without moving the store itself. */

#include "common.h"

extern void func_2B8510(s32 a0);
extern void func_313378(s32 a0);
extern void func_359FD0(char *a0, char *a1);
extern void func_30D710(s32 a0, s32 a1);
extern char D_00189E20_11[];
extern u8   D_001395C0;
extern void func_2BEE78(char *a0, char *a1, s32 a2, s32 a3);
extern void func_311758(void *a0);
extern u8  *D_L03_001B2A1C;
extern void func_32F140(s32 a0, s32 a1);

/* The actor is released: its flags are set, the resident block's word is cleared and
   the two engine routines are run. */
void func_L03_003D20F8(char *a0)
{
    char *s0 = a0;
    char *s1 = *(char **)(s0 + 0x68);
    char *s2;

    func_2B8510(0);
    s2 = D_00189E20_11;
    *(u16 *)(s2 + 0x24FC) = 0;
    func_313378(*(u8 *)(s0 + 0xB0));
    D_001395C0 = 1;
    *(u8 *)0x1A7B32 = 1;
    *(u8 *)0x1A7AFB = 1;
    *(u8 *)0x1A7B33 = 1;
    *(u8 *)0x1A7AFC = 1;
    *(u8 *)0x1A7B34 = 1;
    *(u8 *)0x1A7AFA = 1;
    func_2BEE78(s0 + 0x10, s0 + 0xF0, 0, 1);
    if (*(s32 *)(s1 + 0x2C) != 0) {
        func_311758(*(char **)s1);
    }
    func_311758(D_L03_001B2A1C + (*(s32 *)s1 << 8));
    *(s32 *)(s0 + 0x78) = 0x80468CDC;
    func_32F140(*(s32 *)(s1 + 0x24), -1);
    func_359FD0(s2 + 0x80, s2 + 0x90);
    func_30D710(0, -1);
}
