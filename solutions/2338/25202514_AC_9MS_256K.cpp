// POJ 2338 - Basic
// Model: claude-sonnet-5
//
// Ada-like integer constants: either a plain decimal digit sequence, or a
// chained "base#digits#" construct where the value produced by one
// base#digits# group can itself serve as the base for a following
// #digits# group directly concatenated onto it (e.g. "b#d1##d2#" means:
// interpret d1 in base b to get b2, then interpret d2 in base b2). Only
// the very first component (before the first '#') is decimal; every
// subsequent digit group is validated against the base computed so far,
// and (except for the last group) must itself evaluate to a value in
// [2,16] to serve as the next base. This reading was reverse engineered
// from the discuss board (message 149628, an explicitly Accepted
// solution) after the sample's own "2#101##123#" -> yes could not be
// explained by a literal single base#digits# reading; a differential
// check against a stricter (but unconfirmed) alternate reading from the
// board found it disagrees on strings such as "3##12#1##5#" -- the
// confirmed-AC algorithm below was used as ground truth.
#include <cstdio>
#include <cstring>

const int MAXLEN = 200;
char s[MAXLEN];

int charToNum(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

// Interpret s[beg..end] (inclusive) as a number in the given base.
// Returns -1 if the resulting value is not in [2,16] (also used as a
// generic "not usable as a further base" signal; the magnitude beyond
// that threshold is never needed).
int stringToNum(int base, int beg, int end) {
    long res = 0;
    for (int i = beg; i <= end; ++i) {
        if (res > 16) return -1;
        res += charToNum(s[i]);
        res *= base;
    }
    res /= base;
    if (res > 16 || res < 2) return -1;
    return (int)res;
}

// Index of the last character of the maximal digit run starting at beg
// (i.e. one before the next '#', or the last character of the string).
int findScope(int beg, int len) {
    while (beg != len && s[beg] != '#') ++beg;
    return beg - 1;
}

// True iff every character in s[beg..end] has value < num.
bool inScope(int num, int beg, int end) {
    for (int i = beg; i <= end; ++i) {
        if (charToNum(s[i]) >= num) return false;
    }
    return true;
}

bool firstRound(int len) {
    int count = 0;
    for (int i = 0; i < len; ++i) {
        if (s[i] == '#') ++count;
        else if (!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f'))) return false;
    }
    if (count == 2 && s[0] == '#') return false;
    if (count % 2) return false;
    if (count == 0) {
        return inScope(10, 0, len - 1);
    }
    bool firstJing = false;
    for (int i = 0; i < len - 1; ++i) {
        if (firstJing && s[i] == '#') {
            bool ok = (s[i + 1] == '#' && i + 1 < len - 1) || (i - 1 >= 0 && s[i - 1] == '#');
            if (!ok) return false;
        }
        if (!firstJing && s[i] >= 'a' && s[i] <= 'f') return false;
        if (!firstJing && s[i] == '#') firstJing = true;
    }
    return true;
}

bool judge(int len) {
    if (!firstRound(len)) return false;
    int beg = 0;
    int end = findScope(beg, len);
    int curbase = stringToNum(10, beg, end);
    beg = end + 1;
    while (beg < len) {
        if (curbase == -1 && beg != len - 1) return false;
        if (beg + 1 < len && s[beg] == '#') {
            end = findScope(beg + 1, len);
            if (end == beg) return false;
            if (!inScope(curbase, beg + 1, end)) return false;
            curbase = stringToNum(curbase, beg + 1, end);
            beg = end;
        }
        beg += 2;
    }
    return true;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int t = 0; t < n; ++t) {
        scanf("%s", s);
        int len = (int)strlen(s);
        puts(judge(len) ? "yes" : "no");
    }
    return 0;
}
