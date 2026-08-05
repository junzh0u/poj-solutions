// POJ 3469 - Dual Core CPU
// Min cut / max flow.
// S -> i  capacity B_i   (cost paid if i ends on S side == "core A" assignment)
// i -> T  capacity A_i   (cost paid if i ends on T side == "core B" assignment)
// wait: see derivation below; capacities chosen so a node landing on the
// source side of the min cut is assigned core A and pays A_i via the i->T
// edge crossing the cut, while a node on the sink side is assigned core B
// and pays B_i via the S->i edge crossing the cut.
// For each data-exchange pair (a,b,w): two directed edges a->b and b->a,
// each capacity w. Whichever direction crosses the S/T cut (i.e. exactly
// when a and b land on different sides) gets charged once; if they land on
// the same side neither edge crosses and nothing is charged.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long ll;
const ll INF = (ll)4e18;

const int MAXN = 20005;
const int MAXE = (20000*2 + 200000*2) * 2 + 10; // forward+reverse

int head[MAXN], nxt[MAXE], to_[MAXE], ecnt = 0;
ll cap_[MAXE];

void addEdge(int u, int v, ll c) {
    to_[ecnt] = v; cap_[ecnt] = c; nxt[ecnt] = head[u]; head[u] = ecnt++;
    to_[ecnt] = u; cap_[ecnt] = 0; nxt[ecnt] = head[v]; head[v] = ecnt++;
}

int level[MAXN];
int iter_[MAXN];
int q[MAXN];

int S, T, NN; // NN = total node count (n+2)

bool bfs() {
    memset(level, -1, sizeof(int) * NN);
    int qh = 0, qt = 0;
    level[S] = 0;
    q[qt++] = S;
    while (qh < qt) {
        int u = q[qh++];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (cap_[e] > 0 && level[to_[e]] < 0) {
                level[to_[e]] = level[u] + 1;
                q[qt++] = to_[e];
            }
        }
    }
    return level[T] >= 0;
}

int stkNode[MAXN + 5];
int stkEdge[MAXN + 5];

ll dinicPhase() {
    ll totalFlow = 0;
    memcpy(iter_, head, sizeof(int) * NN);
    int top = 0;
    stkNode[0] = S;
    for (;;) {
        int u = stkNode[top];
        if (u == T) {
            // find bottleneck
            ll f = INF;
            for (int i = 0; i < top; i++) if (cap_[stkEdge[i]] < f) f = cap_[stkEdge[i]];
            for (int i = 0; i < top; i++) {
                cap_[stkEdge[i]] -= f;
                cap_[stkEdge[i] ^ 1] += f;
            }
            totalFlow += f;
            // retreat to first saturated edge
            int cutAt = -1;
            for (int i = 0; i < top; i++) {
                if (cap_[stkEdge[i]] == 0) { cutAt = i; break; }
            }
            top = cutAt; // stkNode[top] is kept, its outgoing edge (stkEdge[top]) is now 0-cap
            continue;
        }
        // try to advance from u
        bool advanced = false;
        for (; iter_[u] != -1; iter_[u] = nxt[iter_[u]]) {
            int e = iter_[u];
            int v = to_[e];
            if (cap_[e] > 0 && level[v] == level[u] + 1) {
                stkEdge[top] = e;
                top++;
                stkNode[top] = v;
                advanced = true;
                break;
            }
        }
        if (!advanced) {
            if (top == 0) break; // no more augmenting paths this phase
            level[u] = -1; // dead end, forget it for this phase
            top--;
            // advance parent's iterator past the edge that led to the dead end
            int parent = stkNode[top];
            iter_[parent] = nxt[iter_[parent]];
        }
    }
    return totalFlow;
}

ll maxflow() {
    ll flow = 0;
    while (bfs()) {
        flow += dinicPhase();
    }
    return flow;
}

void printLL(ll x) {
    if (x == 0) { putchar('0'); putchar('\n'); return; }
    char buf[24];
    int n = 0;
    // x is guaranteed non-negative here
    while (x > 0) { buf[n++] = (char)('0' + (int)(x % 10)); x /= 10; }
    for (int i = n - 1; i >= 0; i--) putchar(buf[i]);
    putchar('\n');
}

int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) c = getchar();
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sign;
}

int main() {
    memset(head, -1, sizeof(head));
    int n = readInt(), m = readInt();
    NN = n + 2;
    S = n; T = n + 1;
    for (int i = 0; i < n; i++) {
        int a = readInt(), b = readInt();
        addEdge(S, i, (ll)b);
        addEdge(i, T, (ll)a);
    }
    for (int i = 0; i < m; i++) {
        int a = readInt() - 1, b = readInt() - 1, w = readInt();
        addEdge(a, b, (ll)w);
        addEdge(b, a, (ll)w);
    }
    ll ans = maxflow();
    printLL(ans);
    return 0;
}
