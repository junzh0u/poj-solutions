// POJ 3123 - Ticket to Ride
// Model: claude-sonnet-5
//
// Four assignments each give a pair of cities that must end up in the same
// connected component of the routes we claim; routes may be shared between
// assignments. This is a Steiner-forest problem over at most 8 terminal
// "slots" (2 endpoints x 4 assignments, possibly repeating vertices or
// coinciding with each other). We:
//   1. Run the classic Dreyfus-Wagner Steiner-tree DP over all 2^8 subsets
//      of the 8 terminal slots, using real graph edges (kept as a multigraph
//      -- the board warns there can be duplicate edges between the same
//      pair of cities, so no adjacency-matrix collapsing) with a Dijkstra
//      relaxation pass per mask.
//   2. For each subset A of the 4 assignments, f(A) = minimum Steiner tree
//      cost connecting the union of A's terminal slots (via the DP table).
//   3. Partition DP over the 4 assignments: g(S) = min cost to satisfy all
//      assignments in S, by choosing the block containing the assignment
//      with S's lowest bit and combining with g(S minus that block). This
//      finds the best way to let independent assignments share a Steiner
//      tree, or keep separate ones, whichever is cheaper.
// Answer is g(full set). No statement ambiguity beyond the "shared routes"
// framing, which the sample (two candidate move sets of equal effect) and
// the board's Steiner-tree consensus both confirm.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int INF = 0x3f3f3f3f;
const int MAXN = 32;
const int MAXK = 8; // 4 assignments * 2 endpoints

int n, m;
map<string,int> nameId;
vector<pair<int,int> > adj[MAXN]; // (to, weight)

int term[MAXK];
int dp[1<<MAXK][MAXN];

int getId(const string &s) {
    map<string,int>::iterator it = nameId.find(s);
    return it->second;
}

void dijkstraRelax(int mask) {
    // dp[mask][*] already holds initial distances; relax using graph edges.
    priority_queue<pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;
    for (int v = 0; v < n; v++) {
        if (dp[mask][v] < INF) pq.push(make_pair(dp[mask][v], v));
    }
    vector<bool> done(n, false);
    while (!pq.empty()) {
        pair<int,int> top = pq.top(); pq.pop();
        int d = top.first, v = top.second;
        if (done[v]) continue;
        done[v] = true;
        if (d > dp[mask][v]) continue;
        for (size_t i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i].first, w = adj[v][i].second;
            if (dp[mask][v] + w < dp[mask][u]) {
                dp[mask][u] = dp[mask][v] + w;
                pq.push(make_pair(dp[mask][u], u));
            }
        }
    }
}

int main() {
    char buf[64];
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        nameId.clear();
        for (int i = 0; i < n; i++) {
            scanf("%s", buf);
            nameId[string(buf)] = i;
            adj[i].clear();
        }
        for (int i = 0; i < m; i++) {
            char a[32], b[32];
            int c;
            scanf("%s %s %d", a, b, &c);
            int u = getId(a), v = getId(b);
            adj[u].push_back(make_pair(v, c));
            adj[v].push_back(make_pair(u, c));
        }
        for (int i = 0; i < 4; i++) {
            char a[32], b[32];
            scanf("%s %s", a, b);
            term[2*i] = getId(a);
            term[2*i+1] = getId(b);
        }

        int K = MAXK; // 8 terminal slots
        int full = (1<<K);
        for (int mask = 0; mask < full; mask++)
            for (int v = 0; v < n; v++)
                dp[mask][v] = INF;
        for (int i = 0; i < K; i++) {
            dp[1<<i][term[i]] = 0;
        }
        for (int mask = 1; mask < full; mask++) {
            for (int v = 0; v < n; v++) {
                for (int sub = (mask-1) & mask; sub > 0; sub = (sub-1) & mask) {
                    int other = mask ^ sub;
                    if (sub < other) continue; // avoid double work, sub>=other covers all splits once (sub==other impossible since mask nonzero split needs sub!=other unless mask has repeated... sub==other only if mask=0)
                    if (dp[sub][v] < INF && dp[other][v] < INF) {
                        int cand = dp[sub][v] + dp[other][v];
                        if (cand < dp[mask][v]) dp[mask][v] = cand;
                    }
                }
            }
            dijkstraRelax(mask);
        }

        // f(A) for A subset of {0,1,2,3} (the 4 assignments)
        int fval[16];
        fval[0] = 0;
        for (int A = 1; A < 16; A++) {
            int M = 0;
            for (int i = 0; i < 4; i++) if (A & (1<<i)) M |= (1<<(2*i)) | (1<<(2*i+1));
            int best = INF;
            for (int v = 0; v < n; v++) best = min(best, dp[M][v]);
            fval[A] = best;
        }

        int g[16];
        g[0] = 0;
        for (int S = 1; S < 16; S++) {
            int lowbit = S & (-S);
            int best = INF;
            for (int A = S; A > 0; A = (A-1) & S) {
                if (!(A & lowbit)) continue;
                int cand = fval[A] + g[S ^ A];
                if (cand < best) best = cand;
            }
            g[S] = best;
        }

        printf("%d\n", g[15]);
    }
    return 0;
}
