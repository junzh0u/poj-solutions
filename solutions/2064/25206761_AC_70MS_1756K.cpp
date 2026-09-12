// POJ 2064 - Pipes
// Model: claude-sonnet-5
// Approach: minimum-cost Hamiltonian cycle on an r x c grid graph (r,c<=10),
// solved with broken-profile "plug DP" (connectivity-state DP) using a
// base-3 encoded profile of length c+1. Positions 0..c-1 hold the vertical
// ("down") plug type for each column; position c is a rolling scratch
// register holding the current horizontal ("left") plug while scanning a
// row left to right. Plug types: 0 = none, 1 = '(' (open, partner lies to
// the right in local spatial order), 2 = ')' (close, partner lies to the
// left); when a cell opens a brand-new pair its down-plug is '(' and its
// right-plug is ')' (empirically confirmed against brute force -- the
// reverse convention produced silently-closed sub-cycles that a naive
// derivation of the spatial order missed). Standard bracket matching is
// used to merge two different paths (relabelling their far endpoints) or,
// at the very last cell, to close the single remaining path into the
// Hamiltonian cycle.
// No ambiguity in the statement: exactly one circuit must visit every
// module (all interior walls carry a cost, i.e. every grid edge exists),
// so the answer is simply the minimum weight Hamiltonian cycle. Verified
// against the given sample and against 10 cases of official contest data
// posted on the problem's discuss board (message 343641/343642).
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

static const int INF = 1000000000;

int R, C;
int costRight[10][9]; // costRight[i][j]: edge (i,j)-(i,j+1), j=0..C-2
int costDown[9][10];  // costDown[i][j]: edge (i,j)-(i+1,j), i=0..R-2

int pow3[13];

int solve() {
    int width = C + 1; // digits 0..C-1 vertical, digit C = horizontal scratch
    int nstates = pow3[width];
    vector<int> dp(nstates, INF), ndp;
    dp[0] = 0;
    vector<int> digits(width), v(width), ndigits(width);

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            ndp.assign(nstates, INF);
            bool canRight = (j < C - 1);
            bool canDown = (i < R - 1);
            for (int s = 0; s < nstates; s++) {
                if (dp[s] >= INF) continue;
                int base = dp[s];
                // decode
                int t = s;
                for (int k = 0; k < width; k++) { digits[k] = t % 3; t /= 3; }
                int L = digits[C];
                int U = digits[j];

                if (L == 0 && U == 0) {
                    if (canRight && canDown) {
                        ndigits = digits;
                        ndigits[C] = 2; // right plug: close
                        ndigits[j] = 1; // down plug: open
                        int ns = 0;
                        for (int k = width - 1; k >= 0; k--) ns = ns * 3 + ndigits[k];
                        int nc = base + costRight[i][j] + costDown[i][j];
                        if (nc < ndp[ns]) ndp[ns] = nc;
                    }
                } else if (L != 0 && U == 0) {
                    if (canRight) {
                        ndigits = digits;
                        ndigits[C] = L;
                        ndigits[j] = 0;
                        int ns = 0;
                        for (int k = width - 1; k >= 0; k--) ns = ns * 3 + ndigits[k];
                        int nc = base + costRight[i][j];
                        if (nc < ndp[ns]) ndp[ns] = nc;
                    }
                    if (canDown) {
                        ndigits = digits;
                        ndigits[j] = L;
                        ndigits[C] = 0;
                        int ns = 0;
                        for (int k = width - 1; k >= 0; k--) ns = ns * 3 + ndigits[k];
                        int nc = base + costDown[i][j];
                        if (nc < ndp[ns]) ndp[ns] = nc;
                    }
                } else if (L == 0 && U != 0) {
                    if (canRight) {
                        ndigits = digits;
                        ndigits[C] = U;
                        ndigits[j] = 0;
                        int ns = 0;
                        for (int k = width - 1; k >= 0; k--) ns = ns * 3 + ndigits[k];
                        int nc = base + costRight[i][j];
                        if (nc < ndp[ns]) ndp[ns] = nc;
                    }
                    if (canDown) {
                        ndigits = digits;
                        ndigits[j] = U;
                        ndigits[C] = 0;
                        int ns = 0;
                        for (int k = width - 1; k >= 0; k--) ns = ns * 3 + ndigits[k];
                        int nc = base + costDown[i][j];
                        if (nc < ndp[ns]) ndp[ns] = nc;
                    }
                } else {
                    // both L,U present: build virtual array v[0..C] in true
                    // spatial order: v[k]=digits[k] for k<j; v[j]=L; v[j+1]=U;
                    // v[k]=digits[k-1] for k>j+1
                    for (int k = 0; k < j; k++) v[k] = digits[k];
                    v[j] = L;
                    v[j + 1] = U;
                    for (int k = j + 2; k <= C; k++) v[k] = digits[k - 1];

                    int PL = -1, PU = -1;
                    if (L == 1) {
                        int bal = 1;
                        for (int k = j + 1; k <= C; k++) {
                            if (v[k] == 1) bal++;
                            else if (v[k] == 2) bal--;
                            if (bal == 0) { PL = k; break; }
                        }
                    } else {
                        int bal = 1;
                        for (int k = j - 1; k >= 0; k--) {
                            if (v[k] == 2) bal++;
                            else if (v[k] == 1) bal--;
                            if (bal == 0) { PL = k; break; }
                        }
                    }
                    if (U == 1) {
                        int bal = 1;
                        for (int k = j + 2; k <= C; k++) {
                            if (v[k] == 1) bal++;
                            else if (v[k] == 2) bal--;
                            if (bal == 0) { PU = k; break; }
                        }
                    } else {
                        int bal = 1;
                        for (int k = j - 1; k >= 0; k--) {
                            if (v[k] == 2) bal++;
                            else if (v[k] == 1) bal--;
                            if (bal == 0) { PU = k; break; }
                        }
                    }

                    bool isLastCell = (i == R - 1 && j == C - 1);
                    if (PL == j + 1) {
                        // matched to each other: closes a cycle here.
                        if (isLastCell) {
                            // resulting state: everything cleared -> state 0
                            if (base < ndp[0]) ndp[0] = base;
                        }
                        // else invalid (premature sub-cycle), skip
                    } else {
                        // merge two different paths
                        v[j] = 0;
                        v[j + 1] = 0;
                        int a = PL < PU ? PL : PU;
                        int b = PL < PU ? PU : PL;
                        v[a] = 1;
                        v[b] = 2;
                        // map back to real digits
                        for (int k = 0; k < j; k++) ndigits[k] = v[k];
                        ndigits[C] = v[j];       // = 0
                        ndigits[j] = v[j + 1];   // = 0
                        for (int k = j + 2; k <= C; k++) ndigits[k - 1] = v[k];
                        int ns = 0;
                        for (int k = width - 1; k >= 0; k--) ns = ns * 3 + ndigits[k];
                        if (base < ndp[ns]) ndp[ns] = base;
                    }
                }
            }
            dp.swap(ndp);
        }
    }
    return dp[0];
}

int main() {
    pow3[0] = 1;
    for (int i = 1; i <= 12; i++) pow3[i] = pow3[i - 1] * 3;

    int n;
    if (!(cin >> n)) return 0;
    string dummy;
    getline(cin, dummy); // consume rest of line
    while (n--) {
        cin >> R >> C;
        getline(cin, dummy); // consume rest of "r c" line
        vector<string> rows(2 * R + 1);
        for (int k = 0; k < 2 * R + 1; k++) {
            getline(cin, rows[k]);
        }
        for (int i = 0; i < R; i++) {
            const string &line = rows[2 * i + 1];
            for (int j = 0; j + 1 < C; j++) {
                char ch = line[2 * j + 2];
                costRight[i][j] = ch - '0';
            }
        }
        for (int i = 0; i + 1 < R; i++) {
            const string &line = rows[2 * i + 2];
            for (int j = 0; j < C; j++) {
                char ch = line[2 * j + 1];
                costDown[i][j] = ch - '0';
            }
        }
        printf("%d\n", solve());
    }
    return 0;
}
