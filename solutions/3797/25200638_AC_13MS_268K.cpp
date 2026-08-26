// POJ 3797 - Tiling a Grid With Dominoes
// Model: gpt-5.6-terra
// Approach: frontier DP over the four cells exposed at the next column.
// Each state records which cells are already occupied by horizontal dominoes
// from the preceding column; a DFS fills the remaining cells and creates the
// next frontier mask. The statement guarantees every requested answer fits a
// signed 32-bit integer.

#include <iostream>
#include <cstring>
using namespace std;

int trans[16][16];

void fillColumn(int occupied, int nextMask, int startMask, int ways) {
    int row;
    for (row = 0; row < 4; ++row) {
        if ((occupied & (1 << row)) == 0) break;
    }
    if (row == 4) {
        trans[startMask][nextMask] += ways;
        return;
    }
    if (row + 1 < 4 && (occupied & (1 << (row + 1))) == 0) {
        fillColumn(occupied | (1 << row) | (1 << (row + 1)), nextMask,
                   startMask, ways);
    }
    fillColumn(occupied | (1 << row), nextMask | (1 << row), startMask, ways);
}

int main() {
    int start, width, cases, number;
    int dp[16], next[16];
    for (start = 0; start < 16; ++start) fillColumn(start, 0, start, 1);

    cin >> cases;
    for (number = 1; number <= cases; ++number) {
        cin >> width;
        memset(dp, 0, sizeof(dp));
        dp[0] = 1;
        while (width-- > 0) {
            int from, to;
            memset(next, 0, sizeof(next));
            for (from = 0; from < 16; ++from) {
                for (to = 0; to < 16; ++to) {
                    next[to] += dp[from] * trans[from][to];
                }
            }
            memcpy(dp, next, sizeof(dp));
        }
        cout << number << " " << dp[0] << "\n";
    }
    return 0;
}
