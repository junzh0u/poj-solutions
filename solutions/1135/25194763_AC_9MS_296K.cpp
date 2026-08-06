// POJ 1135 - Domino Effect
// Model: gpt-5.6-terra
// Dijkstra gives the time each key domino first falls.  The last point is
// either a key domino or the meeting point of waves entering an edge from both ends.
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <utility>
#include <functional>
using namespace std;

struct Edge {
    int to;
    int length;
    Edge(int t, int l) : to(t), length(l) {}
};

struct Row {
    int a;
    int b;
    int length;
    Row(int x, int y, int l) : a(x), b(y), length(l) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    int systemNumber = 1;
    const double INF = 1e100;
    while (cin >> n >> m && (n != 0 || m != 0)) {
        vector< vector<Edge> > graph(n + 1);
        vector<Row> rows;
        int i;
        for (i = 0; i < m; ++i) {
            int a, b, length;
            cin >> a >> b >> length;
            graph[a].push_back(Edge(b, length));
            graph[b].push_back(Edge(a, length));
            rows.push_back(Row(a, b, length));
        }

        vector<double> distance(n + 1, INF);
        priority_queue< pair<double, int>, vector< pair<double, int> >,
                        greater< pair<double, int> > > pending;
        distance[1] = 0.0;
        pending.push(make_pair(0.0, 1));
        while (!pending.empty()) {
            double current = pending.top().first;
            int u = pending.top().second;
            pending.pop();
            if (current != distance[u]) continue;
            for (i = 0; i < (int)graph[u].size(); ++i) {
                int v = graph[u][i].to;
                double next = current + graph[u][i].length;
                if (next < distance[v]) {
                    distance[v] = next;
                    pending.push(make_pair(next, v));
                }
            }
        }

        double latest = -1.0;
        int lastKey = 1;
        int betweenA = -1;
        int betweenB = -1;
        for (i = 1; i <= n; ++i) {
            if (distance[i] > latest) {
                latest = distance[i];
                lastKey = i;
                betweenA = -1;
                betweenB = -1;
            }
        }
        for (i = 0; i < (int)rows.size(); ++i) {
            int a = rows[i].a;
            int b = rows[i].b;
            int length = rows[i].length;
            if (distance[a] + length > distance[b] && distance[b] + length > distance[a]) {
                double meeting = (distance[a] + distance[b] + length) / 2.0;
                if (meeting > latest) {
                    latest = meeting;
                    betweenA = a < b ? a : b;
                    betweenB = a < b ? b : a;
                }
            }
        }

        cout << "System #" << systemNumber++ << '\n';
        cout << fixed << setprecision(1) << "The last domino falls after " << latest << " seconds, ";
        if (betweenA == -1)
            cout << "at key domino " << lastKey << ".\n\n";
        else
            cout << "between key dominoes " << betweenA << " and " << betweenB << ".\n\n";
    }
    return 0;
}
