// POJ 1838 - Banana
// Model: claude-sonnet-5
// Approach: each banana tree is a grid cell; 4-connected components form
// "regions". Tarzan may connect exactly K of the regions (connecting them is
// always possible via lianas regardless of position, so adjacency between
// regions is irrelevant) and wants to maximize the total number of trees in
// the chosen K regions. So: find connected component sizes (union-find over
// the given points, keyed by (x,y), using left/up neighbor lookups via a
// sorted array + binary search since coordinates go up to 10000 and Nr is
// only up to 16000), sort sizes descending, sum the largest K.
// No statement ambiguity found; verified against the sample (regions of
// size 2,4,3,1 -> top 3 sum to 9, matches expected output).
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

const int MAXN = 16005;
int X[MAXN], Y[MAXN];
int keyArr[MAXN]; // x*20001+y fits comfortably in a 32-bit int (<= ~2e8)
int order[MAXN];
int parent_[MAXN], sz[MAXN];

int find(int x) {
    while (parent_[x] != x) {
        parent_[x] = parent_[parent_[x]];
        x = parent_[x];
    }
    return x;
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return;
    if (sz[a] < sz[b]) swap(a, b);
    parent_[b] = a;
    sz[a] += sz[b];
}

bool cmpOrder(int a, int b) {
    return keyArr[a] < keyArr[b];
}

struct KeyLess {
    bool operator()(int idx, int k) const { return keyArr[idx] < k; }
};

int main() {
    int Nr, K;
    if (scanf("%d %d", &Nr, &K) != 2) return 0;
    for (int i = 0; i < Nr; i++) {
        scanf("%d %d", &X[i], &Y[i]);
        keyArr[i] = X[i] * 20001 + Y[i];
        parent_[i] = i;
        sz[i] = 1;
        order[i] = i;
    }
    sort(order, order + Nr, cmpOrder);

    for (int i = 0; i < Nr; i++) {
        int leftKey = X[i] * 20001 + (Y[i] - 1);
        int upKey = (X[i] - 1) * 20001 + Y[i];

        int* it = lower_bound(order, order + Nr, leftKey, KeyLess());
        if (it != order + Nr && keyArr[*it] == leftKey) unite(i, *it);

        it = lower_bound(order, order + Nr, upKey, KeyLess());
        if (it != order + Nr && keyArr[*it] == upKey) unite(i, *it);
    }

    vector<int> sizes;
    sizes.reserve(Nr);
    for (int i = 0; i < Nr; i++) {
        if (find(i) == i) sizes.push_back(sz[i]);
    }
    sort(sizes.rbegin(), sizes.rend());

    int sum = 0;
    for (int i = 0; i < K && i < (int)sizes.size(); i++) sum += sizes[i];

    printf("%d\n", sum);
    return 0;
}
