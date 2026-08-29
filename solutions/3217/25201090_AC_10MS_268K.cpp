// POJ 3217 - Eugenics
// Model: claude-sonnet-5
//
// Approach: parse the family forest (each line: node id, list of direct
// children, then a trailing 0/-1 gender code for that node). Build a
// parent adjacency list (a child may have up to two parents). For the two
// queried persons p and q, BFS upward through parent links to get, for
// every ancestor (including the person itself at distance 0), the minimum
// number of generations needed to reach it.
//
// Relationship rule: p and q are "close" (forbidden) exactly when there is
// some node c (possibly p or q itself) that is an ancestor of both p and q
// within at most 2 generations from EACH of them. Taking c = p (distance 0
// from p) or c = q covers the lineal cases -- parent/grandparent and
// child/grandchild, i.e. the other side must be within 2 generations, NOT
// unlimited generations. Taking c to be a third, strictly-higher ancestor
// covers the collateral cases: siblings (1,1), aunt/uncle-niece/nephew
// (1,2)/(2,1), first cousins (2,2).
//
// This single symmetric rule was checked against the statement's own
// examples (10-24 marriage, 24-26 close as first cousins) and against
// several discuss-board Q&A run on this exact sample tree: 24-29 and
// 24-15 share ancestors only at distance (3,3)/(3,1) and are answered
// "can marry"; 22-32 and 14-32 share no ancestor at all and are answered
// "acceptable" (ignoring gender, which then makes them "same" since both
// are male). Crucially, board message 74776 raises exactly the
// generalization risk of assuming lineal relatives are forbidden at ANY
// distance -- a granddaughter and her great-grandfather (3 generations
// apart) -- and the accepted resolution is that such a pair CAN marry,
// confirming the lineal case is capped at 2 generations exactly like the
// collateral case, not unlimited as the quoted law's general "lineal
// relatives by blood" sentence would otherwise suggest.
//
// If both persons have the same gender, the answer is "same" regardless
// of closeness (the statement gives "same" top priority over "close").
//
// The input is a single test case read to EOF (confirmed on the board).
#include <cstdio>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

int gender[105];
bool hasGender[105];
vector<int> parents[105];

void bfsAncestors(int src, int dist[105]) {
    for (int i = 0; i < 105; i++) dist[i] = -1;
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (size_t i = 0; i < parents[u].size(); i++) {
            int v = parents[u][i];
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    int p, q;
    if (!(cin >> p >> q)) return 0;

    string line;
    getline(cin, line); // consume rest of first line
    while (getline(cin, line)) {
        istringstream iss(line);
        vector<int> nums;
        int x;
        while (iss >> x) nums.push_back(x);
        if (nums.empty()) continue;
        int node = nums[0];
        int g = nums.back();
        hasGender[node] = true;
        gender[node] = g;
        for (size_t i = 1; i + 1 < nums.size(); i++) {
            int child = nums[i];
            parents[child].push_back(node);
        }
    }

    int distP[105], distQ[105];
    bfsAncestors(p, distP);
    bfsAncestors(q, distQ);

    bool close = false;
    for (int c = 1; c <= 100; c++) {
        if (distP[c] != -1 && distQ[c] != -1 && distP[c] <= 2 && distQ[c] <= 2) {
            close = true;
            break;
        }
    }

    if (hasGender[p] && hasGender[q] && gender[p] == gender[q]) {
        printf("same\n");
    } else if (close) {
        printf("close\n");
    } else {
        printf("marriage\n");
    }
    return 0;
}
