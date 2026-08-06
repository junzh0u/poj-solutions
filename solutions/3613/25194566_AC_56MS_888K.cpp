// POJ 3613 - Cow Relays
//
// Shortest walk that uses exactly N trails from S to E, with N up to
// 1,000,000 and only T <= 100 trails (so at most 200 distinct intersections
// actually appear, once coordinate-compressed from the 1..1000 range).  With
// N this large a per-edge DP over N steps is out of the question, but exactly-
// k-edges shortest paths compose associatively under the (min, +) semiring:
// (A (min,+) B)[i][j] = min_k A[i][k] + B[k][j] is the shortest walk i->j that
// splits into an A-part and a B-part at some intermediate vertex k, and
// associativity of that composition follows from min/+ associativity and
// distributivity, exactly as ordinary matrix multiplication follows from
// (+, *).  That makes W^N under this semiring the exactly-N-edges shortest-
// path matrix, computable by binary exponentiation in O(V^3 log N).  The
// identity element for the exponentiation is not the all-zero matrix (0 is
// not the semiring's additive identity here) but the matrix with 0 on the
// diagonal and infinity elsewhere, since composing with it must leave any
// matrix unchanged under (min, +).
//
// The graph is undirected and a walk may reuse a trail (the statement asks
// for a walk of exactly N trail-traversals, not a simple path), which is
// exactly what repeated (min,+) squaring computes.

#include <cstdio>
#include <cstring>

const int INF = 0x3f3f3f3f;
const int MAXV = 205;

int n; // number of distinct compressed vertices

void mul(int c[MAXV][MAXV], int a[MAXV][MAXV], int b[MAXV][MAXV]) {
    static int tmp[MAXV][MAXV];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            tmp[i][j] = INF;
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            if (a[i][k] >= INF) continue;
            int aik = a[i][k];
            for (int j = 0; j < n; ++j) {
                if (b[k][j] >= INF) continue;
                int v = aik + b[k][j];
                if (v < tmp[i][j]) tmp[i][j] = v;
            }
        }
    }
    memcpy(c, tmp, sizeof(int) * MAXV * MAXV);
}

int W[MAXV][MAXV];
int res[MAXV][MAXV];
int base_[MAXV][MAXV];

int main() {
    int N, T, S, E;
    if (scanf("%d %d %d %d", &N, &T, &S, &E) != 4) return 0;

    int idOf[1001];
    memset(idOf, -1, sizeof(idOf));
    n = 0;

    for (int i = 0; i < MAXV; ++i)
        for (int j = 0; j < MAXV; ++j)
            W[i][j] = INF;

    for (int t = 0; t < T; ++t) {
        int len, a, b;
        scanf("%d %d %d", &len, &a, &b);
        if (idOf[a] == -1) idOf[a] = n++;
        if (idOf[b] == -1) idOf[b] = n++;
        int ia = idOf[a], ib = idOf[b];
        if (len < W[ia][ib]) {
            W[ia][ib] = len;
            W[ib][ia] = len;
        }
    }

    // Defensive: the statement guarantees every intersection is the
    // termination of at least two trails, so S and E should already be
    // mapped; but assign them a fresh (isolated) id if not, rather than
    // indexing with -1.
    if (idOf[S] == -1) idOf[S] = n++;
    if (idOf[E] == -1) idOf[E] = n++;
    int idxS = idOf[S];
    int idxE = idOf[E];

    // identity matrix for (min,+) semiring: 0 on diagonal, INF elsewhere
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            res[i][j] = (i == j) ? 0 : INF;

    memcpy(base_, W, sizeof(W));

    int e = N;
    while (e > 0) {
        if (e & 1) mul(res, res, base_);
        mul(base_, base_, base_);
        e >>= 1;
    }

    printf("%d\n", res[idxS][idxE]);
    return 0;
}
