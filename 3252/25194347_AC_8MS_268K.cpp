// POJ 3252 - Round Numbers
#include <cstdio>
#include <algorithm>
using namespace std;

// Start/Finish <= 2,000,000,000 < 2^31-1, so a value never needs more than 31
// bits and free-bit counts r never exceed 30; C(30,15) ~ 1.5e8 is the largest
// entry actually used, comfortably inside a 32-bit int. The table is sized
// 32x32 (rather than something bigger "for headroom") so that even the
// unused entries built along the way, like C(31,15) ~ 3.0e8, stay in range
// too and building the table can't signed-overflow.
// Sticking to plain int/%d also sidesteps POJ's old-Windows-g++ %lld/%I64d quirk.
static int C[32][32];

void initC() {
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) C[i][j] = 0;
    }
    for (int i = 0; i < 32; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            C[i][j] = C[i-1][j-1] + C[i-1][j];
        }
    }
}

// Count round numbers in [1, x] by digit-DP over the binary representation:
// for each bit-length shorter than x's, sum combinations of the free bits
// after the mandatory leading 1; for x's own bit-length, walk MSB to LSB and,
// at every 1-bit, branch to "set this bit to 0" and count the free suffixes
// that keep zeros >= ones; finally check x itself.
int countRound(int x) {
    if (x <= 0) return 0;
    int b[35];
    int n = 0;
    int tmp = x;
    while (tmp > 0) { b[n++] = tmp & 1; tmp >>= 1; }
    // b[0] is LSB..b[n-1] is MSB; reverse so b[0] is MSB
    for (int i = 0; i < n/2; i++) swap(b[i], b[n-1-i]);

    int total = 0;

    // numbers with fewer bits than x (bit-length d = 1..n-1)
    for (int d = 1; d < n; d++) {
        int r = d - 1; // free bits after the leading 1
        if (r == 0) continue; // ones=1,zeros=0 never round
        int ones = 1, zeros = 0;
        int rhs = r + zeros - ones; // = r - 1
        if (rhs < 0) continue;
        int kmax = rhs / 2;
        if (kmax > r) kmax = r;
        for (int k = 0; k <= kmax; k++) total += C[r][k];
    }

    // numbers with same bit-length as x. The leading bit (pos 0) is always 1
    // and must NOT be branched on: flipping it to 0 would just re-produce a
    // shorter number already counted above, not a valid n-bit number.
    int zeros_so_far = 0, ones_so_far = 1;
    for (int pos = 1; pos < n; pos++) {
        if (b[pos] == 1) {
            int r = n - 1 - pos;
            int zsf = zeros_so_far + 1;
            int osf = ones_so_far;
            int rhs = r + zsf - osf;
            if (rhs >= 0) {
                int jmax = rhs / 2;
                if (jmax > r) jmax = r;
                for (int j = 0; j <= jmax; j++) total += C[r][j];
            }
            ones_so_far++;
        } else {
            zeros_so_far++;
        }
    }

    if (zeros_so_far >= ones_so_far) total++;

    return total;
}

int main() {
    initC();
    int start, finish;
    scanf("%d %d", &start, &finish);
    printf("%d\n", countRound(finish) - countRound(start - 1));
    return 0;
}
