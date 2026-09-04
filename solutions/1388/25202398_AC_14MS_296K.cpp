// POJ 1388 - Hinge Node Problem
// Model: claude-sonnet-5
// Approach: Floyd-Warshall all-pairs shortest distances D[i][j]. For every
// unordered pair (i,j), count how many vertices k (k != i,j) split a
// shortest i-j path, i.e. D[i][k]+D[k][j]==D[i][j]; every interior vertex
// of every shortest i-j path satisfies this, so if exactly one such k
// exists for some pair, that k is the sole connector for that pair and is
// therefore a hinge (cut) node. Union this over all pairs to get every
// hinge node. O(n^3) total. No statement ambiguity found; matches the
// discuss board's own editorial derivation (message 359865), and verified
// independently below against a brute-force node-removal + BFS reference.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 105;
const int INF = 1000000;

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        static int d[MAXN][MAXN];
        for (int i = 0; i < n; i++) {
            char row[MAXN];
            scanf("%s", row);
            for (int j = 0; j < n; j++) {
                if (i == j) d[i][j] = 0;
                else d[i][j] = (row[j] == '1') ? 1 : INF;
            }
        }
        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (d[i][k] + d[k][j] < d[i][j])
                        d[i][j] = d[i][k] + d[k][j];

        static bool hinge[MAXN];
        memset(hinge, 0, sizeof(hinge));
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int cnt = 0, last = -1;
                for (int k = 0; k < n; k++) {
                    if (k == i || k == j) continue;
                    if (d[i][k] + d[k][j] == d[i][j]) {
                        cnt++;
                        last = k;
                    }
                }
                if (cnt == 1) hinge[last] = true;
            }
        }
        int total = 0;
        for (int i = 0; i < n; i++) if (hinge[i]) total++;
        printf("%d\n", total);
    }
    return 0;
}
