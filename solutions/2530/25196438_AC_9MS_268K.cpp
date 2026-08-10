// POJ 2530 - Tetris Alphabet
// Model: gpt-5.6-terra
// Build the precedence graph from every lower/upper pair in each column,
// then repeatedly take the smallest available letter in a topological sort.
// Figures only fall vertically, so empty cells between two figures do not
// remove the lower-before-upper constraint.

#include <cstring>
#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        string well[50];
        for (int r = 0; r < n; ++r) {
            cin >> well[r];
        }

        bool present[26];
        bool edge[26][26];
        int indegree[26];
        bool emitted[26];
        memset(present, 0, sizeof(present));
        memset(edge, 0, sizeof(edge));
        memset(indegree, 0, sizeof(indegree));
        memset(emitted, 0, sizeof(emitted));

        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < 20 && c < static_cast<int>(well[r].size()); ++c) {
                if (well[r][c] >= 'A' && well[r][c] <= 'Z') {
                    present[well[r][c] - 'A'] = true;
                }
            }
        }

        for (int c = 0; c < 20; ++c) {
            for (int upper = 0; upper < n; ++upper) {
                if (c >= static_cast<int>(well[upper].size()) ||
                    well[upper][c] < 'A' || well[upper][c] > 'Z') {
                    continue;
                }
                const int to = well[upper][c] - 'A';
                for (int lower = upper + 1; lower < n; ++lower) {
                    if (c >= static_cast<int>(well[lower].size()) ||
                        well[lower][c] < 'A' || well[lower][c] > 'Z') {
                        continue;
                    }
                    const int from = well[lower][c] - 'A';
                    if (from != to && !edge[from][to]) {
                        edge[from][to] = true;
                        ++indegree[to];
                    }
                }
            }
        }

        int remaining = 0;
        for (int i = 0; i < 26; ++i) {
            if (present[i]) {
                ++remaining;
            }
        }

        while (remaining > 0) {
            int next = -1;
            for (int i = 0; i < 26; ++i) {
                if (present[i] && !emitted[i] && indegree[i] == 0) {
                    next = i;
                    break;
                }
            }
            if (next == -1) {
                break;
            }

            cout << static_cast<char>('A' + next);
            emitted[next] = true;
            --remaining;
            for (int to = 0; to < 26; ++to) {
                if (edge[next][to]) {
                    --indegree[to];
                }
            }
        }
        cout << '\n';
    }

    return 0;
}
