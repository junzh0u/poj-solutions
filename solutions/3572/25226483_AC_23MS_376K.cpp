// POJ 3572 - Hanoi Towers
// Model: gpt-6-astra
// Summarize the first transfer of each tower size from each starting peg.
// Between transfers of the smaller tower, the largest disk has one legal
// move: the smallest disk just moved and cannot move again immediately.
// A larger disk below a smaller tower does not affect that tower's strategy.
// Stop at either other peg, as required; a fixed destination is not imposed.
#include <iostream>
#include <string>
using namespace std;
int main() {
    int n;
    if (!(cin >> n)) return 0;
    int to[31][3];
    long long count[31][3];
    for (int s = 0; s < 3; ++s) to[1][s] = -1;
    for (int i = 0; i < 6; ++i) {
        string move; cin >> move;
        int s = move[0] - 'A', t = move[1] - 'A';
        if (to[1][s] == -1) { to[1][s] = t; count[1][s] = 1; }
    }
    for (int k = 2; k <= n; ++k) {
        for (int s = 0; s < 3; ++s) {
            int large = s, small = to[k-1][s];
            long long steps = count[k-1][s];
            while (small != large) {
                large = 3 - large - small;
                ++steps;
                steps += count[k-1][small];
                small = to[k-1][small];
            }
            to[k][s] = large;
            count[k][s] = steps;
        }
    }
    cout << count[n][0] << '\n';
    return 0;
}
