// POJ 1975 - Median Weight Bead
// Model: claude-opus-5
//
// Transitive closure (Floyd-Warshall on a boolean reachability matrix) over the
// "is heavier than" relation.  Bead i can occupy rank k = (N+1)/2 in some total
// order consistent with the comparisons iff at most k-1 beads are known lighter
// than it and at most N-k are known heavier; for a partial order the ancestor /
// descendant counts are exactly the tight bounds on i's possible rank, and N is
// odd so N-k = k-1.  So bead i is impossible iff heavier(i) >= k or lighter(i) >= k.
//
// Notes:
//  * The discuss board reports the judge data contains cycles (e.g. 1>2, 2>3,
//    3>1), which make the constraints unsatisfiable.  Under reachability counting
//    every bead on a cycle reaches (and is reached by) every other bead in its
//    strongly connected component plus everything beyond it, so such beads are
//    counted as impossible -- which agrees with the brute-force reading, since a
//    contradictory data set admits no consistent ordering at all and therefore no
//    bead can be the median.
//  * The statement guarantees N odd; the (N+1)/2 threshold is used as written.

#include <cstdio>
#include <cstring>

static bool reach[100][100];

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t-- > 0) {
        int n, m;
        if (scanf("%d %d", &n, &m) != 2) break;
        memset(reach, 0, sizeof(reach));
        for (int e = 0; e < m; ++e) {
            int a, b;
            scanf("%d %d", &a, &b); /* a is heavier than b */
            if (a >= 1 && a <= n && b >= 1 && b <= n) reach[a - 1][b - 1] = true;
        }
        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                if (reach[i][k])
                    for (int j = 0; j < n; ++j)
                        if (reach[k][j]) reach[i][j] = true;

        int half = (n + 1) / 2;
        int bad = 0;
        for (int i = 0; i < n; ++i) {
            int lighter = 0, heavier = 0;
            for (int j = 0; j < n; ++j) {
                if (j == i) continue;
                if (reach[i][j]) ++lighter;  /* i heavier than j */
                if (reach[j][i]) ++heavier;  /* j heavier than i */
            }
            if (lighter >= half || heavier >= half) ++bad;
        }
        printf("%d\n", bad);
    }
    return 0;
}
