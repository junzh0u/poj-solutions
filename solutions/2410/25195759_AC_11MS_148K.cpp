// POJ 2410 - Simple Computers
// Model: claude-opus-5
//
// Direct interpreter for the toy von Neumann machine: 32 bytes of memory that
// hold code and data alike, an 8-bit accumulator and a 5-bit program counter.
// Each case is 32 whitespace-separated 8-bit binary literals (bytes 0..31),
// read to EOF as the statement specifies.
//
// Per instruction: fetch mem[pc], split into the top 3 opcode bits and the low
// 5 address bits, increment pc, then execute -- so a taken BEQ/JMP overwrites
// the already-incremented pc rather than being incremented afterwards.
//
// The two wrap-arounds are the whole difficulty and both are load-bearing:
// the accumulator is 8-bit, so INC on 255 yields 0 (which the sample relies on
// to make its BEQ fire) and DEC on 0 yields 255; the program counter is 5-bit,
// so stepping off byte 31 wraps to byte 0. Memory is fully re-read every case,
// so no explicit reset is needed for it; accu and pc are reset explicitly.
//
// No ambiguity in the statement. Decoding is done with shifts and masks rather
// than a bit-field union, whose layout is implementation-defined.

#include <cstdio>

int main() {
    char buf[64];
    while (scanf("%63s", buf) == 1) {
        int mem[32];
        int i;
        for (i = 0; i < 32; ++i) {
            if (i > 0 && scanf("%63s", buf) != 1) return 0;
            int v = 0;
            for (int b = 0; b < 8; ++b) v = v * 2 + (buf[b] == '1' ? 1 : 0);
            mem[i] = v;
        }
        int accu = 0, pc = 0;
        for (;;) {
            int ins = mem[pc];
            int op = ins >> 5;
            int x = ins & 31;
            pc = (pc + 1) & 31;
            if (op == 0) mem[x] = accu;              /* STA x */
            else if (op == 1) accu = mem[x];         /* LDA x */
            else if (op == 2) { if (accu == 0) pc = x; }  /* BEQ x */
            else if (op == 3) { /* NOP */ }
            else if (op == 4) accu = (accu - 1) & 255;    /* DEC */
            else if (op == 5) accu = (accu + 1) & 255;    /* INC */
            else if (op == 6) pc = x;                /* JMP x */
            else break;                              /* HLT */
        }
        for (int b = 7; b >= 0; --b) putchar('0' + ((accu >> b) & 1));
        putchar('\n');
    }
    return 0;
}
