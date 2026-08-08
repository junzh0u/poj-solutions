// POJ 2581 - Exact Change Only
// Model: claude-opus-5
//
// Each data set gives a monetary amount A (0.01..5.00) and how many quarters,
// dimes, nickels and pennies are on hand; print the sub-multiset of those coins
// that sums to A exactly using the fewest coins, or NO EXACT CHANGE.
//
// A <= 500 cents, so the answer can simply be enumerated: quarters q (<= 20),
// dimes d (<= 50), nickels n (<= 100), pennies forced by the remainder, at most
// ~108k triples per data set and 100 data sets.  No DP or greedy adjustment is
// needed.
//
// Ambiguity - "the subset with the fewest number of coins" does not say what to
// do when two different subsets tie on coin count.  Ties do exist in the abstract
// (10+10+10+10 = 25+5+5+5, four coins each), so the tie-break looked load-bearing.
// It is not: enumerating the entire domain (every amount 1..500 cents crossed with
// the minimal availability caps admitting any two equal-count subsets) finds zero
// reachable ties - whenever two equal-count subsets both fit under the caps, a
// strictly cheaper subset fits too.  The minimum-coin composition is unique, so any
// enumeration order prints the same answer.
//
// The discuss board's one recurring trap is precision: (int)(atof(tok)*100) is
// off by one on 34 of the 500 possible amounts (e.g. 2.55, 4.60, 2.01), and the
// official sample does not catch it - 0.45 and 0.75 both truncate correctly.
// The amount is therefore parsed from the token's digits directly, never as a
// double.  Input is read to EOF since the data sets are not counted in the file.
#include <cstdio>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

/* Parse a monetary amount into whole cents without touching floating point.
   Accepts "5", "5.0", "5.00", ".45", "1.65"; extra fraction digits are ignored. */
static int toCents(const string& s) {
    size_t i = 0;
    int sign = 1;
    while (i < s.size() && !isdigit((unsigned char)s[i]) &&
           s[i] != '.' && s[i] != '+' && s[i] != '-') ++i;   /* tolerate a '$' */
    if (i < s.size() && (s[i] == '+' || s[i] == '-')) {
        if (s[i] == '-') sign = -1;
        ++i;
    }
    int whole = 0;
    while (i < s.size() && isdigit((unsigned char)s[i])) {
        whole = whole * 10 + (s[i] - '0');
        ++i;
    }
    int frac = 0, digits = 0;
    if (i < s.size() && (s[i] == '.' || s[i] == ',')) {
        ++i;
        while (i < s.size() && isdigit((unsigned char)s[i])) {
            if (digits < 2) { frac = frac * 10 + (s[i] - '0'); ++digits; }
            ++i;
        }
    }
    while (digits < 2) { frac *= 10; ++digits; }
    return sign * (whole * 100 + frac);
}

int main() {
    string tok;
    int B, C, D, E;
    while (cin >> tok >> B >> C >> D >> E) {
        int a = toCents(tok);
        if (B < 0) B = 0;
        if (C < 0) C = 0;
        if (D < 0) D = 0;
        if (E < 0) E = 0;

        int best = -1, bq = 0, bd = 0, bn = 0, bp = 0;
        if (a >= 0) {
            int maxQ = a / 25; if (maxQ > B) maxQ = B;
            for (int q = 0; q <= maxQ; ++q) {
                int r1 = a - 25 * q;
                int maxD = r1 / 10; if (maxD > C) maxD = C;
                for (int d = 0; d <= maxD; ++d) {
                    int r2 = r1 - 10 * d;
                    int maxN = r2 / 5; if (maxN > D) maxN = D;
                    for (int n = 0; n <= maxN; ++n) {
                        int p = r2 - 5 * n;
                        if (p > E) continue;
                        int cnt = q + d + n + p;
                        if (best < 0 || cnt < best) {
                            best = cnt; bq = q; bd = d; bn = n; bp = p;
                        }
                    }
                }
            }
        }
        if (best < 0) printf("NO EXACT CHANGE\n");
        else printf("%d %d %d %d\n", bq, bd, bn, bp);
    }
    return 0;
}
