// POJ 3037 - Skiing
// Model: claude-opus-5
//
// Speed telescopes: moving from height A to B multiplies speed by 2^(A-B), so
// after any path the speed at cell c is V * 2^(E[0][0] - E[c]) -- independent of
// the route taken. The time of a move is the reciprocal of the speed at its
// *source*, hence every edge leaving cell c costs 2^(E[c] - E[0][0]) / V, a
// weight that depends only on the source cell. Plain Dijkstra on the R*C grid.
//
// Exactness: factor out 1/V and rebase the exponents on the grid minimum m.
// Cell weight becomes the integer 1<<(E[c] - m), with E[c]-m in [0,50] since
// -25 <= E <= 25. A shortest path never costs more than the monotone
// right-then-down path, i.e. <= (R-1+C-1) * 2^50 <= 198 * 2^50 ~ 2.2e17, so all
// Dijkstra distances fit exactly in a 64-bit integer -- no floating point until
// the final scaling by 2^-(E[0][0]-m) (exact) and division by V.
//
// Input framing: the statement describes one data set, but several posted
// solutions loop; reading to EOF is safe either way. R==C==1 yields 0.00.
// Output uses %.2f, not %.2lf: the discuss board's "G++ WA / C++ AC" thread
// resolves to exactly that format-specifier difference.

#include <cstdio>
#include <queue>
#include <vector>
#include <utility>

using namespace std;

static const long long INF = 0x7fffffffffffffffLL;

int main() {
    int V, R, C;
    static int E[100][100];

    while (scanf("%d %d %d", &V, &R, &C) == 3) {
        int minE = 1000;
        for (int i = 0; i < R; ++i) {
            for (int j = 0; j < C; ++j) {
                if (scanf("%d", &E[i][j]) != 1) return 0;
                if (E[i][j] < minE) minE = E[i][j];
            }
        }

        /* CAP keeps 1<<e and 198*2^e inside a signed 64-bit integer even if the
           judge's elevations were to leave the stated [-25,25] range; with the
           stated range e is at most 50 and the cap never binds. */
        const int CAP = 55;
        int n = R * C;
        vector<long long> w(n);
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < C; ++j) {
                int e = E[i][j] - minE;
                if (e > CAP) e = CAP;
                w[i * C + j] = 1LL << e;
            }

        vector<long long> dist(n, INF);
        priority_queue<pair<long long, int>,
                       vector<pair<long long, int> >,
                       greater<pair<long long, int> > > pq;
        dist[0] = 0;
        pq.push(make_pair(0LL, 0));

        while (!pq.empty()) {
            pair<long long, int> cur = pq.top();
            pq.pop();
            long long d = cur.first;
            int u = cur.second;
            if (d > dist[u]) continue;
            if (u == n - 1) break;
            int i = u / C, j = u % C;
            long long nd = d + w[u];
            for (int k = 0; k < 4; ++k) {
                static const int dx[4] = {-1, 1, 0, 0};
                static const int dy[4] = {0, 0, -1, 1};
                int ni = i + dx[k], nj = j + dy[k];
                if (ni < 0 || ni >= R || nj < 0 || nj >= C) continue;
                int v = ni * C + nj;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    pq.push(make_pair(nd, v));
                }
            }
        }

        double ans = (double)dist[n - 1];
        int shift = E[0][0] - minE;          /* 0..50 under the stated bounds */
        if (shift > CAP) shift = CAP;
        for (int k = 0; k < shift; ++k) ans /= 2.0;
        ans /= (double)V;
        printf("%.2f\n", ans);
    }
    return 0;
}
