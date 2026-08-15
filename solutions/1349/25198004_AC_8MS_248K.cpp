// POJ 1349 - Coding of Permutations
// Model: claude-sonnet-5
//
// For permutation P = (p1,...,pn), its dictionary rank (1-indexed) is
//   d(P) = 1 + sum_{i=1}^{n} c_i * (n-i)!
// where c_i is the number of elements to the right of position i that are
// smaller than p_i (the standard "inversion table" / factorial-number-system
// encoding, aka Lehmer code).  n <= 50, so 49! has about 63 digits and needs
// bignum arithmetic; implemented here as base-10 digit vectors (little
// endian) with add and multiply-by-small-int.
//
// Input is a sequence of "(n,(p1,p2,...,pn))" tuples, one after another
// (not necessarily one per line -- parsed as a flat token stream skipping
// all punctuation), terminated by a line/token "-1".  Output is a single
// line with all d(P) values in order, separated by commas (see sample:
// "15,67").  Verified against the sample and against a 46-case data set
// posted on the problem's discussion board (message 47662), which matches
// exactly.
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

typedef vector<int> Big; // base 10, little endian, no leading zero digits (except value 0 -> {0})

Big fromSmall(int x) {
    Big r;
    if (x == 0) { r.push_back(0); return r; }
    while (x > 0) { r.push_back(x % 10); x /= 10; }
    return r;
}

Big mulSmall(const Big &a, int k) {
    if (k == 0) return fromSmall(0);
    Big res;
    long carry = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        long cur = (long)a[i] * k + carry;
        res.push_back((int)(cur % 10));
        carry = cur / 10;
    }
    while (carry) { res.push_back((int)(carry % 10)); carry /= 10; }
    while (res.size() > 1 && res.back() == 0) res.pop_back();
    return res;
}

Big add(const Big &a, const Big &b) {
    Big res;
    int carry = 0;
    size_t n = a.size() > b.size() ? a.size() : b.size();
    for (size_t i = 0; i < n || carry; ++i) {
        int x = carry;
        if (i < a.size()) x += a[i];
        if (i < b.size()) x += b[i];
        res.push_back(x % 10);
        carry = x / 10;
    }
    while (res.size() > 1 && res.back() == 0) res.pop_back();
    return res;
}

string toStr(const Big &a) {
    string s;
    for (size_t i = 0; i < a.size(); ++i) s.push_back((char)('0' + a[a.size() - 1 - i]));
    return s;
}

int main() {
    // read all of stdin
    string buf;
    {
        std::ios::sync_with_stdio(false);
        char c;
        while (cin.get(c)) buf.push_back(c);
    }

    // tokenize integers (optionally negative); all values here are tiny
    // (n <= 50, permutation entries <= 50), so plain long suffices.
    vector<long> tok;
    size_t i = 0, n = buf.size();
    while (i < n) {
        char c = buf[i];
        if (c == '-' && i + 1 < n && isdigit((unsigned char)buf[i+1])) {
            size_t j = i + 1;
            while (j < n && isdigit((unsigned char)buf[j])) ++j;
            tok.push_back(-strtol(buf.substr(i+1, j-i-1).c_str(), 0, 10));
            i = j;
        } else if (isdigit((unsigned char)c)) {
            size_t j = i;
            while (j < n && isdigit((unsigned char)buf[j])) ++j;
            tok.push_back(strtol(buf.substr(i, j-i).c_str(), 0, 10));
            i = j;
        } else {
            ++i;
        }
    }

    // precompute factorials 0..50
    static Big fact[51];
    fact[0] = fromSmall(1);
    for (int k = 1; k <= 50; ++k) fact[k] = mulSmall(fact[k-1], k);

    vector<string> results;
    size_t idx = 0;
    while (idx < tok.size()) {
        long nn = tok[idx];
        if (nn == -1) break;
        ++idx;
        int cnt = (int)nn;
        vector<int> perm(cnt);
        for (int k = 0; k < cnt; ++k) perm[k] = (int)tok[idx + k];
        idx += cnt;

        Big sum = fromSmall(0);
        for (int p = 0; p < cnt; ++p) {
            int c = 0;
            for (int q = p + 1; q < cnt; ++q) if (perm[q] < perm[p]) ++c;
            if (c > 0) {
                Big term = mulSmall(fact[cnt - 1 - p], c);
                sum = add(sum, term);
            }
        }
        sum = add(sum, fromSmall(1));
        results.push_back(toStr(sum));
    }

    for (size_t k = 0; k < results.size(); ++k) {
        if (k) printf(",");
        printf("%s", results[k].c_str());
    }
    printf("\n");
    return 0;
}
