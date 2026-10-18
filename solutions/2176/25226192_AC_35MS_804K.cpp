// POJ 2176 - Folding
// Model: gpt-6-astra
// Interval DP tries every concatenation and every exact repeated block.
// Reconstruction retains a shortest expression, including nested repetitions.
// Any shortest expression is valid; discussion reports multiple cases, so read EOF.
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

string best[101][101];
int main() {
    string s;
    while (cin >> s) {
        int n = static_cast<int>(s.size());
        for (int len = 1; len <= n; ++len) {
            for (int i = 0; i + len <= n; ++i) {
                int j = i + len;
                best[i][j] = s.substr(i, len);
                for (int k = i + 1; k < j; ++k) {
                    if (best[i][k].size() + best[k][j].size() < best[i][j].size())
                        best[i][j] = best[i][k] + best[k][j];
                }
                for (int p = 1; p < len; ++p) {
                    if (len % p != 0) continue;
                    bool repeated = true;
                    for (int k = p; k < len; ++k)
                        if (s[i + k] != s[i + k % p]) { repeated = false; break; }
                    if (!repeated) continue;
                    ostringstream count;
                    count << len / p;
                    string candidate = count.str() + "(" + best[i][i + p] + ")";
                    if (candidate.size() < best[i][j].size()) best[i][j] = candidate;
                }
            }
        }
        cout << best[0][n] << '\n';
    }
    return 0;
}
