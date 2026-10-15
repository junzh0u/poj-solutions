// POJ 1544 - A Puzzling Problem
// Model: gpt-6.1-sol
// Enumerate translations of each fixed-orientation piece in input order.
// Try top-left positions in row-major order and stop at the first tiling.
// The board is always 4 by 4; discussion confirms the judge's search order.
#include <iostream>
#include <vector>
#include <string>
using namespace std;
vector<int> moves[5];
int chosen[5], n;
bool solve(int k, int occupied) {
    if (k == n) return occupied == 65535;
    for (int i = 0; i < (int)moves[k].size(); ++i) {
        int mask = moves[k][i];
        if (!(mask & occupied)) {
            chosen[k] = mask;
            if (solve(k + 1, occupied | mask)) return true;
        }
    }
    return false;
}
int main() {
    bool first = true;
    while (cin >> n && n) {
        int area = 0;
        for (int k = 0; k < n; ++k) {
            moves[k].clear();
            int r, c; cin >> r >> c;
            vector<string> a(r);
            int minr = 4, minc = 4, maxr = -1, maxc = -1;
            for (int y = 0; y < r; ++y) {
                cin >> a[y];
                for (int x = 0; x < c; ++x) if (a[y][x] == '1') {
                    ++area;
                    if (y < minr) minr = y;
                    if (y > maxr) maxr = y;
                    if (x < minc) minc = x;
                    if (x > maxc) maxc = x;
                }
            }
            for (int y = 0; y + maxr - minr < 4; ++y)
                for (int x = 0; x + maxc - minc < 4; ++x) {
                    int mask = 0;
                    for (int i = minr; i <= maxr; ++i)
                        for (int j = minc; j <= maxc; ++j)
                            if (a[i][j] == '1') mask |= 1 << ((y+i-minr)*4+x+j-minc);
                    moves[k].push_back(mask);
                }
        }
        if (!first) cout << '\n';
        first = false;
        if (area != 16 || !solve(0, 0)) cout << "No solution possible\n";
        else {
            for (int y = 0; y < 4; ++y) {
                for (int x = 0; x < 4; ++x)
                    for (int k = 0; k < n; ++k)
                        if (chosen[k] & (1 << (4*y+x))) cout << k+1;
                cout << '\n';
            }
        }
    }
}
