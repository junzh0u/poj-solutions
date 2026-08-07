// POJ 1085 - Triangle War
// Model: claude-sonnet-5
//
// Board: 10 dots in a triangular arrangement (rows of 1,2,3,4 dots),
// 18 possible line segments, 9 small triangles (the outer big triangle
// does not count, per the discuss board). A move that completes a
// triangle scores it and grants the same player another turn.
//
// Key fact used: for a fixed set of filled edges X, both the set of
// completed triangles and (therefore) whose turn it is next are
// independent of the *order* the edges in X were filled, because a
// triangle becomes complete exactly when its last missing edge is
// added, regardless of when the other two arrived. So the game value
// of "current mover's future score minus opponent's future score from
// state X" is a pure function f(X) of the edge bitmask X alone, and it
// does not depend on which specific problem instance we are solving.
// We precompute f for all 2^18 masks once (topological DP over the
// DAG where edges are only ever added), then replay each game's given
// moves (which ARE order-dependent, since they are historical, not
// chosen by search) to get the actual current mask, mover, and scores
// already banked, and combine with f[mask] for the final verdict.
//
// f(X) recurrence: X = full board -> f = 0. Otherwise the mover picks
// an unfilled edge e; let t = number of triangles completed by adding
// e. If t>0 the same mover moves again at X|e, contributing
// t + f(X|e). If t==0 the turn passes, contributing -f(X|e) (value is
// from the new mover's perspective, so negate). f(X) = max over e.
//
// Total triangles = 9 (odd), so the difference A-B can never be 0:
// no tie is possible, matching the board's own observation.

#include <cstdio>
#include <cstring>

static const int EU[18] = {1,1,2,2,2,3,3,4,5,4,4,5,5,6,6,7,8,9};
static const int EV[18] = {2,3,3,4,5,5,6,5,6,7,8,8,9,9,10,8,9,10};
static const int TRI[9][3] = {
    {0,1,2},
    {3,4,7},
    {2,4,5},
    {5,6,8},
    {9,10,15},
    {7,10,11},
    {11,12,16},
    {8,12,13},
    {13,14,17}
};

static int eid[11][11];
static int numTri[18];
static int othA[18][2], othB[18][2];
static int fval[1 << 18];

int main() {
    memset(eid, -1, sizeof(eid));
    for (int e = 0; e < 18; e++) {
        eid[EU[e]][EV[e]] = e;
    }
    memset(numTri, 0, sizeof(numTri));
    for (int t = 0; t < 9; t++) {
        for (int k = 0; k < 3; k++) {
            int e = TRI[t][k];
            int o1 = TRI[t][(k + 1) % 3];
            int o2 = TRI[t][(k + 2) % 3];
            int idx = numTri[e]++;
            othA[e][idx] = o1;
            othB[e][idx] = o2;
        }
    }

    const int FULL = (1 << 18) - 1;
    for (int mask = FULL; mask >= 0; mask--) {
        if (mask == FULL) {
            fval[mask] = 0;
            continue;
        }
        int best = -100;
        for (int e = 0; e < 18; e++) {
            if (mask & (1 << e)) continue;
            int t = 0;
            for (int k = 0; k < numTri[e]; k++) {
                int o1 = othA[e][k], o2 = othB[e][k];
                if ((mask & (1 << o1)) && (mask & (1 << o2))) t++;
            }
            int newmask = mask | (1 << e);
            int val = (t > 0) ? (t + fval[newmask]) : (-fval[newmask]);
            if (val > best) best = val;
        }
        fval[mask] = best;
    }

    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int g = 1; g <= T; g++) {
        int m;
        scanf("%d", &m);
        int mask = 0;
        int mover = 0; // 0 = A, 1 = B
        int score[2] = {0, 0};
        for (int i = 0; i < m; i++) {
            int u, v;
            scanf("%d %d", &u, &v);
            int e = eid[u][v];
            int t = 0;
            for (int k = 0; k < numTri[e]; k++) {
                int o1 = othA[e][k], o2 = othB[e][k];
                if ((mask & (1 << o1)) && (mask & (1 << o2))) t++;
            }
            score[mover] += t;
            mask |= (1 << e);
            if (t == 0) mover = 1 - mover;
        }
        int diff = score[0] - score[1];
        diff += (mover == 0) ? fval[mask] : -fval[mask];
        printf("Game %d: %c wins.\n", g, diff > 0 ? 'A' : 'B');
    }
    return 0;
}
