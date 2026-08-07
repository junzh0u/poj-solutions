// POJ 2443 - Set Operation
// Model: claude-opus-5
//
// Approach: bitset over the sets.  N <= 1000 sets and elements are in 1..10000,
// so for every element value v keep a 1000-bit mask bits[v] whose k-th bit says
// "v occurs in S(k)".  That is 10001 * 32 unsigned ints = 1.28 MB.  Building it
// costs one OR per input element; a query (i, j) is answered by scanning the
// W = (N+31)/32 <= 32 words of bits[i] & bits[j] and printing "Yes" as soon as
// one word is non-zero.  200000 queries * 32 words is trivial.
//
// The real cost is input: up to N * C(i) = 10^7 numbers, so reading is done with
// a hand-rolled fread buffer and the answers are accumulated in one output
// buffer flushed once (the statement itself warns that cin/cout is too slow).
//
// Notes on the statement: a "set" may contain duplicates, which changes nothing
// here because the mask bit is idempotent.  i may equal j, and that case needs
// no special handling either -- bits[i] & bits[i] = bits[i] is non-zero exactly
// when i occurs in some set, which is the right answer.  Input is a single data
// set (no EOF loop); an unsolicited second read would print nothing extra
// anyway, but the single read is what the statement describes.

#include <cstdio>

static const int MAXV = 10001;   /* element values 0..10000 */
static const int MAXW = 32;      /* ceil(1000 / 32) */

static unsigned int bits[MAXV][MAXW];

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int gc()
{
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static int readInt()
{
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return -1;
    int x = 0;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = gc();
    }
    return x;
}

static char obuf[200000 * 4 + 16];

int main()
{
    int n = readInt();
    if (n < 0) return 0;

    for (int s = 0; s < n; ++s) {
        int c = readInt();
        int w = s >> 5;
        unsigned int mask = 1u << (s & 31);
        for (int t = 0; t < c; ++t) {
            int v = readInt();
            if (v >= 0 && v < MAXV && w < MAXW) bits[v][w] |= mask;
        }
    }

    int words = (n + 31) >> 5;
    if (words > MAXW) words = MAXW;
    int q = readInt();
    if (q < 0) q = 0;

    int op = 0;
    for (int t = 0; t < q; ++t) {
        int i = readInt();
        int j = readInt();
        int yes = 0;
        if (i >= 0 && i < MAXV && j >= 0 && j < MAXV) {
            const unsigned int *a = bits[i];
            const unsigned int *b = bits[j];
            for (int w = 0; w < words; ++w) {
                if (a[w] & b[w]) { yes = 1; break; }
            }
        }
        if (yes) {
            obuf[op++] = 'Y'; obuf[op++] = 'e'; obuf[op++] = 's';
        } else {
            obuf[op++] = 'N'; obuf[op++] = 'o';
        }
        obuf[op++] = '\n';
    }
    fwrite(obuf, 1, op, stdout);
    return 0;
}
