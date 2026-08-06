// POJ 1985 - Cow Marathon
//
// The farms form a tree (input format borrowed verbatim from "Navigation
// Nightmare": N farms, M roads, each road "F1 F2 L D" with a compass letter D).
// The answer is the tree's diameter - the greatest weighted distance between
// any two nodes - found by two traversals: from an arbitrary node s find the
// farthest node a, then from a find the farthest node b; d(a,b) is the
// diameter.
//
// Why that works: let (x,y) be a diameter pair and let m be the vertex where
// the path s->a meets the path x->y.  Since a is farthest from s,
// d(m,a) >= d(m,x) and d(m,a) >= d(m,y), so
// d(a,y) = d(a,m) + d(m,y) >= d(x,m) + d(m,y) = d(x,y).  Hence a is itself an
// endpoint of some diameter, and the second traversal from a finds its partner.
// This needs non-negative edge weights, which road lengths are.
//
// Notes on the statement / input:
//  * The direction letter carries no information for a distance question, but
//    it must still be consumed by the reader.
//  * The statement never restates the bounds - they come from "Navigation
//    Nightmare": N, M <= 40000, lengths <= 1000.  So the tree can be a 40000-
//    node chain and the traversal must be an explicit BFS queue, never
//    recursion, which would blow the stack.
//  * The diameter fits in a 32-bit int: 40000 edges * length 1000 = 4*10^7.
//  * Defensively the graph is treated as a forest.  Components are labelled in
//    one O(n+m) sweep first, and the diameter pass then touches each component
//    exactly twice; rescanning all n nodes per component would be O(n^2) on an
//    input of isolated farms.  A visit token (stamp_) avoids clearing the
//    visited array between traversals for the same reason.
//  * The outer loop also accepts several test cases concatenated in one file.
#include <cstdio>

static const int MAXN = 40005;
static const int MAXM = 40005;

static int head[MAXN], nxt[2 * MAXM], to[2 * MAXM], wt[2 * MAXM];
static int ecnt;

static int dist_[MAXN];
static int queue_[MAXN];
static int stamp_[MAXN];
static int token;

static char buf[1 << 16];
static int bl, bp;

static int gc() {
    if (bp == bl) {
        bl = (int)fread(buf, 1, sizeof(buf), stdin);
        bp = 0;
        if (bl <= 0) return -1;
    }
    return buf[bp++];
}

static int readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return 0;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sgn;
    return 1;
}

static int readLetter() {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    return c;
}

static void addEdge(int a, int b, int w) {
    to[ecnt] = b; wt[ecnt] = w; nxt[ecnt] = head[a]; head[a] = ecnt++;
}

/* Iterative BFS from src over its component.  Returns the farthest node and
   writes its distance to *best; the visited set is left in stamp_ == token. */
static int farthest(int src, int *best) {
    int qh = 0, qt = 0;
    ++token;
    stamp_[src] = token; dist_[src] = 0; queue_[qt++] = src;
    int bnode = src, bdist = 0;
    while (qh < qt) {
        int u = queue_[qh++];
        if (dist_[u] > bdist) { bdist = dist_[u]; bnode = u; }
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            if (stamp_[v] != token) {
                stamp_[v] = token;
                dist_[v] = dist_[u] + wt[e];
                queue_[qt++] = v;
            }
        }
    }
    *best = bdist;
    return bnode;
}

static int comp_[MAXN];

int main() {
    int n, m;
    while (readInt(n)) {
        if (!readInt(m)) break;
        ecnt = 0;
        token = 0;
        int i;
        for (i = 1; i <= n; ++i) { head[i] = -1; stamp_[i] = 0; comp_[i] = 0; }
        for (i = 0; i < m; ++i) {
            int a, b, w;
            readInt(a); readInt(b); readInt(w);
            readLetter();
            addEdge(a, b, w);
            addEdge(b, a, w);
        }

        /* Label components once (O(n + m)) so the diameter pass below visits
           each component exactly twice rather than rescanning all n nodes. */
        int ncomp = 0;
        for (i = 1; i <= n; ++i) {
            if (comp_[i]) continue;
            ++ncomp;
            int qh = 0, qt = 0;
            queue_[qt++] = i; comp_[i] = ncomp;
            while (qh < qt) {
                int u = queue_[qh++];
                for (int e = head[u]; e != -1; e = nxt[e]) {
                    int v = to[e];
                    if (!comp_[v]) { comp_[v] = ncomp; queue_[qt++] = v; }
                }
            }
        }

        int ans = 0;
        int seen = 0;
        for (i = 1; i <= n && seen < ncomp; ++i) {
            /* i is a component representative iff no smaller node shares it,
               which the labelling guarantees: comp_ ids are assigned in
               increasing order of their smallest member. */
            if (comp_[i] != seen + 1) continue;
            ++seen;
            int d1, d2;
            int a = farthest(i, &d1);
            farthest(a, &d2);
            if (d2 > ans) ans = d2;
        }
        printf("%d\n", ans);
    }
    return 0;
}
