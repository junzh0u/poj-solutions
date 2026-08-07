// POJ 2279 - Mr. Young's Picture Permutations
// Model: claude-opus-5
//
// Count standard Young tableaux of the given shape (at most 5 rows, at most 30
// cells).  Label the students 1..N by decreasing height; a valid photo is
// exactly a filling of the shape in which labels increase left to right along
// each row and increase from the back row to the front row down each column.
//
// DP over the "staircase" of partially filled shapes: a state (a0..a4) is the
// number of students already seated in each row, and it is reachable only when
// a0 >= a1 >= ... >= a4 (the filled cells must themselves form a Young
// diagram).  Placing label m at the end of row i is legal iff row i still has
// room and the row below it is strictly shorter after the placement, so
//   dp[a] = sum over i of dp[a - e_i]   for a[i] > 0 and (i == 4 || a[i] > a[i+1]).
// dp[shape] is the answer.  Rows beyond k are padded with length 0, which makes
// the padded state automatically valid and the k == 4 case need no special code.
//
// The table is allocated flat with size prod(n_i + 1), at most 7^5 = 16807
// entries when the 30 students are split evenly over 5 rows, so both time and
// memory are negligible.
//
// The statement promises every answer fits in an unsigned 32 bit integer (the
// discuss board has two reports of WA from using a *signed* 32 bit type -- e.g.
// the shape 6 5 4 3 2 already gives 141892608 and larger legal shapes overflow
// int).  This uses a 64 bit unsigned accumulator and prints through std::cout
// so no %u/%llu format question arises at all.

#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int k;
    while (scanf("%d", &k) == 1 && k != 0) {
        int n[5];
        int i;
        for (i = 0; i < 5; ++i) n[i] = 0;
        for (i = 0; i < k && i < 5; ++i) scanf("%d", &n[i]);

        int st[5];
        int size = 1;
        for (i = 4; i >= 0; --i) { st[i] = size; size *= n[i] + 1; }

        vector<unsigned long long> dp(size, 0ULL);
        dp[0] = 1ULL;

        int a[5];
        for (a[0] = 0; a[0] <= n[0]; ++a[0])
        for (a[1] = 0; a[1] <= n[1]; ++a[1])
        for (a[2] = 0; a[2] <= n[2]; ++a[2])
        for (a[3] = 0; a[3] <= n[3]; ++a[3])
        for (a[4] = 0; a[4] <= n[4]; ++a[4]) {
            bool ok = true;
            for (i = 0; i < 4; ++i) if (a[i] < a[i + 1]) ok = false;
            if (!ok) continue;

            int idx = 0;
            for (i = 0; i < 5; ++i) idx += a[i] * st[i];
            if (idx == 0) continue;  // dp[0] is the seeded empty shape

            unsigned long long t = 0ULL;
            for (i = 0; i < 5; ++i)
                if (a[i] > 0 && (i == 4 || a[i] > a[i + 1]))
                    t += dp[idx - st[i]];
            dp[idx] = t;
        }

        cout << dp[size - 1] << "\n";
    }
    return 0;
}
