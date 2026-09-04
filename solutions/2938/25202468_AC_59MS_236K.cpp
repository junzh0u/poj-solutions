// POJ 2938 - Economic Phone Calls
// Model: claude-sonnet-5
// Approach: Compute the true (relative) year of each log entry via the
// backward recovery rule. Entries with the same true year form a maximal
// contiguous run of strictly increasing timestamps ("chunk"); consecutive
// chunks' years differ by exactly 1. A kept subsequence's own recovery
// procedure can only ever assign a same-year or exactly-one-year-earlier
// relation between two ADJACENT kept entries, so: (1) no chunk between the
// first kept '+' entry and the very last entry (whose chunk anchors
// "current year") may be left with zero kept entries, and (2) whenever two
// adjacent kept entries belong to different (necessarily adjacent) chunks,
// the earlier one's timestamp must be >= the later one's, exactly mirroring
// the true recovery rule. Entries before the first '+' entry never need to
// be kept (nothing depends on their year), but the entries from the first
// '+' onward through the true last entry's chunk always need a witness,
// because that last chunk anchors "the current year" for whichever entry
// ends up last in the kept list.
// This gives an O(n^2) DP: dp[i] = min kept count of a valid selection
// ending at i (i kept), started at the first '+' (dp[first_plus] = 1).
// dp[i] = min over j in [first_plus, i-1] with no '+' strictly inside (j,i)
// and true-year gap 0 or 1 (with the timestamp condition when gap==1) of
// dp[j] + 1. By induction dp[i] always represents a valid selection
// containing exactly the '+' entries up to i (plus bridge points), so the
// answer is the minimum dp[i] over i that (a) lies in the same chunk as
// the very last entry (year 0) AND (b) is not earlier than the last '+'
// overall (otherwise a mandatory '+' after i would be missing). j = i-1 is
// always a valid fallback transition (by the very definition of the
// year-recovery rule), so dp is always well defined.
// n <= 1000, so plain O(n^2) per test case is comfortably fast.
//
// Ambiguity check: the problem's discuss board (message 51951/51952)
// confirms entries before the first kept '+' never need to be preserved,
// matching this model; verified locally against the sample (answer 6)
// and against that board's own follow-up test case (only calls 5 and 6
// marked '+', expected 3).
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 1005;
int K[MAXN];
bool plus_[MAXN];
int y[MAXN];
int pre[MAXN];
int dp[MAXN];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        char numbuf[32], signbuf[8];
        for (int i = 1; i <= n; i++) {
            int mm, dd, hh, mi;
            scanf("%d:%d:%d:%d", &mm, &dd, &hh, &mi);
            scanf("%s %s", numbuf, signbuf);
            K[i] = ((mm * 100 + dd) * 100 + hh) * 100 + mi;
            plus_[i] = (signbuf[0] == '+');
        }
        y[n] = 0;
        for (int i = n - 1; i >= 1; i--) {
            y[i] = y[i + 1] - (K[i] < K[i + 1] ? 0 : 1);
        }
        pre[0] = 0;
        for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + (plus_[i] ? 1 : 0);

        int firstPlus = -1;
        for (int i = 1; i <= n; i++) {
            if (plus_[i]) { firstPlus = i; break; }
        }
        if (firstPlus == -1) {
            printf("0\n");
            continue;
        }

        dp[firstPlus] = 1;
        for (int i = firstPlus + 1; i <= n; i++) {
            int best = dp[i - 1] + 1; // fallback, always valid
            for (int j = firstPlus; j < i - 1; j++) {
                int betweenCnt = pre[i - 1] - pre[j];
                if (betweenCnt != 0) continue;
                int gap = y[i] - y[j];
                if (gap == 0) {
                    if (dp[j] + 1 < best) best = dp[j] + 1;
                } else if (gap == 1) {
                    if (K[j] >= K[i]) {
                        if (dp[j] + 1 < best) best = dp[j] + 1;
                    }
                }
            }
            dp[i] = best;
        }

        int lastPlus = firstPlus;
        for (int i = 1; i <= n; i++) if (plus_[i]) lastPlus = i;

        // The final answer must still include every '+' entry, so the
        // candidate terminal point can never be earlier than the last '+'.
        int ans = -1;
        for (int i = lastPlus; i <= n; i++) {
            if (y[i] == y[n]) {
                if (ans == -1 || dp[i] < ans) ans = dp[i];
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
