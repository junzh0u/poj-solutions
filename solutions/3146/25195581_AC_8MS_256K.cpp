// POJ 3146 - Interesting Yang Hui Triangle
// Model: claude-opus-5
//
// Line N+1 of the triangle holds C(N,0..N).  By Kummer's theorem the exponent
// of the prime P in C(N,K) is the number of carries when K and N-K are added
// in base P, so C(N,K) is coprime to P exactly when no digit of K exceeds the
// corresponding digit of N (Lucas).  Writing N = sum d_i P^i, each digit of K
// may independently take d_i + 1 values, giving prod (d_i + 1) such K.  Only
// the last four digits are wanted, so the product is kept mod 10000 and
// printed with %04d.  O(log_P N) per case, N < 2^31 and the running product
// stays below 10^7, so plain int suffices.
//
// Input framing: multiple cases to EOF, terminated by "0 0".  The board
// reports data with N <= 0; a non-positive N leaves the digit loop empty and
// yields 1 ("0001"), which is also the correct answer for N = 0 (line 1 is a
// single 1).  P = 0 only ever appears in the sentinel, and bailing out on it
// also keeps the modulus of the digit loop away from zero.

#include <cstdio>

int main() {
    int p, n, caseNo = 0;
    while (scanf("%d %d", &p, &n) == 2) {
        if (p == 0 && n == 0) break;
        if (p < 2) break;
        int ans = 1;
        while (n > 0) {
            ans = ans * (n % p + 1) % 10000;
            n /= p;
        }
        printf("Case %d: %04d\n", ++caseNo, ans);
    }
    return 0;
}
