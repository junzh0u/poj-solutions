// POJ 3450 - Corporate Identity
// Model: claude-sonnet-5
// Approach: build a suffix automaton (SAM) on the first trademark of each
// task. For every other trademark, walk the SAM character by character,
// recording at each visited state the longest match ending there, then
// propagate those match lengths up the suffix-link tree (states processed
// in decreasing len order) so every state's match reflects the longest
// suffix of its class present in that trademark. Clamp each state's running
// best (min across all trademarks processed so far) with these values.
// After all trademarks, the state(s) with the maximum surviving length hold
// the answer; among ties pick the lexicographically smallest substring
// (extracted from the reference string via each state's first-endpos).
// No statement ambiguity found beyond the well-known tie-break note (already
// explicit in the statement): "several tasks" run until a line with 0.
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
using namespace std;

const int MAXLEN = 205;      // max length of a trademark
const int MAXN = 2 * MAXLEN + 5; // SAM states bound

struct SAM {
    int len[MAXN], link[MAXN], nxt[MAXN][26];
    int firstpos[MAXN];
    int sz, last;

    void init() {
        sz = 0;
        last = newNode(0, -1);
        firstpos[0] = -1;
    }
    int newNode(int l, int lk) {
        len[sz] = l; link[sz] = lk;
        memset(nxt[sz], -1, sizeof(nxt[sz]));
        firstpos[sz] = -1;
        return sz++;
    }
    void extend(int c, int pos) {
        int cur = newNode(len[last] + 1, -1);
        firstpos[cur] = pos;
        int p = last;
        while (p != -1 && nxt[p][c] == -1) {
            nxt[p][c] = cur;
            p = link[p];
        }
        if (p == -1) {
            link[cur] = 0;
        } else {
            int q = nxt[p][c];
            if (len[p] + 1 == len[q]) {
                link[cur] = q;
            } else {
                int clone = newNode(len[p] + 1, link[q]);
                firstpos[clone] = firstpos[q];
                memcpy(nxt[clone], nxt[q], sizeof(nxt[q]));
                while (p != -1 && nxt[p][c] == q) {
                    nxt[p][c] = clone;
                    p = link[p];
                }
                link[q] = clone;
                link[cur] = clone;
            }
        }
        last = cur;
    }
};

SAM sam;
int order_[MAXN]; // states sorted by len ascending (via counting sort)
int cnt_[MAXN];
int bestLen[MAXN];
int matchLen[MAXN];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        static char buf[MAXLEN];
        // read first string, build SAM
        scanf("%s", buf);
        string s0(buf);
        sam.init();
        for (size_t i = 0; i < s0.size(); ++i) {
            sam.extend(s0[i] - 'a', (int)i);
        }
        int sz = sam.sz;
        // counting sort states by len (0..s0.size())
        int maxLen = (int)s0.size();
        for (int i = 0; i <= maxLen; ++i) cnt_[i] = 0;
        for (int v = 0; v < sz; ++v) cnt_[sam.len[v]]++;
        for (int i = 1; i <= maxLen; ++i) cnt_[i] += cnt_[i-1];
        for (int v = sz - 1; v >= 0; --v) {
            order_[--cnt_[sam.len[v]]] = v;
        }
        // init bestLen = len[v] (available from string 0 itself)
        for (int v = 0; v < sz; ++v) bestLen[v] = sam.len[v];

        for (int t = 1; t < n; ++t) {
            scanf("%s", buf);
            string si(buf);
            for (int v = 0; v < sz; ++v) matchLen[v] = 0;
            int cur = 0, length = 0;
            for (size_t i = 0; i < si.size(); ++i) {
                int c = si[i] - 'a';
                while (cur != 0 && sam.nxt[cur][c] == -1) {
                    cur = sam.link[cur];
                    length = sam.len[cur];
                }
                if (sam.nxt[cur][c] != -1) {
                    cur = sam.nxt[cur][c];
                    length++;
                } else {
                    cur = 0;
                    length = 0;
                }
                if (length > matchLen[cur]) matchLen[cur] = length;
            }
            // propagate down suffix link tree, states in decreasing len order
            for (int idx = sz - 1; idx >= 1; --idx) {
                int v = order_[idx];
                int lk = sam.link[v];
                if (lk >= 0 && matchLen[v] > matchLen[lk]) {
                    matchLen[lk] = matchLen[v];
                }
            }
            for (int v = 0; v < sz; ++v) {
                if (matchLen[v] < bestLen[v]) bestLen[v] = matchLen[v];
            }
        }

        int ansLen = 0;
        for (int v = 1; v < sz; ++v) {
            if (bestLen[v] > ansLen) ansLen = bestLen[v];
        }
        if (ansLen == 0) {
            printf("IDENTITY LOST\n");
        } else {
            string best;
            bool have = false;
            for (int v = 1; v < sz; ++v) {
                if (bestLen[v] == ansLen) {
                    int endp = sam.firstpos[v];
                    string cand = s0.substr(endp - ansLen + 1, ansLen);
                    if (!have || cand < best) {
                        best = cand;
                        have = true;
                    }
                }
            }
            printf("%s\n", best.c_str());
        }
    }
    return 0;
}
