// POJ 3897 - Maze Stretching
// Model: claude-sonnet-5
// Approach: The shortest-path cost as a function of the vertical stretch X
// (horizontal step cost 1, vertical step cost X) is f(X) = min over S-E paths
// of (h_p + v_p * X), a minimum of nonnegative-slope lines in X, hence
// piecewise-linear, concave and non-decreasing. Because the statement
// guarantees there is no all-horizontal S-E path, every path has v_p >= 1,
// so every segment of f has slope >= 1 and f is strictly increasing -> the
// equation f(X) = L has a unique root, matching "the result is always
// unique". Binary search X in [0,10] (P in [0,1000] per the stated bound),
// evaluating f(X) with a Dijkstra over the grid (4-directional moves,
// horizontal edges weight 1, vertical edges weight X) for each probe.
// Ambiguity: none found on the board beyond confirming binary search + a
// shortest-path search is the intended method (a board post titled
// "binary search + A*, accepted first try").
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;

static int R, C;
static vector<string> grid;
static int sr, sc, er, ec;
static double dist[10005];
static char done_[10005];

double dijkstra(double X) {
    int n = R * C;
    for (int i = 0; i < n; ++i) { dist[i] = 1e18; done_[i] = 0; }
    priority_queue<pair<double,int>, vector<pair<double,int> >, greater<pair<double,int> > > pq;
    int startIdx = sr * C + sc;
    dist[startIdx] = 0.0;
    pq.push(make_pair(0.0, startIdx));
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    while (!pq.empty()) {
        pair<double,int> top = pq.top();
        pq.pop();
        double d = top.first;
        int u = top.second;
        if (done_[u]) continue;
        done_[u] = 1;
        int ur = u / C, uc = u % C;
        if (ur == er && uc == ec) break;
        for (int dir = 0; dir < 4; ++dir) {
            int nr = ur + dr[dir];
            int nc = uc + dc[dir];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            if ((int)grid[nr].size() <= nc) continue;
            if (grid[nr][nc] == '#') continue;
            double w = (dir < 2) ? X : 1.0;
            int v = nr * C + nc;
            if (!done_[v] && d + w < dist[v]) {
                dist[v] = d + w;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return dist[er * C + ec];
}

int main() {
    ios::sync_with_stdio(false);
    int T;
    if (!(cin >> T)) return 0;
    for (int tc = 1; tc <= T; ++tc) {
        double L;
        int N;
        cin >> L >> N;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        grid.clear();
        R = N;
        C = 0;
        sr = sc = er = ec = -1;
        for (int i = 0; i < N; ++i) {
            string line;
            getline(cin, line);
            while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
                line.erase(line.size()-1);
            grid.push_back(line);
            if ((int)line.size() > C) C = (int)line.size();
            for (size_t j = 0; j < line.size(); ++j) {
                if (line[j] == 'S') { sr = i; sc = (int)j; }
                else if (line[j] == 'E') { er = i; ec = (int)j; }
            }
        }
        double lo = 0.0, hi = 10.0;
        for (int iter = 0; iter < 40; ++iter) {
            double mid = (lo + hi) / 2.0;
            double f = dijkstra(mid);
            if (f < L) lo = mid; else hi = mid;
        }
        double X = (lo + hi) / 2.0;
        double P = X * 100.0;
        printf("Case #%d: %.3f%%\n", tc, P);
    }
    return 0;
}
