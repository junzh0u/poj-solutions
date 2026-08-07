// POJ 3345 - Bribing FIPA
// Model: claude-opus-5
//
// Tree (forest) knapsack.  Bribing a country buys the votes of its whole
// subtree; a country votes only if it or one of its ancestors is bribed.
// f[u][k] = minimum cost to secure AT LEAST k votes inside u's subtree:
//   - do not bribe u  -> u itself never votes, merge the children knapsacks
//   - bribe u         -> cost[u] buys all sz[u] votes of the subtree
// A suffix-min turns the exact-count table into "at least k", which keeps the
// merge (descending outer index, so no slot is read after being written) valid
// and lets a cheap high-yield child be used for a smaller demand.
// The roots of the forest are merged the same way; answer = min_{k>=m} g[k].
//
// Input framing (the part the statement leaves out, per the discuss board):
// several data sets, terminated by a single line "#" AFTER the last one - not
// after each one.  The header line may carry leading spaces, tokens may be
// separated by runs of spaces, and a country line can run to ~20KB (200 names
// of 100 letters), so parsing is getline + istringstream rather than fixed
// buffers.  Names may be referenced as dominated countries before their own
// line appears, so ids come from a map; a duplicate child reference is ignored
// (a node keeps its first parent) and a name that never gets its own line is
// given an infinite bribe cost instead of being dropped.
#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

const int MAXN = 512;
const int INF = 1000000000;

static int cost_[MAXN];
static bool hasParent[MAXN];
static int sz[MAXN];
static int f[MAXN][MAXN];
static vector<int> ch[MAXN];

static void dfs(int u) {
    f[u][0] = 0;
    int cur = 0;                       /* votes obtainable without bribing u */
    for (size_t i = 0; i < ch[u].size(); ++i) {
        int v = ch[u][i];
        dfs(v);
        for (int a = cur; a >= 0; --a) {
            if (f[u][a] >= INF) continue;
            for (int b = 1; b <= sz[v]; ++b) {
                if (f[v][b] >= INF) continue;
                int c = f[u][a] + f[v][b];
                if (c < f[u][a + b]) f[u][a + b] = c;
            }
        }
        cur += sz[v];
    }
    sz[u] = cur + 1;
    if (cost_[u] < f[u][sz[u]]) f[u][sz[u]] = cost_[u];
    for (int k = sz[u] - 1; k >= 0; --k)
        if (f[u][k + 1] < f[u][k]) f[u][k] = f[u][k + 1];
}

static bool nextLine(string &line) {
    while (getline(cin, line)) {
        while (!line.empty() &&
               (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);
        for (size_t i = 0; i < line.size(); ++i)
            if (!isspace((unsigned char)line[i])) return true;
    }
    return false;
}

int main() {
    string line;
    while (nextLine(line)) {
        {
            istringstream probe(line);
            string tok;
            probe >> tok;
            if (tok[0] == '#') break;
        }
        int n, m;
        {
            istringstream hs(line);
            if (!(hs >> n >> m)) break;
        }

        map<string, int> id;
        int total = 0;
        for (int i = 0; i < MAXN; ++i) {
            cost_[i] = INF;
            hasParent[i] = false;
            sz[i] = 0;
            ch[i].clear();
        }

        for (int i = 0; i < n; ++i) {
            if (!nextLine(line)) break;
            istringstream ts(line);
            string name;
            int c;
            if (!(ts >> name)) continue;
            if (!(ts >> c)) c = INF;
            int u;
            map<string, int>::iterator it = id.find(name);
            if (it == id.end()) {
                u = total < MAXN - 1 ? total++ : MAXN - 1;
                id[name] = u;
            } else {
                u = it->second;
            }
            cost_[u] = c;
            string cn;
            while (ts >> cn) {
                int v;
                map<string, int>::iterator jt = id.find(cn);
                if (jt == id.end()) {
                    v = total < MAXN - 1 ? total++ : MAXN - 1;
                    id[cn] = v;
                } else {
                    v = jt->second;
                }
                if (v == u || hasParent[v]) continue;
                hasParent[v] = true;
                ch[u].push_back(v);
            }
        }

        for (int i = 0; i < total; ++i)
            for (int k = 0; k <= total; ++k) f[i][k] = INF;

        vector<int> g(total + 2, INF);
        g[0] = 0;
        int gsz = 0;
        for (int r = 0; r < total; ++r) {
            if (hasParent[r]) continue;
            dfs(r);
            for (int a = gsz; a >= 0; --a) {
                if (g[a] >= INF) continue;
                for (int b = 1; b <= sz[r]; ++b) {
                    if (f[r][b] >= INF) continue;
                    int c = g[a] + f[r][b];
                    if (c < g[a + b]) g[a + b] = c;
                }
            }
            gsz += sz[r];
        }

        int ans = INF;
        for (int k = m; k <= gsz; ++k)
            if (g[k] < ans) ans = g[k];
        if (m <= 0) ans = 0;
        printf("%d\n", ans);
    }
    return 0;
}
