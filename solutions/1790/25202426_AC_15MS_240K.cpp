// POJ 1790 - Base Numbers
// Model: claude-sonnet-5
//
// A decimal-encoded number "(d_k-d_{k-1}-...-d_0)_r" with dashes and
// parentheses removed becomes the concatenation of the decimal strings for
// d_k, d_{k-1}, ..., d_0, r (in that order). We must count, for a given
// digit code, how many ways it can be split into such a sequence where:
//   - there is at least one digit d_0 before the base r,
//   - every segment (each digit and the base) has no *internal* leading
//     zero unless the segment is the single character "0",
//   - the base r > 1,
//   - every digit value is strictly less than r,
//   - AND the leading digit d_k (the very first digit segment) is not "0"
//     unless it is the only digit (i.e. the whole number is literally 0).
// That last rule is easy to miss: the statement's own examples state
// "(0-3-6)_8" is invalid even though 0, 3, 6 are all individually < 8 and
// each piece alone has no internal leading zero -- the 0 is invalid only
// because it leads a multi-digit number, exactly like a leading zero in
// ordinary positional notation. Meanwhile "(0)_16" (a lone zero digit) and
// "(1-0-0-0)_7" (zeros in non-leading positions) are explicitly valid. A
// first implementation missed this and got Wrong Answer even though it
// matched the given sample, because the sample happens not to exercise it.
//
// For each split point j (1 <= j <= n-1) the suffix s[j..n-1] is the base r
// (must have no leading zero and be > 1). For the prefix s[0..j-1] we need
// the number of ways to decompose it into valid digit segments (each < r,
// no internal leading zero) such that the first segment isn't "0" unless
// it's the whole prefix. This is computed with a backward DP suf[i] = ways
// to decompose s[i..j-1] with no restriction on its own first piece (that
// restriction only applies once, to position 0): suf[j] = 1, and
// suf[i] = sum over valid piece lengths l of suf[i+l]. Then for position 0
// we sum suf[l1] over first-piece lengths l1 whose piece is valid and not
// "0" (or, if it is "0", accept only when l1 == j, meaning the whole prefix
// is that single 0 digit). n <= 35 so this O(n^3) computation is trivial;
// counts can reach roughly 1e9-scale (checked via local search over
// all-digit strings before this fix), so a 64-bit accumulator is used.

#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

typedef unsigned long long ull;

int main() {
    char buf[64];
    while (scanf("%s", buf) == 1) {
        if (buf[0] == '#' && buf[1] == '\0') break;
        string s(buf);
        int n = (int)s.size();
        ull total = 0;
        for (int j = 1; j < n; ++j) {
            // suffix s[j..n-1] is the base r
            int L = n - j;
            if (s[j] == '0') continue; // leading zero in base
            if (L == 1 && (s[j] == '0' || s[j] == '1')) continue; // r > 1
            string r = s.substr(j, L);

            // piece_ok(a, l): is s[a..a+l-1] a valid digit piece (< r, no
            // internal leading zero)?
            // suf[i] = number of ways to decompose s[i..j-1] into valid
            // pieces, no restriction on the first piece of this sub-range.
            static ull suf[40];
            suf[j] = 1;
            for (int i = j - 1; i >= 0; --i) {
                ull sum = 0;
                int maxl = (L < (j - i)) ? L : (j - i);
                for (int l = 1; l <= maxl; ++l) {
                    if (l > 1 && s[i] == '0') continue; // internal leading zero
                    bool ok;
                    if (l < L) {
                        ok = true;
                    } else { // l == L
                        ok = (s.compare(i, l, r) < 0);
                    }
                    if (ok) sum += suf[i + l];
                }
                suf[i] = sum;
            }

            // Now restrict the very first piece (starting at position 0):
            // it must not be "0" unless it spans the whole prefix (l1 == j).
            ull restricted = 0;
            int maxl1 = (L < j) ? L : j;
            for (int l1 = 1; l1 <= maxl1; ++l1) {
                if (l1 > 1 && s[0] == '0') continue; // internal leading zero
                bool ok;
                if (l1 < L) {
                    ok = true;
                } else { // l1 == L
                    ok = (s.compare(0, l1, r) < 0);
                }
                if (!ok) continue;
                if (l1 == 1 && s[0] == '0' && l1 != j) continue; // leading zero of whole number
                restricted += suf[l1];
            }
            total += restricted;
        }
        if (total == 0) {
            printf("The code %s is invalid.\n", buf);
        } else {
            printf("The code %s can represent %llu numbers.\n", buf, total);
        }
    }
    return 0;
}
