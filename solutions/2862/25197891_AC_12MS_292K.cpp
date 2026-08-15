// POJ 2862 - Tornado!
// Model: claude-sonnet-5
//
// Posts stand every 2m around a circle of N posts (perimeter 2N meters).
// A run of L consecutive missing/broken posts between two surviving posts
// needs floor(L/2) wooden posts: each wooden post can absorb up to 4m of
// slack (two 2m unit-gaps) on each side, so the minimum count to keep every
// resulting sub-gap <= 4m (2 units) over a linear span of L+1 units is
// ceil((L+1)/2)-1, which simplifies to floor(L/2) for all L>=0.
//
// Special case: if ALL N posts are missing (no surviving post to anchor the
// runs), the problem becomes purely circular - place points on a cycle of N
// unit-gaps- so every consecutive gap <= 2 units, minimum count ceil(N/2).
// The discuss board flags this case (odd/even N) as a common WA source.
#include <cstdio>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        static int x[5005];
        int ones = 0;
        for (int i = 0; i < n; ++i) {
            scanf("%d", &x[i]);
            if (x[i] == 1) ones++;
        }
        int ans = 0;
        if (ones == 0) {
            ans = (n + 1) / 2; // ceil(n/2)
        } else {
            int onePos[5005];
            int cnt = 0;
            for (int i = 0; i < n; ++i) if (x[i] == 1) onePos[cnt++] = i;
            for (int i = 0; i < cnt; ++i) {
                int a = onePos[i];
                int b = onePos[(i + 1) % cnt];
                int dist = b - a;
                if (dist <= 0) dist += n;
                int gap = dist - 1; // number of zeros between a and b
                ans += gap / 2;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
