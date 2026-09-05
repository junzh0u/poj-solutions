// POJ 3000 - Frogger
// Model: claude-sonnet-5
//
// Approach: layered BFS over discrete time steps t = 0, 1, ..., x. Represent
// the set of columns the frog could occupy in each row as a bitmask (m <= 50
// fits in a 64-bit word), and likewise represent each lane's car occupancy
// as a bitmask that is rotated by one bit every turn (cars wrap around their
// lane, hence a circular rotation; the frog never wraps, hence plain shifts
// with the overflow bit masked away). At each turn, first advance every
// lane's car mask by one rotation in its own direction, then compute, for
// every row, the set of columns reachable this turn as (stay | move-left |
// move-right | move-down-from-row-below-in-index | move-up-from-row-above)
// intersected with that row's currently-safe columns. Because bitwise ops
// process an entire row in O(1), the whole simulation is O(x*n), comfortably
// fast even though x can be 1e5 and there can be many scenarios.
//
// Row indexing: row 0 is the destination curb (top line of the input, holds
// G), rows 1..n are the lanes in the order given (row 1 is nearest the
// destination, row n is nearest the start), row n+1 is the starting curb
// (last input line, holds F). Curbs never contain cars and are always safe.
//
// Direction: the statement says the lane "closest to the starting point of
// the frog" moves right, and directions alternate lane to lane. That lane is
// row n (adjacent to the starting curb). So row i (1<=i<=n) moves right iff
// (n - i) is even, and left otherwise.
//
// Ambiguity / pitfalls checked against the discuss board: cars wrap around
// at their lane's boundary (circular shift) while the frog cannot (plain
// shift, overflow bit discarded, matching "the frog cannot wrap around");
// the frog may also just stand still, which one poster on the board missed;
// "the frog can jump over an adjacent approaching car" is automatically
// handled since safety is checked only at arrival time t+1, not at
// intermediate positions; and one thread reports a WA from assuming
// (-1 % 5 == 4) in C/C++, which this solution avoids entirely by using
// unsigned bit rotations instead of any signed modulo arithmetic.
#include <cstdio>
#include <cstring>

typedef unsigned long long u64;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int x, n, m;
        scanf("%d %d %d", &x, &n, &m);
        int rows = n + 2;
        static char buf[60];
        u64 full = (m == 64) ? ~0ULL : ((1ULL << m) - 1ULL);
        u64 laneMask[22];
        int dir[22]; // 0 = right (col increases), 1 = left (col decreases); only used for 1..n
        int gcol = -1, fcol = -1;
        for (int r = 0; r < rows; ++r) {
            scanf("%s", buf);
            u64 mask = 0;
            for (int c = 0; c < m; ++c) {
                if (buf[c] == 'X') mask |= (1ULL << c);
                if (buf[c] == 'G') gcol = c;
                if (buf[c] == 'F') fcol = c;
            }
            laneMask[r] = mask;
            if (r >= 1 && r <= n) {
                dir[r] = ((n - r) % 2 == 0) ? 0 : 1;
            }
        }

        u64 reach[22];
        memset(reach, 0, sizeof(reach));
        reach[rows - 1] = (1ULL << fcol);

        int answer = -1;
        u64 goalBit = (1ULL << gcol);

        for (int t = 1; t <= x; ++t) {
            // advance car masks for lanes 1..n
            for (int r = 1; r <= n; ++r) {
                u64 mask = laneMask[r];
                if (m == 1) {
                    // rotation is identity
                    continue;
                }
                if (dir[r] == 0) {
                    // right: col -> col+1, circular
                    u64 top = (mask >> (m - 1)) & 1ULL;
                    mask = ((mask << 1) & full) | top;
                } else {
                    // left: col -> col-1, circular
                    u64 bot = mask & 1ULL;
                    mask = (mask >> 1) | (bot << (m - 1));
                }
                laneMask[r] = mask;
            }

            u64 newReach[22];
            for (int r = 0; r < rows; ++r) {
                u64 cand = reach[r];
                cand |= (reach[r] << 1) & full;
                cand |= (reach[r] >> 1);
                if (r > 0) cand |= reach[r - 1];
                if (r < rows - 1) cand |= reach[r + 1];

                u64 safe;
                if (r == 0 || r == rows - 1) safe = full;
                else safe = full & ~laneMask[r];

                newReach[r] = cand & safe;
            }
            memcpy(reach, newReach, sizeof(u64) * rows);

            if (reach[0] & goalBit) {
                answer = t;
                break;
            }
        }

        if (answer >= 0) {
            printf("The minimum number of turns is %d.\n", answer);
        } else {
            printf("The problem has no solution.\n");
        }
    }
    return 0;
}
