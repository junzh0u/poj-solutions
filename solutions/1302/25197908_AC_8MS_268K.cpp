// POJ 1302 - Blue Gene, Jr.
// Model: claude-sonnet-5
//
// Simulation of a recursive mutation process on a string of up to 20
// alphanumeric characters.  A "viral segment" is always a suffix of the
// string, identified by its starting (1-indexed) position.  process(i)
// mutates position i (if it mutates at all) and everything that gets
// triggered from it, returning the total number of individual character
// mutations that occurred as part of processing the segment starting at i:
//
//  - if s[i] is empty (i > n): no segment, 0 mutations.
//  - if s[i] is a letter: it is unstable.  Let m = process(i+1) (the
//    mutation count of the segment immediately to its right, which is 0
//    both when i == n and when the recursive processing genuinely produces
//    0).  The letter becomes m % 10 (this handles "mutate into n unless
//    n > 9, in which case n % 10" and "no segment to the right -> 0" in one
//    uniform formula, since m == 0 gives m % 10 == 0 as well).  Returns
//    1 + m (its own mutation, plus everything triggered to its right).
//  - if s[i] is a nonzero digit d: it is unstable and becomes d - 1 (one
//    mutation).  It also triggers the segment starting at i + d if that
//    position exists, otherwise the segment immediately to its right
//    (i + 1) if that exists, otherwise nothing.  Returns 1 plus whatever
//    that triggered segment's processing counted.
//  - if s[i] is '0': stable, no mutation, no further triggering; returns 0.
//
// Because the digit/letter rule always advances strictly to the right
// (i+1 or i+d with d >= 1), the chain of positions actually visited is
// strictly increasing, so recursion depth is bounded by n (<= 20) and the
// whole thing terminates trivially; verified against all 6 sample cases.
//
// Statement ambiguity: none found once the suffix/segment reading above is
// applied consistently -- the discuss board's own paraphrase and multiple
// posted accepted-shaped solutions (e.g. message 351873) agree with this
// recursive reading, and it reproduces every sample line exactly.

#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

static char s[30];
static int n;

int process(int i) {
    if (i > n) return 0;
    char c = s[i];
    if (c >= 'A' && c <= 'Z') {
        int m = process(i + 1);
        s[i] = static_cast<char>('0' + (m % 10));
        return 1 + m;
    } else if (c >= '1' && c <= '9') {
        int d = c - '0';
        s[i] = static_cast<char>('0' + (d - 1));
        int rest;
        if (i + d <= n) rest = process(i + d);
        else if (i + 1 <= n) rest = process(i + 1);
        else rest = 0;
        return 1 + rest;
    } else {
        return 0; // '0': stable
    }
}

int main() {
    string tok;
    while (cin >> tok) {
        if (tok == "ENDOFINPUT") break;
        // tok == "START"
        cin >> n;
        cin >> (s + 1);
        cin >> tok; // "END"
        process(1);
        s[n + 1] = '\0';
        cout << (s + 1) << "\n";
    }
    return 0;
}
