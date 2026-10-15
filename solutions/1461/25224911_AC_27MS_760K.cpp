// POJ 1461 - Binary Polynomials
// Model: gpt-6.1-sol
// A monomial contributes exactly when its variable mask is a subset of the
// assignment. The subset XOR transform evaluates every assignment in O(n*2^n)
// time and O(2^n) space. Addition is modulo two (XOR), as the prose specifies.
#include <cstdio>
static char value[1 << 18 | 1];
static unsigned char weight[1 << 18];
int main() {
    for (int i = 1; i < (1 << 18); ++i)
        weight[i] = weight[i >> 1] + (i & 1);
    int tests;
    if (scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int n, k;
        scanf("%d%d%s", &n, &k, value);
        int size = 1 << n;
        for (int i = 0; i < size; ++i) value[i] -= '0';
        for (int bit = 1; bit < size; bit <<= 1)
            for (int base = 0; base < size; base += bit << 1)
                for (int j = 0; j < bit; ++j)
                    value[base + bit + j] ^= value[base + j];
        int answer = 0;
        for (int i = 0; i < size; ++i)
            if (weight[i] == k) answer += value[i];
        printf("%d\n", answer);
    }
    return 0;
}
