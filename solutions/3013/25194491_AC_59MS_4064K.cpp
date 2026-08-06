// POJ 3013 - Big Christmas Tree
//
// Given a graph on v nodes (root is node 1) with node weights and per-edge unit
// prices, choose a spanning tree minimising the sum over edges of
// (unit price) * (total weight of the subtree hanging below that edge).
//
// Key identity: an edge's contribution counts every descendant leaf's weight
// once, so summing over edges is the same as summing over nodes:
//   cost(tree) = sum over nodes i of w[i] * (root-to-i distance in the tree).
// Each node's term is minimised independently by its shortest path from the
// root, and the union of all shortest paths is itself a spanning tree, so the
// minimum is simply one Dijkstra from node 1 followed by sum w[i]*dist[i].
// No tree is ever built.
//
// Notes on the statement:
//   - The input is an arbitrary graph, not a tree; edges may be parallel or
//     self-loops, and the graph need not be connected. Any node unreachable
//     from the root makes the tree impossible -> "No Answer".
//   - "All numbers in input are less than 216" means 2^16 = 65536 (the page
//     loses the superscript). The answer therefore reaches ~5.4e18 on a
//     50000-node path with maximal weights and prices: accumulate in long long
//     and print the digits by hand rather than relying on a format specifier.
//   - v may be 0 (and v == 1 needs no edges); both answer 0.
//
// Dijkstra with a binary heap over a CSR adjacency list, plus a getchar-style
// reader -- input runs to tens of MB at the stated maxima.
#include <cstdio>
#include <vector>
#include <queue>
#include <utility>

static const int BUFSZ = 1 << 16;
static char ibuf[BUFSZ];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, BUFSZ, stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos++];
}

static bool readInt(long& out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    long x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sign;
    return true;
}

static void writeLL(long long x) {
    char tmp[24];
    int n = 0;
    if (x == 0) { putchar('0'); return; }
    if (x < 0) { putchar('-'); x = -x; }
    while (x > 0) { tmp[n++] = (char)('0' + (int)(x % 10)); x /= 10; }
    while (n > 0) putchar(tmp[--n]);
}

static const long long INF = 0x3f3f3f3f3f3f3f3fLL;

int main() {
    long T;
    if (!readInt(T)) return 0;
    for (long tc = 0; tc < T; tc++) {
        long v, e;
        readInt(v);
        readInt(e);

        std::vector<long> w((size_t)(v + 1), 0);
        for (long i = 1; i <= v; i++) readInt(w[(size_t)i]);

        // CSR adjacency
        std::vector<long> ea((size_t)e), eb((size_t)e), ec((size_t)e);
        std::vector<int> deg((size_t)(v + 2), 0);
        long m = 0;
        for (long i = 0; i < e; i++) {
            long a, b, c;
            readInt(a); readInt(b); readInt(c);
            if (a < 1 || a > v || b < 1 || b > v) continue;
            ea[(size_t)m] = a; eb[(size_t)m] = b; ec[(size_t)m] = c;
            deg[(size_t)a]++; deg[(size_t)b]++;
            m++;
        }
        std::vector<int> start((size_t)(v + 2), 0);
        for (long i = 1; i <= v; i++) start[(size_t)(i + 1)] = start[(size_t)i] + deg[(size_t)i];
        std::vector<int> fill(start.begin(), start.end());
        std::vector<int> to((size_t)(2 * m));
        std::vector<long> cost((size_t)(2 * m));
        for (long i = 0; i < m; i++) {
            long a = ea[(size_t)i], b = eb[(size_t)i], c = ec[(size_t)i];
            to[(size_t)fill[(size_t)a]] = (int)b; cost[(size_t)fill[(size_t)a]] = c; fill[(size_t)a]++;
            to[(size_t)fill[(size_t)b]] = (int)a; cost[(size_t)fill[(size_t)b]] = c; fill[(size_t)b]++;
        }

        if (v <= 1) {
            // no edges needed; cost is 0 (root alone, or empty tree)
            putchar('0');
            putchar('\n');
            continue;
        }

        std::vector<long long> dist((size_t)(v + 1), INF);
        dist[1] = 0;
        std::priority_queue<std::pair<long long, int>,
                            std::vector<std::pair<long long, int> >,
                            std::greater<std::pair<long long, int> > > pq;
        pq.push(std::make_pair(0LL, 1));
        while (!pq.empty()) {
            std::pair<long long, int> top = pq.top();
            pq.pop();
            long long d = top.first;
            int u = top.second;
            if (d != dist[(size_t)u]) continue;
            for (int j = start[(size_t)u]; j < start[(size_t)(u + 1)]; j++) {
                int y = to[(size_t)j];
                long long nd = d + (long long)cost[(size_t)j];
                if (nd < dist[(size_t)y]) {
                    dist[(size_t)y] = nd;
                    pq.push(std::make_pair(nd, y));
                }
            }
        }

        long long total = 0;
        bool ok = true;
        for (long i = 1; i <= v; i++) {
            if (dist[(size_t)i] == INF) { ok = false; break; }
            total += dist[(size_t)i] * (long long)w[(size_t)i];
        }
        if (!ok) {
            fputs("No Answer\n", stdout);
        } else {
            writeLL(total);
            putchar('\n');
        }
    }
    return 0;
}
