// POJ 1618 - Decorations
// Model: claude-sonnet-5
//
// All m combinations share one length k. A streamer of length l is valid
// iff every window of k consecutive beads equals one of the m allowed
// words. Consecutive windows overlap in k-1 characters, so build a graph
// on the (deduplicated) allowed words: edge u->v when u's suffix of
// length k-1 equals v's prefix of length k-1 (v may legally follow u).
// A valid streamer of length l corresponds to a walk of l-k edges
// starting at any node; answer = sum of walk counts after l-k steps of
// forward DP (counts init to 1 per node). l==k gives answer m directly
// (0 steps). Per the discuss board (message 2546/2590), when the shared
// combination length k exceeds l, the intended/official answer is 0
// (not n^l as a literal reading might suggest) -- POJ's own judge data
// treats "no window of length k fits" as no valid streamer at all.
// n (alphabet size) turns out to be unused beyond appearing in input
// (confirmed on the board, message 2529): it plays no role once the
// word list is known, since the words themselves already restrict which
// letters/combinations are usable.
#include <cstdio>
#include <string>
#include <vector>
#include <set>
using namespace std;

int main(){
    int n, l, m;
    while (scanf("%d %d %d", &n, &l, &m) == 3) {
        if (n == 0 && l == 0 && m == 0) break;
        vector<string> raw(m);
        char buf[32];
        for (int i = 0; i < m; i++) {
            scanf("%s", buf);
            raw[i] = buf;
        }
        // dedupe
        set<string> uniq(raw.begin(), raw.end());
        vector<string> w(uniq.begin(), uniq.end());
        int mm = (int)w.size();
        int k = (int)w[0].size();

        if (l < k) {
            printf("0\n");
            continue;
        }

        vector< vector<int> > adj(mm);
        for (int u = 0; u < mm; u++) {
            for (int v = 0; v < mm; v++) {
                bool ok = true;
                for (int t = 0; t < k - 1; t++) {
                    if (w[u][t + 1] != w[v][t]) { ok = false; break; }
                }
                if (ok) adj[u].push_back(v);
            }
        }

        vector<unsigned long long> cnt(mm, 1ULL);
        int steps = l - k;
        for (int s = 0; s < steps; s++) {
            vector<unsigned long long> ncnt(mm, 0ULL);
            for (int u = 0; u < mm; u++) {
                if (cnt[u] == 0) continue;
                const vector<int>& av = adj[u];
                for (size_t idx = 0; idx < av.size(); idx++) {
                    ncnt[av[idx]] += cnt[u];
                }
            }
            cnt.swap(ncnt);
        }

        unsigned long long ans = 0;
        for (int i = 0; i < mm; i++) ans += cnt[i];
        printf("%llu\n", ans);
    }
    return 0;
}
