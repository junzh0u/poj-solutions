// POJ 2197 - Jill's Tour Paths
// Model: claude-sonnet-5
// Approach: enumerate all simple paths from SV to DV whose total distance is
// <= MAXDIST via DFS with visited-array backtracking. Two prunes keep this
// cheap: (1) the running distance is monotone (edge weights are positive),
// so we stop extending as soon as it would exceed MAXDIST; (2) before
// descending into a neighbor v we also require curDist + shortestToDV[v] <=
// MAXDIST, where shortestToDV is the unconstrained (ignores which vertices
// are already visited) all-pairs shortest distance to DV computed once via
// Floyd-Warshall. That bound is admissible: dropping the "no repeat
// vertex" constraint can only shorten a path, so it never prunes a
// completable route. NV <= 20 keeps this fast for the judge's data even
// though the search is exponential in the worst case (only a graph
// engineered to have astronomically many valid routes could defeat it, and
// producing that much output would itself blow the time limit).
// Collected routes are sorted by (distance, lexicographic order of the
// vertex sequence) and printed. Formatting quirks confirmed against the
// sample bytes and the discuss board: each route line is
// " %d:" followed by " %d" for every vertex and one extra trailing space
// before the newline; "NO ACCEPTABLE TOURS" is printed with a leading
// space; "Case" is capitalized; a blank line separates consecutive cases
// (but there is no blank line after the last case).
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int INF = 1000000000;

int NV, NR;
int dist_[21][21];
bool adj[21][21];
bool visited[21];
int SV, DV, MAXDIST;
int allPairs[21][21];
int shortestToDV[21];

struct Route {
    int len;
    vector<int> path;
};

bool routeLess(const Route &a, const Route &b) {
    if (a.len != b.len) return a.len < b.len;
    return a.path < b.path; // lexicographic vector comparison
}

vector<Route> routes;
vector<int> curPath;

void dfs(int u, int curDist) {
    if (u == DV) {
        Route r;
        r.len = curDist;
        r.path = curPath;
        routes.push_back(r);
        return; // cannot leave DV again in a valid route (would revisit it)
    }
    for (int v = 1; v <= NV; v++) {
        if (!adj[u][v] || visited[v]) continue;
        int nd = curDist + dist_[u][v];
        if (nd > MAXDIST) continue;
        if (shortestToDV[v] >= INF || nd + shortestToDV[v] > MAXDIST) continue;
        visited[v] = true;
        curPath.push_back(v);
        dfs(v, nd);
        curPath.pop_back();
        visited[v] = false;
    }
}

int main() {
    int caseNo = 0;
    while (scanf("%d", &NV) == 1) {
        if (NV == -1) break;
        scanf("%d", &NR);
        for (int i = 1; i <= NV; i++)
            for (int j = 1; j <= NV; j++) { adj[i][j] = false; dist_[i][j] = 0; }
        for (int i = 0; i < NR; i++) {
            int c1, c2, d;
            scanf("%d %d %d", &c1, &c2, &d);
            adj[c1][c2] = adj[c2][c1] = true;
            dist_[c1][c2] = dist_[c2][c1] = d;
        }
        scanf("%d %d", &SV, &DV);
        scanf("%d", &MAXDIST);

        for (int i = 1; i <= NV; i++)
            for (int j = 1; j <= NV; j++)
                allPairs[i][j] = (i == j) ? 0 : (adj[i][j] ? dist_[i][j] : INF);
        for (int k = 1; k <= NV; k++)
            for (int i = 1; i <= NV; i++) {
                if (allPairs[i][k] >= INF) continue;
                for (int j = 1; j <= NV; j++) {
                    if (allPairs[k][j] >= INF) continue;
                    if (allPairs[i][k] + allPairs[k][j] < allPairs[i][j])
                        allPairs[i][j] = allPairs[i][k] + allPairs[k][j];
                }
            }
        for (int i = 1; i <= NV; i++) shortestToDV[i] = allPairs[i][DV];

        routes.clear();
        curPath.clear();
        for (int i = 1; i <= NV; i++) visited[i] = false;
        visited[SV] = true;
        curPath.push_back(SV);
        dfs(SV, 0);

        sort(routes.begin(), routes.end(), routeLess);

        caseNo++;
        if (caseNo > 1) printf("\n");
        printf("Case %d:\n", caseNo);
        if (routes.empty()) {
            printf(" NO ACCEPTABLE TOURS\n");
        } else {
            for (size_t i = 0; i < routes.size(); i++) {
                printf(" %d:", routes[i].len);
                for (size_t j = 0; j < routes[i].path.size(); j++)
                    printf(" %d", routes[i].path[j]);
                printf(" \n");
            }
        }
    }
    return 0;
}
