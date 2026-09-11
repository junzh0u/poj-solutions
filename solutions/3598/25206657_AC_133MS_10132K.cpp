// POJ 3598 - PopKart
// Model: claude-sonnet-5
//
// A kart X dominates kart Y iff X.v>=Y.v and X.w>=Y.w and (X.v,X.w)!=(Y.v,Y.w)
// (superior in at least one coordinate, not inferior in the other). Class 1 =
// karts dominated by nobody; class m = karts whose dominators all lie in
// classes 1..m-1 (the standard "onion peeling" of the Pareto/skyline front).
// This is equivalent to: layer(p) = 1 + max(layer(q) : q dominates p), 0 if
// none. Computed with a single sweep over points sorted by v desc, then w
// desc, using a Fenwick tree (max) over compressed w to query the best layer
// among already-seen points with w' >= current w (those either have v'>v,
// or v'==v with w' already inserted from a strictly larger w in this pass).
// Points with identical (v,w) are batched together (queried before any of
// the batch is inserted) so exact duplicates never dominate each other but
// always land in the same class, since they share the same dominator set.
// Ambiguity check (POJ board, message 93890/93768): for (5,3)(4,2)(3,4)(3,2)
// the confirmed answer is class1={(3,4),(5,3)}, class2={(4,2)}, class3={(3,2)},
// which matches this recurrence and was used as a regression test.
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

static int V[100005], W[100005];
static int order_idx[100005];
static int layerArr[100005];
static int tree_[100005];
static int treeSize;

bool cmpDesc(int a, int b) {
    if (V[a] != V[b]) return V[a] > V[b];
    return W[a] > W[b];
}

bool cmpAsc(int a, int b) {
    if (V[a] != V[b]) return V[a] < V[b];
    return W[a] < W[b];
}

void update(int idx, int val) {
    for (; idx <= treeSize; idx += idx & (-idx))
        if (tree_[idx] < val) tree_[idx] = val;
}

int query(int idx) {
    int res = 0;
    for (; idx > 0; idx -= idx & (-idx))
        if (tree_[idx] > res) res = tree_[idx];
    return res;
}

int main() {
    int n;
    scanf("%d", &n);
    vector<int> wvals(n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &V[i], &W[i]);
        wvals[i] = W[i];
    }
    vector<int> sortedW(wvals);
    sort(sortedW.begin(), sortedW.end());
    sortedW.erase(unique(sortedW.begin(), sortedW.end()), sortedW.end());
    int m = (int)sortedW.size();
    treeSize = m;
    for (int i = 1; i <= m; i++) tree_[i] = 0;

    for (int i = 0; i < n; i++) order_idx[i] = i;
    sort(order_idx, order_idx + n, cmpDesc);

    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && V[order_idx[j]] == V[order_idx[i]] && W[order_idx[j]] == W[order_idx[i]]) j++;
        int w = W[order_idx[i]];
        int rank = (int)(lower_bound(sortedW.begin(), sortedW.end(), w) - sortedW.begin()) + 1;
        int revrank = m - rank + 1;
        int q = query(revrank);
        int lay = q + 1;
        for (int k = i; k < j; k++) layerArr[order_idx[k]] = lay;
        update(revrank, lay);
        i = j;
    }

    int maxLayer = 0;
    for (int i = 0; i < n; i++) if (layerArr[i] > maxLayer) maxLayer = layerArr[i];

    vector<vector<int> > byLayer(maxLayer + 1);
    for (int i = 0; i < n; i++) byLayer[layerArr[i]].push_back(i);

    for (int L = 1; L <= maxLayer; L++) {
        vector<int>& idxs = byLayer[L];
        sort(idxs.begin(), idxs.end(), cmpAsc);
        printf("%d:", (int)idxs.size());
        for (size_t t = 0; t < idxs.size(); t++) {
            printf(" (%d,%d)", V[idxs[t]], W[idxs[t]]);
        }
        printf("\n");
    }
    return 0;
}
