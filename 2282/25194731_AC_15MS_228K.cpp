// POJ 2282 - The Counting Problem
// Model: claude-sonnet-5
// Approach: For each query (a,b), let lo=min(a,b), hi=max(a,b) (statement's own
// sample gives a>b at least once, e.g. "1496 1403"), and for each digit d in
// 0..9 answer with f(hi,d)-f(lo-1,d), where f(n,d) counts occurrences of digit
// d among the decimal representations of 1..n. f is the classic digit-DP-free
// closed form built by scanning place values p=1,10,100,...: at place p, the
// "high" part is n/(p*10), the current digit is (n/p)%10, and "low" is n%p.
// For d>0 each full cycle of the high part contributes p occurrences at this
// place, plus p more if cur>d, or low+1 more if cur==d. Digit 0 needs the
// leading-zero correction: a leading digit position can't itself be 0, so the
// high-part cycle count is (high-1) instead of high, and once high==0 there is
// no valid (non-leading) occurrence left at this or any higher place, so the
// loop stops. Ambiguity: none noted beyond the a>b ordering already flagged
// by the discuss board.
#include <cstdio>

static int f(int n, int d) {
    if (n < 1) return 0;
    int count = 0;
    int p = 1;
    while (p <= n) {
        int high = n / (p * 10);
        int cur = (n / p) % 10;
        int low = n % p;
        if (d == 0) {
            if (high == 0) break;
            count += (high - 1) * p;
        } else {
            count += high * p;
        }
        if (cur > d) count += p;
        else if (cur == d) count += low + 1;
        p *= 10;
    }
    return count;
}

int main() {
    int a, b;
    while (scanf("%d %d", &a, &b) == 2) {
        if (a == 0 && b == 0) break;
        int lo = a < b ? a : b;
        int hi = a < b ? b : a;
        for (int d = 0; d < 10; ++d) {
            int ans = f(hi, d) - f(lo - 1, d);
            printf("%d%c", ans, d == 9 ? '\n' : ' ');
        }
    }
    return 0;
}
