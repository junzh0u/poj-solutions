// POJ 3394 - Route Planning
// Model: claude-sonnet-5
//
// n <= 100 spots with a direct-edge distance matrix (0 = no direct road,
// 1..10 = direct road weight). Each of r <= 10 routes lists a fixed set of
// spots that must appear on that route, in an unspecified middle order,
// first entry = start, last entry = end, every listed spot used exactly
// once, and non-listed spots must not be included (so consecutive route
// stops must be joined by a *direct* edge from the matrix, not a shortest
// path through other spots). This is a per-route minimum-weight Hamiltonian
// path over exactly the listed spots -> Held-Karp bitmask DP local to each
// route's stop list.
//
// The route lists have no announced length; the discuss board (message
// 79113/79140) reports the official data caps each route at 15 stops, which
// keeps 2^k*k well inside the 5000ms / 64MB limits. The same board (122454,
// 138992, 80071) reports scanf-based readers desyncing/WA/TLE on this input:
// each route line has a variable token count with no leading count field,
// so it must be read whole-line (getline) and tokenized, not scanf'd
// integer-by-integer against an assumed count.
//
// If no permutation of the middle stops yields an all-edges-present path
// from start to end, output 0 per the statement.
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

static int M[101][101];

int main(){
    int n, r;
    if (!(cin >> n >> r)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 1; i <= n; ++i) {
        string line;
        // A matrix row could in principle be split oddly; keep reading
        // lines until we have n values for this row.
        vector<int> vals;
        while ((int)vals.size() < n) {
            if (!getline(cin, line)) break;
            istringstream iss(line);
            int x;
            while (iss >> x) vals.push_back(x);
        }
        for (int j = 1; j <= n && j <= (int)vals.size(); ++j) M[i][j] = vals[j-1];
    }

    for (int t = 0; t < r; ++t) {
        string line;
        vector<int> stops;
        while (stops.empty()) {
            if (!getline(cin, line)) break;
            istringstream iss(line);
            int x;
            while (iss >> x) stops.push_back(x);
        }
        int k = (int)stops.size();
        if (k == 0) { printf("0\n"); continue; }
        if (k == 1) { printf("0\n"); continue; }

        int full = 1 << k;
        vector<vector<int> > dp(full, vector<int>(k, -1));
        dp[1][0] = 0; // start at index 0, mask={0}

        for (int mask = 1; mask < full; ++mask) {
            if (!(mask & 1)) continue; // must always include start
            for (int i = 0; i < k; ++i) {
                if (!(mask & (1 << i))) continue;
                int cur = dp[mask][i];
                if (cur < 0) continue;
                int from = stops[i];
                for (int j = 0; j < k; ++j) {
                    if (mask & (1 << j)) continue;
                    int to = stops[j];
                    int w = M[from][to];
                    if (w <= 0) continue; // no direct road
                    int nmask = mask | (1 << j);
                    int cand = cur + w;
                    if (dp[nmask][j] < 0 || cand < dp[nmask][j]) dp[nmask][j] = cand;
                }
            }
        }

        int ans = dp[full - 1][k - 1];
        if (ans < 0) ans = 0;
        printf("%d\n", ans);
    }
    return 0;
}
