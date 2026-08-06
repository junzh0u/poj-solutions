// POJ 1466 - Girls and Boys
//
// The "romantically involved" relation only ever holds between a girl and
// a boy, so the compatibility graph is bipartite even though the input
// never says which side a student is on. Choosing a maximum set with no
// two students "involved" is therefore Maximum Independent Set on a
// bipartite graph, which by Konig's theorem equals n - (maximum matching).
//
// Since the two sides are unknown, we can't run a textbook one-sided
// Kuhn's algorithm. Instead we run the augmenting-path search from every
// vertex (0..n-1): bipartiteness guarantees this still behaves correctly
// (matchTo[x] always holds the partner from the side opposite x), and
// each true matched edge gets discovered once from each endpoint, so the
// count of successful searches is exactly twice the matching size.
//
// The statement doesn't specify whether a relation is listed from one
// side, the other, or both (the samples list it from both sides), so
// each edge read is added to both adjacency lists defensively; duplicate
// edges are harmless for Kuhn's algorithm.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

int n;
vector<int> adj[505];
int matchTo[505];
bool visited[505];

bool tryKuhn(int u) {
    for (size_t i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        if (visited[v]) continue;
        visited[v] = true;
        if (matchTo[v] == -1 || tryKuhn(matchTo[v])) {
            matchTo[v] = u;
            return true;
        }
    }
    return false;
}

int main() {
    while (scanf("%d", &n) == 1) {
        for (int i = 0; i < n; ++i) adj[i].clear();
        for (int i = 0; i < n; ++i) {
            int id, cnt;
            // format: id:(cnt) v1 v2 ...
            scanf("%d", &id);
            // consume ':' '(' cnt ')'
            int c;
            do { c = getchar(); } while (c != '(');
            scanf("%d", &cnt);
            do { c = getchar(); } while (c != ')');
            for (int j = 0; j < cnt; ++j) {
                int v;
                scanf("%d", &v);
                // The relation is symmetric but the input may list it
                // one-sidedly (only from id's line, or from v's line, or
                // both) -- add both directions defensively; duplicate
                // edges are harmless for Kuhn's algorithm.
                adj[id].push_back(v);
                adj[v].push_back(id);
            }
        }
        memset(matchTo, -1, sizeof(matchTo));
        int matched = 0;
        for (int u = 0; u < n; ++u) {
            memset(visited, 0, sizeof(visited));
            if (tryKuhn(u)) matched++;
        }
        // matched counts each matched edge twice (once from each side's
        // adjacency traversal), since the input lists the relation
        // one-sidedly but we run the augmenting-path search from every
        // node using the combined (symmetric-in-effect) adjacency.
        printf("%d\n", n - matched / 2);
    }
    return 0;
}
