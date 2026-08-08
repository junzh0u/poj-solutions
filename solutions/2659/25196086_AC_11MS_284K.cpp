// POJ 2659 - Bomb Game
// Model: claude-opus-5
//
// A bomb of odd diameter P at (R, S) covers the axis-aligned square of side P
// centred there, i.e. every field (r, c) with |r-R| <= P/2 and |c-S| <= P/2
// (Chebyshev ball of radius P/2). A field is a possible pillbox position iff
// its membership in each bomb's square equals that bomb's reported T. With
// A, B, K <= 100 that is at most 100*100*100 = 1e6 predicate evaluations, so
// enumerate every field and test all K bombs directly.
//
// Nothing is ever indexed by a bomb's coordinates, which is what the discuss
// board's Runtime Error reports were: a bomb centre plus/minus its radius runs
// off the board (P can be 99 with R = 1) and those solutions wrote there. Here
// the out-of-board part of a square simply never matches a field.
//
// The statement gives no dataset count, so input is read to EOF; the board says
// the data is a single case, and for a single case the loop runs exactly once.
#include <cstdio>
#include <cstdlib>

const int MAXK = 10005; /* stated K <= 100; sized well past it */
int R[MAXK], S[MAXK], P[MAXK], T[MAXK];

int main() {
    int A, B, K;
    while (scanf("%d %d %d", &A, &B, &K) == 3) {
        int i;
        if (K > MAXK) K = MAXK;
        for (i = 0; i < K; ++i) {
            if (scanf("%d %d %d %d", &R[i], &S[i], &P[i], &T[i]) != 4) return 0;
        }
        int count = 0;
        for (int r = 1; r <= A; ++r) {
            for (int c = 1; c <= B; ++c) {
                bool ok = true;
                for (i = 0; i < K && ok; ++i) {
                    int rad = P[i] / 2;
                    int inside = (abs(r - R[i]) <= rad && abs(c - S[i]) <= rad) ? 1 : 0;
                    if (inside != T[i]) ok = false;
                }
                if (ok) ++count;
            }
        }
        printf("%d\n", count);
    }
    return 0;
}
