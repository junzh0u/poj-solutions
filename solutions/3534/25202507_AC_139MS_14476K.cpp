// POJ 3534 - Escape of Life and Death
// Model: claude-sonnet-5
// Approach: The seismic wave spreads isotropically from the epicenter (the
// location of chamber 0) at speed v, so chamber i (1<=i<=n) collapses at
// time D(i)/v where D(i) is its Euclidean distance to chamber 0. Yangyang
// may move along a corridor only to a chamber strictly farther from the
// epicenter than his current chamber, except that the final hop into the
// safe chamber n+1 (on the ground, immune to collapse) is exempt from that
// rule. Per the sample/hint, if the wave reaches a chamber at (or before)
// the exact moment he would leave it, he is trapped -- so a chamber can be
// used as a source for further travel only while its arrival time is
// strictly less than its collapse time. Chamber 0 itself is exempt from
// this check (he starts there before the quake fully engages).
// Because every usable edge goes from strictly smaller D to strictly larger
// D (or directly into n+1), sorting nodes 0..n by squared distance from the
// epicenter gives a valid topological order (confirmed on the discuss
// board: "topo sort should really be by distance to the source, then
// update node times in that order") for a DAG shortest-path DP: process
// nodes in that order and, once a node passes its collapse check, relax its
// incident edges (undirected corridors tried in both directions, filtered
// by the direction rule).
// All arrival times are integers (edge weights are integers, T(0)=0), so
// the strict comparison against the generally-irrational collapse time
// D(u)/v is decided by an exact integer comparison (v*T(u))^2 < D(u)^2
// using 128-bit arithmetic -- avoiding floating point/epsilon issues that
// the board's WA reports (e.g. from imprecise distance sorting) suggest are
// a real pitfall here. No other statement ambiguity: corridors confirmed
// undirected on the board, and the sample's exact-tie case (both the
// direct edge and the exemption for n+1) was traced by hand and matches
// "Impossible".
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef __int128 lll;

static const int MAXNODE = 50005;

int n, m, v;
ll X[MAXNODE], Y[MAXNODE];
ll sqDist_[MAXNODE];
ll T_[MAXNODE];
vector<pair<int, ll> > adj[MAXNODE];
int order_[MAXNODE];

bool cmpOrder(int a, int b) {
    return sqDist_[a] < sqDist_[b];
}

int main() {
    if (scanf("%d %d %d", &n, &m, &v) != 3) return 0;
    for (int i = 1; i <= n; ++i) {
        scanf("%lld %lld", &X[i], &Y[i]);
    }
    vector<int> A(m), B(m);
    vector<ll> Wt(m);
    for (int i = 0; i < m; ++i) {
        int a, b; ll t;
        scanf("%d %d %lld", &a, &b, &t);
        A[i] = a; B[i] = b; Wt[i] = t;
    }
    ll x0, y0, x1, y1;
    scanf("%lld %lld %lld %lld", &x0, &y0, &x1, &y1);
    X[0] = x0; Y[0] = y0;
    X[n + 1] = x1; Y[n + 1] = y1;

    for (int i = 0; i <= n + 1; ++i) {
        ll dx = X[i] - X[0];
        ll dy = Y[i] - Y[0];
        sqDist_[i] = dx * dx + dy * dy;
    }

    for (int i = 0; i < m; ++i) {
        int a = A[i], b = B[i]; ll t = Wt[i];
        adj[a].push_back(make_pair(b, t));
        adj[b].push_back(make_pair(a, t));
    }

    for (int i = 0; i <= n; ++i) order_[i] = i;
    sort(order_, order_ + n + 1, cmpOrder);

    const ll INF = (ll)4e18;
    for (int i = 0; i <= n + 1; ++i) T_[i] = INF;
    T_[0] = 0;

    for (int idx = 0; idx <= n; ++idx) {
        int u = order_[idx];
        if (u != 0) {
            if (T_[u] >= INF) continue;
            ll lhs = (ll)v * T_[u];
            lll lhs2 = (lll)lhs * (lll)lhs;
            if (!(lhs2 < (lll)sqDist_[u])) continue; // already collapsed, can't leave
        }
        ll Tu = T_[u];
        for (size_t k = 0; k < adj[u].size(); ++k) {
            int w = adj[u][k].first;
            ll wt = adj[u][k].second;
            ll cand = Tu + wt;
            if (w == n + 1) {
                if (cand < T_[n + 1]) T_[n + 1] = cand;
            } else if (sqDist_[w] > sqDist_[u]) {
                if (cand < T_[w]) T_[w] = cand;
            }
        }
    }

    if (T_[n + 1] < INF) {
        printf("%lld\n", T_[n + 1]);
    } else {
        printf("Impossible\n");
    }
    return 0;
}
