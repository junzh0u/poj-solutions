// POJ 3456 - Frobenius
// Model: gpt-5.6-terra
// Dijkstra's algorithm finds, for every residue modulo the smallest generator,
// the least representable value in that residue.  Adding the smallest
// generator then represents every larger value with the same residue.
#include <cstdio>
#include <queue>
#include <vector>
#include <functional>

const int LIMIT = 1000000;
const int INF = 1000000000;

int main() {
    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int a[4];
        std::scanf("%d%d%d%d", &a[0], &a[1], &a[2], &a[3]);
        int smallest = a[0];
        for (int j = 1; j < 4; ++j)
            if (a[j] < smallest) smallest = a[j];
        std::vector<int> dist(smallest, INF);
        std::priority_queue< std::pair<int, int>,
                             std::vector< std::pair<int, int> >,
                             std::greater< std::pair<int, int> > > queue;
        dist[0] = 0;
        queue.push(std::make_pair(0, 0));
        while (!queue.empty()) {
            int value = queue.top().first;
            int residue = queue.top().second;
            queue.pop();
            if (value != dist[residue]) continue;
            for (int j = 0; j < 4; ++j) {
                int next = (residue + a[j]) % smallest;
                if (value + a[j] < dist[next]) {
                    dist[next] = value + a[j];
                    queue.push(std::make_pair(dist[next], next));
                }
            }
        }
        int missing = 0;
        int frobenius = -1;
        for (int residue = 0; residue < smallest; ++residue) {
            int before = (dist[residue] - residue + smallest - 1) / smallest;
            int total = (LIMIT - residue) / smallest + 1;
            if (before > total) before = total;
            missing += before;
            if (dist[residue] - smallest > frobenius)
                frobenius = dist[residue] - smallest;
        }
        std::printf("%d\n%d\n", missing, frobenius <= LIMIT ? frobenius : -1);
    }
    return 0;
}
