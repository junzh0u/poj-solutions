// POJ 2640 - Playground
// Model: claude-opus-5
//
// A half-circle of radius a joins two solder points that are exactly 2a apart
// (its diameter), so a chain of half-circles closes up iff the chords 2a_i can
// be arranged as a closed polygon: max(2a) <= sum of the remaining 2a, i.e.
// max(a) <= sum of the rest.  With the semicircles bulging outward from a
// convex arrangement the figure never self-intersects, and the degenerate
// (collinear) case max == sum-of-rest is still a valid closed figure -- the
// discuss board's "closed figures include the straight line".  Two equal wires
// (a circle) is the smallest YES, matching the sample.
//
// Not every wire has to be used.  Adding a wire that is not the new maximum
// only helps, so the only subsets worth testing are "all wires with radius <=
// a_i, one of them acting as the maximum": sort ascending and answer YES iff
// some a[i] (i >= 1) is <= a[0] + ... + a[i-1].
//
// Exactness: radii carry at most 3 decimals and are < 1e7, so each is parsed
// by hand into an exact integer scaled by 1000 (< 1e10).  Twenty of those sum
// to < 2e11, well inside the 2^53 range where a double holds integers exactly,
// so every addition and comparison below is exact and the statement's warning
// about floating point errors does not bite.

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <string>

// Parse a decimal literal with at most 3 fractional digits into an exact
// integer scaled by 1000, returned as a double (exactly representable).
static double parse_milli(const std::string &s) {
    size_t i = 0;
    double sign = 1.0;
    if (i < s.size() && (s[i] == '+' || s[i] == '-')) {
        if (s[i] == '-') sign = -1.0;
        ++i;
    }
    double whole = 0.0;
    while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
        whole = whole * 10.0 + (s[i] - '0');
        ++i;
    }
    double frac = 0.0;
    int digits = 0;
    if (i < s.size() && s[i] == '.') {
        ++i;
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
            if (digits < 3) {
                frac = frac * 10.0 + (s[i] - '0');
                ++digits;
            }
            ++i;
        }
    }
    while (digits < 3) { frac *= 10.0; ++digits; }
    return sign * (whole * 1000.0 + frac);
}

int main() {
    int k;
    while (std::cin >> k) {
        if (k <= 0) break;
        double a[32];
        for (int i = 0; i < k; ++i) {
            std::string tok;
            if (!(std::cin >> tok)) return 0;
            a[i] = parse_milli(tok);
        }
        std::sort(a, a + k);
        bool ok = false;
        double sum = 0.0;
        for (int i = 0; i + 1 < k; ++i) {
            sum += a[i];
            if (a[i + 1] <= sum) { ok = true; break; }
        }
        std::cout << (ok ? "YES" : "NO") << "\n";
    }
    return 0;
}
