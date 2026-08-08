// POJ 2527 - Polynomial Remains
// Model: claude-opus-5
//
// Remainder of a(x) = a_n x^n + ... + a_0 modulo x^k + 1.
// In the quotient ring, x^k == -1, so x^(qk+s) == (-1)^q x^s and the whole
// division collapses to a single fold: r[j mod k] += (-1)^(j/k) * a_j.
// One O(n) pass, no long division needed.
//
// k == 0 is the special case: the divisor is the constant polynomial 2, so
// the remainder is identically 0 (the statement's samples "1 0 / 5 1" and
// "0 0 / 7" both print 0, which is what pins this reading).
//
// Output: trim trailing zero coefficients so exactly deg(r)+1 of them print;
// if every coefficient is zero, print a single "0".
//
// Input framing hardening, none of it pinned by the statement: coefficients
// may be spread across lines arbitrarily, so tokens are read individually
// rather than by line; the loop also stops at EOF, not only at the "-1 -1"
// sentinel; n and k are not assumed to respect the stated 10000 bound (the
// buffer is a vector); and accumulation is in long long, since the statement
// bounds only the *final* remainder coefficients to 32 bits.
//
// Submitted under language 0 (G++) rather than 4 (C++) for long long; output
// goes through iostream, not printf, to avoid the MinGW %lld handling.

#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static bool readLL(long long &out) {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-' || c == '+') { neg = (c == '-'); c = gc(); }
    long long v = 0;
    bool any = false;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); any = true; c = gc(); }
    if (!any) return false;
    out = neg ? -v : v;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    long long n, k;
    vector<long long> r;
    while (readLL(n) && readLL(k)) {
        if (n < 0 || k < 0) break;
        r.assign((size_t)(k > 0 ? k : 1), 0LL);
        for (long long j = 0; j <= n; ++j) {
            long long a;
            if (!readLL(a)) a = 0;
            if (k > 0) {
                long long q = j / k, s = j % k;
                if (q & 1LL) r[(size_t)s] -= a; else r[(size_t)s] += a;
            }
        }
        size_t d = (k > 0) ? r.size() : 0;
        while (d > 0 && r[d - 1] == 0) --d;
        if (d == 0) {
            cout << 0 << "\n";
        } else {
            for (size_t i = 0; i < d; ++i) {
                if (i) cout << ' ';
                cout << r[i];
            }
            cout << "\n";
        }
    }
    return 0;
}
