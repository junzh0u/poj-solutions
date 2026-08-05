// POJ 2455 - Secret Milking Machine
// Model: gpt-5.6-terra
// Approach: Binary-search the sorted distinct trail lengths.  For a candidate
// limit, keep only trails no longer than it and compute the number of
// edge-disjoint paths from 1 to N with Dinic's algorithm.  An undirected trail
// is represented by one paired residual-edge pair, so its net use in either
// direction is at most one.  The predicate is monotone, and the first limit
// supporting T paths is the answer.  Parallel trails are retained separately.

#include <cstdio>
#include <cstring>
#include <algorithm>

const int MAXN = 205;
const int MAXP = 40005;
const int MAXE = MAXP * 2;
const int INF = 1000000000;

struct Trail {
    int u;
    int v;
    int length;
};

struct Edge {
    int to;
    int next;
    int cap;
};

int n, p, trips;
Trail trails[MAXP];
int head[MAXN], level[MAXN], work[MAXN], queue_[MAXN];
Edge edges[MAXE];
int edge_count;

void add_undirected_edge(int u, int v) {
    edges[edge_count].to = v;
    edges[edge_count].next = head[u];
    edges[edge_count].cap = 1;
    head[u] = edge_count++;
    edges[edge_count].to = u;
    edges[edge_count].next = head[v];
    edges[edge_count].cap = 1;
    head[v] = edge_count++;
}

bool build_level_graph() {
    int front = 0;
    int back = 0;
    int i;
    std::memset(level, -1, sizeof(level));
    level[1] = 0;
    queue_[back++] = 1;
    while (front < back) {
        int u = queue_[front++];
        for (i = head[u]; i != -1; i = edges[i].next) {
            int v = edges[i].to;
            if (edges[i].cap > 0 && level[v] == -1) {
                level[v] = level[u] + 1;
                if (v == n) return true;
                queue_[back++] = v;
            }
        }
    }
    return level[n] != -1;
}

int send_flow(int u, int amount) {
    int i;
    if (u == n) return amount;
    for (i = work[u]; i != -1; i = edges[i].next) {
        int v = edges[i].to;
        int pushed;
        work[u] = i;
        if (edges[i].cap == 0 || level[v] != level[u] + 1) continue;
        pushed = send_flow(v, std::min(amount, edges[i].cap));
        if (pushed > 0) {
            edges[i].cap -= pushed;
            edges[i ^ 1].cap += pushed;
            return pushed;
        }
    }
    level[u] = -1;
    return 0;
}

bool supports(int limit) {
    int i;
    int flow = 0;
    std::memset(head, -1, sizeof(head));
    edge_count = 0;
    for (i = 0; i < p; ++i) {
        if (trails[i].length <= limit) add_undirected_edge(trails[i].u, trails[i].v);
    }
    while (build_level_graph()) {
        int pushed;
        std::memcpy(work, head, sizeof(head));
        while ((pushed = send_flow(1, INF)) > 0) {
            flow += pushed;
            if (flow >= trips) return true;
        }
    }
    return false;
}

bool less_trail_length(const Trail &a, const Trail &b) {
    return a.length < b.length;
}

int main() {
    int low;
    int high;
    if (std::scanf("%d%d%d", &n, &p, &trips) != 3) return 0;
    for (int i = 0; i < p; ++i) {
        std::scanf("%d%d%d", &trails[i].u, &trails[i].v, &trails[i].length);
    }
    std::sort(trails, trails + p, less_trail_length);
    low = 0;
    high = p - 1;
    while (low < high) {
        int middle = low + (high - low) / 2;
        if (supports(trails[middle].length)) high = middle;
        else low = middle + 1;
    }
    std::printf("%d\n", trails[low].length);
    return 0;
}
