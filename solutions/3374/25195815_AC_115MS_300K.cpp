// POJ 3374 - Cake Share
// Model: claude-opus-5
//
// Cutting the cake into s pieces (1 <= s <= N) and handing over t of them
// (0 <= t <= s) gives GtDzx the fraction t/s, and two (s,t) count as the same
// case exactly when that fraction is equal.  So the sorted list of cases is the
// set of distinct reduced fractions in [0,1] with denominator <= N, in
// increasing order: the Farey sequence F_N, from 0/1 to 1/1.  The k-th case is
// its k-th term, and k beyond |F_N| = 1 + sum_{s=1..N} phi(s) has no answer.
//
// |F_5000| = 7600459, far too many to tabulate for 3000 queries (the discuss
// board is a row of TLE/MLE reports from people who built the table).  Instead
// read all C queries first, sort them by k, and walk F_N once with the standard
// successor recurrence -- given consecutive a/b < c/d, the next term is
// (m*c-a)/(m*d-b) with m = (N+b)/d -- emitting answers as the walk passes their
// indices.  One pass, O(|F_N| + C log C) time and O(C) memory; the walk needs no
// gcd and no table at all.
//
// All arithmetic stays in 32-bit int: m <= N+1 and c,d <= N, so m*c <= 2.5e7,
// and the largest index 7600459 fits comfortably.  Queries are read as text and
// anything past 9 digits is treated as out of range, so an oversized k cannot
// wrap into a valid index.
//
// The statement fixes the input framing: one data set, and exactly C query
// lines, which is what is read -- no EOF loop.

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

const int MAXC = 3005;

struct Q {
    int k;   /* query value; 0 marks "too large to be any index" */
    int id;
};

static bool byK(const Q& a, const Q& b) { return a.k < b.k; }

static Q qs[MAXC];
static int ansNum[MAXC], ansDen[MAXC];
static bool ok[MAXC];

int main() {
    int n, C;
    if (scanf("%d %d", &n, &C) != 2) return 0;
    if (n < 1) n = 1;

    char buf[64];
    int i;
    for (i = 0; i < C; i++) {
        if (scanf("%63s", buf) != 1) { C = i; break; }
        const char* p = buf;
        while (*p == '0') p++;                  /* strip leading zeros */
        if (strlen(p) > 9) qs[i].k = 0;         /* larger than any Farey index */
        else qs[i].k = atoi(p);
        qs[i].id = i;
        ok[i] = false;
    }

    sort(qs, qs + C, byK);

    int qi = 0;
    while (qi < C && qs[qi].k <= 0) qi++;

    /* Walk F_n with the standard next-term recurrence, answering queries in order. */
    int a = 0, b = 1, c = 1, d = n;
    int idx = 1;
    while (qi < C && qs[qi].k == idx) { int id = qs[qi].id; ansNum[id] = a; ansDen[id] = b; ok[id] = true; qi++; }
    while (qi < C && !(a == 1 && b == 1)) {
        idx++;
        while (qi < C && qs[qi].k == idx) { int id = qs[qi].id; ansNum[id] = c; ansDen[id] = d; ok[id] = true; qi++; }
        if (c == 1 && d == 1) { a = c; b = d; break; }
        int m = (n + b) / d;
        int nc = m * c - a, nd = m * d - b;
        a = c; b = d; c = nc; d = nd;
    }

    for (i = 0; i < C; i++) {
        if (ok[i]) printf("%d/%d\n", ansNum[i], ansDen[i]);
        else printf("No Solution\n");
    }
    return 0;
}
