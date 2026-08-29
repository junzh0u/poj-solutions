// POJ 3056 - The Bavarian Beer Party
// Model: claude-sonnet-5
// Approach: p people sit in a circle, each holding a beer brand. We must
// form a perfect non-crossing matching of all p people (chords on a circle
// that never cross), maximizing the number of matched pairs that share the
// same brand. A non-crossing perfect matching on a circle, read starting
// from any fixed point, is exactly a properly nested (balanced-parenthesis
// style) matching on the corresponding linear sequence -- no separate
// "wrap-around" case is needed (confirmed on the discuss board). So fix
// position 1 as the start and run the classic interval DP: dp[i][j] is the
// best value for the sub-interval [i,j] (empty when j==i-1). The leftmost
// point i of a non-empty interval must be matched to some k in (i,j] with
// k-i odd (so [i+1,k-1] has even length), giving
//   dp[i][j] = max_{k} dp[i+1][k-1] + dp[k+1][j] + [a[i]==a[k]]
// Answer is dp[1][p]. This is O(p^3) worst case (~8e7 for p=1000), well
// within the 6000ms limit.
#include <cstdio>
#include <cstring>
using namespace std;

const int MAXP = 1005;
static short dp[MAXP][MAXP];
static int a[MAXP];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int p;
        scanf("%d", &p);
        for (int i = 1; i <= p; ++i) scanf("%d", &a[i]);
        for (int i = 1; i <= p + 1; ++i) dp[i][i - 1] = 0;
        for (int len = 2; len <= p; len += 2) {
            for (int i = 1; i + len - 1 <= p; ++i) {
                int j = i + len - 1;
                int ai = a[i];
                int best = 0;
                for (int k = i + 1; k <= j; k += 2) {
                    int val = dp[i + 1][k - 1] + dp[k + 1][j];
                    if (ai == a[k]) val += 1;
                    if (val > best) best = val;
                }
                dp[i][j] = (short)best;
            }
        }
        printf("%d\n", (int)dp[1][p]);
    }
    return 0;
}
