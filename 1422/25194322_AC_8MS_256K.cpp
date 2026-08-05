// POJ 1422 - Air Raid
//
// The streets form a DAG. We must cover every intersection with the fewest
// vertex-disjoint directed paths (a paratrooper's route), where a lone
// intersection with no outgoing move is itself a valid one-vertex path.
// This is the classic minimum path cover on a DAG: split each vertex u into
// an "out" copy and an "in" copy, add a bipartite edge out(u) -> in(v) for
// every street u->v, and take a maximum bipartite matching M. Each matched
// edge chains two intersections into the same path, so the minimum number
// of paths needed is n - |M|. Kuhn's augmenting-path algorithm finds M in
// O(V*E), plenty fast for n <= 120.
//
// Statement ambiguities resolved: "more than one paratrooper visits no
// intersection" means the paths must be vertex-disjoint (a matching, not a
// cover with overlaps); "no restrictions about the starting intersection"
// together with the phrasing confirms a paratrooper that never moves (a
// single, untouched intersection) still counts as a valid assignment.
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int MAXN = 125;
vector<int> adj[MAXN];
int matchR[MAXN]; // matchR[v] = which left node is matched to right node v, -1 if none
bool visited[MAXN];
int n;

bool tryKuhn(int u) {
    for (size_t i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        if (!visited[v]) {
            visited[v] = true;
            if (matchR[v] == -1 || tryKuhn(matchR[v])) {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        scanf("%d", &n);
        int m;
        scanf("%d", &m);
        for (int i = 1; i <= n; ++i) adj[i].clear();
        for (int i = 0; i < m; ++i) {
            int s, e;
            scanf("%d %d", &s, &e);
            adj[s].push_back(e);
        }
        for (int i = 1; i <= n; ++i) matchR[i] = -1;
        int matching = 0;
        for (int u = 1; u <= n; ++u) {
            for (int i = 1; i <= n; ++i) visited[i] = false;
            if (tryKuhn(u)) ++matching;
        }
        printf("%d\n", n - matching);
    }
    return 0;
}
