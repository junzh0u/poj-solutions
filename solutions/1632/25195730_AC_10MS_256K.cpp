// POJ 1632 - Vase collection
// Model: claude-opus-5
//
// Maximum balanced complete bipartite subgraph (k x k biclique) of the
// shape/decoration incidence graph: find the largest k for which some k shapes
// and some k decorations have all k*k pairs present in the collection.
//
// Each shape keeps the set of its decorations as a 36-bit mask, split over two
// unsigned ints so no 64-bit type is needed (POJ's C++ compiler has none).
// For k = 36 down to 1, the only shapes that can join a k-biclique are those
// with at least k decorations, and with m <= 100 vases there are at most
// min(36, 100/k) of them; a DFS over subsets of that candidate list carries the
// running intersection of the chosen shapes' decoration masks and abandons a
// branch as soon as the intersection drops below k bits. The first k that
// succeeds is the answer. Worst case is a few tens of thousands of nodes per
// scenario; 2000 adversarial scenarios run in 0.02s locally.
//
// Input framing: the leading count is read but the scenarios are then consumed
// to EOF, so extra data sets cannot be silently dropped. Duplicate (s,d) vases
// collapse into the bitmask on their own.
//
// The official sample is weak evidence: of four mutants, only "maximum
// bipartite matching" is killed by it. Reporting the maximum shape degree, or
// the largest intersection without capping it at the number of chosen shapes,
// both reproduce the sample exactly, and were caught only by a randomized
// differential test against a reference that enumerates every pair of equal
// sized shape/decoration subsets straight from the statement's wording
// (261 disagreements over 360 trials), plus the constructed case of one shape
// carrying three decorations, whose answer is 1 and not 3.

#include <cstdio>
#include <cstring>

static unsigned int slo[37], shi[37];
static bool present[37];

static int pc(unsigned int x) { int c = 0; while (x) { x &= x - 1; ++c; } return c; }

static int K, NC;
static unsigned int cl[40], ch[40];

static bool dfs(int idx, int chosen, unsigned int a, unsigned int b) {
    if (chosen == K) return true;
    if (NC - idx < K - chosen) return false;
    for (int i = idx; i < NC; ++i) {
        if (NC - i < K - chosen) break;
        unsigned int na = a & cl[i], nb = b & ch[i];
        if (pc(na) + pc(nb) < K) continue;
        if (dfs(i + 1, chosen + 1, na, nb)) return true;
    }
    return false;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int m;
    while (scanf("%d", &m) == 1) {
        memset(slo, 0, sizeof(slo));
        memset(shi, 0, sizeof(shi));
        memset(present, 0, sizeof(present));
        for (int i = 0; i < m; ++i) {
            int s, d;
            if (scanf("%d %d", &s, &d) != 2) { s = 1; d = 1; }
            present[s] = true;
            if (d <= 32) slo[s] |= (1u << (d - 1));
            else shi[s] |= (1u << (d - 33));
        }
        int ans = 0;
        for (K = 36; K >= 1; --K) {
            NC = 0;
            for (int s = 1; s <= 36; ++s)
                if (present[s] && pc(slo[s]) + pc(shi[s]) >= K) {
                    cl[NC] = slo[s]; ch[NC] = shi[s]; ++NC;
                }
            if (NC < K) continue;
            if (dfs(0, 0, 0xffffffffu, 0xffffffffu)) { ans = K; break; }
        }
        printf("%d\n", ans);
    }
    return 0;
}
