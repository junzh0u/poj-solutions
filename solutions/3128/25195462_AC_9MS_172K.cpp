// POJ 3128 - Leonardo's Notebook
// Model: claude-sonnet-5
// Approach: a permutation p is expressible as q*q (q applied twice) iff,
// decomposing p into disjoint cycles, every EVEN cycle length occurs an
// EVEN number of times. Reason: squaring a single q-cycle of odd length m
// yields one p-cycle of length m; squaring a q-cycle of even length 2k
// yields two p-cycles of length k. So an odd p-cycle length is always
// achievable (as a leftover odd q-cycle), while an even p-cycle length L
// must be reconstructible by pairing up p-cycles of length L into a single
// q-cycle of length 2L, hence their count must be even. Ambiguity: none;
// the statement is a direct classification problem, confirmed by the
// board's own phrasing of the same even-length-count rule.
#include <cstdio>
#include <cstring>

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        char buf[64];
        scanf("%s", buf);
        int p[26];
        for (int i = 0; i < 26; ++i) p[i] = buf[i] - 'A';
        bool visited[26];
        memset(visited, 0, sizeof(visited));
        int cnt[27];
        memset(cnt, 0, sizeof(cnt));
        for (int i = 0; i < 26; ++i) {
            if (visited[i]) continue;
            int len = 0;
            int j = i;
            while (!visited[j]) {
                visited[j] = true;
                j = p[j];
                ++len;
            }
            ++cnt[len];
        }
        bool ok = true;
        for (int L = 2; L <= 26; L += 2) {
            if (cnt[L] % 2 != 0) { ok = false; break; }
        }
        printf(ok ? "Yes\n" : "No\n");
    }
    return 0;
}
