// POJ 2920 - Mine Map
// Model: gpt-5.6-terra
// Flood-fill from the centre through rooms whose detector finds no adjacent
// mine.  Visited boundary rooms are reachable but do not expand further.
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main() {
    int cases;
    cin >> cases;
    for (int scenario = 1; scenario <= cases; ++scenario) {
        int n, m;
        cin >> n >> m;
        vector< vector<int> > mine(n, vector<int>(n, 0));
        for (int i = 0; i < m; ++i) {
            int r, c;
            cin >> r >> c;
            mine[r - 1][c - 1] = 1;
        }

        vector< vector<int> > adjacent(n, vector<int>(n, 0));
        const int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                for (int d = 0; d < 8; ++d) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (nr >= 0 && nr < n && nc >= 0 && nc < n && mine[nr][nc])
                        adjacent[r][c] = 1;
                }
            }
        }

        vector< vector<int> > reached(n, vector<int>(n, 0));
        queue< pair<int, int> > q;
        int center = n / 2;
        reached[center][center] = 1;
        q.push(make_pair(center, center));
        while (!q.empty()) {
            int r = q.front().first, c = q.front().second;
            q.pop();
            if (adjacent[r][c])
                continue;
            for (int d = 0; d < 8; ++d) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr >= 0 && nr < n && nc >= 0 && nc < n &&
                    !mine[nr][nc] && !reached[nr][nc]) {
                    reached[nr][nc] = 1;
                    q.push(make_pair(nr, nc));
                }
            }
        }

        cout << "Scenario #" << scenario << ":\n";
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (mine[r][c])
                    cout << '*';
                else if (!reached[r][c])
                    cout << '?';
                else if (adjacent[r][c])
                    cout << '#';
                else
                    cout << '.';
            }
            cout << '\n';
        }
        cout << '\n';
    }
    return 0;
}
