// POJ 1620 - Phone Home
// Model: claude-sonnet-5
// Approach: "near" towers (distance <= 20) must get frequencies differing by
// at least 2; non-near towers have no constraint. Mapping k proper-coloring
// classes to frequencies 1,3,5,...,2k-1 shows the minimum number of distinct
// frequencies needed equals the chromatic number of the "near" graph, since
// adjacent classes automatically differ by >=2 and any valid frequency
// assignment induces a proper coloring with the same number of classes
// (adjacent towers must simply be in different classes). So we just need the
// chromatic number of a graph with n<=12 vertices and max degree 4 -- found
// by trying k=1,2,... and backtracking-searching for a proper k-coloring,
// ordering vertices by descending degree and using symmetry breaking (a
// vertex may only use a color already used by an earlier vertex, or the
// next fresh color) to prune heavily. The graph need not be connected (the
// board raised this as a WA cause), so no assumption of connectivity is
// made.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;

int n;
double x[12], y[12];
bool adj[12][12];
int order_[12];
int color[12];
int maxColorUsed;

bool tryColor(int idx, int k) {
    if (idx == n) return true;
    int v = order_[idx];
    int upper = min(k - 1, maxColorUsed + 1);
    for (int c = 0; c <= upper; ++c) {
        bool ok = true;
        for (int u = 0; u < n; ++u) {
            if (adj[v][u] && color[u] == c) { ok = false; break; }
        }
        if (!ok) continue;
        color[v] = c;
        int prevMax = maxColorUsed;
        if (c > maxColorUsed) maxColorUsed = c;
        if (tryColor(idx + 1, k)) return true;
        maxColorUsed = prevMax;
        color[v] = -1;
    }
    return false;
}

int chromaticNumber() {
    // order vertices by descending degree for better pruning
    int deg[12];
    for (int i = 0; i < n; ++i) {
        deg[i] = 0;
        for (int j = 0; j < n; ++j) if (adj[i][j]) deg[i]++;
    }
    for (int i = 0; i < n; ++i) order_[i] = i;
    // simple selection sort by degree desc
    for (int i = 0; i < n; ++i) {
        int best = i;
        for (int j = i + 1; j < n; ++j) if (deg[order_[j]] > deg[order_[best]]) best = j;
        swap(order_[i], order_[best]);
    }
    for (int k = 1; k <= n; ++k) {
        for (int i = 0; i < n; ++i) color[i] = -1;
        maxColorUsed = -1;
        if (tryColor(0, k)) return k;
    }
    return n;
}

int main() {
    int caseNo = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        ++caseNo;
        for (int i = 0; i < n; ++i) scanf("%lf %lf", &x[i], &y[i]);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) adj[i][j] = false;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double dx = x[i] - x[j], dy = y[i] - y[j];
                double d2 = dx * dx + dy * dy;
                if (d2 <= 400.0 + 1e-9) {
                    adj[i][j] = adj[j][i] = true;
                }
            }
        }
        int f = chromaticNumber();
        printf("The towers in case %d can be covered in %d frequencies.\n", caseNo, f);
    }
    return 0;
}
