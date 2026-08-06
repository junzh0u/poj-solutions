// POJ 1390 - Blocks
// Model: gpt-5.6-terra
// Compress adjacent equal boxes, then use interval DP.
// solve(left, right, extra) keeps extra boxes of color[right] attached on its right.
// Either remove that enlarged segment now, or first clear a matching earlier segment to merge it.

#include <algorithm>
#include <cstdio>
#include <cstring>

int color[205];
int length[205];
int dp[205][205][205];

int solve(int left, int right, int extra) {
    int middle;
    if (left > right) {
        return 0;
    }
    int &result = dp[left][right][extra];
    if (result != -1) {
        return result;
    }
    result = solve(left, right - 1, 0) + (length[right] + extra) * (length[right] + extra);
    for (middle = left; middle < right; ++middle) {
        if (color[middle] == color[right]) {
            result = std::max(result,
                               solve(left, middle, length[right] + extra) +
                                   solve(middle + 1, right - 1, 0));
        }
    }
    return result;
}

int main() {
    int tests;
    int caseNumber;
    if (scanf("%d", &tests) != 1) {
        return 0;
    }
    for (caseNumber = 1; caseNumber <= tests; ++caseNumber) {
        int boxes;
        int groups;
        int index;
        int value;
        scanf("%d", &boxes);
        groups = 0;
        for (index = 0; index < boxes; ++index) {
            scanf("%d", &value);
            if (groups == 0 || color[groups] != value) {
                ++groups;
                color[groups] = value;
                length[groups] = 1;
            } else {
                ++length[groups];
            }
        }
        memset(dp, -1, sizeof(dp));
        printf("Case %d: %d\n", caseNumber, solve(1, groups, 0));
    }
    return 0;
}
