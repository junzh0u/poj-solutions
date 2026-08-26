// POJ 1682 - Clans on the Three Gorges
// Model: gpt-5.6-terra
// A noncrossing set of bridges meets each pair of banks in a monotone path.
// Dynamic programming finds the cost of each such path; enumerating the three
// boundary transitions joins those paths into every possible outer face.

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

const int INF = 100000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int tests;
    cin >> tests;
    while (tests--) {
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> x(n), y(m), z(k);
        int i, j;
        for (i = 0; i < n; ++i) cin >> x[i];
        for (i = 0; i < m; ++i) cin >> y[i];
        for (i = 0; i < k; ++i) cin >> z[i];

        vector< vector<int> > xz(n, vector<int>(k, INF));
        vector< vector<int> > xy(n, vector<int>(m, INF));
        vector< vector<int> > yz(m, vector<int>(k, INF));
        for (i = 0; i < n; ++i) {
            for (j = k - 1; j >= 0; --j) {
                int best = (i == 0 && j == k - 1) ? 0 : INF;
                if (i > 0) best = min(best, xz[i - 1][j]);
                if (j + 1 < k) best = min(best, xz[i][j + 1]);
                if (i > 0 && j + 1 < k) best = min(best, xz[i - 1][j + 1]);
                xz[i][j] = best + abs(x[i] - z[j]);
            }
        }
        for (i = n - 1; i >= 0; --i) {
            for (j = 0; j < m; ++j) {
                int best = (i == n - 1 && j == 0) ? 0 : INF;
                if (i + 1 < n) best = min(best, xy[i + 1][j]);
                if (j > 0) best = min(best, xy[i][j - 1]);
                if (i + 1 < n && j > 0) best = min(best, xy[i + 1][j - 1]);
                xy[i][j] = best + abs(x[i] - y[j]);
            }
        }
        for (i = m - 1; i >= 0; --i) {
            for (j = 0; j < k; ++j) {
                int best = (i == m - 1 && j == 0) ? 0 : INF;
                if (i + 1 < m) best = min(best, yz[i + 1][j]);
                if (j > 0) best = min(best, yz[i][j - 1]);
                if (i + 1 < m && j > 0) best = min(best, yz[i + 1][j - 1]);
                yz[i][j] = best + abs(y[i] - z[j]);
            }
        }

        int answer = INF;
        for (i = 0; i < n; ++i) {
            answer = min(answer, xz[i][0] + xy[i][m - 1]);
            if (i + 1 < n) answer = min(answer, xz[i][0] + xy[i + 1][m - 1]);
        }
        for (i = 0; i < k; ++i) {
            answer = min(answer, xz[n - 1][i] + yz[0][i]);
            if (i + 1 < k) answer = min(answer, xz[n - 1][i + 1] + yz[0][i]);
        }
        for (i = 0; i < m; ++i) {
            answer = min(answer, xy[0][i] + yz[i][k - 1]);
            if (i + 1 < m) answer = min(answer, xy[0][i + 1] + yz[i][k - 1]);
        }
        for (int a = 0; a < n; ++a)
            for (int aa = a; aa <= a + 1 && aa < n; ++aa)
                for (int b = 0; b < m; ++b)
                    for (int bb = b; bb <= b + 1 && bb < m; ++bb)
                        for (int c = 0; c < k; ++c)
                            for (int cc = c; cc <= c + 1 && cc < k; ++cc)
                                answer = min(answer, xz[a][cc] + xy[aa][b] + yz[bb][c]);
        cout << answer << '\n';
    }
    return 0;
}
