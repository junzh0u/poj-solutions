// POJ 2671 - Jimmy's Bad Day
// Model: claude-sonnet-5
//
// Circle 0..N-1, point 0=start. Visited set is always a contiguous arc:
// {1..a} cw + {N-b..N-1} ccw, ending at whichever frontier moved last.
// A single direction switch is NOT always optimal (grab a cheap nearby
// point, sweep the other side, come finish the first side can beat it),
// so this is a real O(N^2) DP over (a,b,side), matching the discuss
// board's "O(n^2) dp" hint (a single-switch O(N) formula got WA first).
// For two ways to reach the same (a,b), total final cost differs by
// (their time difference)*W(a,b) [W=undelivered weight, same for both],
// so comparing cost+W*time picks the right predecessor.
// Verified vs both samples (240,92), a board case (N=9->47152), and a
// permutation brute force (also caught the single-switch formula's bug
// on a hand-built counterexample).

#include <cstdio>
#include <cstring>
using namespace std;

static const long long INF = (long long)4e18;
static const int MAXN = 305;

static long long m[MAXN], t[MAXN];
static long long C[MAXN], D[MAXN + 1];
static long long prefR[MAXN], prefL[MAXN];
static long long timeR[MAXN][MAXN], costR[MAXN][MAXN];
static long long timeL[MAXN][MAXN], costL[MAXN][MAXN];
static bool validR[MAXN][MAXN], validL[MAXN][MAXN];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++) scanf("%lld %lld", &m[i], &t[i]);

        C[0] = 0;
        for (int i = 1; i < n; i++) C[i] = C[i - 1] + t[i - 1];
        D[n] = 0;
        for (int i = n - 1; i >= 1; i--) D[i] = D[i + 1] + t[i];
        D[0] = 0;

        prefR[0] = 0;
        for (int a = 1; a < n; a++) prefR[a] = prefR[a - 1] + m[a];
        prefL[0] = 0;
        for (int b = 1; b < n; b++) prefL[b] = prefL[b - 1] + m[n - b];
        long long totalW = prefR[n - 1];

        for (int a = 0; a < n; a++) memset(validR[a], 0, sizeof(bool) * n);
        for (int a = 0; a < n; a++) memset(validL[a], 0, sizeof(bool) * n);

        timeR[0][0] = 0; costR[0][0] = 0; validR[0][0] = true;
        timeL[0][0] = 0; costL[0][0] = 0; validL[0][0] = true;

        for (int a = 0; a < n; a++) {
            for (int b = 0; b + a < n; b++) {
                if (a == 0 && b == 0) continue;
                long long W = totalW - prefR[a] - prefL[b];

                if (a >= 1) {
                    long long bestCost = INF, bestTime = -1;
                    bool have = false;
                    if (validR[a - 1][b]) {
                        long long dist = C[a] - C[a - 1];
                        long long nt = timeR[a - 1][b] + dist;
                        long long nc = costR[a - 1][b] + m[a] * nt;
                        if (!have || nc + W * nt < bestCost + W * bestTime) { bestCost = nc; bestTime = nt; have = true; }
                    }
                    if (validL[a - 1][b]) {
                        long long dist = D[n - b] + C[a];
                        long long nt = timeL[a - 1][b] + dist;
                        long long nc = costL[a - 1][b] + m[a] * nt;
                        if (!have || nc + W * nt < bestCost + W * bestTime) { bestCost = nc; bestTime = nt; have = true; }
                    }
                    if (have) { costR[a][b] = bestCost; timeR[a][b] = bestTime; validR[a][b] = true; }
                }
                if (b >= 1) {
                    long long bestCost = INF, bestTime = -1;
                    bool have = false;
                    if (validL[a][b - 1]) {
                        long long dist = D[n - b] - D[n - b + 1];
                        long long nt = timeL[a][b - 1] + dist;
                        long long nc = costL[a][b - 1] + m[n - b] * nt;
                        if (!have || nc + W * nt < bestCost + W * bestTime) { bestCost = nc; bestTime = nt; have = true; }
                    }
                    if (validR[a][b - 1]) {
                        long long dist = C[a] + D[n - b];
                        long long nt = timeR[a][b - 1] + dist;
                        long long nc = costR[a][b - 1] + m[n - b] * nt;
                        if (!have || nc + W * nt < bestCost + W * bestTime) { bestCost = nc; bestTime = nt; have = true; }
                    }
                    if (have) { costL[a][b] = bestCost; timeL[a][b] = bestTime; validL[a][b] = true; }
                }
            }
        }

        long long best = INF;
        for (int a = 0; a < n; a++) {
            int b = n - 1 - a;
            if (validR[a][b] && costR[a][b] < best) best = costR[a][b];
            if (validL[a][b] && costL[a][b] < best) best = costL[a][b];
        }
        if (best == INF) best = 0; // n == 1 case
        printf("%lld\n", best);
    }
    return 0;
}
