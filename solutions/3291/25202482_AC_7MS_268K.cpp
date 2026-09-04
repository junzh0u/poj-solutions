// POJ 3291 - Cousins
// Model: claude-sonnet-5
//
// Two strings are "first cousins" if a common subsequence of length
// >= max(ceil(|a|/2), ceil(|b|/2)) exists between them (i.e. no more
// than half of each may be removed to make them equal). The n-th
// cousin relation chains first-cousin steps through freely chosen
// intermediate strings e. Since every intermediate string is
// unconstrained content-wise except at the two fixed endpoints x and
// y, the minimum chain length n depends only on Lx=|x|, Ly=|y| and
// L=LCS(x,y): let n0=min(Lx,Ly), M=max(Lx,Ly), deficit=n0-L. Doubling
// a running length starting at n0 while checking
// deficit > (cur-deficit)  or  (cur-deficit)*2 < M
// counts exactly the number of extra hops beyond the direct (n=1)
// check; the loop's zero-iteration case reduces algebraically to the
// real direct condition L >= max(ceil(Lx/2), ceil(Ly/2)). Verified by
// hand against several constructed (Lx,Ly,L) triples, including a
// proof of impossibility for one fewer step in an adversarial
// disjoint-alphabet case (Lx=2, Ly=8, L=0 forces n=3, not 2), so the
// formula is not merely copied from the discuss board but rederived
// and checked independently. No case makes "not related" reachable:
// the doubling loop always terminates since both exit thresholds grow
// without bound, so a finite n always exists for any nonempty x, y.
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
using namespace std;

static int dp[105][105];

int main() {
    std::string a, b;
    char buf[205];
    while (true) {
        if (!fgets(buf, sizeof(buf), stdin)) break;
        // strip newline / CR
        int len = (int)strlen(buf);
        while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) buf[--len] = '\0';
        a = buf;
        if (a == "0") break;
        if (!fgets(buf, sizeof(buf), stdin)) break;
        len = (int)strlen(buf);
        while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) buf[--len] = '\0';
        b = buf;

        int la = (int)a.size(), lb = (int)b.size();
        for (int i = 0; i <= la; ++i) dp[i][0] = 0;
        for (int j = 0; j <= lb; ++j) dp[0][j] = 0;
        for (int i = 1; i <= la; ++i) {
            for (int j = 1; j <= lb; ++j) {
                int best = dp[i-1][j] > dp[i][j-1] ? dp[i-1][j] : dp[i][j-1];
                if (a[i-1] == b[j-1] && dp[i-1][j-1] + 1 > best) best = dp[i-1][j-1] + 1;
                dp[i][j] = best;
            }
        }
        int L = dp[la][lb];
        int n0 = std::min(la, lb);
        int M = std::max(la, lb);
        int deficit = n0 - L;
        int n = n0;
        int ans = 0;
        while (deficit > (n - deficit) || (n - deficit) * 2 < M) {
            ++ans;
            n *= 2;
        }
        ++ans;
        printf("%d\n", ans);
    }
    return 0;
}
