// POJ 3028 - Shoot-out
// Model: claude-sonnet-5
//
// n cowboys (n<=13) shoot in a fixed cyclic order; each shooter always
// aims at the opponent that maximizes his own eventual survival
// probability (ties broken by uniform random choice among tied targets),
// then hits with his own fixed probability. We compute, for every subset
// S of survivors and every possible "whose turn" index i in S, the vector
// f[S][i][k] = probability that cowboy k is the ultimate survivor.
//
// Key structural fact: within a fixed subset S, one full round (going
// once through every alive shooter in the fixed cyclic order starting at
// i) either ends with some hit (transitioning to a strictly smaller
// subset, whose f-vector is already known) or ends with everybody
// missing, which returns exactly to state (S,i) again. That self-loop
// resolves in closed form: f(S,i) = raw(S,i) / (1 - q(S)), where q(S) is
// the probability the whole subset S misses in one round (independent of
// which i starts the round, since it's a product over all members of S)
// and raw(S,i) sums each round-step's "reached this shooter times he
// hits times the resulting vector". No iterative/relaxation solving is
// needed, and there is no cross-dependency between different subsets of
// the same size or between full rounds started at different i (each i's
// equation is self-contained) -- only strictly smaller subsets feed in,
// so processing subsets in increasing numeric order (S\{t} < S always)
// gives a valid computation order.
//
// Target selection for shooter j in state S only ever looks at f[S\{t}]
// for t != j, i.e. strictly smaller subsets, so it never depends on the
// current subset's own (not yet known) values -- no circularity there
// either. Ties in the argmax are averaged uniformly over the tied
// targets (this changes who *else* survives, even though it can't change
// the shooter's own value, since it's an average of equal values there).
//
// Ambiguity check: sample case "3 100 99 98" gives "2.00 0.00 98.00".
// Manually verified: cowboy1 (100%) must pick the bigger threat (99%
// cowboy2) over the 98% cowboy3, leaving a cowboy1/cowboy3 duel that
// cowboy1 wins iff cowboy3's first (98%) shot misses -> P(cowboy1)=0.02,
// matching the sample and confirming the target-selection rule.
#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 13;
const double EPS = 1e-9;

double f[1 << MAXN][MAXN][MAXN];
double p[MAXN];
int n;
unsigned char popcnt[1 << MAXN];
signed char nextAlive[1 << MAXN][MAXN]; // valid only for i actually set in S

void build_next_alive(int FULL) {
    static int bits[MAXN];
    for (int S = 1; S < FULL; ++S) {
        int len = 0;
        for (int j = 0; j < n; ++j) if (S & (1 << j)) bits[len++] = j;
        for (int k = 0; k < len; ++k) nextAlive[S][bits[k]] = (signed char)bits[(k + 1) % len];
    }
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            int h;
            scanf("%d", &h);
            p[i] = h / 100.0;
        }
        int FULL = 1 << n;
        for (int S = 1; S < FULL; ++S) {
            int c = 0, x = S;
            while (x) { x &= x - 1; ++c; }
            popcnt[S] = (unsigned char)c;
        }
        build_next_alive(FULL);
        // base cases: singleton subsets
        for (int i = 0; i < n; ++i) {
            memset(f[1 << i][i], 0, sizeof(double) * n);
            f[1 << i][i][i] = 1.0;
        }
        static double hitVec[MAXN][MAXN];
        static int ties[MAXN];
        for (int S = 1; S < FULL; ++S) {
            int m = popcnt[S];
            if (m < 2) continue;
            // Step 1: for each shooter j in S, compute averaged "if he
            // hits" outcome vector using optimal (possibly tied) target.
            for (int j = 0; j < n; ++j) {
                if (!(S & (1 << j))) continue;
                double best = -1.0;
                int nt = 0;
                for (int tt = 0; tt < n; ++tt) {
                    if (tt == j || !(S & (1 << tt))) continue;
                    int S2 = S ^ (1 << tt);
                    int nxt = nextAlive[S2][j];
                    double val = f[S2][nxt][j];
                    if (val > best + EPS) {
                        best = val;
                        nt = 0;
                        ties[nt++] = tt;
                    } else if (val > best - EPS) {
                        if (val > best) best = val;
                        ties[nt++] = tt;
                    }
                }
                memset(hitVec[j], 0, sizeof(double) * n);
                for (int idx = 0; idx < nt; ++idx) {
                    int tt = ties[idx];
                    int S2 = S ^ (1 << tt);
                    int nxt = nextAlive[S2][j];
                    for (int k = 0; k < n; ++k) {
                        if (S2 & (1 << k)) hitVec[j][k] += f[S2][nxt][k] / nt;
                    }
                }
            }
            double q = 1.0;
            for (int j = 0; j < n; ++j) if (S & (1 << j)) q *= (1.0 - p[j]);
            double denom = 1.0 - q;
            // Step 2: for each possible starting shooter i, roll one
            // round in cyclic order and close the self-loop.
            for (int i = 0; i < n; ++i) {
                if (!(S & (1 << i))) continue;
                static int order[MAXN];
                order[0] = i;
                for (int idx = 1; idx < m; ++idx) order[idx] = nextAlive[S][order[idx - 1]];
                double raw[MAXN];
                memset(raw, 0, sizeof(double) * n);
                double Pk = 1.0;
                for (int idx = 0; idx < m; ++idx) {
                    int oj = order[idx];
                    double hit = p[oj];
                    for (int k = 0; k < n; ++k) {
                        if (S & (1 << k)) raw[k] += Pk * hit * hitVec[oj][k];
                    }
                    Pk *= (1.0 - hit);
                }
                for (int k = 0; k < n; ++k) {
                    if (S & (1 << k)) f[S][i][k] = raw[k] / denom;
                }
            }
        }
        for (int k = 0; k < n; ++k) {
            double v = f[FULL - 1][0][k] * 100.0;
            if (v < 0) v = 0;
            if (v > 100) v = 100;
            printf("%.2f%c", v, k + 1 < n ? ' ' : '\n');
        }
    }
    return 0;
}
