// POJ 2894 - Ancient Keyboard
// Model: claude-sonnet-5
// Approach: For each of the n keys given as (letter, a, b), the key's LED is
// on during time steps a, a+1, ..., b-1. Accumulate, for every time step
// t in [0, 1000], the count of LEDs simultaneously on (direct increment
// since n <= 26 and b <= 1000, so O(26*1000) per test case is trivial).
// At each time step, if the count is > 0, the count-th letter of the
// alphabet is written to the tape; if the count is 0, nothing is written
// for that step (verified against sample 2, where t=8 has 0 LEDs on and
// is skipped rather than emitting a blank/placeholder).
#include <cstdio>
#include <cstring>

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int n;
        scanf("%d", &n);
        int cnt[1002];
        memset(cnt, 0, sizeof(cnt));
        for (int i = 0; i < n; i++) {
            char buf[8];
            int a, b;
            scanf("%s %d %d", buf, &a, &b);
            for (int tm = a; tm < b; tm++) cnt[tm]++;
        }
        char out[1002];
        int len = 0;
        for (int tm = 0; tm <= 1000; tm++) {
            if (cnt[tm] > 0) out[len++] = 'A' + (cnt[tm] - 1);
        }
        out[len] = '\0';
        printf("%s\n", out);
    }
    return 0;
}
