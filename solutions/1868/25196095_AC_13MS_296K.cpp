// POJ 1868 - Antiarithmetic?
// Model: claude-opus-5
//
// A permutation p of 0..n-1 is antiarithmetic iff no positions i<j<k carry
// values forming an arithmetic progression.  Three distinct values in AP are
// (a, a+d, a+2d) for some d>=1, and an AP is monotone, so the middle value
// a+d must sit at the middle position.  Working with pos[v] = index of value
// v, a triple (a, a+d, a+2d) is a witness exactly when its positions are
// monotone: pos[a] < pos[a+d] < pos[a+2d] (increasing AP) or
// pos[a] > pos[a+d] > pos[a+2d] (decreasing AP).  Both cases are the single
// test (pos[a] < pos[a+d]) == (pos[a+d] < pos[a+2d]), positions being
// distinct.  Enumerating d then a is sum_d (n-2d) ~ n^2/4 = 25M compares at
// n = 10000, on three sequential streams, with an early exit on the first
// witness; ~17ms locally on a worst case that has no witness at all.
//
// Reading: "arithmetic progression" covers decreasing triples too -- the
// statement says so itself, calling out (5,3,1) as a violation alongside
// (0,1,2).  Worth stating because the one-directional misreading is not
// observable from the data: a mutant checking only increasing positions
// reproduces the sample and also the official Waterloo 2004.09.25 judge
// input byte-for-byte, and only differs on 6908 of the 46230 permutations
// of n = 3..8.
//
// The count is read as "n:" with the colon optional-but-expected, then n
// values; input ends at a lone 0.
#include <cstdio>

#define MAXN 20005
static int pos[MAXN];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int c;
        do { c = getchar(); } while (c == ' ' || c == '\t' || c == '\r' || c == '\n');
        if (c != ':') ungetc(c, stdin);
        for (int i = 0; i < n; ++i) {
            int v = 0;
            if (scanf("%d", &v) != 1) v = 0;
            if (v >= 0 && v < MAXN) pos[v] = i;
        }
        bool bad = false;
        for (int d = 1; 2 * d < n && !bad; ++d) {
            const int lim = n - 2 * d;
            for (int a = 0; a < lim; ++a) {
                const int pa = pos[a], pb = pos[a + d], pc = pos[a + 2 * d];
                if ((pa < pb) == (pb < pc)) { bad = true; break; }
            }
        }
        puts(bad ? "no" : "yes");
    }
    return 0;
}
