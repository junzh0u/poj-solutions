// POJ 3145 - Harmony Forever
// Model: gpt-5.6-terra
// Small divisors are maintained directly; large-divisor queries scan quotient
// intervals through a two-level bitset that finds the first present value.
#include <cstdio>
#include <cstring>
using namespace std;

const int MAXV = 1000000;
const int CUT = 1000;
const int WORD_BITS = 64;
const int WORDS = (MAXV + WORD_BITS) / WORD_BITS;
const int WORDS_PER_BLOCK = 4;
const int BLOCKS = (WORDS + WORDS_PER_BLOCK - 1) / WORDS_PER_BLOCK;

unsigned long long present[WORDS];
unsigned long long blockOr[BLOCKS];
int insertedTime[MAXV + 1];
int bestRem[CUT + 1], bestTime[CUT + 1];

int lowestBit(unsigned long long x) {
    int p = 0;
    while ((x & 0xffffffffULL) == 0) {
        x >>= 32;
        p += 32;
    }
    if ((x & 0xffffULL) == 0) {
        x >>= 16;
        p += 16;
    }
    if ((x & 0xffULL) == 0) {
        x >>= 8;
        p += 8;
    }
    if ((x & 0xfULL) == 0) {
        x >>= 4;
        p += 4;
    }
    if ((x & 3ULL) == 0) {
        x >>= 2;
        p += 2;
    }
    if ((x & 1ULL) == 0) ++p;
    return p;
}

int firstInWord(int w, int lo, int hi) {
    if (w < 0 || w >= WORDS || lo > hi) return -1;
    unsigned long long mask = present[w];
    int base = w * WORD_BITS;
    if (lo > base) mask &= (~0ULL << (lo - base));
    if (hi < base + 63) mask &= ((1ULL << (hi - base + 1)) - 1ULL);
    if (!mask) return -1;
    return base + lowestBit(mask);
}

int firstPresent(int lo, int hi) {
    if (lo < 1) lo = 1;
    if (hi > MAXV) hi = MAXV;
    if (lo > hi) return -1;
    int lw = lo / WORD_BITS, rw = hi / WORD_BITS;
    if (lw == rw) return firstInWord(lw, lo, hi);
    int ans = firstInWord(lw, lo, lw * WORD_BITS + 63);
    if (ans != -1) return ans;
    ++lw;
    int lb = (lw + WORDS_PER_BLOCK - 1) / WORDS_PER_BLOCK;
    int rb = rw / WORDS_PER_BLOCK - 1;
    int endLeft = lb * WORDS_PER_BLOCK;
    for (int w = lw; w < endLeft && w < rw; ++w) {
        ans = firstInWord(w, w * WORD_BITS, w * WORD_BITS + 63);
        if (ans != -1) return ans;
    }
    for (int b = lb; b <= rb; ++b) {
        if (!blockOr[b]) continue;
        int start = b * WORDS_PER_BLOCK;
        for (int w = start; w < start + WORDS_PER_BLOCK; ++w) {
            if (!present[w]) continue;
            return w * WORD_BITS + lowestBit(present[w]);
        }
    }
    int startRight = (rb + 1) * WORDS_PER_BLOCK;
    if (startRight < lw) startRight = lw;
    for (int w = startRight; w < rw; ++w) {
        ans = firstInWord(w, w * WORD_BITS, w * WORD_BITS + 63);
        if (ans != -1) return ans;
    }
    return firstInWord(rw, rw * WORD_BITS, hi);
}

int main() {
    int t, tc = 0;
    while (scanf("%d", &t) == 1 && t) {
        memset(present, 0, sizeof(present));
        memset(blockOr, 0, sizeof(blockOr));
        memset(insertedTime, 0, sizeof(insertedTime));
        for (int d = 1; d <= CUT; ++d) {
            bestRem[d] = d;
            bestTime[d] = -1;
        }
        printf("Case %d:\n", ++tc);
        int insertionCount = 0;
        for (int k = 1; k <= t; ++k) {
            char command[2];
            int value;
            scanf("%s%d", command, &value);
            if (command[0] == 'B') {
                insertedTime[value] = ++insertionCount;
                int w = value / WORD_BITS;
                present[w] |= 1ULL << (value % WORD_BITS);
                blockOr[w / WORDS_PER_BLOCK] |= present[w];
                for (int d = 1; d <= CUT; ++d) {
                    int rem = value % d;
                    if (rem <= bestRem[d]) {
                        bestRem[d] = rem;
                        bestTime[d] = insertionCount;
                    }
                }
            } else if (bestTime[1] == -1) {
                puts("-1");
            } else if (value <= CUT) {
                printf("%d\n", bestTime[value]);
            } else {
                int answer = -1, minimum = value;
                for (int left = 0; left <= MAXV; left += value) {
                    int right = left + value - 1;
                    int found = firstPresent(left, right);
                    if (found == -1) continue;
                    int rem = found % value;
                    if (rem < minimum || (rem == minimum && insertedTime[found] > answer)) {
                        minimum = rem;
                        answer = insertedTime[found];
                    }
                }
                printf("%d\n", answer);
            }
        }
        putchar('\n');
    }
    return 0;
}
