// POJ 3894 - System Engineer
// Model: claude-sonnet-5
// Approach: Maximum bipartite matching between n jobs (0..n-1) and n servers
// (numbered n..2n-1, remapped to 0..n-1) via Hopcroft-Karp for O(E*sqrt(V))
// performance -- a discuss-board note (969MS on this 1000MS limit) suggests
// naive augmenting-path matching is too close to the limit, so HK is used.
// Input format allows whitespace/colons/parens freely between numbers; a
// single "skip to next digit" tokenizer handles job_number: (nr) s1 ... sk
// uniformly. Job identity is read explicitly (job_number) rather than
// assumed from line position, since the archive's own pitfalls warn against
// that assumption even though this statement's sample lists jobs in order.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

static char *buf;
static long bufLen;
static long pos;

static bool nextInt(int &out) {
    // skip non-digit characters
    while (pos < bufLen && (buf[pos] < '0' || buf[pos] > '9')) pos++;
    if (pos >= bufLen) return false;
    long val = 0;
    while (pos < bufLen && buf[pos] >= '0' && buf[pos] <= '9') {
        val = val * 10 + (buf[pos] - '0');
        pos++;
    }
    out = (int)val;
    return true;
}

int n;
vector<vector<int> > adj;
vector<int> matchL, matchR, dist_;
const int INF = 1 << 30;

bool bfs() {
    queue<int> q;
    for (int u = 0; u < n; u++) {
        if (matchL[u] == -1) { dist_[u] = 0; q.push(u); }
        else dist_[u] = INF;
    }
    bool found = false;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        const vector<int> &lst = adj[u];
        for (size_t i = 0; i < lst.size(); i++) {
            int v = lst[i];
            int w = matchR[v];
            if (w == -1) {
                found = true;
            } else if (dist_[w] == INF) {
                dist_[w] = dist_[u] + 1;
                q.push(w);
            }
        }
    }
    return found;
}

bool dfs(int u) {
    const vector<int> &lst = adj[u];
    for (size_t i = 0; i < lst.size(); i++) {
        int v = lst[i];
        int w = matchR[v];
        if (w == -1 || (dist_[w] == dist_[u] + 1 && dfs(w))) {
            matchL[u] = v;
            matchR[v] = u;
            return true;
        }
    }
    dist_[u] = INF;
    return false;
}

int main() {
    // read entire stdin
    long cap = 1 << 20; // 1MB base, grow if needed
    buf = (char*)malloc(cap);
    bufLen = 0;
    size_t r;
    while ((r = fread(buf + bufLen, 1, cap - bufLen, stdin)) > 0) {
        bufLen += (long)r;
        if (bufLen == cap) {
            cap *= 2;
            buf = (char*)realloc(buf, cap);
        }
    }
    pos = 0;

    int nn;
    while (nextInt(nn)) {
        n = nn;
        adj.assign(n, vector<int>());
        matchL.assign(n, -1);
        matchR.assign(n, -1);
        dist_.assign(n, 0);

        for (int i = 0; i < n; i++) {
            int jobNum, nrServers;
            nextInt(jobNum);
            nextInt(nrServers);
            for (int k = 0; k < nrServers; k++) {
                int s;
                nextInt(s);
                int sidx = s - n;
                if (sidx >= 0 && sidx < n && jobNum >= 0 && jobNum < n) {
                    adj[jobNum].push_back(sidx);
                }
            }
        }

        int matching = 0;
        while (bfs()) {
            for (int u = 0; u < n; u++) {
                if (matchL[u] == -1) {
                    if (dfs(u)) matching++;
                }
            }
        }
        printf("%d\n", matching);
    }

    return 0;
}
