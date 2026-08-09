// POJ 2989 - All Friends
// Model: claude-sonnet-5
//
// Enumerate maximal cliques (maximal "sets of friends") in an undirected
// graph on n<=128 vertices, counting them up to a cap of 1000 (report
// "Too many maximal sets of friends." beyond that).
//
// Approach: classic incremental maximal-clique generation. Build up the
// family of maximal cliques of the induced subgraph on vertices {0..i-1}
// one vertex at a time. Given the family F for vertices {0..i-1} and the
// new vertex v=i-1 (0-indexed) being added:
//   - for each clique C in F that is fully connected to v (C subset of
//     N(v)): C+{v} remains a maximal clique of the extended graph, so C is
//     updated in place.
//   - otherwise, candidate = (C ∩ N(v)) + {v} may be a new maximal clique;
//     it is kept if it is not already present and is actually maximal in
//     the extended graph.
// This is a known polynomial-delay algorithm and is exactly what the
// contest's own reference solution (CTU Open 2005, problem A) implements.
// The generation is capped: as soon as the family exceeds 1000 cliques we
// stop immediately (a graph can have exponentially many maximal cliques,
// so this cap is essential for correctness of the "too many" case within
// the time limit).
//
// Statement ambiguity: an isolated vertex (no friends at all) is itself a
// maximal set of friends (a clique of size 1 that cannot be extended).
// This is not obvious from the prose alone and was confirmed against the
// official CTU Open 2005 judge data (contest.felk.cvut.cz/05prg/solved/
// a.in / a.out): the case "111 0" (111 people, 0 relations) expects output
// "111", i.e. every isolated vertex counts separately. A discuss-board
// post that assumed isolated vertices do NOT count (giving answer 6
// instead of 8 for an 11-vertex test case with two isolated vertices) was
// never confirmed and is inconsistent with the official data; this
// solution follows the official data.
//
// Multiple instances are separated by blank lines and are simply read
// until EOF via scanf, which naturally skips blank lines.

#include <cstdio>
#include <cstring>

const int MAXN = 128;
const int MAXS = 1000;      // cap on number of maximal cliques to enumerate
const int SETCAP = MAXS + 8; // small safety margin for scratch slot writes

// 128 bits packed as four 32-bit words (no 64-bit types needed anywhere:
// POJ's language 4 / old MSVC has no `long long`, so this keeps the
// solution portable across languages without relying on it).
struct Set128 {
    unsigned w[4];
};

static inline void set_clear(Set128 &s) {
    s.w[0] = 0u; s.w[1] = 0u; s.w[2] = 0u; s.w[3] = 0u;
}

static inline void set_add(Set128 &s, int elt) {
    s.w[elt >> 5] |= (1u << (elt & 31));
}

static inline bool set_in(const Set128 &s, int elt) {
    return (s.w[elt >> 5] >> (elt & 31)) & 1u;
}

// c = a & b; returns true if a is a subset of b (a & b == a)
static inline bool set_and_subset(const Set128 &a, const Set128 &b, Set128 &c) {
    bool eq = true;
    for (int i = 0; i < 4; i++) {
        c.w[i] = a.w[i] & b.w[i];
        if (c.w[i] != a.w[i]) eq = false;
    }
    return eq;
}

static inline bool set_eq(const Set128 &a, const Set128 &b) {
    return a.w[0] == b.w[0] && a.w[1] == b.w[1] && a.w[2] == b.w[2] && a.w[3] == b.w[3];
}

Set128 graph[MAXN];
Set128 sets[SETCAP];

// Simple open-addressing hash set over Set128 values, cleared once per
// test instance. Table size is a fixed prime comfortably larger than the
// max family size we ever track (<= MAXS+1).
const int HSIZE = 4099;
struct HEntry {
    bool occupied;
    bool tomb;
    Set128 val;
};
HEntry htab[HSIZE];

static inline unsigned hash_of(const Set128 &s) {
    unsigned h = 2166136261u;
    for (int i = 0; i < 4; i++) {
        h ^= s.w[i];
        h *= 16777619u; // 32-bit FNV-1a style mix, wraps naturally on unsigned overflow
    }
    return h % (unsigned)HSIZE;
}

void htab_clear() {
    memset(htab, 0, sizeof(htab));
}

bool htab_contains(const Set128 &s) {
    unsigned idx = hash_of(s);
    for (;;) {
        if (!htab[idx].occupied && !htab[idx].tomb) return false;
        if (htab[idx].occupied && set_eq(htab[idx].val, s)) return true;
        idx++;
        if (idx == (unsigned)HSIZE) idx = 0;
    }
}

void htab_insert(const Set128 &s) {
    unsigned idx = hash_of(s);
    for (;;) {
        if (!htab[idx].occupied) {
            htab[idx].occupied = true;
            htab[idx].tomb = false;
            htab[idx].val = s;
            return;
        }
        idx++;
        if (idx == (unsigned)HSIZE) idx = 0;
    }
}

void htab_erase(const Set128 &s) {
    unsigned idx = hash_of(s);
    for (;;) {
        if (!htab[idx].occupied && !htab[idx].tomb) return; // shouldn't happen
        if (htab[idx].occupied && set_eq(htab[idx].val, s)) {
            htab[idx].occupied = false;
            htab[idx].tomb = true;
            return;
        }
        idx++;
        if (idx == (unsigned)HSIZE) idx = 0;
    }
}

// Is candidate maximal within the induced subgraph on vertices [0, nLevel)?
bool is_maximal(const Set128 &cand, int nLevel) {
    Set128 tmp;
    for (int i = 0; i < nLevel; i++) {
        if (!set_in(cand, i)) {
            if (set_and_subset(cand, graph[i], tmp)) {
                // vertex i is connected to every member of cand -> extendable
                return false;
            }
        }
    }
    return true;
}

// Returns number of maximal cliques (capped: any value > MAXS means "too many",
// and generation stops as soon as that is detected).
int count_maximal_cliques(int n) {
    if (n <= 0) return 0;

    int k = 1;
    set_clear(sets[0]);
    set_add(sets[0], 0);
    htab_clear();
    htab_insert(sets[0]);

    for (int elt = 1; elt < n; elt++) {
        if (k > MAXS) break;
        int newK = k;
        bool overflow = false;
        for (int i = 0; i < k; i++) {
            Set128 inter;
            bool subset = set_and_subset(sets[i], graph[elt], inter);
            if (subset) {
                // elt is connected to all of sets[i]: extend in place
                htab_erase(sets[i]);
                set_add(sets[i], elt);
                htab_insert(sets[i]);
            } else {
                Set128 cand = inter;
                set_add(cand, elt);
                if (!htab_contains(cand)) {
                    if (is_maximal(cand, elt + 1)) {
                        sets[newK] = cand;
                        htab_insert(sets[newK]);
                        newK++;
                        if (newK > MAXS) {
                            overflow = true;
                            break;
                        }
                    }
                }
            }
        }
        k = newK;
        if (overflow) break;
    }
    return k;
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        for (int i = 0; i < n; i++) set_clear(graph[i]);
        for (int i = 0; i < m; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            a--; b--;
            set_add(graph[a], b);
            set_add(graph[b], a);
        }
        int k = count_maximal_cliques(n);
        if (k > MAXS) {
            printf("Too many maximal sets of friends.\n");
        } else {
            printf("%d\n", k);
        }
    }
    return 0;
}
