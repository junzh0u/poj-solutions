// POJ 1613 - Cave Raider
// Model: claude-sonnet-5
// Approach: time-dependent shortest path (earliest arrival). Each tunnel has a
// schedule of alternating closing/opening times (tunnel starts open). From this
// we derive the tunnel's open intervals [lo,hi] (closed intervals are the gaps).
// Given the time `avail` we are ready to enter a tunnel, we scan its open
// intervals in increasing order and, for the first interval where a crossing
// dep=max(avail,lo), dep+w<=hi fits, that is the earliest possible arrival via
// this edge (the earliest-arrival function of avail is monotone non-decreasing,
// a staircase, so this is well defined and Dijkstra's label-setting algorithm
// -- always extending the currently-known-earliest node -- remains correct,
// same FIFO argument as ordinary time-dependent shortest paths with waiting
// allowed at nodes). n<=50 so a simple O(n^2) Dijkstra suffices.
// The schedule line can hold 0..32 times (odd count => closed forever after the
// last time, even count => open forever after it), parsed by reading whole
// lines (not just tokens) since the per-edge integer count is not given up
// front; standard istream whitespace skipping (including any stray \r) makes
// this robust to the trailing-whitespace quirks reported on the discuss board.
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
static const ll BIGHI = 2000000000LL; // sentinel for "open forever" interval end
static const ll INF = (ll)4e18;

struct Interval { ll lo, hi; };

struct Edge {
    int u, v;
    ll w;
    vector<Interval> iv;
};

bool readLineTokens(vector<ll>& out) {
    string line;
    while (std::getline(cin, line)) {
        istringstream iss(line);
        ll x;
        out.clear();
        bool any = false;
        while (iss >> x) { out.push_back(x); any = true; }
        if (any) return true;
    }
    return false;
}

ll earliestArrival(ll avail, const vector<Interval>& iv, ll w) {
    for (size_t k = 0; k < iv.size(); ++k) {
        ll lo = iv[k].lo, hi = iv[k].hi;
        ll dep = avail > lo ? avail : lo;
        if (dep > hi) continue;
        if (dep + w <= hi) return dep + w;
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    vector<ll> tok;
    while (readLineTokens(tok)) {
        if (tok.size() < 1) continue;
        int n = (int)tok[0];
        if (n == 0) break;
        // header line should have n m s t
        int m, s, t;
        if (tok.size() >= 4) {
            m = (int)tok[1]; s = (int)tok[2]; t = (int)tok[3];
        } else {
            // extremely defensive fallback (shouldn't happen with well-formed input)
            vector<ll> more;
            readLineTokens(more);
            m = (int)tok[1]; s = (int)tok[2]; t = (int)more[0];
        }

        vector<vector<Edge> > adj(n + 1);
        for (int e = 0; e < m; ++e) {
            vector<ll> et;
            readLineTokens(et);
            Edge ed;
            ed.u = (int)et[0];
            ed.v = (int)et[1];
            ed.w = et[2];
            vector<ll> times;
            for (size_t i = 3; i < et.size(); ++i) times.push_back(et[i]);
            vector<Interval> iv;
            if (times.empty()) {
                Interval it; it.lo = 0; it.hi = BIGHI; iv.push_back(it);
            } else {
                Interval it; it.lo = 0; it.hi = times[0]; iv.push_back(it);
                size_t i = 1;
                while (i + 1 < times.size()) {
                    Interval it2; it2.lo = times[i]; it2.hi = times[i + 1];
                    iv.push_back(it2);
                    i += 2;
                }
                if (times.size() % 2 == 0) {
                    Interval it3; it3.lo = times[times.size() - 1]; it3.hi = BIGHI;
                    iv.push_back(it3);
                }
            }
            ed.iv = iv;
            adj[ed.u].push_back(ed);
            Edge ed2 = ed;
            ed2.u = ed.v; ed2.v = ed.u;
            adj[ed2.u].push_back(ed2);
        }

        vector<ll> dist(n + 1, INF);
        vector<bool> vis(n + 1, false);
        dist[s] = 0;
        for (int iter = 0; iter < n; ++iter) {
            int u = -1; ll best = INF;
            for (int i = 1; i <= n; ++i) if (!vis[i] && dist[i] < best) { best = dist[i]; u = i; }
            if (u == -1) break;
            vis[u] = true;
            if (u == t) break;
            for (size_t k = 0; k < adj[u].size(); ++k) {
                const Edge& ed = adj[u][k];
                int v = ed.v;
                if (vis[v]) continue;
                if (dist[u] >= INF) continue;
                ll arr = earliestArrival(dist[u], ed.iv, ed.w);
                if (arr >= 0 && arr < dist[v]) dist[v] = arr;
            }
        }

        if (dist[t] >= INF) printf("*\n");
        else printf("%lld\n", dist[t]);
    }
    return 0;
}
