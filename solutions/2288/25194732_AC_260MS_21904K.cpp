// POJ 2288 - Islands and Bridges
// Model: claude-sonnet-5
// Approach: bitmask DP over Hamilton paths. dp[mask][i][j] = best value / count
// of ways for a path visiting exactly the islands in mask, ending with the
// last two islands being i then j (j is the most recent). Extending the path
// by island k (not in mask, edge j-k required) adds V[k] + V[j]*V[k], plus
// V[i]*V[j]*V[k] if the edge i-k also exists (triangle Ci Cj Ck). Init states
// are the two-island paths formed by every bridge, seeded symmetrically in
// both directions so the DP can grow a physical path from either endpoint.
// Since building forward from each end of a path yields two distinct final
// (mask=full,i,j) states for the same physical path (a path and its reverse
// are declared equal by the statement), the summed count over all best-value
// end states is halved at the end. n=1 has no bridges to seed the DP, so it
// is handled directly: the lone island is itself the (only) Hamilton path.
// Values and counts can each need more than 32 bits (discuss board flags
// count overflow at n=13), hence long long throughout.
#include <cstdio>
#include <cstring>
using namespace std;

typedef long long ll;

static int n, m;
static ll V[13];
static int adjMask[13];
static ll val[1 << 13][13][13];
static ll cnt[1 << 13][13][13];

int main() {
    int q;
    scanf("%d", &q);
    while (q--) {
        scanf("%d %d", &n, &m);
        for (int i = 0; i < n; i++) scanf("%lld", &V[i]);
        for (int i = 0; i < n; i++) adjMask[i] = 0;
        for (int e = 0; e < m; e++) {
            int x, y;
            scanf("%d %d", &x, &y);
            x--; y--;
            adjMask[x] |= (1 << y);
            adjMask[y] |= (1 << x);
        }

        if (n == 1) {
            printf("%lld %lld\n", V[0], (ll)1);
            continue;
        }

        int full = (1 << n) - 1;
        for (int s = 0; s <= full; s++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    val[s][i][j] = -1;
                    cnt[s][i][j] = 0;
                }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                if (adjMask[i] & (1 << j)) {
                    int s = (1 << i) | (1 << j);
                    ll v = V[i] + V[j] + V[i] * V[j];
                    val[s][i][j] = v;
                    cnt[s][i][j] = 1;
                }
            }

        for (int s = 0; s <= full; s++) {
            for (int i = 0; i < n; i++) {
                if (!(s & (1 << i))) continue;
                for (int j = 0; j < n; j++) {
                    if (i == j || !(s & (1 << j))) continue;
                    if (val[s][i][j] < 0) continue;
                    ll base = val[s][i][j];
                    ll c0 = cnt[s][i][j];
                    for (int k = 0; k < n; k++) {
                        if (s & (1 << k)) continue;
                        if (!(adjMask[j] & (1 << k))) continue;
                        ll add = V[k] + V[j] * V[k];
                        if (adjMask[i] & (1 << k)) add += V[i] * V[j] * V[k];
                        ll nv = base + add;
                        int ns = s | (1 << k);
                        if (nv > val[ns][j][k]) {
                            val[ns][j][k] = nv;
                            cnt[ns][j][k] = c0;
                        } else if (nv == val[ns][j][k]) {
                            cnt[ns][j][k] += c0;
                        }
                    }
                }
            }
        }

        ll best = -1, total = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                ll v = val[full][i][j];
                if (v < 0) continue;
                if (v > best) {
                    best = v;
                    total = cnt[full][i][j];
                } else if (v == best) {
                    total += cnt[full][i][j];
                }
            }

        if (best < 0) printf("0 0\n");
        else printf("%lld %lld\n", best, total / 2);
    }
    return 0;
}
