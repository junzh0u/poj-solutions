// POJ 1733 - Parity game
//
// Answers of the form "the number of ones in positions [a, b] is even/odd" are
// statements about prefix parities: with pre[i] the parity of the first i
// digits, the answer for [a, b] says pre[a-1] xor pre[b] == t.  So each answer
// is a parity relation between the two *endpoints* a-1 and b -- note the -1,
// which is the off-by-one the whole problem turns on -- and the question is
// whether the accumulated relations stay consistent.
//
// That is a weighted (parity) union-find over the endpoints: each node stores
// its parity relative to its parent, find compresses the path while folding the
// parities together, and union links the two roots with the parity that makes
// the new relation hold.  An answer whose endpoints already share a root is
// checked instead of merged; the first one that disagrees is the first provably
// wrong answer, and the output is the number of answers before it.
//
// The sequence length runs to 1e9 but there are at most 5000 answers, hence at
// most 10000 distinct endpoints, so the endpoints are coordinate-compressed
// through a std::map instead of being indexed directly.
//
// Two statement details worth spelling out:
//  * the input always contains exactly the stated number of answers, even when
//    a contradiction was reached earlier, so every line is consumed regardless;
//  * if all answers are consistent the output is the full count of questions.
//
// The outer while loop over test cases is defensive -- the judge data appears to
// hold a single case, and the loop simply stops at EOF either way.

#include <cstdio>
#include <map>

using std::map;

/* Two endpoints per question, at most 5000 questions. */
static const int MAXN = 10005;

static int par[MAXN];
static int rel[MAXN]; /* parity of node relative to par[node] */
static int rnk[MAXN];
static int cnt;

static void init_dsu()
{
    for (int i = 0; i < MAXN; ++i) {
        par[i] = i;
        rel[i] = 0;
        rnk[i] = 0;
    }
    cnt = 0;
}

/* Find the root of x, compressing the path so that afterwards
   par[x] == root and rel[x] == parity of x relative to root. */
static int find_set(int x)
{
    int root = x;
    while (par[root] != root)
        root = par[root];

    /* parity of x relative to root */
    int acc = 0;
    for (int cur = x; cur != root; cur = par[cur])
        acc ^= rel[cur];

    int cur = x;
    int nrel = acc;
    while (cur != root) {
        int next = par[cur];
        int r = rel[cur];
        par[cur] = root;
        rel[cur] = nrel;
        nrel ^= r; /* parity of next relative to root */
        cur = next;
    }
    return root;
}

static int node_of(map<int, int> &idx, int key)
{
    map<int, int>::iterator it = idx.find(key);
    if (it != idx.end())
        return it->second;
    int id = cnt++;
    idx[key] = id;
    return id;
}

int main()
{
    int n, q;
    while (scanf("%d", &n) == 1) {
        if (scanf("%d", &q) != 1)
            break;
        (void)n;
        init_dsu();
        map<int, int> idx;
        int ans = q;
        int broken = 0;

        for (int i = 1; i <= q; ++i) {
            int a, b;
            char w[16];
            if (scanf("%d %d %15s", &a, &b, w) != 3)
                return 0;
            if (broken)
                continue;
            if (a > b) { int tmp = a; a = b; b = tmp; }
            int t = (w[0] == 'o') ? 1 : 0;

            int ia = node_of(idx, a - 1);
            int ib = node_of(idx, b);

            int ra = find_set(ia);
            int rb = find_set(ib);
            int pa = (ia == ra) ? 0 : rel[ia];
            int pb = (ib == rb) ? 0 : rel[ib];

            if (ra == rb) {
                if ((pa ^ pb) != t) {
                    ans = i - 1;
                    broken = 1;
                }
            } else {
                int d = pa ^ pb ^ t; /* parity between the two roots */
                if (rnk[ra] < rnk[rb]) {
                    par[ra] = rb;
                    rel[ra] = d;
                } else {
                    par[rb] = ra;
                    rel[rb] = d;
                    if (rnk[ra] == rnk[rb])
                        rnk[ra]++;
                }
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
