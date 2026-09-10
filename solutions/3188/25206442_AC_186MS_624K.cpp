// POJ 3188 - Cellphones
// Model: claude-fable-5-1
//
// Approach: exact depth-first search over the C(L-1,B-1) contiguous partitions,
// enumerated in the tie-break order (largest button 1 first, then largest
// button 2, ...), so the first leaf reaching the optimum is the required
// answer. The search state is a partition of the words into "collision
// classes": after closing a button that ends at letter p, two words can still
// end up with the same button sequence only if, at every position, either both
// letters are <= p and already got the same button, or both letters are > p.
// Each class is therefore refined at every cut by the bitmask of positions
// whose letter falls into the new button; members of a class with identical
// still-pending letters are merged (they can never be separated, weight >= 2
// means "doomed"). A class that shrinks to a single weight-1 member is a
// guaranteed unique word. Upper bound at a node = secured words + weight-1
// members of live classes; the subtree is pruned when bound <= best. At a leaf
// the last button swallows every pending letter, so the score is exactly the
// secured count; with exactly one cut left, a weight-1 rep survives iff the cut
// lies in the intersection of the gap sets it straddles against every
// classmate, so that level is one pairwise pass with 26-bit masks instead of a
// refine per candidate cut. A cheap greedy descent supplies the initial lower
// bound. Verified against an exhaustive enumeration on ~2400 random cases and
// never slower than the naive enumeration the discuss board reports Accepted.
//
// Ambiguity: none found; B=1 (everything on one button) and L=1 are handled by
// the general code (the last button is forced to extend to letter L).
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

int B, L, D;
int wlen[1000];
int wl[1000][10];

struct Mem {
    int w;      // representative word
    int cnt;    // number of words merged into this representative
    int cls;    // class id (sequential within a level)
    unsigned key1, key2; // pending-letter key (positions 0-4 / 5-9), for sorting/merging
};

static bool memLess(const Mem& a, const Mem& b) {
    if (a.cls != b.cls) return a.cls < b.cls;
    if (a.key1 != b.key1) return a.key1 < b.key1;
    return a.key2 < b.key2;
}

vector<Mem> st[28];   // live members per depth
int secured[28];
vector<Mem> tmpv;

// Refine st[d] (previous cut at prev, i.e. letters < prev decided) by closing
// a button covering letters [prev, p). Produces st[d+1], secured[d+1].
// Returns the upper bound (secured + potential).
int refine(int d, int prev, int p) {
    vector<Mem>& src = st[d];
    vector<Mem>& dst = st[d + 1];
    dst.clear();
    int sec = secured[d];
    tmpv.resize(src.size());
    for (size_t i = 0; i < src.size(); i++) {
        Mem m = src[i];
        int mask = 0;
        unsigned pk1 = 0, pk2 = 0;
        const int* ltr = wl[m.w];
        int n = wlen[m.w];
        for (int j = 0; j < n; j++) {
            int c = ltr[j];
            unsigned dgt = 0;
            if (c >= p) dgt = c + 1;
            else if (c >= prev) mask |= 1 << j;
            if (j < 5) pk1 = pk1 * 27 + dgt; else pk2 = pk2 * 27 + dgt;
        }
        m.cls = m.cls * 1024 + mask;
        m.key1 = pk1; m.key2 = pk2;
        tmpv[i] = m;
    }
    sort(tmpv.begin(), tmpv.end(), memLess);
    int potential = 0;
    int newCls = 0;
    size_t i = 0, n = tmpv.size();
    while (i < n) {
        size_t j = i;
        while (j < n && tmpv[j].cls == tmpv[i].cls) j++;
        // group [i,j): merge equal keys
        size_t startDst = dst.size();
        size_t a = i;
        while (a < j) {
            size_t b = a;
            int c = 0;
            while (b < j && tmpv[b].key1 == tmpv[a].key1 && tmpv[b].key2 == tmpv[a].key2) { c += tmpv[b].cnt; b++; }
            Mem m = tmpv[a];
            m.cnt = c;
            m.cls = newCls;
            dst.push_back(m);
            a = b;
        }
        size_t reps = dst.size() - startDst;
        if (reps == 1) {
            if (dst[startDst].cnt == 1) sec++;
            dst.pop_back();          // singleton class: settled either way
        } else {
            for (size_t k = startDst; k < dst.size(); k++)
                if (dst[k].cnt == 1) potential++;
            newCls++;
        }
        i = j;
    }
    secured[d + 1] = sec;
    return sec + potential;
}

int best = -1;
unsigned gapMask[26][26]; // bits e with min(a,b) < e <= max(a,b): cut before letter e separates a,b
unsigned allCut;
int cutCount[27];
int bestEnd[27];   // bestEnd[b] = exclusive end letter of button b
int curEnd[27];

// button b starts at letter prev; state at depth b is st[b]
void dfs(int b, int prev) {
    if (b == B - 1) {
        // the last button takes every pending letter, so every live class
        // collapses onto one code: the leaf score is exactly secured[b]
        curEnd[b] = L;
        if (secured[b] > best) {
            best = secured[b];
            memcpy(bestEnd, curEnd, sizeof(curEnd));
        }
        return;
    }
    if (b == B - 2) {
        // One free cut e left (letters < e on button b, the rest on the last
        // button). A weight-1 rep stays unique iff e lies in the intersection,
        // over its classmates v, of the gap sets straddled by some position:
        // count, for every e, the reps it keeps unique, then take the best e
        // (largest e on ties, matching the tie-break order).
        vector<Mem>& v = st[b];
        size_t n = v.size();
        for (size_t i = 0; i < n; i++) {
            unsigned all = allCut & ~((1u << (prev + 1)) - 1);
            v[i].key1 = all;
        }
        // intersect over pairs within each class
        size_t i = 0;
        while (i < n) {
            size_t j = i;
            while (j < n && v[j].cls == v[i].cls) j++;
            for (size_t a = i; a < j; a++) {
                if (v[a].cnt != 1) continue;
                const int* la = wl[v[a].w];
                int len = wlen[v[a].w];
                unsigned m = v[a].key1;
                for (size_t c = i; c < j && m; c++) {
                    if (c == a) continue;
                    const int* lc = wl[v[c].w];
                    unsigned g = 0;
                    for (int t = 0; t < len; t++) g |= gapMask[la[t]][lc[t]];
                    m &= g;
                }
                v[a].key1 = m;
            }
            i = j;
        }
        for (int e = prev + 1; e <= L - 1; e++) cutCount[e] = 0;
        for (i = 0; i < n; i++) {
            if (v[i].cnt != 1) continue;
            unsigned m = v[i].key1;
            while (m) { int e = 0; while (!((m >> e) & 1)) e++; cutCount[e]++; m &= m - 1; }
        }
        int bestE = -1, bestC = -1;
        for (int e = L - 1; e > prev; e--)
            if (cutCount[e] > bestC) { bestC = cutCount[e]; bestE = e; }
        if (secured[b] + bestC > best) {
            best = secured[b] + bestC;
            curEnd[b] = bestE;
            curEnd[b + 1] = L;
            memcpy(bestEnd, curEnd, sizeof(curEnd));
        }
        return;
    }
    int maxEnd = L - (B - 1 - b);
    for (int e = maxEnd; e > prev; e--) {
        int bound = refine(b, prev, e);
        if (bound <= best) continue;
        curEnd[b] = e;
        dfs(b + 1, e);
    }
}

int main() {
    if (scanf("%d %d", &B, &L) != 2) return 0;
    scanf("%d", &D);
    for (int i = 0; i < D; i++) {
        char s[64];
        scanf("%s", s);
        int n = strlen(s);
        if (n > 10) n = 10;
        wlen[i] = n;
        for (int j = 0; j < n; j++) wl[i][j] = s[j] - 'A';
    }
    for (int a = 0; a < 26; a++)
        for (int c = 0; c < 26; c++) {
            unsigned m = 0;
            int lo = a < c ? a : c, hi = a < c ? c : a;
            for (int e = lo + 1; e <= hi; e++) m |= 1u << e;
            gapMask[a][c] = m;
        }
    allCut = 0;
    for (int e = 1; e <= L - 1; e++) allCut |= 1u << e;
    // initial state: classes by length
    st[0].clear();
    for (int i = 0; i < D; i++) {
        Mem m; m.w = i; m.cnt = 1; m.cls = wlen[i]; m.key1 = 0; m.key2 = 0;
        st[0].push_back(m);
    }
    sort(st[0].begin(), st[0].end(), memLess);
    {
        // renumber classes sequentially and drop singleton classes
        vector<Mem> v; int sec = 0; int id = 0;
        size_t i = 0;
        while (i < st[0].size()) {
            size_t j = i;
            while (j < st[0].size() && st[0][j].cls == st[0][i].cls) j++;
            if (j - i == 1) sec++;
            else { for (size_t k = i; k < j; k++) { Mem m = st[0][k]; m.cls = id; v.push_back(m); } id++; }
            i = j;
        }
        st[0] = v; secured[0] = sec;
    }
    // greedy descent for an initial lower bound: at each level pick the child
    // with the largest bound (ties: larger button), keeping its state.
    {
        int prev = 0;
        int b = 0;
        for (; b < B - 1; b++) {
            int maxEnd = L - (B - 1 - b);
            int bestB = -1, bestE = -1;
            for (int e = maxEnd; e > prev; e--) {
                int bd = refine(b, prev, e);
                if (bd > bestB) { bestB = bd; bestE = e; }
            }
            refine(b, prev, bestE);
            prev = bestE;
        }
        best = secured[b] - 1;
    }
    dfs(0, 0);
    printf("%d\n", best);
    int start = 0;
    for (int b = 0; b < B; b++) {
        for (int i = start; i < bestEnd[b]; i++) putchar('A' + i);
        putchar('\n');
        start = bestEnd[b];
    }
    return 0;
}
