// POJ 2912 - Rochambeau
// Model: claude-opus-5
//
// N children, one of them the judge; the rest are split into three gesture
// groups.  A round "a=b" / "a>b" / "a<b" between two non-judges constrains their
// groups modulo 3: with rock=0, paper=1, scissors=2 and the usual cycle,
// a beats b exactly when (g[a]-g[b]) mod 3 == 1.  Rounds involving the judge
// carry no information at all, since the judge plays randomly.
//
// So: child j can be the judge iff the rounds *not* involving j are mutually
// consistent, which a weighted (mod 3) union-find decides in one pass.  Enumerate
// every j, recording for each rejected j the 1-based index of the round at which
// its scan first contradicts itself.
//   * no consistent j            -> the data itself is impossible
//   * two or more consistent j   -> the judge cannot be pinned down
//   * exactly one consistent j   -> j is the judge, and the earliest round after
//     which he is uniquely determined is the LARGEST of the other children's
//     first-contradiction indices: only once every rival has contradicted itself
//     is one candidate left.  (N = 1 leaves an empty maximum, hence 0 lines.)
// Total work is O(N*M*alpha) per test case, 500*2000 = 1e6 union-find steps.
//
// Fast path: if the whole round list is consistent with nobody removed, then
// every child is a possible judge, so the answer is immediate.
//
// Input notes (the statement is thin here, the discuss board fills it in):
//   * multiple test cases, read to EOF;
//   * '=' really occurs in the data even though the sample never shows it;
//   * the board disputes whether spaces surround the operator, so the scanf
//     format tolerates them either way.
#include <cstdio>

const int MAXN = 505;
const int MAXM = 2005;

int par[MAXN], rel_[MAXN];
int A[MAXM], B[MAXM], W[MAXM];

// rel_[x] = (group(x) - group(par[x])) mod 3
static int find(int x) {
    if (par[x] == x) return x;
    int p = par[x];
    int r = find(p);
    rel_[x] = (rel_[x] + rel_[p]) % 3;
    par[x] = r;
    return r;
}

// Scan every round except those involving `skip`; return the 1-based index of
// the first contradiction, or 0 if the rounds are consistent.
static int scan(int n, int m, int skip) {
    for (int i = 0; i < n; ++i) { par[i] = i; rel_[i] = 0; }
    for (int i = 0; i < m; ++i) {
        if (A[i] == skip || B[i] == skip) continue;
        int ra = find(A[i]), rb = find(B[i]);
        if (ra == rb) {
            if ((rel_[A[i]] - rel_[B[i]] + 3) % 3 != W[i]) return i + 1;
        } else {
            par[ra] = rb;
            rel_[ra] = ((W[i] - rel_[A[i]] + rel_[B[i]]) % 3 + 3) % 3;
        }
    }
    return 0;
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        int i, j;
        for (i = 0; i < m; ++i) {
            char c;
            scanf(" %d %c %d", &A[i], &c, &B[i]);
            if (c == '=') W[i] = 0;
            else if (c == '>') W[i] = 1;
            else W[i] = 2;
        }
        if (scan(n, m, -1) == 0) {
            // Nobody has to be removed to make the rounds consistent, so every
            // child is a possible judge.
            if (n == 1) printf("Player 0 can be determined to be the judge after 0 lines\n");
            else printf("Can not determine\n");
            continue;
        }
        int cand = -1, ncand = 0, worst = 0;
        for (j = 0; j < n; ++j) {
            int conflict = scan(n, m, j);
            if (conflict == 0) { cand = j; ++ncand; }
            else if (conflict > worst) worst = conflict;
        }
        if (ncand == 0) printf("Impossible\n");
        else if (ncand > 1) printf("Can not determine\n");
        else printf("Player %d can be determined to be the judge after %d lines\n", cand, worst);
    }
    return 0;
}
