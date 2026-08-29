// POJ 1522 - N-Credible Mazes
// Model: claude-sonnet-5
// Approach: each maze is a set of axis-aligned unit edges between n-dim
// lattice points (n<=10, each coordinate <10). Each point is keyed by its
// plain vector<int> of coordinates (std::map's default lexicographic
// comparison handles this directly), mapped to a union-find node id.
// Union the endpoints of every edge, then answer whether start and end
// land in the same component. The board confirms the question is
// strictly start<->end reachability, not whole-graph connectivity (a
// documented WA trap), and confirms only axis-aligned unit moves count
// as adjacency. Deliberately avoids long long/%lld: all coordinates are
// plain single-digit ints, so no 64-bit encoding is ever needed, which
// sidesteps any old-MSVC scanf("%lld") quirk under POJ's C++ (lang 4).
// Input is read token-by-token (not line-by-line): each maze's edge list
// is terminated by a lone -1 token, unambiguous from the 2n non-negative
// integers of a real edge. Union-find storage grows dynamically (no
// assumed cap on distinct points/edges in a maze).
#include <cstdio>
#include <map>
#include <vector>
using namespace std;

vector<int> par;
vector<int> rnk;

int find(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return;
    if (rnk[a] < rnk[b]) { int t = a; a = b; b = t; }
    par[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
}

int getId(map<vector<int>, int> &id, const vector<int> &key) {
    map<vector<int>, int>::iterator it = id.find(key);
    if (it != id.end()) return it->second;
    int newId = (int)par.size();
    id[key] = newId;
    par.push_back(newId);
    rnk.push_back(0);
    return newId;
}

int main() {
    int n;
    int caseNo = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        caseNo++;
        map<vector<int>, int> id;
        par.clear();
        rnk.clear();

        vector<int> startCoord(n), endCoord(n);
        for (int i = 0; i < n; i++) scanf("%d", &startCoord[i]);
        for (int i = 0; i < n; i++) scanf("%d", &endCoord[i]);

        int sId = getId(id, startCoord);
        int eId = getId(id, endCoord);

        int first;
        while (scanf("%d", &first) == 1 && first != -1) {
            vector<int> a(n), b(n);
            a[0] = first;
            for (int i = 1; i < n; i++) scanf("%d", &a[i]);
            for (int i = 0; i < n; i++) scanf("%d", &b[i]);

            int aId = getId(id, a);
            int bId = getId(id, b);
            unite(aId, bId);
        }

        if (find(sId) == find(eId))
            printf("Maze #%d can be travelled\n", caseNo);
        else
            printf("Maze #%d cannot be travelled\n", caseNo);
    }
    return 0;
}
