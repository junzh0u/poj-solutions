// POJ 1280 - Game
// Model: claude-sonnet-5
//
// Two-level probability DP.
//
// Inner level: for a single game (first to L points wins), compute the
// probability that A wins the game given which team serves the very first
// round of that game. Within a game, the server of round i>1 is whoever won
// round i-1, so this is a DP over (pointsA, pointsB, currentServer) filled
// in order of increasing pointsA+pointsB.
//
// Outer level: within a match (first to K game wins), the server of round 1
// of game i (i>1) is fixed to be the team that did NOT serve round 1 of game
// i-1 -- this is a deterministic rule, independent of who actually won that
// previous game. So once the coin flip for game 1's first server is fixed,
// every game's first server is fixed too (strictly alternating A,B,A,B,...
// or B,A,B,A,...). The match outcome is then a DP over (gamesWonByA,
// gamesWonByB) using the appropriate single-game win probability (GA or GB,
// precomputed once) for whichever team serves first in that game index.
// The overall answer averages the two coin-flip branches with weight 1/2
// each, since the very first game of the very first match gives each team
// an equal chance to serve first.
//
// Ambiguity check: statement's rule for a game's first server depends only
// on who served first in the *previous* game, not on who won it -- this was
// read literally and verified against both samples (93.8 and 99.5) by hand
// before coding, since a subtly different reading (server determined by
// game winner) would not have reproduced them.

#include <cstdio>
#include <cstring>
using namespace std;

static double gdp[101][101][2];

// probability A wins a single game to L points, given which team serves
// round 1 of the game (0 = A, 1 = B).
double gameWinProb(double pa, double pb, int L, int startServer) {
    for (int a = 0; a < L; a++)
        for (int b = 0; b < L; b++) {
            gdp[a][b][0] = 0.0;
            gdp[a][b][1] = 0.0;
        }
    gdp[0][0][startServer] = 1.0;
    double winA = 0.0;
    for (int sum = 0; sum <= 2 * (L - 1); sum++) {
        int aLo = sum - (L - 1);
        if (aLo < 0) aLo = 0;
        int aHi = sum < (L - 1) ? sum : (L - 1);
        for (int a = aLo; a <= aHi; a++) {
            int b = sum - a;
            for (int srv = 0; srv < 2; srv++) {
                double prob = gdp[a][b][srv];
                if (prob <= 0.0) continue;
                double pAWinsRound = (srv == 0) ? pa : (1.0 - pb);
                double pBWinsRound = 1.0 - pAWinsRound;
                if (a + 1 == L) winA += prob * pAWinsRound;
                else gdp[a + 1][b][0] += prob * pAWinsRound;
                if (b + 1 == L) { /* B wins the game */ }
                else gdp[a][b + 1][1] += prob * pBWinsRound;
            }
        }
    }
    return winA;
}

static double mdp[101][101];

// probability A wins the match given game 1's first server, using the
// precomputed single-game win probabilities GA/GB.
double matchWinProb(double GA, double GB, int K, int startServer) {
    for (int a = 0; a < K; a++)
        for (int b = 0; b < K; b++)
            mdp[a][b] = 0.0;
    mdp[0][0] = 1.0;
    double totalA = 0.0;
    for (int sum = 0; sum <= 2 * (K - 1); sum++) {
        int aLo = sum - (K - 1);
        if (aLo < 0) aLo = 0;
        int aHi = sum < (K - 1) ? sum : (K - 1);
        for (int a = aLo; a <= aHi; a++) {
            int b = sum - a;
            double prob = mdp[a][b];
            if (prob <= 0.0) continue;
            int gameIdx = a + b;
            int server = (gameIdx % 2 == 0) ? startServer : (1 - startServer);
            double g = (server == 0) ? GA : GB;
            if (a + 1 == K) totalA += prob * g;
            else mdp[a + 1][b] += prob * g;
            if (b + 1 == K) { /* B wins the match */ }
            else mdp[a][b + 1] += prob * (1.0 - g);
        }
    }
    return totalA;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int Pa, Pb, K, L;
        scanf("%d %d %d %d", &Pa, &Pb, &K, &L);
        double pa = Pa / 100.0, pb = Pb / 100.0;
        double GA = gameWinProb(pa, pb, L, 0);
        double GB = gameWinProb(pa, pb, L, 1);
        double ans = 0.5 * matchWinProb(GA, GB, K, 0) + 0.5 * matchWinProb(GA, GB, K, 1);
        printf("%.1f\n", ans * 100.0);
    }
    return 0;
}
