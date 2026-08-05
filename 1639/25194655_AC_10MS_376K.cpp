// POJ 1639 - Picnic Planning
// Model: gpt-5.6-terra
// Build a minimum tree inside each component formed without Park, attach every
// component by its cheapest Park edge, then make the best profitable exchanges.
// The parking-space count is an upper bound, so stop when no exchange saves miles.
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

const int MAXV = 21;
const int INF = 1000000000;

int weight[MAXV][MAXV];
bool chosen[MAXV][MAXV];

int main() {
    int connections;
    if (!(cin >> connections)) {
        return 0;
    }

    map<string, int> id;
    id["Park"] = 0;
    int vertices = 1;
    int i, j;
    for (i = 0; i < MAXV; ++i) {
        for (j = 0; j < MAXV; ++j) {
            weight[i][j] = (i == j) ? 0 : INF;
        }
    }

    for (i = 0; i < connections; ++i) {
        string a, b;
        int d;
        cin >> a >> b >> d;
        if (id.find(a) == id.end()) {
            id[a] = vertices++;
        }
        if (id.find(b) == id.end()) {
            id[b] = vertices++;
        }
        int u = id[a];
        int v = id[b];
        if (d < weight[u][v]) {
            weight[u][v] = weight[v][u] = d;
        }
    }
    int spaces;
    cin >> spaces;

    bool used[MAXV];
    memset(used, 0, sizeof(used));
    int total = 0;
    int parkDegree = 0;

    /* Build a minimum tree in each component that remains after Park is removed,
       then use that component's cheapest edge to Park. */
    for (int start = 1; start < vertices; ++start) {
        if (used[start]) {
            continue;
        }
        ++parkDegree;
        int dist[MAXV], parent[MAXV];
        bool inTree[MAXV], component[MAXV];
        for (i = 0; i < vertices; ++i) {
            dist[i] = INF;
            parent[i] = -1;
            inTree[i] = used[i];
            component[i] = false;
        }
        dist[start] = 0;
        while (true) {
            int u = -1;
            for (i = 1; i < vertices; ++i) {
                if (!inTree[i] && (u == -1 || dist[i] < dist[u])) {
                    u = i;
                }
            }
            if (u == -1 || dist[u] == INF) {
                break;
            }
            inTree[u] = true;
            used[u] = true;
            component[u] = true;
            if (parent[u] != -1) {
                int p = parent[u];
                chosen[u][p] = chosen[p][u] = true;
                total += weight[u][p];
            }
            for (i = 1; i < vertices; ++i) {
                if (!inTree[i] && weight[u][i] < dist[i]) {
                    dist[i] = weight[u][i];
                    parent[i] = u;
                }
            }
        }

        int nearest = -1;
        /* The vertices selected by this Prim run are exactly those newly marked
           used. Find the least costly Park edge that can attach its component. */
        for (i = 1; i < vertices; ++i) {
            if (component[i] && weight[0][i] < INF &&
                (nearest == -1 || weight[0][i] < weight[0][nearest])) {
                nearest = i;
            }
        }
        chosen[0][nearest] = chosen[nearest][0] = true;
        total += weight[0][nearest];
    }

    /* Each exchange adds one Park edge and removes the heaviest non-Park edge
       on its cycle, giving the best next degree of Park. */
    while (parkDegree < spaces) {
        int parent[MAXV], parentWeight[MAXV];
        for (i = 0; i < vertices; ++i) {
            parent[i] = -1;
            parentWeight[i] = 0;
        }
        queue<int> q;
        parent[0] = 0;
        q.push(0);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (i = 0; i < vertices; ++i) {
                if (chosen[u][i] && parent[i] == -1) {
                    parent[i] = u;
                    parentWeight[i] = weight[u][i];
                    q.push(i);
                }
            }
        }

        int addAt = -1, removeChild = -1;
        int bestSaving = 0;
        for (i = 1; i < vertices; ++i) {
            if (chosen[0][i] || weight[0][i] == INF) {
                continue;
            }
            int x = i;
            int maximum = -1;
            int maximumChild = -1;
            while (parent[x] != 0) {
                if (parentWeight[x] > maximum) {
                    maximum = parentWeight[x];
                    maximumChild = x;
                }
                x = parent[x];
            }
            if (maximumChild != -1 && maximum - weight[0][i] > bestSaving) {
                bestSaving = maximum - weight[0][i];
                addAt = i;
                removeChild = maximumChild;
            }
        }
        if (addAt == -1) {
            break;
        }
        int removeParent = parent[removeChild];
        chosen[0][addAt] = chosen[addAt][0] = true;
        chosen[removeChild][removeParent] = chosen[removeParent][removeChild] = false;
        total -= bestSaving;
        ++parkDegree;
    }

    cout << "Total miles driven: " << total << endl;
    return 0;
}
