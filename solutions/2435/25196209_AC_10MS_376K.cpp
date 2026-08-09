// POJ 2435 - Navigating the City
// Model: claude-sonnet-5
// Approach: Grid of N x E intersections read from a 2N-1 x 2E-1 char map
// (even map-rows/cols are intersections, odd ones are the connecting road
// segments, '.' meaning no road). BFS (unit-weight edges, since every road
// segment is one block) from S to E gives a shortest path; the problem
// guarantees uniqueness so any shortest BFS path is THE answer. Reconstruct
// the path via parent pointers, recover the direction of each step, then
// merge consecutive equal directions into one instruction with a summed
// block count.
// No ambiguity found in the statement; the discuss board for this problem
// is empty.
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <string>
using namespace std;

int N, E; // N rows, E cols of intersections
char grid[65][85]; // raw map lines, up to 2*30-1=59 rows, 2*40-1=79 cols

int id(int r, int c) { return r * E + c; }

int main() {
    char firstLine[64];
    fgets(firstLine, sizeof(firstLine), stdin);
    sscanf(firstLine, "%d %d", &N, &E);
    int rows = 2 * N - 1;
    for (int i = 0; i < rows; ++i) {
        if (!fgets(grid[i], sizeof(grid[i]), stdin)) {
            grid[i][0] = '\0';
        }
        // strip trailing CR/LF
        int len = strlen(grid[i]);
        while (len > 0 && (grid[i][len-1] == '\n' || grid[i][len-1] == '\r')) {
            grid[i][--len] = '\0';
        }
    }

    int totalNodes = N * E;
    int sNode = -1, eNode = -1;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < E; ++c) {
            char ch = grid[2*r][2*c];
            if (ch == 'S') sNode = id(r, c);
            else if (ch == 'E') eNode = id(r, c);
        }
    }

    vector<int> dist(totalNodes, -1);
    vector<int> parent(totalNodes, -1);
    vector<char> pdir(totalNodes, 0); // direction taken from parent to reach this node

    queue<int> q;
    dist[sNode] = 0;
    q.push(sNode);
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        int r = cur / E, c = cur % E;
        // East: (r, c) -> (r, c+1), connector at grid[2r][2c+1]
        if (c + 1 < E && grid[2*r][2*c+1] != '.') {
            int nxt = id(r, c+1);
            if (dist[nxt] == -1) {
                dist[nxt] = dist[cur] + 1;
                parent[nxt] = cur;
                pdir[nxt] = 'E';
                q.push(nxt);
            }
        }
        // West: (r, c) -> (r, c-1), connector at grid[2r][2c-1]
        if (c - 1 >= 0 && grid[2*r][2*c-1] != '.') {
            int nxt = id(r, c-1);
            if (dist[nxt] == -1) {
                dist[nxt] = dist[cur] + 1;
                parent[nxt] = cur;
                pdir[nxt] = 'W';
                q.push(nxt);
            }
        }
        // South: (r, c) -> (r+1, c), connector at grid[2r+1][2c]
        if (r + 1 < N && grid[2*r+1][2*c] != '.') {
            int nxt = id(r+1, c);
            if (dist[nxt] == -1) {
                dist[nxt] = dist[cur] + 1;
                parent[nxt] = cur;
                pdir[nxt] = 'S';
                q.push(nxt);
            }
        }
        // North: (r, c) -> (r-1, c), connector at grid[2r-1][2c]
        if (r - 1 >= 0 && grid[2*r-1][2*c] != '.') {
            int nxt = id(r-1, c);
            if (dist[nxt] == -1) {
                dist[nxt] = dist[cur] + 1;
                parent[nxt] = cur;
                pdir[nxt] = 'N';
                q.push(nxt);
            }
        }
    }

    // reconstruct path directions from S to E
    vector<char> dirs;
    int cur = eNode;
    while (cur != sNode) {
        dirs.push_back(pdir[cur]);
        cur = parent[cur];
    }
    // dirs is in reverse order (E back to S); reverse it
    int n = dirs.size();
    for (int i = 0; i < n / 2; ++i) {
        char t = dirs[i]; dirs[i] = dirs[n-1-i]; dirs[n-1-i] = t;
    }

    // merge consecutive equal directions
    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && dirs[j] == dirs[i]) ++j;
        printf("%c %d\n", dirs[i], j - i);
        i = j;
    }

    return 0;
}
