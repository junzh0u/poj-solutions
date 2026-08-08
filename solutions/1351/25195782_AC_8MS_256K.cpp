// POJ 1351 - Number of Locks
// Model: claude-opus-5
//
// Count strings of length n over the alphabet {1,2,3,4} such that
//   (a) some adjacent pair differs by exactly 3, i.e. some 1 sits next to a 4, and
//   (b) at least 3 of the 4 heights occur somewhere in the string.
// The slots form a row, not a ring: only positions i, i+1 are neighbours.  The
// official sample settles that reading (n = 3 gives 8 linearly and 12 cyclically).
//
// Straight DP over prefixes with state (last height, set of heights used so far,
// whether a 1-4 adjacency has already appeared): 4 * 16 * 2 = 128 states, one
// pass to n = 31, all answers cached.  Only the two flags are subtle -- "at
// least one" adjacency and "at least three" distinct values are both monotone,
// so they can be carried as sticky bits instead of being inclusion-excluded.
//
// 64-bit is required: n = 16 already gives 3553389280, past the range of a
// signed 32-bit int, so this goes out under G++ (language 0) with %lld rather
// than under POJ's C++ compiler, which has no long long.
//
// Table runs to n = 31 (4^31 still fits a signed 64-bit int) even though the
// statement promises 1 < n < 17, so data outside the stated range cannot make
// this read past the end of the array.

#include <cstdio>

const int MAXN = 31;

static long long answer[MAXN + 1];

static int popcount4(int mask) {
    int c = 0;
    for (int i = 0; i < 4; i++)
        if (mask & (1 << i)) c++;
    return c;
}

static void build() {
    // cur[last][mask][flag] = number of prefixes in that state
    static long long cur[4][16][2];
    static long long nxt[4][16][2];
    int a, m, f, h;

    for (a = 0; a < 4; a++)
        for (m = 0; m < 16; m++)
            for (f = 0; f < 2; f++) cur[a][m][f] = 0;

    answer[0] = 0;
    for (h = 0; h < 4; h++) cur[h][1 << h][0] = 1;

    for (int n = 1; n <= MAXN; n++) {
        long long total = 0;
        for (a = 0; a < 4; a++)
            for (m = 0; m < 16; m++)
                if (popcount4(m) >= 3) total += cur[a][m][1];
        answer[n] = total;

        if (n == MAXN) break;

        for (a = 0; a < 4; a++)
            for (m = 0; m < 16; m++)
                for (f = 0; f < 2; f++) nxt[a][m][f] = 0;

        for (a = 0; a < 4; a++)
            for (m = 0; m < 16; m++)
                for (f = 0; f < 2; f++) {
                    long long v = cur[a][m][f];
                    if (v == 0) continue;
                    for (h = 0; h < 4; h++) {
                        int nf = f;
                        if ((a == 0 && h == 3) || (a == 3 && h == 0)) nf = 1;
                        nxt[h][m | (1 << h)][nf] += v;
                    }
                }

        for (a = 0; a < 4; a++)
            for (m = 0; m < 16; m++)
                for (f = 0; f < 2; f++) cur[a][m][f] = nxt[a][m][f];
    }
}

int main() {
    build();
    int n;
    while (scanf("%d", &n) == 1 && n != -1) {
        long long r = (n >= 0 && n <= MAXN) ? answer[n] : 0;
        printf("%d: %lld\n", n, r);
    }
    return 0;
}
