// POJ 2969 - Divisibility by 15
// Model: claude-opus-5
//
// Pick a sub-multiset of the given digits and order it freely so the value is
// a multiple of 15, i.e. of both 5 and 3.  The last digit must therefore be 0
// or 5, and the digit sum must be divisible by 3.
//
// Try both terminals.  With terminal t reserved, the best arrangement of the
// rest is the descending sort, so maximising the number means dropping as few
// digits as possible and, among equal-size drops, the smallest ones.  Writing
// r for (total digit sum) mod 3, a drop must have sum = r (mod 3): if some
// digit d = r (mod 3) is left, drop the smallest such (one digit); otherwise
// drop the two smallest digits = -r (mod 3).  Three or more is never needed --
// if neither of those exists there is no digit = r and at most one = -r, and
// no subset of the rest can sum to r (mod 3) at all.  Zeros are never dropped
// (they are 0 mod 3), so a string containing a zero always has an answer, "0"
// in the worst case; without a zero and without a usable 5 it is impossible.
//
// Comparison of the two terminals is by length then lexicographically, though
// a 0 terminal in fact always wins when it is available: appending a spare 0
// to any 5-terminated solution multiplies its value by 10 and stays valid.
//
// Verified against the sample, 32 cases posted on the discuss board (the board
// corrects two of its own entries: 8885830 -> 88830 and 456277 -> 77625), and
// exhaustively against a permutation brute force for every string of length
// <= 4 over all ten digits plus lengths 5 and 6 over three restricted digit
// alphabets.

#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

/* drop leading zeroes; an all-zero number prints as "0" */
static string strip(const string &s) {
    size_t i = 0;
    while (i + 1 < s.size() && s[i] == '0') ++i;
    return s.substr(i);
}

static bool better(const string &a, const string &b) { /* a > b ? */
    if (a.size() != b.size()) return a.size() > b.size();
    return a > b;
}

/* build the best number that uses terminal digit t (0 or 5) */
static bool build(const int cnt0[10], int total, int t, string &out) {
    if (cnt0[t] == 0) return false;
    int c[10];
    for (int i = 0; i < 10; ++i) c[i] = cnt0[i];
    c[t]--;
    int r = total % 3;
    if (r != 0) {
        int one = -1;
        for (int d = 1; d <= 9; ++d)
            if (c[d] > 0 && d % 3 == r) { one = d; break; }
        if (one >= 0) {
            c[one]--;
        } else {
            int need = 3 - r, got = 0;
            for (int d = 1; d <= 9 && got < 2; ++d) {
                if (d % 3 != need) continue;
                while (c[d] > 0 && got < 2) { c[d]--; got++; }
            }
            if (got < 2) return false;
        }
    }
    string s;
    for (int d = 9; d >= 0; --d)
        for (int k = 0; k < c[d]; ++k) s += char('0' + d);
    s += char('0' + t);
    out = strip(s);
    return true;
}

int main() {
    static char buf[1100];
    if (scanf("%1050s", buf) != 1) return 0;
    int cnt[10];
    memset(cnt, 0, sizeof(cnt));
    int total = 0;
    for (int i = 0; buf[i]; ++i) {
        int d = buf[i] - '0';
        cnt[d]++;
        total += d;
    }
    string best = "", cand;
    if (build(cnt, total, 0, cand)) best = cand;
    if (build(cnt, total, 5, cand))
        if (best.empty() || better(cand, best)) best = cand;
    if (best.empty()) printf("impossible\n");
    else printf("%s\n", best.c_str());
    return 0;
}
