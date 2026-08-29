// POJ 3423 - Automaton optimization
// Model: claude-sonnet-5
//
// Rule 90 linear cellular automaton: new[x] = old[x-1] XOR old[x+1].
// "S-th state" means the initial configuration IS state 1 (verified by
// brute-force simulation against both samples: with S=1 the output equals
// the initial state unchanged), so state S is reached after T = S-1 steps.
//
// By linearity (XOR), the value contributed by a single seed 1 at position i
// after T steps at position x follows the classic Rule-90 closed form:
// value = C(T, m) mod 2 where m = (T + (x-i)) / 2, valid only when
// T+(x-i) is even and 0 <= m <= T; otherwise the contribution is 0.
// The total value at x is the XOR of the contributions of all N seeds.
// C(T,m) mod 2 (Lucas/Kummer) is 1 iff m is a submask of T, i.e. (m & T) == m.
// This gives an O(N*L) algorithm (N,L <= 2000 so <= 4*10^6 operations),
// confirmed against the discuss board's own O(N*L) solution sketch.
//
// All index arithmetic (T + x - i) is done in long long: F can be as low as
// -10^9 and as high as 10^9+L, i up to 1000 in magnitude, T up to ~10^9-1,
// so the sum can approach 2*10^9+ and must not overflow a 32-bit int.
#include <cstdio>
using namespace std;

int main() {
    int N;
    scanf("%d", &N);
    static long long idx[2005];
    for (int i = 0; i < N; i++) scanf("%lld", &idx[i]);
    long long S, F, L;
    scanf("%lld %lld %lld", &S, &F, &L);
    long long T = S - 1;

    for (long long k = 0; k < L; k++) {
        long long x = F + k;
        int val = 0;
        for (int i = 0; i < N; i++) {
            long long s = T + (x - idx[i]);
            if (s < 0) continue;
            if (s & 1LL) continue;
            long long m = s >> 1;
            if (m > T) continue;
            if ((m & T) == m) val ^= 1;
        }
        printf(k ? " %d" : "%d", val);
    }
    printf("\n");
    return 0;
}
