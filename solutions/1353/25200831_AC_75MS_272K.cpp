// POJ 1353 - Color Change of Go Game Pieces
// Model: gpt-5.6-terra
// Approach: Store white pieces as bits.  A new piece is white precisely when
// the two adjacent old pieces have different colors, so each repetition is
// one circular adjacent XOR.  Simulate the at most 10000 repetitions.
// The generated circle is indexed by its left old endpoint; this rotation does
// not affect the requested white-piece count.
#include <iostream>

using namespace std;

int main() {
    int n, m, k;
    while (cin >> n) {
        if (n == -1) break;
        char comma;
        cin >> comma >> m >> comma >> k;

        int cur[128] = {0};
        int next[128];
        int i, step;
        for (i = 0; i < m; ++i) cur[i] = 1;
        for (step = 0; step < k; ++step) {
            for (i = 0; i < n; ++i) next[i] = cur[i] ^ cur[(i + 1) % n];
            for (i = 0; i < n; ++i) cur[i] = next[i];
        }

        int answer = 0;
        for (i = 0; i < n; ++i) answer += cur[i];
        cout << n << ',' << m << ',' << k << ": " << answer << '\n';
    }
    return 0;
}
