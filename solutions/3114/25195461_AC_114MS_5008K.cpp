// POJ 3114 - Countries in War
// Model: claude-sonnet-5
// Approach: Two agencies are "in the same country" iff mutually reachable via
// printed-letter edges, i.e. strongly connected components. Inside a country,
// electronic media send a letter instantly (cost 0) between any two agencies,
// even ones without a direct agreement. Between countries, only the printed
// letters (edge weight H) matter. So: compute SCCs of the directed weighted
// graph, condense to a DAG of countries (edge country(u)->country(v) with
// weight H for every original edge with u,v in different SCCs; same-SCC
// edges become free), then answer each query as: 0 if O,D share a country,
// else the shortest path in the condensed graph (Dijkstra, run once per
// distinct source that appears among this test case's queries and cached,
// since K <= 100). The discuss board confirms Floyd (O(N^3)) TLEs at N=500
// and Dijkstra is required; it also warns the real input terminates on a
// bare "N=0" without a trailing E, unlike the two-zero sample, so N is read
// and checked for 0 before E is ever read.
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int MAXN = 505;
const int MAXE = 250005;
const int INF = 1000000000;

int n, e;
int head[MAXN], nxt[MAXE], to_[MAXE], wt[MAXE], ecnt;
int chead[MAXN], cnxt[MAXE], cto[MAXE], cwt[MAXE], cecnt;

int dfn[MAXN], low[MAXN], idxCounter;
int stk[MAXN], stkTop;
bool inStack[MAXN];
int comp[MAXN], sccCnt;

void addEdge(int u, int v, int w) {
    to_[++ecnt] = v; wt[ecnt] = w; nxt[ecnt] = head[u]; head[u] = ecnt;
}
void addCondEdge(int u, int v, int w) {
    cto[++cecnt] = v; cwt[cecnt] = w; cnxt[cecnt] = chead[u]; chead[u] = cecnt;
}

void tarjan(int u) {
    dfn[u] = low[u] = ++idxCounter;
    stk[++stkTop] = u; inStack[u] = true;
    for (int ei = head[u]; ei; ei = nxt[ei]) {
        int v = to_[ei];
        if (!dfn[v]) {
            tarjan(v);
            if (low[v] < low[u]) low[u] = low[v];
        } else if (inStack[v]) {
            if (dfn[v] < low[u]) low[u] = dfn[v];
        }
    }
    if (low[u] == dfn[u]) {
        ++sccCnt;
        int v;
        do {
            v = stk[stkTop--];
            inStack[v] = false;
            comp[v] = sccCnt;
        } while (v != u);
    }
}

int distCache[MAXN][MAXN];
int cacheVer[MAXN];
int curVer;

void dijkstra(int src) {
    int *d = distCache[src];
    for (int i = 1; i <= sccCnt; i++) d[i] = INF;
    d[src] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;
    pq.push(make_pair(0, src));
    while (!pq.empty()) {
        pair<int,int> top = pq.top(); pq.pop();
        int du = top.first, u = top.second;
        if (du > d[u]) continue;
        for (int ei = chead[u]; ei; ei = cnxt[ei]) {
            int v = cto[ei];
            int nd = du + cwt[ei];
            if (nd < d[v]) {
                d[v] = nd;
                pq.push(make_pair(nd, v));
            }
        }
    }
    cacheVer[src] = curVer;
}

int main() {
    curVer = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        scanf("%d", &e);
        ++curVer;

        for (int i = 1; i <= n; i++) {
            head[i] = 0; chead[i] = 0;
            dfn[i] = 0; low[i] = 0; inStack[i] = false; comp[i] = 0;
        }
        ecnt = 0; cecnt = 0; idxCounter = 0; stkTop = 0; sccCnt = 0;

        static int ex[MAXE], ey[MAXE], ew[MAXE];
        for (int i = 0; i < e; i++) {
            int x, y, h;
            scanf("%d %d %d", &x, &y, &h);
            ex[i] = x; ey[i] = y; ew[i] = h;
            addEdge(x, y, h);
        }

        for (int i = 1; i <= n; i++) {
            if (!dfn[i]) tarjan(i);
        }

        for (int i = 0; i < e; i++) {
            int cu = comp[ex[i]], cv = comp[ey[i]];
            if (cu != cv) addCondEdge(cu, cv, ew[i]);
        }

        int k;
        scanf("%d", &k);
        for (int i = 0; i < k; i++) {
            int o, d;
            scanf("%d %d", &o, &d);
            int co = comp[o], cd = comp[d];
            if (co == cd) {
                printf("0\n");
                continue;
            }
            if (cacheVer[co] != curVer) dijkstra(co);
            int ans = distCache[co][cd];
            if (ans >= INF) printf("Nao e possivel entregar a carta\n");
            else printf("%d\n", ans);
        }
        printf("\n");
    }
    return 0;
}
