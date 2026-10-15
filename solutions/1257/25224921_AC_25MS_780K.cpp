// POJ 1257 - Cross-stitch.
// Model: gpt-6.1-sol
// Join endpoints of every diagonal and count face/rear degree differences.
// In each nonempty component, the answer is half the total imbalance,
// or one thread when all vertices balance. Rear coordinates are not mirrored.
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
const int V = 201 * 201;
int parent[V], diffdeg[V], total[V];
bool used[V];
int root(int x) { return parent[x] == x ? x : parent[x] = root(parent[x]); }
void edge(int a, int b, int sign) {
    int x = root(a), y = root(b);
    if (x != y) parent[x] = y;
    used[a] = used[b] = true;
    diffdeg[a] += sign; diffdeg[b] += sign;
}
int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;
    int count = (n + 1) * (m + 1);
    for (int i = 0; i < count; ++i) parent[i] = i;
    for (int side = 0; side < 2; ++side) {
        for (int i = 0; i < n; ++i) {
            string s; cin >> s;
            for (int j = 0; j < m; ++j) {
                int a = i * (m + 1) + j, sign = side ? -1 : 1;
                if (s[j] == '\\' || s[j] == 'X') edge(a, a + m + 2, sign);
                if (s[j] == '/' || s[j] == 'X') edge(a + 1, a + m + 1, sign);
            }
        }
    }
    for (int i = 0; i < count; ++i)
        if (used[i]) total[root(i)] += abs(diffdeg[i]);
    int answer = 0;
    for (int i = 0; i < count; ++i)
        if (used[i] && root(i) == i) answer += total[i] ? total[i] / 2 : 1;
    cout << answer << '\n';
    return 0;
}
