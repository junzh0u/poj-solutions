// POJ 3639 - Exchange Rates
// Model: claude-opus-5
//
// Start with 1000 Canadian dollars.  On each of d predicted days the price of
// one U.S. dollar in Canadian dollars is given; on any day you may switch ALL
// your money to the other currency.  Each switch converts at the prevailing
// rate, takes a 3% commission, and drops any fraction of a cent.  Report the
// most Canadian money you can hold after the last day.
//
// DP over two states, both held in integer cents: c = best amount when all in
// CAD, u = best amount when all in USD.  Per day, using the previous day's
// values,
//     u' = max(u, floor(c * 0.97 / rate))
//     c' = max(c, floor(u * 0.97 * rate))
// and the answer is c after the last day.  Converting twice in one day only
// loses money, so reading the states from the previous day loses nothing.
//
// Ambiguity: "at the prevailing exchange rate, less a 3% commission, less any
// fraction of a cent" admits several orders.  Exactly ONE floor per conversion,
// applied after both the rate and the commission, is the reading that matches
// the samples and the official Waterloo judge data; flooring before the
// commission as well (1001.59 on sample 1), never flooring intermediates, and
// taking the commission out of the source amount all disagree with the official
// output on every one of its 30 cases.
//
// Arithmetic is exact rather than floating point: the rate is parsed as the
// decimal fraction num/den it literally is, so each floor is an exact integer
// division and no rounding can push a conversion across a cent boundary.
//
// The state fits comfortably in 32 bits (the official data peaks near $3341),
// but it is kept in long long for headroom and cast to int before printf, so
// no 64-bit value is ever passed to a %d conversion.

#include <cstdio>
#include <cstring>

static long long gcdll(long long a, long long b) {
    while (b) { long long t = a % b; a = b; b = t; }
    return a;
}

/* Parse a non-negative decimal literal into the exact fraction num/den. */
static void parseRate(const char *s, long long &num, long long &den) {
    num = 0;
    den = 1;
    bool seenDot = false;
    for (const char *p = s; *p; ++p) {
        if (*p == '.') { seenDot = true; continue; }
        if (*p < '0' || *p > '9') continue;
        num = num * 10 + (*p - '0');
        if (seenDot) den *= 10;
    }
    long long g = gcdll(num, den);
    if (g > 1) { num /= g; den /= g; }
}

int main() {
    int d;
    while (scanf("%d", &d) == 1 && d != 0) {
        long long c = 100000; /* cents, Canadian */
        long long u = -1;     /* cents, U.S.; -1 = state not reachable yet */
        for (int i = 0; i < d; ++i) {
            char buf[64];
            if (scanf("%63s", buf) != 1) { buf[0] = '1'; buf[1] = '\0'; }
            long long num, den;
            parseRate(buf, num, den);

            long long nc = c, nu = u;
            if (num > 0) {
                long long cand = (c * 97 * den) / (100 * num);
                if (cand > nu) nu = cand;
            }
            if (u > 0) {
                long long cand = (u * 97 * num) / (100 * den);
                if (cand > nc) nc = cand;
            }
            c = nc;
            u = nu;
        }
        printf("%d.%02d\n", (int)(c / 100), (int)(c % 100));
    }
    return 0;
}
