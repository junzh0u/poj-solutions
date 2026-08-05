// POJ 2502 - Subway
//
// Shortest home-to-school time where walking is possible between any two
// points and the subway only between stations that are CONSECUTIVE on the
// same line. That adjacency is the whole problem: joining every station on
// a line to every other one lets the rider teleport past intermediate stops
// and gives a wrong, too-small answer.
//
// Node 0 is home and node 1 the school; stations are appended as they are
// read. A line is a run of coordinates terminated by the -1 -1 sentinel,
// which resets the "previous station" marker, so a subway edge is added
// only between a station and the one read immediately before it within the
// same run. Walking edges are then laid over every pair, taking the minimum
// so an existing subway edge survives where it is faster.
//
// Speeds are converted once to metres per minute, since coordinates are in
// metres and the speeds are given in km/h. With at most ~200 nodes a dense
// O(n^2) Dijkstra is ample, and the answer is rounded to the nearest minute.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXN = 210;
double X[MAXN], Y[MAXN];
double dist_[MAXN][MAXN];
bool visited[MAXN];
double d[MAXN];
int n;

const double WALK = 10000.0 / 60.0;   // metres per minute
const double SUBWAY = 40000.0 / 60.0; // metres per minute

double euclid(int i, int j) {
    double dx = X[i] - X[j];
    double dy = Y[i] - Y[j];
    return sqrt(dx * dx + dy * dy);
}

int main() {
    // node 0 = home, node 1 = school
    if (scanf("%lf %lf", &X[0], &Y[0]) != 2) return 0;
    scanf("%lf %lf", &X[1], &Y[1]);
    n = 2;

    for (int i = 0; i < MAXN; i++)
        for (int j = 0; j < MAXN; j++)
            dist_[i][j] = 1e18;

    double x, y;
    int prevIdx = -1;
    while (scanf("%lf %lf", &x, &y) == 2) {
        if (x == -1 && y == -1) {
            prevIdx = -1;
            continue;
        }
        int cur = n;
        X[n] = x;
        Y[n] = y;
        n++;
        if (prevIdx != -1) {
            double t = euclid(prevIdx, cur) / SUBWAY;
            if (t < dist_[prevIdx][cur]) {
                dist_[prevIdx][cur] = t;
                dist_[cur][prevIdx] = t;
            }
        }
        prevIdx = cur;
    }

    // walking edges between every pair
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double t = euclid(i, j) / WALK;
            if (t < dist_[i][j]) {
                dist_[i][j] = t;
                dist_[j][i] = t;
            }
        }
    }

    // Dijkstra from node 0
    for (int i = 0; i < n; i++) {
        d[i] = 1e18;
        visited[i] = false;
    }
    d[0] = 0;
    for (int iter = 0; iter < n; iter++) {
        int u = -1;
        double best = 1e18;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && d[i] < best) {
                best = d[i];
                u = i;
            }
        }
        if (u == -1) break;
        visited[u] = true;
        for (int v = 0; v < n; v++) {
            if (dist_[u][v] < 1e17) {
                if (d[u] + dist_[u][v] < d[v]) {
                    d[v] = d[u] + dist_[u][v];
                }
            }
        }
    }

    int ans = (int)floor(d[1] + 0.5);
    printf("%d\n", ans);
    return 0;
}
