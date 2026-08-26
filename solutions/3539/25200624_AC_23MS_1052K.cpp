// POJ 3539 - Elevator
// Model: gpt-5.6-terra
// Dijkstra finds the smallest reachable displacement in every residue class
// modulo the smallest button. Every larger displacement in that class differs
// by a nonnegative number of presses of that smallest button.

#include <cstdio>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

typedef long long int64;
const int64 INF = (1LL << 62);

int main() {
    int64 h;
    int x[3];
    if (scanf("%lld", &h) != 1) return 0;
    if (scanf("%d%d%d", &x[0], &x[1], &x[2]) != 3) return 0;

    int base = x[0];
    int i;
    for (i = 1; i < 3; ++i)
        if (x[i] < base) base = x[i];

    vector<int64> dist(base, INF);
    priority_queue< pair<int64, int>, vector< pair<int64, int> >,
                    greater< pair<int64, int> > > pq;
    dist[0] = 0;
    pq.push(make_pair(0, 0));
    while (!pq.empty()) {
        int64 d = pq.top().first;
        int r = pq.top().second;
        pq.pop();
        if (d != dist[r]) continue;
        for (i = 0; i < 3; ++i) {
            int nr = (r + x[i]) % base;
            int64 nd = d + x[i];
            if (nd < dist[nr]) {
                dist[nr] = nd;
                pq.push(make_pair(nd, nr));
            }
        }
    }

    int64 limit = h - 1;
    int64 answer = 0;
    for (i = 0; i < base; ++i)
        if (dist[i] <= limit)
            answer += (limit - dist[i]) / base + 1;
    printf("%lld\n", answer);
    return 0;
}
