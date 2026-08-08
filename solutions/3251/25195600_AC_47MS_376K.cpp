// POJ 3251 - Big Square
// Model: claude-opus-5
//
// N x N grid of gridpoints; 'J' John cow, 'B' Bob cow, '*' free. Exactly one
// more J (Bessie) may be placed on a free point. Find the max area of a square
// whose four corners are John cows, tilted squares included. So a square counts
// iff no corner is 'B' and at most one corner is '*'. The statement says the
// best square need not contain Bessie, and at least one free point is
// guaranteed, so an all-J square never conflicts with having to place her.
//
// Instead of enumerating pairs of J points (O(cows^2), which the board reports
// as TLE under G++), enumerate each square by its axis-aligned bounding box:
// a lattice square with bounding box side s has its corners at
//   (r, c+k)  (r+k, c+s)  (r+s, c+s-k)  (r+s-k, c),  0 <= k < s,
// and its own side^2 (= area) is k*k + (s-k)*(s-k). Every lattice square is hit
// exactly once, and the count is sum_s s*(N-s)^2 ~ N^4/12 = 8.3M for N = 100.
// Ordering by (s, k) and skipping any (s, k) whose area cannot beat the current
// best prunes further, but the unpruned bound is already fast enough.
//
// Verified against an independent brute force that rotates each candidate side
// vector by 90 degrees: 6000 random boards with n <= 9 and five n = 100 boards
// agree. The official sample is weak - of six mutants it kills only "require
// all four corners to be J" and "let B corners count"; allowing two free
// corners, restricting to axis-aligned squares, using s*s as the area, and
// moving one corner all reproduce it. The differential test kills all six.

#include <iostream>
#include <string>
using namespace std;

static char g[105][105];

int main() {
    int n;
    while (cin >> n) {
        if (n <= 0) break;
        for (int i = 0; i < n; i++) {
            string row;
            if (!(cin >> row)) row = "";
            for (int j = 0; j < n; j++)
                g[i][j] = (j < (int)row.size()) ? row[j] : '*';
        }
        int best = 0;
        for (int s = 1; s <= n - 1; s++) {
            for (int k = 0; k < s; k++) {
                int area = k * k + (s - k) * (s - k);
                if (area <= best) continue;
                bool found = false;
                for (int r = 0; r + s <= n - 1 && !found; r++) {
                    for (int c = 0; c + s <= n - 1; c++) {
                        char a = g[r][c + k];
                        if (a == 'B') continue;
                        char b = g[r + k][c + s];
                        if (b == 'B') continue;
                        char cc = g[r + s][c + s - k];
                        if (cc == 'B') continue;
                        char d = g[r + s - k][c];
                        if (d == 'B') continue;
                        int stars = (a == '*') + (b == '*') + (cc == '*') + (d == '*');
                        if (stars <= 1) { found = true; break; }
                    }
                }
                if (found) best = area;
            }
        }
        cout << best << endl;
    }
    return 0;
}
