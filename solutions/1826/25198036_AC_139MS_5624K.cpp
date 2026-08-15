// POJ 1826 - The Best Farm
// Model: claude-sonnet-5
//
// Since every square's value is nonnegative, the maximum-value connected
// subset of any connected component of available squares is the entire
// component itself (dropping a square from a connected shape can only
// lower or keep the same the total, never raise it). So the answer per
// test case is simply the largest sum over the 4-connected components of
// the listed squares.
//
// Squares are given as arbitrary (x,y) pairs; a plain int holds them
// either way the range is read (statement says "signed 16-bit integer",
// an old board post from the setter says int32 -- the board never
// settled it, but int32 is a superset so a plain int is safe regardless).
// With N up to 200000 we cannot index a grid directly, so points are
// sorted lexicographically by (x,y) and each point's left ((x-1,y)) and
// down ((x,y-1)) neighbors are located with binary search; checking only
// those two directions still discovers every 4-adjacency edge exactly
// once (the complementary right/up edges are found from the other
// endpoint). A union-find (union by size, path compression) accumulates
// each component's value sum; the answer is the maximum sum seen.
//
// Max possible single-component sum is 200000 * 9999 < 2^31, so plain
// int suffices throughout -- no need for 64-bit arithmetic.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 200005;
int X[MAXN], Y[MAXN], V[MAXN];
int par[MAXN], sz[MAXN], sum_[MAXN];
int idx[MAXN];

int find(int a) {
    while (par[a] != a) {
        par[a] = par[par[a]];
        a = par[a];
    }
    return a;
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return;
    if (sz[a] < sz[b]) swap(a, b);
    par[b] = a;
    sz[a] += sz[b];
    sum_[a] += sum_[b];
}

bool cmp(int a, int b) {
    if (X[a] != X[b]) return X[a] < X[b];
    return Y[a] < Y[b];
}

inline int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) c = getchar();
    bool neg = false;
    if (c == '-') { neg = true; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return neg ? -x : x;
}

int main() {
    int n;
    while (true) {
        n = readInt();
        if (n == 0) break;
        for (int i = 0; i < n; i++) {
            X[i] = readInt();
            Y[i] = readInt();
            V[i] = readInt();
        }
        for (int i = 0; i < n; i++) idx[i] = i;
        sort(idx, idx + n, cmp);
        for (int p = 0; p < n; p++) {
            par[p] = p;
            sz[p] = 1;
            sum_[p] = V[idx[p]];
        }
        for (int p = 0; p < n; p++) {
            int cx = X[idx[p]], cy = Y[idx[p]];
            {
                int lo = 0, hi = n - 1, found = -1;
                int tx = cx - 1, ty = cy;
                while (lo <= hi) {
                    int mid = (lo + hi) / 2;
                    int mx = X[idx[mid]], my = Y[idx[mid]];
                    if (mx == tx && my == ty) { found = mid; break; }
                    if (mx < tx || (mx == tx && my < ty)) lo = mid + 1;
                    else hi = mid - 1;
                }
                if (found != -1) unite(p, found);
            }
            {
                int lo = 0, hi = n - 1, found = -1;
                int tx = cx, ty = cy - 1;
                while (lo <= hi) {
                    int mid = (lo + hi) / 2;
                    int mx = X[idx[mid]], my = Y[idx[mid]];
                    if (mx == tx && my == ty) { found = mid; break; }
                    if (mx < tx || (mx == tx && my < ty)) lo = mid + 1;
                    else hi = mid - 1;
                }
                if (found != -1) unite(p, found);
            }
        }
        int best = 0;
        for (int p = 0; p < n; p++) {
            int r = find(p);
            if (sum_[r] > best) best = sum_[r];
        }
        printf("%d\n", best);
    }
    return 0;
}
