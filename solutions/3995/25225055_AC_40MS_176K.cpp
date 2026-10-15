// POJ 3995 - Hop - Don't Walk!
// Model: gpt-6.1-sol
// Enumerate move sequences of length at most nine, pruning immediate inverses.
// Maintain the number of black runs while ignoring F; at most one is a goal.
// The strict "less than 10" limit excludes solutions needing ten moves.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
string s;
int n, best;
int contribution(int i) {
    if (s[i] != 'B') return 0;
    int j = i - 1;
    if (j >= 0 && s[j] == 'F') --j;
    return j < 0 || s[j] != 'B';
}
void search(int p, int previous, int depth, int runs) {
    if (runs <= 1) { best = min(best, depth); return; }
    if (depth + 1 >= best) return;
    for (int d = -2; d <= 2; ++d) {
        if (!d) continue;
        int q = p + d;
        if (q < 0 || q >= n || q == previous) continue;
        int left = min(p, q), right = min(n - 1, max(p, q) + 2);
        int changed = runs;
        for (int i = left; i <= right; ++i) changed -= contribution(i);
        char tile = s[q];
        s[p] = (d == 2 || d == -2) ? (tile == 'B' ? 'W' : 'B') : tile;
        s[q] = 'F';
        for (int i = left; i <= right; ++i) changed += contribution(i);
        search(q, p, depth + 1, changed);
        s[p] = 'F'; s[q] = tile;
    }
}
int main() {
    int tc = 0;
    while (cin >> s && s[0] != '-') {
        n = s.size(); best = 10;
        int runs = 0, p = 0;
        for (int i = 0; i < n; ++i) {
            runs += contribution(i);
            if (s[i] == 'F') p = i;
        }
        search(p, -1, 0, runs);
        cout << ++tc << ". " << (best == 10 ? -1 : best) << '\n';
    }
}
