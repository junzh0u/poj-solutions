// POJ 3532 - Resistance
// Model: claude-sonnet-5
// Nodal (modified nodal) analysis: build the conductance (weighted) Laplacian
// of the resistor network, inject 1A at node 1 and ground node N (V=0),
// solve the resulting (k-1)x(k-1) linear system by Gaussian elimination with
// partial pivoting; the equivalent resistance between 1 and N equals the
// resulting voltage at node 1.
//
// Board note: a zero-resistance wire is not explicitly ruled out by the
// statement (one poster only guessed it can't happen), so zero-resistance
// wires are defensively short-circuited via union-find before the matrix is
// built, rather than assumed away. The board also confirms there is always
// a route from node 1 to node N, so only the connected component reachable
// from node 1 (via positive-resistance edges, after the zero-resistance
// merges) is used when building the linear system -- this avoids a singular
// system from any other node components that have no current reference.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXN = 105;
int par[MAXN];
int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }
void unite(int a, int b) { a = find(a); b = find(b); if (a != b) par[a] = b; }

int N, M;
int ex_x[MAXN], ex_y[MAXN];
double ex_r[MAXN];

double cond[MAXN][MAXN];
bool visited[MAXN];
int idxOf[MAXN];
double G[MAXN][MAXN], rhs[MAXN];
double A[MAXN][MAXN], b[MAXN];

int main() {
    if (scanf("%d %d", &N, &M) != 2) return 0;
    for (int i = 1; i <= N; i++) par[i] = i;

    int ex = 0;
    for (int i = 0; i < M; i++) {
        int x, y; double r;
        scanf("%d %d %lf", &x, &y, &r);
        if (x == y) continue; // self loop, no effect
        if (r == 0.0) {
            unite(x, y);
        } else {
            ex_x[ex] = x; ex_y[ex] = y; ex_r[ex] = r; ex++;
        }
    }

    if (N == 1) { printf("0.00\n"); return 0; }

    int s = find(1), t = find(N);
    if (s == t) { printf("0.00\n"); return 0; }

    memset(cond, 0, sizeof(cond));
    for (int i = 0; i < ex; i++) {
        int rx = find(ex_x[i]), ry = find(ex_y[i]);
        if (rx == ry) continue;
        double c = 1.0 / ex_r[i];
        cond[rx][ry] += c;
        cond[ry][rx] += c;
    }

    memset(visited, 0, sizeof(visited));
    static int q[MAXN];
    int qh = 0, qt = 0;
    visited[s] = true; q[qt++] = s;
    while (qh < qt) {
        int u = q[qh++];
        for (int v = 1; v <= N; v++) {
            if (!visited[v] && cond[u][v] > 0) { visited[v] = true; q[qt++] = v; }
        }
    }

    // Per problem/board guarantee, t must be visited (there is always a route
    // from 1 to N). Defensive fallback if not: treat as open circuit is not
    // well-defined, but we won't hit this given the guarantee.

    memset(idxOf, -1, sizeof(idxOf));
    int sz = 0;
    for (int v = 1; v <= N; v++) if (visited[v]) idxOf[v] = sz++;

    memset(G, 0, sizeof(G));
    memset(rhs, 0, sizeof(rhs));
    for (int u = 1; u <= N; u++) {
        if (!visited[u]) continue;
        for (int v = u + 1; v <= N; v++) {
            if (!visited[v]) continue;
            double c = cond[u][v];
            if (c > 0) {
                int iu = idxOf[u], iv = idxOf[v];
                G[iu][iu] += c; G[iv][iv] += c;
                G[iu][iv] -= c; G[iv][iu] -= c;
            }
        }
    }

    int ls = idxOf[s], lt = idxOf[t];
    rhs[ls] += 1.0;

    static int map2[MAXN];
    int rsz = 0;
    for (int i = 0; i < sz; i++) {
        if (i == lt) map2[i] = -1;
        else map2[i] = rsz++;
    }

    for (int i = 0; i < sz; i++) {
        if (i == lt) continue;
        int ri = map2[i];
        b[ri] = rhs[i];
        for (int j = 0; j < sz; j++) {
            if (j == lt) continue;
            A[ri][map2[j]] = G[i][j];
        }
    }

    int n = rsz;
    for (int col = 0; col < n; col++) {
        int piv = col;
        for (int row = col + 1; row < n; row++)
            if (fabs(A[row][col]) > fabs(A[piv][col])) piv = row;
        if (piv != col) {
            for (int k = 0; k < n; k++) swap(A[col][k], A[piv][k]);
            swap(b[col], b[piv]);
        }
        for (int row = 0; row < n; row++) {
            if (row == col) continue;
            if (A[col][col] == 0) continue;
            double factor = A[row][col] / A[col][col];
            if (factor == 0) continue;
            for (int k = col; k < n; k++) A[row][k] -= factor * A[col][k];
            b[row] -= factor * b[col];
        }
    }

    int rls = map2[ls];
    double Vs = b[rls] / A[rls][rls];
    if (fabs(Vs) < 1e-9) Vs = 0;
    printf("%.2f\n", Vs);
    return 0;
}
