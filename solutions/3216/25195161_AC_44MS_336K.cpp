// POJ 3216 - Repairing Company
// Model: claude-opus-5
//
// Each task i must be *started* exactly at t_i in block p_i and occupies one
// repairman for d_i time, so a repairman can chain i -> j iff
//     t_i + d_i + sp(p_i, p_j) <= t_j
// where sp is the all-pairs shortest path over the given matrix (delta_ij = -1
// means "no direct road", NOT "unreachable" -- it must become +inf before
// Floyd-Warshall, which is the pitfall the discuss board keeps reporting).
//
// That relation is transitive (d, sp >= 0), so the DAG it induces is already
// transitively closed and the answer is its minimum path cover:
//     answer = M - (maximum bipartite matching of i -> j)
// solved with Kuhn's augmenting-path algorithm.
//
// Degenerate case worth guarding: with d_i = 0 and sp = 0 two tasks sharing the
// same t admit edges in *both* directions, which is a 2-cycle and breaks the
// path-cover formula (the board's "1 3 / 0 / 1 2 0 / 1 2 0 / 1 2 0" case would
// print 0 instead of 1). Sorting the tasks by (t, d) with a stable sort and
// only emitting edges forward in that order restores a DAG without losing any
// chain: an edge with t_i == t_j forces d_i == 0 <= d_j, and tasks tied on both
// keys are mutually interchangeable.
//
// Ambiguity: the statement's "deadline t_i ... which is also its starting time"
// means an early repairman waits; the task never starts before t_i. The discuss
// board settles this in the author's own words ("deadlines are starting times").

#include <cstdio>
#include <iostream>
#include <algorithm>

using namespace std;

const long long INF = 1LL << 60;

struct Task {
    long long p, t, d;
};

static bool cmpTask(const Task &a, const Task &b) {
    if (a.t != b.t) return a.t < b.t;
    return a.d < b.d;
}

static int Q, M;
static long long sp[25][25];
static Task task[205];
static bool adj[205][205];
static int matchR[205];
static bool used[205];

static bool augment(int u) {
    for (int v = 0; v < M; ++v) {
        if (!adj[u][v] || used[v]) continue;
        used[v] = true;
        if (matchR[v] == -1 || augment(matchR[v])) {
            matchR[v] = u;
            return true;
        }
    }
    return false;
}

int main() {
    while ((cin >> Q >> M) && (Q != 0 || M != 0)) {
        for (int i = 0; i < Q; ++i) {
            for (int j = 0; j < Q; ++j) {
                long long w;
                cin >> w;
                sp[i][j] = (w < 0) ? INF : w;
            }
        }
        for (int i = 0; i < Q; ++i) sp[i][i] = 0;
        for (int k = 0; k < Q; ++k)
            for (int i = 0; i < Q; ++i) {
                if (sp[i][k] >= INF) continue;
                for (int j = 0; j < Q; ++j) {
                    if (sp[k][j] >= INF) continue;
                    if (sp[i][k] + sp[k][j] < sp[i][j]) sp[i][j] = sp[i][k] + sp[k][j];
                }
            }

        for (int i = 0; i < M; ++i) {
            cin >> task[i].p >> task[i].t >> task[i].d;
            --task[i].p;
        }
        stable_sort(task, task + M, cmpTask);

        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j) adj[i][j] = false;
        for (int i = 0; i < M; ++i) {
            for (int j = i + 1; j < M; ++j) {
                long long w = sp[task[i].p][task[j].p];
                if (w >= INF) continue;
                if (task[i].t + task[i].d + w <= task[j].t) adj[i][j] = true;
            }
        }

        for (int i = 0; i < M; ++i) matchR[i] = -1;
        int matching = 0;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) used[j] = false;
            if (augment(i)) ++matching;
        }
        printf("%d\n", M - matching);
    }
    return 0;
}
