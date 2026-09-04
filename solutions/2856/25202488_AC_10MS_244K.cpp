// POJ 2856 - Medals
// Model: claude-sonnet-5
//
// A ranking scheme is a weight vector (n^-j, n^-k, n^-l) for integers j,k,l,
// where n is the total number of medals awarded in the whole test case
// (n < 100). Only the relative gaps between j,k,l matter (scaling all three
// exponents by the same constant does not change which country scores
// highest), so we may normalize the smallest exponent to 0 and ask how large
// the remaining gaps need to be. Because n is a shared budget across every
// country and every medal category, a gap of exactly 1 already guarantees
// full lexicographic domination: if two countries differ by d>=1 in a
// higher-priority category, that difference alone is worth d*n^e >= n^e,
// while everything of lower priority for every country combined can use at
// most n-d <= n-1 medals, worth at most (n-1)*n^(e-1) < n^e. So checking
// exponents drawn from {0,1,2} (all 27 combinations, ties included) already
// covers every distinct ranking rule reachable with three categories; larger
// gaps never produce a new outcome. (Verified separately against an exact
// rational brute force scanning exponents over a much wider range.)
//
// Ambiguity settled from the discuss board (message 49418/49381 and the
// USA 1 1 1 / Canada 1 1 1 example in 49376-49380): a tie counts as ranking
// first, so Canada wins whenever some scheme makes no other country's score
// strictly greater. Also (message 72733/75879): Canada may simply be absent
// from a test case's country list, in which case it cannot win.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

int main() {
    int c;
    while (scanf("%d", &c) == 1 && c != 0) {
        vector<string> names(c);
        vector<int> g(c), s(c), b(c);
        int total = 0;
        int canadaIdx = -1;
        char buf[64];
        for (int i = 0; i < c; i++) {
            scanf("%s %d %d %d", buf, &g[i], &s[i], &b[i]);
            names[i] = buf;
            total += g[i] + s[i] + b[i];
            if (names[i] == "Canada") canadaIdx = i;
        }

        bool win = false;
        if (canadaIdx != -1) {
            int n = total > 0 ? total : 1;
            int pw[3] = {1, n, n * n}; // pw[e] = n^e
            for (int j = 0; j < 3 && !win; j++) {
                for (int k = 0; k < 3 && !win; k++) {
                    for (int l = 0; l < 3 && !win; l++) {
                        int canScore = g[canadaIdx] * pw[2 - j] +
                                       s[canadaIdx] * pw[2 - k] +
                                       b[canadaIdx] * pw[2 - l];
                        bool ok = true;
                        for (int t = 0; t < c && ok; t++) {
                            if (t == canadaIdx) continue;
                            int sc = g[t] * pw[2 - j] + s[t] * pw[2 - k] + b[t] * pw[2 - l];
                            if (sc > canScore) ok = false;
                        }
                        if (ok) win = true;
                    }
                }
            }
        }

        printf(win ? "Canada wins!\n" : "Canada cannot win.\n");
    }
    return 0;
}
