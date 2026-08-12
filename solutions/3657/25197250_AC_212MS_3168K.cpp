// POJ 3657 - Haybale Guessing
// Model: gpt-5.6-terra
// Binary-search the first inconsistent prefix.  For one prefix, every query
// raises a range lower bound; queries with the same reported minimum must share
// one position, whose final lower bound must not exceed that minimum.
#include <algorithm>
#include <cstdio>
#include <map>
#include <vector>
using namespace std;

struct Query { int l, r, a; };
struct Node { int mn, second, tag; };

struct SegTree {
    vector<Node> t;
    int n;
    SegTree(int n_) : n(n_) { t.resize(n * 4 + 8); build(1, 0, n - 1); }
    void build(int p, int l, int r) {
        t[p].mn = t[p].tag = 0;
        t[p].second = 0x3f3f3f3f;
        if (l != r) { int m = (l + r) / 2; build(p * 2, l, m); build(p * 2 + 1, m + 1, r); }
    }
    void raise_min(int p, int x) {
        if (x <= t[p].mn) return;
        t[p].mn = x;
        if (x > t[p].tag) t[p].tag = x;
    }
    void push(int p) {
        if (t[p].tag) { raise_min(p * 2, t[p].tag); raise_min(p * 2 + 1, t[p].tag); t[p].tag = 0; }
    }
    void pull(int p) {
        Node &x = t[p]; Node &a = t[p * 2]; Node &b = t[p * 2 + 1];
        x.mn = min(a.mn, b.mn);
        x.second = min(a.mn == x.mn ? a.second : a.mn, b.mn == x.mn ? b.second : b.mn);
    }
    void chmax(int p, int l, int r, int ql, int qr, int x) {
        if (qr < l || r < ql || x <= t[p].mn) return;
        if (ql <= l && r <= qr && x < t[p].second) { raise_min(p, x); return; }
        push(p); int m = (l + r) / 2;
        chmax(p * 2, l, m, ql, qr, x); chmax(p * 2 + 1, m + 1, r, ql, qr, x); pull(p);
    }
    int query_min(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p].mn;
        push(p); int m = (l + r) / 2, ans = 0x3f3f3f3f;
        if (ql <= m) ans = min(ans, query_min(p * 2, l, m, ql, qr));
        if (qr > m) ans = min(ans, query_min(p * 2 + 1, m + 1, r, ql, qr));
        return ans;
    }
};

int n, q;
vector<Query> qs;
vector<int> coord;

bool consistent(int cnt) {
    SegTree st((int)coord.size() - 1);
    map<int, pair<int, int> > common;
    int i;
    for (i = 0; i < cnt; ++i) {
        int left = lower_bound(coord.begin(), coord.end(), qs[i].l) - coord.begin();
        int right = lower_bound(coord.begin(), coord.end(), qs[i].r + 1) - coord.begin() - 1;
        st.chmax(1, 0, st.n - 1, left, right, qs[i].a);
        if (common.find(qs[i].a) == common.end()) common[qs[i].a] = make_pair(qs[i].l, qs[i].r);
        else {
            common[qs[i].a].first = max(common[qs[i].a].first, qs[i].l);
            common[qs[i].a].second = min(common[qs[i].a].second, qs[i].r);
        }
    }
    map<int, pair<int, int> >::iterator it;
    for (it = common.begin(); it != common.end(); ++it) {
        int l = it->second.first, r = it->second.second;
        if (l > r) return false;
        int left = lower_bound(coord.begin(), coord.end(), l) - coord.begin();
        int right = lower_bound(coord.begin(), coord.end(), r + 1) - coord.begin() - 1;
        if (st.query_min(1, 0, st.n - 1, left, right) > it->first) return false;
    }
    return true;
}

int main() {
    if (scanf("%d%d", &n, &q) != 2) return 0;
    qs.resize(q); coord.push_back(1); coord.push_back(n + 1);
    for (int i = 0; i < q; ++i) {
        scanf("%d%d%d", &qs[i].l, &qs[i].r, &qs[i].a);
        coord.push_back(qs[i].l); coord.push_back(qs[i].r + 1);
    }
    sort(coord.begin(), coord.end()); coord.erase(unique(coord.begin(), coord.end()), coord.end());
    if (consistent(q)) { printf("0\n"); return 0; }
    int lo = 1, hi = q;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (consistent(mid)) lo = mid + 1; else hi = mid;
    }
    printf("%d\n", lo);
    return 0;
}
