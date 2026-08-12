// POJ 3702 - Chessman
// Model: gpt-5.6-terra
// A move by four only rearranges chessmen within one residue class modulo 4.
// The unbounded empty suffix lets every required local pattern be assembled
// there, so positions are irrelevant.  BFS the four residue-class populations:
// *.*. becomes .*.* and **. becomes ..*.

#include <iostream>
#include <string>
#include <queue>
#include <cstring>
using namespace std;

int seen[31][31][31][31];

int code(const int c[4]) {
    return ((c[0] * 31 + c[1]) * 31 + c[2]) * 31 + c[3];
}

int main() {
    ios::sync_with_stdio(false);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        string a, b;
        cin >> a >> b;
        int start[4] = {0, 0, 0, 0};
        int goal[4] = {0, 0, 0, 0};
        for (int i = 0; i < 30; ++i) {
            if (a[i] == '*') ++start[i % 4];
            if (b[i] == '*') ++goal[i % 4];
        }
        memset(seen, 0, sizeof(seen));
        queue<int> q;
        seen[start[0]][start[1]][start[2]][start[3]] = true;
        q.push(code(start));
        while (!q.empty()) {
            int v = q.front(); q.pop();
            int c[4];
            for (int i = 3; i >= 0; --i) { c[i] = v % 31; v /= 31; }
            for (int r = 0; r < 4; ++r) {
                int d[4]; memcpy(d, c, sizeof(d));
                if (d[r] && d[(r + 2) % 4]) {
                    --d[r]; --d[(r + 2) % 4];
                    ++d[(r + 1) % 4]; ++d[(r + 3) % 4];
                    if (!seen[d[0]][d[1]][d[2]][d[3]]) {
                        seen[d[0]][d[1]][d[2]][d[3]] = true;
                        q.push(code(d));
                    }
                }
                memcpy(d, c, sizeof(d));
                if (d[r] && d[(r + 1) % 4]) {
                    --d[r]; --d[(r + 1) % 4]; ++d[(r + 2) % 4];
                    if (!seen[d[0]][d[1]][d[2]][d[3]]) {
                        seen[d[0]][d[1]][d[2]][d[3]] = true;
                        q.push(code(d));
                    }
                }
            }
        }
        cout << (seen[goal[0]][goal[1]][goal[2]][goal[3]] ? "YES" : "NO") << '\n';
    }
    return 0;
}
