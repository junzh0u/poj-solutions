// POJ 3729 - Facer's string
// Model: gpt-5.6-terra
// Build a suffix array for S1, a separator, and S2.  For every suffix of S1,
// its longest common prefix with any suffix of S2 is attained by the nearest
// S2 suffix on either side in suffix-array order.  It is valid exactly when
// that maximum is k: k characters occur in S2 but the following character
// does not (and a suffix ending at S1's end uses the stipulated blank).
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

static void suffix_array(const vector<int>& a, vector<int>& sa, vector<int>& ht) {
    int n = (int)a.size(), m = 10005;
    sa.resize(n);
    vector<int> x(n), y(n), c(max(n, m) + 1);
    for (int i = 0; i < n; ++i) ++c[x[i] = a[i]];
    for (int i = 1; i <= m; ++i) c[i] += c[i - 1];
    for (int i = n - 1; i >= 0; --i) sa[--c[x[i]]] = i;
    for (int k = 1, p = 0; k < n; k <<= 1, m = p) {
        p = 0;
        for (int i = n - k; i < n; ++i) y[p++] = i;
        for (int i = 0; i < n; ++i) if (sa[i] >= k) y[p++] = sa[i] - k;
        fill(c.begin(), c.begin() + m + 1, 0);
        for (int i = 0; i < n; ++i) ++c[x[y[i]]];
        for (int i = 1; i <= m; ++i) c[i] += c[i - 1];
        for (int i = n - 1; i >= 0; --i) sa[--c[x[y[i]]]] = y[i];
        y[sa[0]] = 0; p = 1;
        for (int i = 1; i < n; ++i) {
            int u = sa[i], v = sa[i - 1];
            if (x[u] == x[v] && (u + k < n ? x[u + k] : -1) == (v + k < n ? x[v + k] : -1)) y[u] = p - 1;
            else y[u] = p++;
        }
        x.swap(y);
        if (p >= n) break;
    }
    vector<int> rank(n);
    for (int i = 0; i < n; ++i) rank[sa[i]] = i;
    ht.assign(n, 0);
    for (int i = 0, h = 0; i < n; ++i) {
        int r = rank[i];
        if (r == 0) continue;
        int j = sa[r - 1];
        while (i + h < n && j + h < n && a[i + h] == a[j + h]) ++h;
        ht[r] = h;
        if (h) --h;
    }
}

int main() {
    int n, m, k;
    while (scanf("%d%d%d", &n, &m, &k) == 3) {
        vector<int> a(n + m + 1);
        for (int i = 0; i < n; ++i) { scanf("%d", &a[i]); ++a[i]; }
        a[n] = 0;
        for (int i = 0; i < m; ++i) { scanf("%d", &a[n + 1 + i]); ++a[n + 1 + i]; }
        vector<int> sa, ht;
        suffix_array(a, sa, ht);
        int N = (int)a.size(), inf = N + 1;
        vector<int> best(N, -1);
        int last = -1, mn = inf;
        for (int r = 0; r < N; ++r) {
            if (r) mn = min(mn, ht[r]);
            int pos = sa[r];
            if (pos > n) { last = r; mn = inf; }
            else if (pos < n && last != -1) best[pos] = mn;
        }
        last = -1; mn = inf;
        for (int r = N - 1; r >= 0; --r) {
            int pos = sa[r];
            if (pos > n) { last = r; mn = inf; }
            else if (pos < n && last != -1) best[pos] = max(best[pos], mn);
            if (r) mn = min(mn, ht[r]);
        }
        int ans = 0;
        for (int i = 0; i < n; ++i) if (best[i] == k) ++ans;
        printf("%d\n", ans);
    }
    return 0;
}
