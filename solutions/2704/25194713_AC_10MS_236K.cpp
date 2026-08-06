// POJ 2704 - Pascal's Travels
// Model: gpt-5.6-terra
// Dynamic programming: ways[r][c] is the number of paths reaching this square.
// Each square sends its count to the square exactly its digit away to the right
// and down, provided that destination remains on the board. The destination is
// excluded from expansion, since its zero is the terminal marker.
#include <iostream>

using namespace std;

int main() {
    int n;
    while (cin >> n && n != -1) {
        int board[35][35];
        long long ways[35][35];
        int r;
        int c;

        for (r = 0; r < n; ++r) {
            for (c = 0; c < n; ++c) {
                char digit;
                cin >> digit;
                board[r][c] = digit - '0';
                ways[r][c] = 0;
            }
        }

        ways[0][0] = 1;
        for (r = 0; r < n; ++r) {
            for (c = 0; c < n; ++c) {
                int step;
                if (r == n - 1 && c == n - 1) {
                    continue;
                }
                step = board[r][c];
                if (step == 0) {
                    continue;
                }
                if (r + step < n) {
                    ways[r + step][c] += ways[r][c];
                }
                if (c + step < n) {
                    ways[r][c + step] += ways[r][c];
                }
            }
        }
        cout << ways[n - 1][n - 1] << '\n';
    }
    return 0;
}
