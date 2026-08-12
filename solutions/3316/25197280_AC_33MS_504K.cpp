// POJ 3316 - Snakes on a Plane
// Model: gpt-5.6-terra
// Treat each 4-neighbour connected component of 1 cells as a graph.  It is a
// snake exactly when it is a simple path: it is acyclic and no vertex branches.
// It is maximal only if no endpoint can grow into a zero with just that one
// neighbouring 1; the one-vertex path is included.

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main() {
    int n, m;
    const int dr[4] = {-1, 0, 1, 0};
    const int dc[4] = {0, 1, 0, -1};

    while (cin >> n >> m && (n || m)) {
        vector< vector<char> > grid(n, vector<char>(m));
        vector< vector<char> > seen(n, vector<char>(m, 0));
        vector< vector<int> > degree(n, vector<int>(m, 0));
        int answer = 0;

        for (int r = 0; r < n; ++r)
            for (int c = 0; c < m; ++c)
                cin >> grid[r][c];

        for (int r = 0; r < n; ++r)
            for (int c = 0; c < m; ++c)
                if (grid[r][c] == '1')
                    for (int d = 0; d < 4; ++d) {
                        int nr = r + dr[d];
                        int nc = c + dc[d];
                        if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '1')
                            ++degree[r][c];
                    }

        for (int sr = 0; sr < n; ++sr) {
            for (int sc = 0; sc < m; ++sc) {
                if (grid[sr][sc] != '1' || seen[sr][sc])
                    continue;

                queue<int> q;
                q.push(sr * m + sc);
                seen[sr][sc] = 1;
                int vertices = 0;
                int degreeSum = 0;
                int maximumDegree = 0;
                vector<int> ends;

                while (!q.empty()) {
                    int v = q.front();
                    q.pop();
                    int r = v / m;
                    int c = v % m;
                    ++vertices;
                    for (int d = 0; d < 4; ++d) {
                        int nr = r + dr[d];
                        int nc = c + dc[d];
                        if (nr < 0 || nr >= n || nc < 0 || nc >= m || grid[nr][nc] != '1')
                            continue;
                        if (!seen[nr][nc]) {
                            seen[nr][nc] = 1;
                            q.push(nr * m + nc);
                        }
                    }
                    degreeSum += degree[r][c];
                    if (degree[r][c] > maximumDegree)
                        maximumDegree = degree[r][c];
                    if (degree[r][c] <= 1)
                        ends.push_back(v);
                }

                int edges = degreeSum / 2;
                bool extendible = false;
                for (int i = 0; i < (int)ends.size(); ++i) {
                    int r = ends[i] / m;
                    int c = ends[i] % m;
                    for (int d = 0; d < 4; ++d) {
                        int nr = r + dr[d];
                        int nc = c + dc[d];
                        if (nr < 0 || nr >= n || nc < 0 || nc >= m || grid[nr][nc] == '1')
                            continue;
                        int neighbours = 0;
                        for (int e = 0; e < 4; ++e) {
                            int ar = nr + dr[e];
                            int ac = nc + dc[e];
                            if (ar >= 0 && ar < n && ac >= 0 && ac < m && grid[ar][ac] == '1')
                                ++neighbours;
                        }
                        if (neighbours == 1)
                            extendible = true;
                    }
                }
                if (maximumDegree <= 2 && edges == vertices - 1 && !extendible)
                    ++answer;
            }
        }
        cout << answer << '\n';
    }
    return 0;
}
