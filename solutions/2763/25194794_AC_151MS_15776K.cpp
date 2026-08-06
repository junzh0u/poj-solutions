// POJ 2763 - Housewife Wind
// Model: gpt-5.6-terra
// Root the fixed tree at hut 1.  A change to a parent-child edge shifts the
// root distance of exactly that child's Euler-tour subtree.  A Fenwick tree
// stores those range-add shifts, and binary-lifting LCA turns each move into
// dist(current) + dist(target) - 2 * dist(lca).  In particular, equal huts
// have an empty path and correctly print zero.
#include <cstdio>

const int MAXN = 100005;
const int MAXE = 200005;
const int LOG = 18;

struct Edge {
    int to;
    int next;
    int id;
    int weight;
};

int head[MAXN], edgeCount;
Edge edges[MAXE];
int parentNode[MAXN], depthNode[MAXN], up[LOG][MAXN];
int tin[MAXN], tout[MAXN], iteratorEdge[MAXN], edgeChild[MAXN];
int edgeWeight[MAXN];
long long initialDistance[MAXN], bit[MAXN];

int readInt() {
    int c = std::getchar();
    while (c <= ' ' && c != EOF) c = std::getchar();
    int sign = 1;
    if (c == '-') {
        sign = -1;
        c = std::getchar();
    }
    int value = 0;
    while (c > ' ') {
        value = value * 10 + c - '0';
        c = std::getchar();
    }
    return value * sign;
}

void addEdge(int a, int b, int w, int id) {
    edges[edgeCount].to = b;
    edges[edgeCount].next = head[a];
    edges[edgeCount].id = id;
    edges[edgeCount].weight = w;
    head[a] = edgeCount++;
}

void bitAdd(int index, long long value, int n) {
    while (index <= n) {
        bit[index] += value;
        index += index & -index;
    }
}

long long bitSum(int index) {
    long long result = 0;
    while (index > 0) {
        result += bit[index];
        index -= index & -index;
    }
    return result;
}

int lca(int a, int b) {
    int k;
    if (depthNode[a] < depthNode[b]) {
        int temp = a;
        a = b;
        b = temp;
    }
    int difference = depthNode[a] - depthNode[b];
    for (k = 0; k < LOG; ++k) {
        if (difference & (1 << k)) a = up[k][a];
    }
    if (a == b) return a;
    for (k = LOG - 1; k >= 0; --k) {
        if (up[k][a] != up[k][b]) {
            a = up[k][a];
            b = up[k][b];
        }
    }
    return parentNode[a];
}

int main() {
    int n = readInt();
    int q = readInt();
    int current = readInt();
    int i, k;
    for (i = 1; i <= n; ++i) head[i] = -1;
    for (i = 1; i < n; ++i) {
        int a = readInt();
        int b = readInt();
        int w = readInt();
        edgeWeight[i] = w;
        addEdge(a, b, w, i);
        addEdge(b, a, w, i);
    }

    int stack[MAXN], top = 0, timer = 0;
    stack[0] = 1;
    parentNode[1] = 0;
    depthNode[1] = 0;
    initialDistance[1] = 0;
    tin[1] = ++timer;
    iteratorEdge[1] = head[1];
    while (top >= 0) {
        int u = stack[top];
        int e = iteratorEdge[u];
        while (e != -1 && edges[e].to == parentNode[u]) e = edges[e].next;
        iteratorEdge[u] = e;
        if (e == -1) {
            tout[u] = timer;
            --top;
        } else {
            int v = edges[e].to;
            iteratorEdge[u] = edges[e].next;
            parentNode[v] = u;
            depthNode[v] = depthNode[u] + 1;
            initialDistance[v] = initialDistance[u] + edges[e].weight;
            edgeChild[edges[e].id] = v;
            tin[v] = ++timer;
            iteratorEdge[v] = head[v];
            stack[++top] = v;
        }
    }
    for (i = 1; i <= n; ++i) up[0][i] = parentNode[i];
    for (k = 1; k < LOG; ++k) {
        for (i = 1; i <= n; ++i) up[k][i] = up[k - 1][up[k - 1][i]];
    }

    while (q--) {
        int type = readInt();
        if (type == 0) {
            int target = readInt();
            int common = lca(current, target);
            long long distanceCurrent = initialDistance[current] + bitSum(tin[current]);
            long long distanceTarget = initialDistance[target] + bitSum(tin[target]);
            long long distanceCommon = initialDistance[common] + bitSum(tin[common]);
            std::printf("%lld\n", distanceCurrent + distanceTarget - 2 * distanceCommon);
            current = target;
        } else {
            int id = readInt();
            int weight = readInt();
            long long delta = (long long)weight - edgeWeight[id];
            int child = edgeChild[id];
            bitAdd(tin[child], delta, n);
            bitAdd(tout[child] + 1, -delta, n);
            edgeWeight[id] = weight;
        }
    }
    return 0;
}
