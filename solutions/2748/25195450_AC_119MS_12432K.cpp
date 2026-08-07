// POJ 2748 - Logs Stacking
// Model: claude-sonnet-5
// Approach: the number of stacking figures for a base of n logs equals
// F(2n-1), the (2n-1)-th Fibonacci number (F(1)=F(2)=1). Derived directly
// from the physical rule (an upper layer of k logs must occupy a
// contiguous run of the m-1 gaps of an m-log layer below, k<m, giving
// m-k placements), which yields f(1)=1, f(m)=1+sum_{k<m}(m-k)f(k); this
// matches the closed form F(2m-1) for m=1..5 (1,2,5,13,34), and all four
// sample cases (n=1,2,3,5 -> 1,2,5,34) check out. Since only the answer
// mod 1e5 is needed and the Pisano period of Fibonacci mod 1e5 is 150000
// (lcm of the periods mod 32 and mod 3125, both well known), we precompute
// Fibonacci mod 1e5 for indices 0..149999 once, then answer each query in
// O(1) via idx = (2n-1) mod 150000. Fast manual I/O (buffered read with
// getchar, single fwrite at the end) is used because T can be up to 1e6
// and the discuss board reports TLE from naive per-query approaches
// (matrix power / per-line printf) even though the recurrence itself is
// simple.

#include <cstdio>

const int PERIOD = 150000;
const int MOD = 100000;
int fib[PERIOD];

inline int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) c = getchar();
    bool neg = false;
    if (c == '-') { neg = true; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return neg ? -x : x;
}

char outbuf[7000000];
int outpos = 0;

inline void writeInt(int x) {
    char tmp[8];
    int len = 0;
    if (x == 0) tmp[len++] = '0';
    while (x > 0) { tmp[len++] = char('0' + x % 10); x /= 10; }
    while (len > 0) outbuf[outpos++] = tmp[--len];
    outbuf[outpos++] = '\n';
}

int main() {
    fib[0] = 0;
    fib[1] = 1 % MOD;
    for (int i = 2; i < PERIOD; ++i) {
        fib[i] = (fib[i-1] + fib[i-2]) % MOD;
    }

    int T = readInt();
    for (int t = 0; t < T; ++t) {
        long long n = readInt();
        long long idx = (2 * n - 1) % PERIOD;
        writeInt(fib[(int)idx]);
    }
    fwrite(outbuf, 1, outpos, stdout);
    return 0;
}
