// POJ 2807 - Bingo!
// Model: gpt-6.1-sol
// Enumerate unions of exactly Y input patterns and minimize column deficits.
// The center is free. More than Y patterns cannot improve a union's cost;
// called counts describe a possible card, so each column uses its best overlap.
#include <iostream>
#include <string>
using namespace std;
unsigned int pat[19];
int called[5], bits[32], n, need, best;
void search(int first, int left, unsigned int mask) {
    if (best == 0) return;
    if (left == 0) {
        int cost = 0;
        for (int c = 0; c < 5; ++c) {
            int deficit = bits[(mask >> (5*c)) & 31] - called[c];
            if (deficit > 0) cost += deficit;
        }
        if (cost < best) best = cost;
        return;
    }
    for (int i = first; i <= n-left; ++i)
        search(i+1, left-1, mask | pat[i]);
}
int main() {
    for (int i = 1; i < 32; ++i) bits[i] = bits[i/2] + i%2;
    int cases;
    cin >> cases;
    while (cases--) {
        for (int c = 0; c < 5; ++c) cin >> called[c];
        cin >> n >> need;
        for (int i = 0; i < n; ++i) pat[i] = 0;
        for (int r = 0; r < 5; ++r) {
            for (int i = 0; i < n; ++i) {
                string row; cin >> row;
                for (int c = 0; c < 5; ++c)
                    if (row[c] == 'X' && !(r == 2 && c == 2))
                        pat[i] |= 1U << (5*c+r);
            }
        }
        best = 25;
        search(0, need, 0);
        cout << best << '\n';
    }
    return 0;
}
