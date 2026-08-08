// POJ 3439 - Server Relocation
// Model: gpt-5.6-terra
// Breadth-first search over outlets.  A handoff is possible exactly when the
// two cord-radius disks intersect, i.e. when outlet distance is at most L1+L2.
// Squared distances avoid square roots and keep the comparison exact enough
// for the decimal input precision.
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

struct Point {
    int x, y;
};

int main() {
    ios::sync_with_stdio(false);
    int cases;
    if (!(cin >> cases)) return 0;
    while (cases--) {
        int n, start, target;
        double l1, l2;
        cin >> n >> start >> target >> l1 >> l2;
        --start;
        --target;
        vector<Point> p(n);
        int i;
        for (i = 0; i < n; ++i) cin >> p[i].x >> p[i].y;

        double reach = l1 + l2;
        double limit = reach * reach + 1e-7;
        vector<int> dist(n, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == target) break;
            for (i = 0; i < n; ++i) {
                if (dist[i] != -1) continue;
                double dx = (double)p[u].x - p[i].x;
                double dy = (double)p[u].y - p[i].y;
                if (dx * dx + dy * dy <= limit) {
                    dist[i] = dist[u] + 1;
                    q.push(i);
                }
            }
        }
        if (dist[target] == -1) cout << "Impossible\n";
        else cout << dist[target] << '\n';
    }
    return 0;
}
