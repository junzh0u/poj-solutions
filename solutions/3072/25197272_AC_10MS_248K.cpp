// POJ 3072 - Robot
// Model: gpt-5.6-terra
// State-space Dijkstra: a state records the current point and the direction
// of the last move.  The initial state has the prescribed direction to n.
// Turning is the smaller circular difference between the two bearings.

#include <cmath>
#include <cstdio>
#include <queue>
#include <vector>

using namespace std;

const double INF = 1e100;
const double PI = 3.1415926535897932384626433832795;

struct Point {
    int x, y;
};

struct State {
    double time;
    int at;
    int from;
    double bearing;
    State(double t, int a, int f, double b) : time(t), at(a), from(f), bearing(b) {}
};

struct Later {
    bool operator()(const State &a, const State &b) const {
        return a.time > b.time;
    }
};

double turnDegrees(double a, double b) {
    double d = fabs(a - b);
    if (d > PI) d = 2.0 * PI - d;
    return d * 180.0 / PI;
}

int main() {
    int R, n;
    while (scanf("%d%d", &R, &n) == 2 && R != -1) {
        vector<Point> p(n);
        int i, j;
        for (i = 0; i < n; ++i) scanf("%d%d", &p[i].x, &p[i].y);

        vector< vector<double> > len(n, vector<double>(n));
        vector< vector<double> > angle(n, vector<double>(n));
        for (i = 0; i < n; ++i) {
            for (j = 0; j < n; ++j) if (i != j) {
                double dx = p[j].x - p[i].x;
                double dy = p[j].y - p[i].y;
                len[i][j] = sqrt(dx * dx + dy * dy);
                angle[i][j] = atan2(dy, dx);
            }
        }

        vector< vector<double> > best(n, vector<double>(n, INF));
        priority_queue<State, vector<State>, Later> q;
        double initialBearing = angle[0][n - 1];
        q.push(State(0.0, 0, n, initialBearing));
        double answer = INF;

        while (!q.empty()) {
            State cur = q.top(); q.pop();
            if (cur.at == n - 1) {
                answer = cur.time;
                break;
            }
            if (cur.from < n && cur.time != best[cur.at][cur.from]) continue;
            for (j = 0; j < n; ++j) {
                if (j == cur.at || len[cur.at][j] > R + 1e-10) continue;
                double nt = cur.time + len[cur.at][j] + turnDegrees(cur.bearing, angle[cur.at][j]);
                if (nt + 1e-12 < best[j][cur.at]) {
                    best[j][cur.at] = nt;
                    q.push(State(nt, j, cur.at, angle[cur.at][j]));
                }
            }
        }
        if (answer == INF) puts("impossible");
        else printf("%.0f\n", answer);
    }
    return 0;
}
