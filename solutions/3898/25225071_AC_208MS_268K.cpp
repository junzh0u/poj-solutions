// POJ 3898 - Software Industry Revolution
// Model: gpt-6.1-sol
// Dynamic programming over pattern prefixes and text endpoints. Empty pattern
// matches at every starting position; '*' either consumes nothing or extends
// its current match by one letter. Costs include every consumed text letter.
// Empty substrings are permitted, so an all-star pattern has complexity zero.
// O(pattern length * text length) time and O(text length) memory.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
char p[1010], s[10010];
int a[10010], b[10010];
int main() {
    const int INF = 1000000000;
    while (scanf("%1009s%10009s", p, s) == 2) {
        int n = (int)strlen(s), m = (int)strlen(p);
        int *old = a, *cur = b;
        for (int j = 0; j <= n; ++j) old[j] = 0;
        for (int i = 0; i < m; ++i) {
            if (p[i] == '*') {
                cur[0] = old[0];
                for (int j = 1; j <= n; ++j)
                    cur[j] = min(old[j], cur[j-1] + s[j-1]-'a'+1);
            } else {
                cur[0] = INF;
                for (int j = 1; j <= n; ++j)
                    cur[j] = (p[i] == '?' || p[i] == s[j-1])
                        ? old[j-1] + s[j-1]-'a'+1 : INF;
            }
            swap(old, cur);
        }
        int ans = *min_element(old, old+n+1);
        printf("%d\n", ans >= INF ? -1 : ans);
    }
    return 0;
}
