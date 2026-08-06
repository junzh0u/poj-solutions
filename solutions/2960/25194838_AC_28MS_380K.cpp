// POJ 2960 - S-Nim
// Model: gpt-5.6-terra
// Compute each one-heap subtraction game's Sprague-Grundy value up to the
// largest queried heap size, then xor the values for every queried position.
// A zero-sized heap is legal input and has Grundy value zero.
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int k;
    while (cin >> k && k != 0) {
        vector<int> moves(k);
        int i;
        for (i = 0; i < k; ++i) cin >> moves[i];
        sort(moves.begin(), moves.end());
        moves.erase(unique(moves.begin(), moves.end()), moves.end());

        int m;
        cin >> m;
        vector< vector<int> > positions(m);
        int max_heap = 0;
        for (i = 0; i < m; ++i) {
            int heaps;
            cin >> heaps;
            positions[i].resize(heaps);
            int j;
            for (j = 0; j < heaps; ++j) {
                cin >> positions[i][j];
                if (positions[i][j] > max_heap) max_heap = positions[i][j];
            }
        }

        vector<int> grundy(max_heap + 1, 0);
        bool seen[102];
        for (i = 1; i <= max_heap; ++i) {
            memset(seen, 0, sizeof(seen));
            vector<int>::const_iterator it;
            for (it = moves.begin(); it != moves.end() && *it <= i; ++it) {
                seen[grundy[i - *it]] = true;
            }
            int g = 0;
            while (seen[g]) ++g;
            grundy[i] = g;
        }

        for (i = 0; i < m; ++i) {
            int nim_sum = 0;
            vector<int>::const_iterator it;
            for (it = positions[i].begin(); it != positions[i].end(); ++it) {
                nim_sum ^= grundy[*it];
            }
            cout << (nim_sum == 0 ? 'L' : 'W');
        }
        cout << '\n';
    }
    return 0;
}
