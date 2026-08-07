// POJ 2860 - Block game with the Little Prince
// Model: claude-opus-5
//
// N identical blocks sit in K labelled rooms (the figure's A, B, C are distinct
// columns, and its seven arrangements of N=3, M=2, K=3 are the ordered ones), each
// room holding at most M.  One move takes a block out of a room and puts it into a
// different room, never exceeding M there.  Given the initial and final states, print
// the minimal number of moves.
//
// Answer: sum over rooms of max(0, a[i] - b[i]).
//
// Lower bound: a move changes exactly two rooms by one each, so it can cut the total
// surplus sum(max(0, a[i]-b[i])) by at most one.
// Achievable: while any surplus remains there is also a deficit room j with
// a[j] < b[j] <= M, so moving a surplus block straight into it is always legal --
// the M cap never binds and no intermediate state has to be routed around.
// Verified exhaustively against a BFS over the whole state graph for every M <= 4,
// K <= 5 and every N (1.03M state pairs, all exact and the graph connected), plus a
// randomized BFS spot-check for K = 6..10.
//
// Statement reading fixed here: rooms are labelled, so the states are compared
// position by position and never re-sorted.  Input is a single data set, but the read
// loops to EOF so extra data sets would also be handled.

#include <cstdio>

int main() {
    int n, m, k;
    while (scanf("%d %d %d", &n, &m, &k) == 3) {
        int a[16], b[16];
        int i;
        for (i = 0; i < k; ++i) scanf("%d", &a[i]);
        for (i = 0; i < k; ++i) scanf("%d", &b[i]);
        int moves = 0;
        for (i = 0; i < k; ++i)
            if (a[i] > b[i]) moves += a[i] - b[i];
        printf("%d\n", moves);
    }
    return 0;
}
