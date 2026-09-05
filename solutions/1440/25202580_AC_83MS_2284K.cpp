// POJ 1440 - Varacious Steve
// Model: claude-sonnet-5
//
// Approach: the game is not simple Nim - "winning" a round (emptying the
// box) is not automatically good, because the round's loser's donuts go
// back into a smaller box for the next round, and the loser starts that
// next round. So the value of a position is a recursive game value, not a
// win/lose flag: f(n,m) = the total donuts the player who STARTS a round
// with n donuts in the box eventually eats under optimal play by both
// (the opponent, over the whole rest of the game, eats n - f(n,m), since
// every donut is eventually eaten by exactly one player - conservation
// holds because a round's winner eats w immediately and the round's
// loser's stash n' < n recurses into the same kind of subgame).
//
// Within one round we run the exact turn-by-turn minimax: state
// (r = donuts left in the box, accX/accY = what each player has taken so
// far this round, whose turn). When a move empties the box, the mover
// keeps their own round total w permanently; the other player's round
// total n' becomes the next round's box and that other player starts the
// next round, contributing f(n',m) to their own grand total and leaving
// n'-f(n',m) to the mover. This state naturally has no explicit
// dependence on which round-start n it came from (only r,accX,accY,turn
// matter, since accX+accY+r always equals that round's start size), so
// memoizing it once per m and reusing it across every round-start n from
// 1..100 keeps the whole table cheap (a couple MB, well under the 10MB
// limit, and comfortably inside the time limit even if every distinct m
// from 1..100 appears in the input, verified empirically).
//
// An initial hand-derived closed form (seize control at n mod (m+1)) was
// checked against a from-scratch brute force and found WRONG - a
// mover doesn't always want to "control" the exchange to be the one who
// empties the box (n=5,m=3 is a concrete counterexample: taking the
// control-seizing move of 1 gives only 2, but taking all 3 up front
// gives 3, which is the actual optimum) - so the full turn-level minimax
// is used instead of a shortcut formula. The discuss board's posted
// formula f(n,m)=min(n,m)+f(n-2m,m) was checked the same way and
// disagrees with this solution on the majority of small cases (e.g.
// n=9,m=1), consistent with the board's own reply calling it wrong.
// Verified against a hand-traced simulation for m=1 (fully deterministic
// game) for several n, and matches the sample (n=5,m=2 -> 3) exactly.
//
// Input is read to EOF (no case count given).

#include <cstdio>
using namespace std;

static int M;
static int fval[101];
// memo cube for the intra-round game, valid for the CURRENT M only.
// memo[r][accX][accY][turn] = X's total from this state onward, or -1 if unknown.
static signed char memo[101][101][101][2];

static int solve(int r, int accX, int accY, int turn){
    signed char &slot = memo[r][accX][accY][turn];
    if(slot != -1) return slot;
    int best = -1;
    int mm = r < M ? r : M;
    for(int t=1; t<=mm; t++){
        int nr = r - t;
        int xt;
        if(nr == 0){
            if(turn == 0){
                int w = accX + t;
                int nprime = accY;
                xt = w + (nprime - fval[nprime]);
            } else {
                int nprime = accX;
                xt = fval[nprime];
            }
        } else {
            if(turn == 0) xt = solve(nr, accX+t, accY, 1);
            else xt = solve(nr, accX, accY+t, 0);
        }
        if(turn == 0){ if(xt > best) best = xt; }
        else { if(best == -1 || xt < best) best = xt; }
    }
    slot = (signed char)best;
    return best;
}

static int computeF(int n){
    if(n == 0) return 0;
    if(n <= M) return n;
    return solve(n, 0, 0, 0);
}

static bool haveCache[101];
static signed char fvalCache[101][101];

int getF(int n, int m){
    if(!haveCache[m]){
        M = m;
        for(int r=0;r<=100;r++)
            for(int x=0;x<=100;x++)
                for(int y=0;y<=100;y++){
                    memo[r][x][y][0] = -1;
                    memo[r][x][y][1] = -1;
                }
        fval[0] = 0;
        for(int k=1;k<=100;k++) fval[k] = computeF(k);
        for(int k=0;k<=100;k++) fvalCache[m][k] = (signed char)fval[k];
        haveCache[m] = true;
    }
    return fvalCache[m][n];
}

int main(){
    int n,m;
    while(scanf("%d %d", &n, &m) == 2){
        printf("%d\n", getF(n,m));
    }
    return 0;
}
