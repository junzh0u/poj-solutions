// POJ 3373 - Changing Digits
// Model: claude-sonnet-5
// Approach: digit DP. Let L = number of digits of n, w[i] = 10^(L-1-i) mod k
// (place value of position i, 0-indexed from the left). c[i][s] = minimum
// number of digit changes among positions i..L-1 so that the value formed
// by those digits contributes residue s (mod k) to the total. Computed
// backward from c[L][0]=0 (empty suffix has value 0). Position 0 may not
// be '0' unless L==1 (statement: zero itself is a valid one-digit number).
// The answer's change count is c[0][0]; reconstruct forward, at each
// position picking the smallest digit that still attains c[i][s] exactly,
// which simultaneously minimizes changes and, among ties, yields the
// numerically smallest m (board message 77884/77885 confirms "different"
// means differing positions, not total value distance).
#include <cstdio>
#include <cstring>
using namespace std;

const short INF = 30000;
const int LMAX = 110;
const int KMAX = 10001;

// short (not int) keeps this ~2MB table cache-friendly: with L,k both near
// their maxima (101 * 10 * 10000 inner updates per test case) memory
// bandwidth, not arithmetic, is the bottleneck.
static short c[LMAX + 1][KMAX];
static int w[LMAX];
static char nbuf[LMAX];
static char result[LMAX];

int main() {
    int k;
    while (scanf("%s", nbuf) == 1) {
        scanf("%d", &k);
        int L = (int)strlen(nbuf);

        w[L - 1] = 1 % k;
        for (int i = L - 2; i >= 0; i--) w[i] = (w[i + 1] * 10) % k;

        for (int s = 0; s < k; s++) c[L][s] = INF;
        c[L][0] = 0;

        for (int i = L - 1; i >= 0; i--) {
            for (int s = 0; s < k; s++) c[i][s] = INF;
            int dlo = (i == 0 && L > 1) ? 1 : 0;
            int digit = nbuf[i] - '0';
            for (int d = dlo; d <= 9; d++) {
                short cost = (d == digit) ? 0 : 1;
                int shift = (d * w[i]) % k;
                const short *src = c[i + 1];
                short *dst = c[i];
                int part1 = k - shift;
                for (int idx = 0; idx < part1; idx++) {
                    short val = (short)(src[idx] + cost);
                    int t = idx + shift;
                    if (val < dst[t]) dst[t] = val;
                }
                for (int idx = part1; idx < k; idx++) {
                    short val = (short)(src[idx] + cost);
                    int t = idx + shift - k;
                    if (val < dst[t]) dst[t] = val;
                }
            }
        }

        int s = 0;
        for (int i = 0; i < L; i++) {
            int dlo = (i == 0 && L > 1) ? 1 : 0;
            int digit = nbuf[i] - '0';
            for (int d = dlo; d <= 9; d++) {
                int cost = (d == digit) ? 0 : 1;
                int shift = (d * w[i]) % k;
                int idx = s - shift;
                if (idx < 0) idx += k;
                if (cost + c[i + 1][idx] == c[i][s]) {
                    result[i] = (char)('0' + d);
                    s = idx;
                    break;
                }
            }
        }
        result[L] = 0;
        printf("%s\n", result);
    }
    return 0;
}
