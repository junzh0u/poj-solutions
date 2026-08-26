// POJ 2358 - Queens in peaceful positions
// Model: gpt-5.6-terra
// A final board retains all but three rows of the given N-queens permutation.
// Its changed rows therefore use the same columns; the two three-cycles are
// the only rearrangements that move exactly three occupied squares.
#include <iostream>
#include <cstring>
using namespace std;

int n, col[51], d1[101], d2[101];

bool put(int r, int c) {
    if (d1[r + c] || d2[r - c + n]) return false;
    ++d1[r + c];
    ++d2[r - c + n];
    return true;
}

void take(int r, int c) {
    --d1[r + c];
    --d2[r - c + n];
}

bool good3(int a, int b, int c, int x, int y, int z) {
    take(a, col[a]); take(b, col[b]); take(c, col[c]);
    bool p = put(a, x);
    bool q = p && put(b, y);
    bool r = q && put(c, z);
    if (r) take(c, z);
    if (q) take(b, y);
    if (p) take(a, x);
    put(a, col[a]); put(b, col[b]); put(c, col[c]);
    return r;
}

int main() {
    if (!(cin >> n)) return 0;
    memset(d1, 0, sizeof(d1));
    memset(d2, 0, sizeof(d2));
    for (int i = 1; i <= n; ++i) {
        int x, y; cin >> x >> y;
        col[x] = y; put(x, y);
    }
    long ans = 0;
    for (int a = 1; a <= n; ++a)
        for (int b = a + 1; b <= n; ++b)
            for (int c = b + 1; c <= n; ++c) {
                if (good3(a, b, c, col[b], col[c], col[a])) ++ans;
                if (good3(a, b, c, col[c], col[a], col[b])) ++ans;
            }
    cout << ans << '\n';
    return 0;
}
