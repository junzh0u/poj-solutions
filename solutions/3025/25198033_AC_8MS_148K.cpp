// POJ 3025 - Rings
// Model: claude-sonnet-5
//
// Union-find over the n rings (n <= 100 per test case). Two rings (circles)
// are glued together iff their boundaries cross at two points, i.e.
//   |r1-r2| < dist(center1,center2) < r1+r2   (strict, since the statement
// guarantees no two rings ever touch at exactly one point).
// The discuss board (message 62321) documents an extra case the naive
// crossing test misses: two rings that coincide exactly (same center, same
// radius) share their entire circumference, which is "overlap at more than
// one point" but fails the strict crossing inequalities (dist=0 on both
// sides). That case is glued too and is checked explicitly.
// Input is a sequence of test cases (n then n rings) terminated by a line
// containing -1; n itself may be 0, in which case there are no rings and
// the largest component has size 0.

#include <cstdio>
#include <cmath>
using namespace std;

const int MAXN = 105;
const double EPS = 1e-9;

int par[MAXN];
double X[MAXN], Y[MAXN], R[MAXN];

int find(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) par[a] = b;
}

bool glued(int i, int j) {
    double dx = X[i] - X[j];
    double dy = Y[i] - Y[j];
    double d = sqrt(dx * dx + dy * dy);
    double s = R[i] + R[j];
    double diff = fabs(R[i] - R[j]);
    if (d > diff + EPS && d < s - EPS) return true; // proper 2-point crossing
    if (fabs(dx) < EPS && fabs(dy) < EPS && fabs(R[i] - R[j]) < EPS) return true; // coincident rings
    return false;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n == -1) break;
        for (int i = 0; i < n; ++i) {
            scanf("%lf %lf %lf", &X[i], &Y[i], &R[i]);
            par[i] = i;
        }
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (glued(i, j)) unite(i, j);
            }
        }
        int cnt[MAXN];
        for (int i = 0; i < n; ++i) cnt[i] = 0;
        int best = 0;
        for (int i = 0; i < n; ++i) {
            int r = find(i);
            cnt[r]++;
            if (cnt[r] > best) best = cnt[r];
        }
        printf("The largest component contains %d rings\n", best);
    }
    return 0;
}
