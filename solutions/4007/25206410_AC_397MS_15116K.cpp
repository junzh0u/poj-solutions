// POJ 4007 - Flood-it!
// Model: claude-sonnet-5
//
// Approach: merge the board's maximal same-color connected components into
// "regions" (a region adjacency graph, where by construction every edge
// connects two regions of different colors -- if they were the same color
// they'd have been merged). The flood-it process is then: repeatedly pick a
// color, and absorb every not-yet-flooded region that is currently adjacent
// to the flooded set and has that color. Find the minimum number of picks to
// absorb every region, via IDA* with two admissible lower bounds combined by
// max: (1) the BFS eccentricity of the flooded set in the region graph (each
// move can only advance the flood frontier by one graph layer, since
// adjacent regions never share a color); (2) the number of distinct colors
// still present among unflooded regions (each such color must be chosen at
// least once). Move ordering within a bound tries the color leading to the
// smallest resulting heuristic first.
//
// The sample statement is ambiguous only in wording, not in mechanics; no
// discuss-board pitfalls were found for this problem (board was empty at
// solve time).

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef unsigned long long u64;

static int N;
static int grid[8][8];
static int regionOf[8][8];
static int regionColor[64];
static u64 adj[64];
static int R;
static u64 FULL;

static const int dr[4] = {-1, 1, 0, 0};
static const int dc[4] = {0, 0, -1, 1};

static void buildRegions() {
    memset(regionOf, -1, sizeof(regionOf));
    R = 0;
    int qr[64], qc[64];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (regionOf[i][j] == -1) {
                int color = grid[i][j];
                int id = R++;
                regionColor[id] = color;
                int head = 0, tail = 0;
                qr[tail] = i; qc[tail] = j; tail++;
                regionOf[i][j] = id;
                while (head < tail) {
                    int r = qr[head], c = qc[head]; head++;
                    for (int d = 0; d < 4; d++) {
                        int nr = r + dr[d], nc = c + dc[d];
                        if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                        if (regionOf[nr][nc] == -1 && grid[nr][nc] == color) {
                            regionOf[nr][nc] = id;
                            qr[tail] = nr; qc[tail] = nc; tail++;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < R; i++) adj[i] = 0ULL;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int id = regionOf[i][j];
            for (int d = 0; d < 4; d++) {
                int nr = i + dr[d], nc = j + dc[d];
                if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                int id2 = regionOf[nr][nc];
                if (id2 != id) {
                    adj[id] |= (1ULL << id2);
                    adj[id2] |= (1ULL << id);
                }
            }
        }
    }
    FULL = 0ULL;
    for (int i = 0; i < R; i++) FULL |= (1ULL << i);
}

static inline int lowbit_index(u64 x) {
    int c = 0;
    while (!(x & 1ULL)) { x >>= 1; c++; }
    return c;
}

static int heuristic(u64 mask) {
    // h1: BFS eccentricity (max layers) from the flooded set
    u64 visited = mask;
    u64 frontier = mask;
    int dist = 0;
    while (visited != FULL) {
        u64 nf = 0ULL;
        u64 f = frontier;
        while (f) {
            int i = lowbit_index(f);
            f &= f - 1;
            nf |= adj[i];
        }
        nf &= ~visited;
        if (nf == 0ULL) break;
        visited |= nf;
        frontier = nf;
        dist++;
    }
    int h1 = dist;

    // h2: distinct colors among unflooded regions
    bool colorPresent[6] = {false, false, false, false, false, false};
    u64 rem = FULL & ~mask;
    u64 r = rem;
    while (r) {
        int i = lowbit_index(r);
        r &= r - 1;
        colorPresent[regionColor[i]] = true;
    }
    int h2 = 0;
    for (int c = 0; c < 6; c++) if (colorPresent[c]) h2++;

    return h1 > h2 ? h1 : h2;
}

struct Cand {
    int color;
    u64 newmask;
    int h;
};

static bool candLess(const Cand &a, const Cand &b) {
    return a.h < b.h;
}

static int bound;
static int foundG;
static const int FOUND = -1;

// Transposition table: many different move orderings absorb the same set of
// regions (different color sequences can commute when they touch disjoint
// parts of the frontier), so the same mask recurs very often. Cache the best
// g seen for each mask within the current bound iteration and prune a
// re-visit at g' >= stored g: with less-or-equal remaining budget than an
// already-exhausted visit, it cannot find anything new.
// A generation stamp per slot avoids an O(TSIZE) clear every bound
// iteration (and every test case) -- a slot only counts as occupied if its
// stamp matches the current generation.
struct Entry { u64 key; int g; unsigned gen; };
static const unsigned TBITS = 20;
static const unsigned TSIZE = 1u << TBITS;
static Entry table[TSIZE];
static unsigned curGen = 0;

static void clearTable() {
    curGen++;
    if (curGen == 0) { // wrapped around (never happens in practice); hard reset
        for (unsigned i = 0; i < TSIZE; i++) table[i].gen = 0;
        curGen = 1;
    }
}

static inline unsigned hashKey(u64 k) {
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    return (unsigned)(k & (TSIZE - 1));
}

// Returns true if this (mask, g) should be pruned as redundant.
static bool seenBetterOrEqual(u64 mask, int g) {
    unsigned idx = hashKey(mask);
    for (int probe = 0; probe < 64; probe++) {
        unsigned p = (idx + (unsigned)probe) & (TSIZE - 1);
        if (table[p].gen != curGen) { table[p].key = mask; table[p].g = g; table[p].gen = curGen; return false; }
        if (table[p].key == mask) {
            if (g >= table[p].g) return true;
            table[p].g = g;
            return false;
        }
    }
    return false; // probe window exhausted; don't cache, don't prune
}

static int dfs(u64 mask, int g) {
    int h = heuristic(mask);
    int f = g + h;
    if (f > bound) return f;
    if (mask == FULL) { foundG = g; return FOUND; }
    if (seenBetterOrEqual(mask, g)) return bound + 1;

    u64 front = 0ULL;
    u64 m = mask;
    while (m) {
        int i = lowbit_index(m);
        m &= m - 1;
        front |= adj[i];
    }
    front &= ~mask;

    bool colorPresent[6] = {false, false, false, false, false, false};
    u64 f2 = front;
    while (f2) {
        int i = lowbit_index(f2);
        f2 &= f2 - 1;
        colorPresent[regionColor[i]] = true;
    }

    Cand cands[6];
    int nc = 0;
    for (int c = 0; c < 6; c++) {
        if (!colorPresent[c]) continue;
        u64 addition = 0ULL;
        u64 f3 = front;
        while (f3) {
            int i = lowbit_index(f3);
            f3 &= f3 - 1;
            if (regionColor[i] == c) addition |= (1ULL << i);
        }
        u64 nm = mask | addition;
        cands[nc].color = c;
        cands[nc].newmask = nm;
        cands[nc].h = heuristic(nm);
        nc++;
    }
    sort(cands, cands + nc, candLess);

    int minNext = 1 << 30;
    for (int k = 0; k < nc; k++) {
        int t = dfs(cands[k].newmask, g + 1);
        if (t == FOUND) return FOUND;
        if (t < minNext) minNext = t;
    }
    return minNext;
}

static int solve() {
    buildRegions();
    u64 start = 1ULL << regionOf[0][0];
    if (start == FULL) return 0;
    bound = heuristic(start);
    while (true) {
        clearTable();
        int t = dfs(start, 0);
        if (t == FOUND) return foundG;
        bound = t;
    }
}

int main() {
    while (scanf("%d", &N) == 1 && N != 0) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                scanf("%d", &grid[i][j]);
        printf("%d\n", solve());
    }
    return 0;
}
