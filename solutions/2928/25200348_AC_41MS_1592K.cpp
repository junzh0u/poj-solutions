// POJ 2928 - A City of Skyscrapers
// Model: gpt-5.6-terra
// Each row and column contributes one required maximum. Equal-height row and
// column requirements can share a cell; all other requirements need distinct
// cells. For the maximum, every cell can have height min(rowMax, colMax).
#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int m, n, c, C;
    if (scanf("%d%d%d%d", &m, &n, &c, &C) != 4) return 0;
    vector<int> west(m), south(n);
    vector<int> cw(20001, 0), cs(20001, 0);
    int mw = 0, ms = 0;
    int i;
    for (i = 0; i < m; ++i) {
        scanf("%d", &west[i]);
        ++cw[west[i]];
        if (west[i] > mw) mw = west[i];
    }
    for (i = 0; i < n; ++i) {
        scanf("%d", &south[i]);
        ++cs[south[i]];
        if (south[i] > ms) ms = south[i];
    }
    if (mw != ms) {
        printf("Impossible\n");
        return 0;
    }

    int minimumFloors = 0;
    if (c != 0) {
        for (i = 1; i <= 20000; ++i) {
            minimumFloors += i * max(cw[i], cs[i]);
        }
    }

    sort(south.begin(), south.end());
    vector<int> prefix(n + 1, 0);
    for (i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + south[i];
    int maximumFloors = 0;
    if (C != 0) {
        for (i = 0; i < m; ++i) {
            int p = (int)(lower_bound(south.begin(), south.end(), west[i]) - south.begin());
            maximumFloors += prefix[p] + (n - p) * west[i];
        }
    }

    printf("Minimum: %d, maximum: %d\n", minimumFloors * c, maximumFloors * C);
    return 0;
}
