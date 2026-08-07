// POJ 3257 - Cow Roller Coaster
// Model: claude-opus-5
//
// Approach: DP over (position, cost spent).
//   dp[x][b] = maximum total fun of a chain of components that starts at 0,
//              ends exactly at x, and costs exactly b; -1 if no such chain.
//   dp[0][0] = 0.  Sort the components by starting position X, then relax
//   dp[X+W][b+C] = max(dp[X+W][b+C], dp[X][b] + F) for every b with dp[X][b] >= 0.
//   Because W >= 1, X+W > X, so once the components are processed in order of
//   non-decreasing X every dp[X][*] row is final before it is read.
//   Answer: max over b in [0,B] of dp[L][b], or -1 if the whole row is -1.
//
// The -1 sentinel matters: position x may be unreachable even though 0 is a
// legal fun total, and treating an unreachable state as fun 0 (a bug visible in
// a wrong solution posted on the discuss board) invents chains that do not exist.
//
// Bounds: a chain costs at least 1 per component and at most B <= 1000, so it
// has at most 1000 components of fun <= 1,000,000 each -> total fun <= 1e9,
// which fits in a 32-bit int.  Work is O(N*B) = 1e7.
//
// No statement ambiguity: exactly one data set, the chain must tile [0,L]
// end-to-end with no gaps or overlaps, and -1 is printed when none fits B.

#include <cstdio>
#include <cstdlib>
#include <algorithm>

struct Comp {
    int x, w, f, c;
};

static bool byX(const Comp &a, const Comp &b) {
    return a.x < b.x;
}

static const int MAXL = 1001;
static const int MAXB = 1001;

static int dp[MAXL][MAXB];
static Comp comp[10001];

int main() {
    int L, N, B;
    if (scanf("%d %d %d", &L, &N, &B) != 3) return 0;
    for (int i = 0; i < N; ++i) {
        scanf("%d %d %d %d", &comp[i].x, &comp[i].w, &comp[i].f, &comp[i].c);
    }
    std::sort(comp, comp + N, byX);

    for (int x = 0; x <= L; ++x)
        for (int b = 0; b <= B; ++b)
            dp[x][b] = -1;
    dp[0][0] = 0;

    for (int i = 0; i < N; ++i) {
        const int x = comp[i].x, w = comp[i].w, f = comp[i].f, c = comp[i].c;
        if (x > L || x + w > L) continue;   /* defensive: statement forbids it */
        int *from = dp[x];
        int *to = dp[x + w];
        for (int b = 0; b + c <= B; ++b) {
            if (from[b] < 0) continue;
            const int v = from[b] + f;
            if (v > to[b + c]) to[b + c] = v;
        }
    }

    int best = -1;
    for (int b = 0; b <= B; ++b)
        if (dp[L][b] > best) best = dp[L][b];

    printf("%d\n", best);
    return 0;
}
