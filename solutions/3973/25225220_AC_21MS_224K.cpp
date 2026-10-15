// POJ 3973 - Highlights
// Model: gpt-6.1-sol
// Interviews and game plays are the two sides of a bipartite conflict graph.
// By Konig's theorem, maximum independent set size is N + M - matching.
// Game plays may share players: only interview/game-play pairs conflict.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
vector<int> edges[105];
int mate[105];
bool seen[105];
bool augment(int u) {
    for (int i = 0; i < (int)edges[u].size(); ++i) {
        int v = edges[u][i];
        if (seen[v]) continue;
        seen[v] = true;
        if (mate[v] == -1 || augment(mate[v])) {
            mate[v] = u;
            return true;
        }
    }
    return false;
}
int main() {
    int tests;
    if (scanf("%d", &tests) != 1) return 0;
    for (int tc = 1; tc <= tests; ++tc) {
        int n, m;
        scanf("%d%d", &n, &m);
        for (int i = 0; i < m; ++i) {
            edges[i].clear();
            int p;
            scanf("%d", &p);
            while (p--) {
                int v;
                scanf("%d", &v);
                edges[i].push_back(v - 1);
            }
        }
        memset(mate, -1, sizeof(mate));
        int matching = 0;
        for (int i = 0; i < m; ++i) {
            memset(seen, 0, sizeof(seen));
            if (augment(i)) ++matching;
        }
        printf("Case %d: %d\n", tc, n + m - matching);
    }
    return 0;
}
