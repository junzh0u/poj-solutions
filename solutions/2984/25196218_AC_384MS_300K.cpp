// POJ 2984 - A New Joseph Problem
// Model: claude-sonnet-5
//
// Repeatedly applying the Josephus survivor function J (kill every 2nd
// person, 1-indexed) is, in binary, a left-rotation of N's bit string:
// if N's binary representation is 1 b_{m-1} ... b_0, then J(N)'s binary
// representation is b_{m-1} ... b_0 1 (leading 1 moved to the end,
// leading zeros then stripped as the bit-length shrinks). Rotation never
// changes the multiset of bits, and stripping leading zeros never
// removes a 1-bit, so popcount(N) is an invariant of the whole iteration.
// A fixed point J(K)=K forces K = 2^t - 1 (all-ones binary), whose
// popcount is t. Hence the limit K equals 2^popcount(N) - 1.
//
// N can have up to 10^10000 (about 10001 decimal digits / 33220 bits),
// so both the decimal->binary popcount extraction and the 2^P-1
// reconstruction are done with base-10000 bignum arithmetic (repeated
// divide-by-2 for popcount, repeated double-and-add-1 to rebuild the
// decimal result) to keep the O(digits * bits) work fast enough.
//
// Ambiguity: none in the statement; the discuss board independently
// confirms the same "rotate left, ones collect together" derivation and
// a posted example (999...9, 39 nines) => 19342813113834066795298815,
// which matches this formula and was checked against a local brute-force
// Josephus simulation (deque-based) plus the derivation above for all
// N in [1, 499].

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

typedef vector<int> Big; // MSB-first, base 10000 limbs

static Big fromDecimalString(const string &s) {
    int len = (int)s.size();
    int pad = (4 - len % 4) % 4;
    string t(pad, '0');
    t += s;
    Big v;
    v.reserve(t.size() / 4);
    for (size_t i = 0; i < t.size(); i += 4) {
        int x = (t[i] - '0') * 1000 + (t[i+1] - '0') * 100 +
                (t[i+2] - '0') * 10 + (t[i+3] - '0');
        v.push_back(x);
    }
    size_t idx = 0;
    while (idx + 1 < v.size() && v[idx] == 0) idx++;
    if (idx > 0) v.erase(v.begin(), v.begin() + idx);
    return v;
}

static bool isZero(const Big &v) {
    return v.size() == 1 && v[0] == 0;
}

// divide by 2 in place, return remainder bit (0 or 1)
static int divBy2(Big &v) {
    int carry = 0;
    for (size_t i = 0; i < v.size(); i++) {
        int cur = carry * 10000 + v[i];
        v[i] = cur / 2;
        carry = cur % 2;
    }
    size_t idx = 0;
    while (idx + 1 < v.size() && v[idx] == 0) idx++;
    if (idx > 0) v.erase(v.begin(), v.begin() + idx);
    return carry;
}

static int popcountOf(Big v) {
    int p = 0;
    while (!isZero(v)) {
        p += divBy2(v);
    }
    return p;
}

int main() {
    string s;
    if (!(std::cin >> s)) return 0;
    // strip any accidental leading zeros (n >= 1 per statement, but be safe)
    size_t st = 0;
    while (st + 1 < s.size() && s[st] == '0') st++;
    s = s.substr(st);

    Big n = fromDecimalString(s);
    int P = popcountOf(n);

    // build 2^P - 1 in base-10000, little-endian limbs, via
    // result = result*2 + 1, repeated P times, starting from 0.
    vector<int> r;
    r.push_back(0);
    for (int i = 0; i < P; i++) {
        int carry = 1;
        for (size_t j = 0; j < r.size(); j++) {
            int t = r[j] * 2 + carry;
            r[j] = t % 10000;
            carry = t / 10000;
        }
        if (carry) r.push_back(carry);
    }

    // print little-endian limbs from most significant to least
    int idx = (int)r.size() - 1;
    printf("%d", r[idx]);
    for (int i = idx - 1; i >= 0; i--) {
        printf("%04d", r[i]);
    }
    printf("\n");
    return 0;
}
