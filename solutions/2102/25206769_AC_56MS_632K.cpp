// POJ 2102 - Incredible! Impossible!
// Model: claude-sonnet-5
//
// n x 3 table of non-negative integers with given row sums r[1..n] and
// column sums c1,c2,c3. Count the number of such tables mod 10^17.
//
// For each row i we pick (a_i,b_i) with a_i,b_i>=0, a_i+b_i<=r_i; the third
// cell is c_i = r_i-a_i-b_i >= 0 automatically. We need sum(a_i)=c1 and
// sum(b_i)=c2 (which forces sum(c_i)=total-c1-c2; this only equals the
// given c3 when total==c1+c2+c3, so that is checked separately -- the DP
// itself does not know about c3 at all).
//
// DP over rows: dp[x][y] = number of ways, after some prefix of rows, to
// have sum a = x, sum b = y (x<=c1, y<=c2 always, larger partial sums are
// never useful since we only ever query dp[c1][c2] at the end and every
// a_i,b_i>=0). The per-row transition is a 2D convolution with the
// indicator kernel {(a,b): a,b>=0, a+b<=r}. Computed via a 1D prefix sum
// along y so each row costs O(c1*c2*r) instead of O(c1*c2*r^2); with
// n,c1,c2,r <= 125 that is about 2*10^8 additions worst case, well inside
// the (generous, 20s) time limit.
//
// Statement ambiguity: none really -- n, c1, c2, c3 and the n row sums are
// each <=125 per the statement, single data set (verified against sibling
// problem 2100 from the same contest/judge family, which also carries a
// "Case Time Limit" annotation yet is a plain single-dataset problem, so
// that annotation does not imply multiple test cases here).

#include <cstdio>
using namespace std;

static const long long MOD = 100000000000000000LL; // 10^17

int n, c1, c2, c3;
int rowSum[130];

long long dp[126][126];
long long S[126][127]; // S[x][y+1] = sum_{b=0}^{y} dp[x][b], S[x][0] = 0

static inline long long getS(int x, int y) {
    if (y < 0) return 0;
    return S[x][y + 1];
}

int main() {
    if (scanf("%d %d %d %d", &n, &c1, &c2, &c3) != 4) return 0;
    long long total = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &rowSum[i]);
        total += rowSum[i];
    }

    if (total != (long long)c1 + c2 + c3) {
        printf("0\n");
        return 0;
    }

    for (int x = 0; x <= c1; x++)
        for (int y = 0; y <= c2; y++)
            dp[x][y] = 0;
    dp[0][0] = 1;

    for (int i = 0; i < n; i++) {
        int r = rowSum[i];

        // build prefix sums of current dp along y
        for (int x = 0; x <= c1; x++) {
            S[x][0] = 0;
            for (int y = 0; y <= c2; y++) {
                long long v = S[x][y] + dp[x][y];
                if (v >= MOD) v -= MOD;
                S[x][y + 1] = v;
            }
        }

        static long long ndp[126][126];
        for (int x = 0; x <= c1; x++)
            for (int y = 0; y <= c2; y++)
                ndp[x][y] = 0;

        for (int xp = 0; xp <= c1; xp++) {
            int maxA = r < xp ? r : xp;
            for (int a = 0; a <= maxA; a++) {
                int x0 = xp - a;
                int k = r - a; // max b allowed by the row budget
                for (int yp = 0; yp <= c2; yp++) {
                    int B = k < yp ? k : yp;
                    long long top = getS(x0, yp);
                    long long bot = getS(x0, yp - B - 1);
                    long long val = top - bot;
                    if (val < 0) val += MOD;
                    long long sum = ndp[xp][yp] + val;
                    if (sum >= MOD) sum -= MOD;
                    ndp[xp][yp] = sum;
                }
            }
        }

        for (int x = 0; x <= c1; x++)
            for (int y = 0; y <= c2; y++)
                dp[x][y] = ndp[x][y];
    }

    printf("%lld\n", dp[c1][c2]);
    return 0;
}
