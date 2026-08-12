// POJ 2283 - Different Digits
// Model: gpt-5.6-terra
// A two-digit multiple always exists: append enough zeroes to a repunit for
// the part of n coprime to 10.  Try every possible one-digit multiple first;
// otherwise BFS residues for every digit pair. BFS in digit order gives the
// shortest, then lexicographically smallest, multiple for that pair. The
// board's 18 discussion confirms that "smallest" means the numeric result.
#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

static string smallestWithDigits(int n, int a, int b) {
    vector<int> parent(n, -2), used(n, -1);
    queue<int> q;
    int d;
    for (d = 1; d <= 9; ++d) {
        if (d != a && d != b) continue;
        int r = d % n;
        if (parent[r] != -2) continue;
        parent[r] = -1;
        used[r] = d;
        q.push(r);
    }
    while (!q.empty()) {
        int r = q.front(); q.pop();
        if (r == 0) break;
        for (d = 0; d <= 9; ++d) {
            if (d != a && d != b) continue;
            int nr = (r * 10 + d) % n;
            if (parent[nr] != -2) continue;
            parent[nr] = r;
            used[nr] = d;
            q.push(nr);
        }
    }
    if (parent[0] == -2) return "";
    string ans;
    for (int r = 0; r != -1; r = parent[r]) ans += char('0' + used[r]);
    string rev;
    for (int i = (int)ans.size() - 1; i >= 0; --i) rev += ans[i];
    return rev;
}

int main() {
    int n;
    while (cin >> n && n) {
        string best;
        for (int d = 1; d <= 9; ++d) {
            string s = smallestWithDigits(n, d, d);
            if (!s.empty() && (best.empty() || s.size() < best.size() ||
                (s.size() == best.size() && s < best))) best = s;
        }
        if (best.empty()) {
            for (int a = 0; a <= 9; ++a)
                for (int b = a + 1; b <= 9; ++b) {
                    string s = smallestWithDigits(n, a, b);
                    if (!s.empty() && (best.empty() || s.size() < best.size() ||
                        (s.size() == best.size() && s < best))) best = s;
                }
        }
        cout << best << '\n';
    }
    return 0;
}
