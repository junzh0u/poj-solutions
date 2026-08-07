// POJ 3084 - Panic Room
// Model: claude-sonnet-5
//
// Each door has a control panel located in exactly one of its two rooms.
// The occupant of the panel room can unlock/relock the door at will and so
// can always walk from the panel room into the other room, regardless of
// what we lock. So that direction is uncuttable (treated as infinite
// capacity). The reverse direction -- from the non-panel room into the
// panel room -- is blocked for good once we lock that door, since the
// occupant there has no access to the panel. That direction is the one
// lock we can spend, capacity 1 per physical door (accumulated on repeated
// doors between the same ordered pair, per the discuss board's warning
// about multi-edges).
//
// This turns the problem into a multi-source min-cut: source edges from a
// super source to every intruder room (infinite capacity), sink is the
// panic room, and we want the minimum number of "lockable" edges to cut so
// no residual path reaches the sink. Max-flow = min-cut (Edmond-Karp on an
// up-to-21-node graph is trivial in time). If the max flow reaches the
// "infinite" magnitude, some path uses only uncuttable edges and no finite
// lock set can secure the room -> PANIC ROOM BREACH.
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

const int MAXN = 22;
const int INF = 100000;

int cap_[MAXN][MAXN];
int n_nodes;
int S, T;

bool bfs(int parent[]) {
    bool visited[MAXN];
    memset(visited, 0, sizeof(visited));
    queue<int> q;
    q.push(S);
    visited[S] = true;
    parent[S] = -1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < n_nodes; v++) {
            if (!visited[v] && cap_[u][v] > 0) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    return visited[T];
}

int maxflow() {
    int parent[MAXN];
    int flow = 0;
    while (bfs(parent)) {
        int pathFlow = 1 << 30;
        for (int v = T; v != S; v = parent[v]) {
            int u = parent[v];
            if (cap_[u][v] < pathFlow) pathFlow = cap_[u][v];
        }
        for (int v = T; v != S; v = parent[v]) {
            int u = parent[v];
            cap_[u][v] -= pathFlow;
            cap_[v][u] += pathFlow;
        }
        flow += pathFlow;
        if (flow >= INF) return flow; // already unbounded, no need to continue
    }
    return flow;
}

int main() {
    int x;
    if (scanf("%d", &x) != 1) return 0;
    while (x--) {
        int m, n;
        scanf("%d %d", &m, &n);
        n_nodes = m + 1;
        S = m; T = n;
        memset(cap_, 0, sizeof(cap_));
        bool intruder[MAXN];
        memset(intruder, 0, sizeof(intruder));
        for (int i = 0; i < m; i++) {
            char tag[4];
            int c;
            scanf("%s %d", tag, &c);
            if (tag[0] == 'I') intruder[i] = true;
            for (int k = 0; k < c; k++) {
                int r;
                scanf("%d", &r);
                cap_[i][r] += INF; // uncuttable: panel room -> target room
                cap_[r][i] += 1;   // cuttable: target room -> panel room
            }
        }
        for (int i = 0; i < m; i++) {
            if (intruder[i]) cap_[S][i] += INF;
        }
        int f = maxflow();
        if (f >= INF) {
            printf("PANIC ROOM BREACH\n");
        } else {
            printf("%d\n", f);
        }
    }
    return 0;
}
