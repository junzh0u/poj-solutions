// POJ 3662 - Telephone Lines
//
// Farmer John buys a path of cables from pole 1 to pole N; the phone company
// gives away K cables for free, and he pays the length of the longest cable he
// still has to buy.  On a chosen path the best use of the free cables is on the
// K longest edges, so the price of a path is its (K+1)-th largest edge length,
// or 0 when the path has at most K edges.  Minimising that over all paths is a
// minimax question, which makes it a binary search on the answer X:
//
//   X is affordable  <=>  some 1..N path uses at most K edges longer than X.
//
// "At most K edges longer than X" is a shortest path where an edge costs 1 if
// its length exceeds X and 0 otherwise, so each feasibility test is a 0-1 BFS
// over a deque: relax across a 0-edge to the front, across a 1-edge to the back,
// O(N + P) per test.  The count of long edges is non-increasing in X, so the
// predicate is monotone and the search is valid.
//
// The answer is always 0 or the length of some cable, so the search runs over
// the sorted distinct lengths with 0 prepended (~14 iterations for P = 10,000)
// rather than over 1..1,000,000.  Prepending 0 covers the case where K is large
// enough to cover the whole path and nothing has to be paid for.
//
// Note the graph may leave N unreachable from 1 - that is -1, not 0 - so a
// single 0-1 BFS with X set to the largest length (every edge free) is run
// first purely as a connectivity test.
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <deque>

using namespace std;

const int MAXN = 1005;
const int MAXP = 10005;
const int INF = 0x3f3f3f3f;

int head[MAXN], nxt[2 * MAXP], to[2 * MAXP], wt[2 * MAXP];
int ecnt;

static void addEdge(int a, int b, int w) {
    to[ecnt] = b; wt[ecnt] = w; nxt[ecnt] = head[a]; head[a] = ecnt++;
}

int n, p, k;
int dist_[MAXN];

/* number of edges with length > x on the cheapest such path from 1 to n,
   computed by 0-1 BFS; INF if unreachable */
static int minOver(int x) {
    int i;
    for (i = 0; i < MAXN; ++i) dist_[i] = INF;
    deque<int> dq;
    dist_[1] = 0;
    dq.push_back(1);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            int c = (wt[e] > x) ? 1 : 0;
            if (dist_[u] + c < dist_[v]) {
                dist_[v] = dist_[u] + c;
                if (c) dq.push_back(v); else dq.push_front(v);
            }
        }
    }
    return dist_[n];
}

int main() {
    if (scanf("%d %d %d", &n, &p, &k) != 3) return 0;
    int i;
    for (i = 0; i < MAXN; ++i) head[i] = -1;
    ecnt = 0;
    vector<int> cand;
    cand.push_back(0);
    for (i = 0; i < p; ++i) {
        int a, b, l;
        scanf("%d %d %d", &a, &b, &l);
        addEdge(a, b, l);
        addEdge(b, a, l);
        cand.push_back(l);
    }
    sort(cand.begin(), cand.end());
    cand.erase(unique(cand.begin(), cand.end()), cand.end());

    /* unreachable check: with x = max length every edge costs 0 */
    if (minOver(cand[cand.size() - 1]) >= INF) {
        printf("-1\n");
        return 0;
    }
    int lo = 0, hi = (int)cand.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (minOver(cand[mid]) <= k) hi = mid; else lo = mid + 1;
    }
    printf("%d\n", cand[lo]);
    return 0;
}
