// POJ 3343 - Against Mammoths
// Model: claude-sonnet-5
//
// For a candidate deadline T, human planet i can defeat alien planet j iff
// T >= d[i][j] and there exists an integer departure year t in
// [0, T-d[i][j]] with n_i + t*m_i >= a_j + (t+d[i][j])*p_j.  The difference
// (LHS-RHS) is linear in t, so its extremum over the interval is attained at
// one of the two endpoints t=0 or t=T-d[i][j]; checking the OR of both
// endpoints exactly decides feasibility for that T (board thread 356045 /
// 359375 confirms naive single-endpoint checks break monotonicity in T,
// while checking both endpoints keeps feasibility monotone non-decreasing in
// T, which is what makes binary search on T valid together with a maximum
// bipartite matching feasibility test).  We binary search the minimal T for
// which a matching saturating all A alien planets exists (Hopcroft-Karp),
// bounded above by 2e9 (board thread 124081 exhibits a case requiring
// T=1,600,040,000, near the theoretical worst case n,m,p,d<=40000 giving
// numerator ~40000+40000*40000 over a denominator as small as 1).
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <climits>
using namespace std;
typedef long long ll;

static const int MAXN = 260;
int H, A;
ll n_[MAXN], m_[MAXN];
ll a_[MAXN], p_[MAXN];
ll d_[MAXN][MAXN];

vector<int> adj[MAXN];
int matchL[MAXN], matchR[MAXN], dist_[MAXN];

bool feas(int i, int j, ll T) {
    if (T < d_[i][j]) return false;
    if (n_[i] >= a_[j] + p_[j] * d_[i][j]) return true; // depart at t=0
    ll t = T - d_[i][j];
    if (n_[i] + m_[i] * t >= a_[j] + p_[j] * T) return true; // depart at t=T-d
    return false;
}

bool bfs() {
    queue<int> q;
    for (int u = 1; u <= H; u++) {
        if (matchL[u] == 0) { dist_[u] = 0; q.push(u); }
        else dist_[u] = INT_MAX;
    }
    dist_[0] = INT_MAX;
    bool found = false;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (dist_[u] < dist_[0]) {
            for (size_t k = 0; k < adj[u].size(); k++) {
                int v = adj[u][k];
                int w = matchR[v];
                if (w == 0) found = true;
                else if (dist_[w] == INT_MAX) { dist_[w] = dist_[u] + 1; q.push(w); }
            }
        }
    }
    return found;
}

bool dfs(int u) {
    if (u == 0) return true;
    for (size_t k = 0; k < adj[u].size(); k++) {
        int v = adj[u][k];
        int w = matchR[v];
        if (w == 0 || (dist_[w] == dist_[u] + 1 && dfs(w))) {
            matchL[u] = v; matchR[v] = u;
            return true;
        }
    }
    dist_[u] = INT_MAX;
    return false;
}

int hopcroftKarp() {
    memset(matchL, 0, sizeof(int) * (H + 1));
    memset(matchR, 0, sizeof(int) * (A + 1));
    int matching = 0;
    while (bfs()) {
        for (int u = 1; u <= H; u++)
            if (matchL[u] == 0)
                if (dfs(u)) matching++;
    }
    return matching;
}

int maxMatchAt(ll T) {
    for (int i = 1; i <= H; i++) {
        adj[i].clear();
        for (int j = 1; j <= A; j++)
            if (feas(i, j, T)) adj[i].push_back(j);
    }
    return hopcroftKarp();
}

int main() {
    while (scanf("%d %d", &H, &A) == 2) {
        if (H == 0 && A == 0) break;
        for (int i = 1; i <= H; i++) scanf("%lld %lld", &n_[i], &m_[i]);
        for (int j = 1; j <= A; j++) scanf("%lld %lld", &a_[j], &p_[j]);
        for (int i = 1; i <= H; i++)
            for (int j = 1; j <= A; j++)
                scanf("%lld", &d_[i][j]);

        ll lo = 0, hi = 2000000000LL;
        if (maxMatchAt(hi) < A) {
            printf("IMPOSSIBLE\n");
            continue;
        }
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (maxMatchAt(mid) >= A) hi = mid;
            else lo = mid + 1;
        }
        printf("%lld\n", lo);
    }
    return 0;
}
