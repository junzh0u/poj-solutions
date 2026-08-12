// POJ 2541 - Binary Witch
// Model: gpt-5.6-terra
// Maintain, for every binary word of length at most 13, the bit that followed
// its rightmost completed occurrence.  Appending a bit completes exactly one
// occurrence at each length, so both the initial history and every forecast
// are processed in O(13) time.
#include <cstdio>

int main() {
    int n, l;
    static char input[1000005];
    static signed char next[14][8192];
    int mask = 0;
    int i, t;

    if (std::scanf("%d%d%s", &n, &l, input) != 3) return 0;
    for (t = 1; t <= 13; ++t)
        for (i = 0; i < (1 << t); ++i) next[t][i] = -1;

    for (i = 0; i < n; ++i) {
        int bit = input[i] - '0';
        for (t = 1; t <= 13 && t <= i; ++t)
            next[t][mask & ((1 << t) - 1)] = (signed char)bit;
        mask = ((mask << 1) | bit) & 8191;
    }

    for (i = 0; i < l; ++i) {
        int bit = 0;
        for (t = 13; t >= 1; --t) {
            int value = next[t][mask & ((1 << t) - 1)];
            if (value != -1) {
                bit = value;
                break;
            }
        }
        std::putchar('0' + bit);
        for (t = 1; t <= 13 && t <= n + i; ++t)
            next[t][mask & ((1 << t) - 1)] = (signed char)bit;
        mask = ((mask << 1) | bit) & 8191;
    }
    std::putchar('\n');
    return 0;
}
