// POJ 1916 - Rat Attack
// Model: gpt-5.6-terra
// Each nest contributes its population to every legal bomb center in a
// clipped Chebyshev square. A two-dimensional difference array applies all
// these rectangle additions, then prefix sums recover every candidate total.
// Bomb centers may lie on the 0..1024 boundary; clipping is therefore to the
// center grid, not to an interior region.
#include <iostream>
#include <cstring>

using namespace std;

const int LIMIT = 1025;
int diffGrid[LIMIT + 1][LIMIT + 1];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int scenarios;
    cin >> scenarios;
    while (scenarios--) {
        int d, n;
        cin >> d >> n;
        memset(diffGrid, 0, sizeof(diffGrid));
        int k;
        for (k = 0; k < n; ++k) {
            int x, y, population;
            cin >> x >> y >> population;
            int x1 = x - d < 0 ? 0 : x - d;
            int y1 = y - d < 0 ? 0 : y - d;
            int x2 = x + d >= LIMIT ? LIMIT - 1 : x + d;
            int y2 = y + d >= LIMIT ? LIMIT - 1 : y + d;
            diffGrid[x1][y1] += population;
            diffGrid[x2 + 1][y1] -= population;
            diffGrid[x1][y2 + 1] -= population;
            diffGrid[x2 + 1][y2 + 1] += population;
        }

        int best = -1;
        int bestX = 0;
        int bestY = 0;
        int x, y;
        for (x = 0; x < LIMIT; ++x) {
            for (y = 0; y < LIMIT; ++y) {
                if (x > 0) diffGrid[x][y] += diffGrid[x - 1][y];
                if (y > 0) diffGrid[x][y] += diffGrid[x][y - 1];
                if (x > 0 && y > 0) diffGrid[x][y] -= diffGrid[x - 1][y - 1];
                if (diffGrid[x][y] > best) {
                    best = diffGrid[x][y];
                    bestX = x;
                    bestY = y;
                }
            }
        }
        cout << bestX << ' ' << bestY << ' ' << best << '\n';
    }
    return 0;
}
