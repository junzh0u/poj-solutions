// POJ 3691 - DNA repair
// Model: claude-sonnet-5
// Approach: Aho-Corasick automaton over the N disease-segment patterns
// (total pattern length <= 50*20 = 1000 nodes), with each node's "danger"
// flag propagated across fail links so a node is dangerous iff itself or
// any of its suffix-links corresponds to the end of some pattern. Then a
// DP over (position in DNA, automaton state): dp[i][s] = min edits to
// reach state s having processed i characters, never passing through a
// dangerous state. At each step try all 4 bases; cost 0 if it matches the
// original character, else 1; transitioning into a dangerous state is
// forbidden. Answer is the min dp[len][*] over all non-dangerous states,
// or -1 if unreachable.
// Statement note: nothing ambiguous here — patterns and DNA use only
// "AGCT", N <= 50, pattern length <= 20, DNA length <= 1000, matching the
// well-known HDU2457/POJ3691 "DNA repair" sample exactly (case 3's four
// single-character patterns A/G/C/T make every string dangerous, hence -1).

#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

const int MAXNODE = 1005;
int ch[MAXNODE][4];
int fail_[MAXNODE];
bool danger[MAXNODE];
int sz;

int idx(char c) {
    switch (c) {
        case 'A': return 0;
        case 'G': return 1;
        case 'C': return 2;
        case 'T': return 3;
    }
    return -1;
}

void insertPattern(const char *s) {
    int cur = 0;
    for (int i = 0; s[i]; ++i) {
        int c = idx(s[i]);
        if (!ch[cur][c]) {
            memset(ch[sz], 0, sizeof(ch[sz]));
            danger[sz] = false;
            fail_[sz] = 0;
            ch[cur][c] = sz++;
        }
        cur = ch[cur][c];
    }
    danger[cur] = true;
}

void build() {
    queue<int> q;
    for (int c = 0; c < 4; ++c) {
        if (ch[0][c]) {
            fail_[ch[0][c]] = 0;
            q.push(ch[0][c]);
        }
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        danger[u] = danger[u] || danger[fail_[u]];
        for (int c = 0; c < 4; ++c) {
            int v = ch[u][c];
            if (v) {
                fail_[v] = ch[fail_[u]][c];
                q.push(v);
            } else {
                ch[u][c] = ch[fail_[u]][c];
            }
        }
    }
}

const int INF = 0x3f3f3f3f;
int dp[2][MAXNODE];

int main() {
    int n;
    int caseNo = 0;
    char buf[30];
    char dna[1005];
    while (scanf("%d", &n) == 1 && n) {
        sz = 1;
        memset(ch[0], 0, sizeof(ch[0]));
        danger[0] = false;
        fail_[0] = 0;
        for (int i = 0; i < n; ++i) {
            scanf("%s", buf);
            insertPattern(buf);
        }
        build();
        scanf("%s", dna);
        int len = (int)strlen(dna);

        int cur = 0, nxt = 1;
        for (int s = 0; s < sz; ++s) dp[cur][s] = INF;
        dp[cur][0] = 0;
        for (int i = 0; i < len; ++i) {
            for (int s = 0; s < sz; ++s) dp[nxt][s] = INF;
            int orig = idx(dna[i]);
            for (int s = 0; s < sz; ++s) {
                if (dp[cur][s] >= INF) continue;
                for (int c = 0; c < 4; ++c) {
                    int v = ch[s][c];
                    if (danger[v]) continue;
                    int cost = dp[cur][s] + (c == orig ? 0 : 1);
                    if (cost < dp[nxt][v]) dp[nxt][v] = cost;
                }
            }
            swap(cur, nxt);
        }

        int ans = INF;
        for (int s = 0; s < sz; ++s) ans = min(ans, dp[cur][s]);
        if (ans >= INF) ans = -1;
        printf("Case %d: %d\n", ++caseNo, ans);
    }
    return 0;
}
