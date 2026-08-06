// POJ 2311 - Cutting Game
// Model: claude-sonnet-5
// Approach: A rectangular piece can only be safely cut into two sub-pieces
// that both still have both dimensions >= 2; cutting off a 1xk (k>=2) strip
// instead hands the opponent an immediate win (they cut the strip into a
// 1x1 + 1x(k-1), which is the stated win condition). So a piece with both
// dimensions in {2,3} has no safe move and is a Grundy-0 terminal state;
// every other piece's Grundy number is the mex over splitting along either
// dimension, restricted to safe cuts, XORing the two resulting pieces'
// Grundy numbers (standard disjunctive-sum theory once the "instant win by
// exposing a stick" case is folded into "no safe move = terminal"). The
// overall game is then a normal Sprague-Grundy game: first player wins iff
// g(W,H) != 0.
// No statement ambiguity beyond the above; confirmed against poj bbs
// discussion for problem 2311, which independently states 2x2/2x3/3x2/3x3
// are the terminal (SG=0) shapes and everything else recurses from them.
#include <cstdio>
#include <cstring>

static int g[201][201];

int main() {
    for (int w = 2; w <= 200; w++) {
        for (int h = 2; h <= 200; h++) {
            if (w < 4 && h < 4) {
                g[w][h] = 0;
                continue;
            }
            bool seen[512];
            memset(seen, 0, sizeof(seen));
            if (w >= 4) {
                for (int a = 2; a <= w - 2; a++) {
                    int v = g[a][h] ^ g[w - a][h];
                    if (v < 512) seen[v] = true;
                }
            }
            if (h >= 4) {
                for (int b = 2; b <= h - 2; b++) {
                    int v = g[w][b] ^ g[w][h - b];
                    if (v < 512) seen[v] = true;
                }
            }
            int m = 0;
            while (m < 512 && seen[m]) m++;
            g[w][h] = m;
        }
    }

    int W, H;
    while (scanf("%d %d", &W, &H) == 2) {
        printf(g[W][H] != 0 ? "WIN\n" : "LOSE\n");
    }
    return 0;
}
