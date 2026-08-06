// POJ 1949 - Chores
//
// N chores, each with a duration and a list of prerequisite chores that must
// finish before it may start. Any number of chores may run at the same time
// (the sample starts chores 2 and 4 together at t=5), so with unlimited
// workers the makespan is exactly the longest weighted path through the
// prerequisite DAG — no scheduling decision is ever needed, every chore just
// starts the instant its last prerequisite finishes.
//
// finish[k] = length[k] + max(finish[p] for p in prereq[k]), 0 if no prereq,
// and the answer is max over all k of finish[k] — not finish[N], since the
// last chore in the numbering need not be the last to complete.
//
// The statement guarantees chore K may only depend on 1..K-1, so the input
// order is already a topological order and a single forward pass suffices;
// nothing has to be stored but the finish times. O(sum of prerequisite counts)
// which is at most 10,000 * 100 = 1e6 numbers, so input parsing dominates and
// is done with a getchar-based reader.
//
// Answer fits in int: 10,000 chores * 100 time units = 1,000,000 at most.

#include <cstdio>

static const int MAXN = 10005;
static int finishTime[MAXN];

static int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return -1;
        c = getchar();
    }
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = getchar(); }
    return v * sign;
}

int main() {
    int n = readInt();
    if (n <= 0) { printf("0\n"); return 0; }
    int best = 0;
    for (int i = 1; i <= n; ++i) {
        int len = readInt();
        int p = readInt();
        int start = 0;
        for (int j = 0; j < p; ++j) {
            int q = readInt();
            if (q >= 1 && q <= n && finishTime[q] > start) start = finishTime[q];
        }
        finishTime[i] = start + len;
        if (finishTime[i] > best) best = finishTime[i];
    }
    printf("%d\n", best);
    return 0;
}
