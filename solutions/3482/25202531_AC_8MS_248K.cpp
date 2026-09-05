// POJ 3482 - 'JBC'
// Model: claude-sonnet-5
//
// The first line of a data set defines a digit alphabet: the i-th character
// (0-indexed) has decimal value i. A data set has N = length of that string
// defined digit symbols with values 0..N-1. For each following number (until
// a blank line / EOF), it may be interpreted in any base b with
// max(2, maxDigit+1) <= b <= N (maxDigit = largest digit value appearing in
// the number); the answer is the sum, over all such valid bases, of the
// number's decimal value when read in base b. Values can have many decimal
// digits (per the discuss board, an unbounded-precision sum is required), so
// bignum arithmetic (base 1e9 limbs) is used throughout.
//
// Ambiguity/robustness notes taken from the discuss board (bbs?problem_id=3482):
//  - Data sets are separated by ONE OR MORE blank lines; a line that becomes
//    empty after trimming never occurs as actual number/definition data, so
//    every blank line (after trim) is purely a separator.
//  - Every input line may carry leading/trailing whitespace that must be
//    stripped before use (this includes stray '\r' from CRLF endings).
//  - The very last data set may end at EOF with no trailing newline at all.
//  - This problem is notorious for WA from insufficient-precision arithmetic
//    (a message explicitly reports "WA without a real bignum template").
//
// Verified against both official samples and a community-posted stress case
// (ordering "0123456789abcdef", numbers "111".."aaaaaaaaaaaa") whose expected
// sums were reproduced exactly by hand derivation before coding, then by the
// program itself.

#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

typedef vector<unsigned int> Big; // little-endian, base 1e9 limbs
const unsigned int BASE = 1000000000u;

static void trimTrailingZeroLimbs(Big &a) {
    while (a.size() > 1 && a.back() == 0) a.pop_back();
}

// a = a * m + add, where m and add are small non-negative integers (< 1e5 here, base<=94)
static void mulAdd(Big &a, unsigned long long m, unsigned long long add) {
    unsigned long long carry = add;
    for (size_t i = 0; i < a.size(); ++i) {
        unsigned long long cur = (unsigned long long)a[i] * m + carry;
        a[i] = (unsigned int)(cur % BASE);
        carry = cur / BASE;
    }
    while (carry > 0) {
        a.push_back((unsigned int)(carry % BASE));
        carry /= BASE;
    }
    if (a.empty()) a.push_back(0);
}

static void addTo(Big &a, const Big &b) {
    if (a.size() < b.size()) a.resize(b.size(), 0);
    unsigned long long carry = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        unsigned long long cur = (unsigned long long)a[i] + carry + (i < b.size() ? b[i] : 0);
        a[i] = (unsigned int)(cur % BASE);
        carry = cur / BASE;
    }
    if (carry) a.push_back((unsigned int)carry);
}

static string toString(const Big &a) {
    Big b = a;
    trimTrailingZeroLimbs(b);
    char buf[16];
    string s;
    sprintf(buf, "%u", b.back());
    s += buf;
    for (int i = (int)b.size() - 2; i >= 0; --i) {
        sprintf(buf, "%09u", b[i]);
        s += buf;
    }
    return s;
}

static string trim(const string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b - 1])) --b;
    return s.substr(a, b - a);
}

int main() {
    ios_base::sync_with_stdio(false);
    vector<string> lines;
    {
        string line;
        while (getline(cin, line)) {
            lines.push_back(trim(line));
        }
    }

    size_t i = 0;
    bool firstDataset = true;
    int digitVal[256];

    while (i < lines.size()) {
        while (i < lines.size() && lines[i].empty()) ++i;
        if (i >= lines.size()) break;
        const string &ordering = lines[i++];
        vector<string> numbers;
        while (i < lines.size() && !lines[i].empty()) {
            numbers.push_back(lines[i++]);
        }

        for (int k = 0; k < 256; ++k) digitVal[k] = -1;
        for (size_t p = 0; p < ordering.size(); ++p) {
            digitVal[(unsigned char)ordering[p]] = (int)p;
        }
        long long N = (long long)ordering.size();

        if (!firstDataset) printf("\n");
        firstDataset = false;

        for (size_t ni = 0; ni < numbers.size(); ++ni) {
            const string &num = numbers[ni];
            int maxDigit = 0;
            for (size_t c = 0; c < num.size(); ++c) {
                int v = digitVal[(unsigned char)num[c]];
                if (v > maxDigit) maxDigit = v;
            }
            long long minBase = maxDigit + 1;
            if (minBase < 2) minBase = 2;
            long long maxBase = N;

            Big total;
            total.push_back(0);
            for (long long b = minBase; b <= maxBase; ++b) {
                Big val;
                val.push_back(0);
                for (size_t c = 0; c < num.size(); ++c) {
                    int v = digitVal[(unsigned char)num[c]];
                    mulAdd(val, (unsigned long long)b, (unsigned long long)v);
                }
                addTo(total, val);
            }
            printf("%s\n", toString(total).c_str());
        }
    }
    return 0;
}
