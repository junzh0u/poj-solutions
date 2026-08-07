// POJ 2573 - Bridge
// Model: claude-opus-5
//
// Classic flashlight/bridge crossing. Sort the crossing times ascending and
// ferry the two slowest people across together, repeatedly. With t[0..i]
// still on the near side (i >= 3) there are only two candidate ways to move
// the two slowest across while leaving the near side one flashlight-ready:
//
//   A: t0,t1 cross; t0 returns; t[i-1],t[i] cross; t1 returns
//        cost 2*t1 + t0 + t[i]
//   B: t0,t[i] cross; t0 returns; t0,t[i-1] cross; t0 returns
//        cost 2*t0 + t[i-1] + t[i]
//
// Take the cheaper, drop i by 2, and finish the tail: three left costs
// t2 + t0 + t1 (t0,t2 cross; t0 returns; t0,t1 cross), two left costs t1,
// one left costs t0.
//
// The output is a witness checked by a special judge, so both the total and
// the move list are emitted; the total is computed in a first pass and the
// identical decision replayed in a second so the two can never disagree.
//
// Statement/board notes:
//  - Exactly ONE data set: the discuss board reports WA for solutions that
//    loop `while (scanf("%d", &n) == 1)`.
//  - n == 1 (one line, cost t0) and n == 0 must both be handled; the board
//    names n == 1 as a common WA.
//  - The board also reports RE with an array of 1020 despite the stated
//    "not more than 1000 people", so the array is heavily oversized.
//  - "Each person is indicated by the crossing time" and duplicates are
//    explicitly stated to be of no consequence, so printing times is fine.
//
// Verified locally against a Dijkstra brute force over (set on near side,
// flashlight side) derived from the statement's own wording, exhaustively
// for every sorted multiset of n <= 6 over a value pool and every value
// tuple in 1..4 for n <= 7 (~25k cases), plus randomized trials to n = 14.
// Every emitted move list was replayed by a checker asserting the judge's
// own properties: groups of size <= 2, alternating directions, each named
// person actually present on the flashlight's side, everyone across at the
// end, and the replayed cost equal to the printed total.

#include <cstdio>
#include <algorithm>

static int t[1000006];

int main() {
    int n;
    if (scanf("%d", &n) != 1) { printf("0\n"); return 0; }
    if (n < 0) n = 0;
    for (int i = 0; i < n; ++i) scanf("%d", &t[i]);
    std::sort(t, t + n);

    if (n == 0) { printf("0\n"); return 0; }

    int total = 0;
    int i = n - 1;
    while (i >= 3) {
        int a = t[1] + t[0] + t[i] + t[1];
        int b = t[i] + t[0] + t[i - 1] + t[0];
        total += (a <= b) ? a : b;
        i -= 2;
    }
    if (i == 2) total += t[2] + t[0] + t[1];
    else if (i == 1) total += t[1];
    else total += t[0];

    printf("%d\n", total);

    i = n - 1;
    while (i >= 3) {
        int a = t[1] + t[0] + t[i] + t[1];
        int b = t[i] + t[0] + t[i - 1] + t[0];
        if (a <= b) {
            printf("%d %d\n", t[0], t[1]);
            printf("%d\n", t[0]);
            printf("%d %d\n", t[i - 1], t[i]);
            printf("%d\n", t[1]);
        } else {
            printf("%d %d\n", t[0], t[i]);
            printf("%d\n", t[0]);
            printf("%d %d\n", t[0], t[i - 1]);
            printf("%d\n", t[0]);
        }
        i -= 2;
    }
    if (i == 2) {
        printf("%d %d\n", t[0], t[2]);
        printf("%d\n", t[0]);
        printf("%d %d\n", t[0], t[1]);
    } else if (i == 1) {
        printf("%d %d\n", t[0], t[1]);
    } else {
        printf("%d\n", t[0]);
    }
    return 0;
}
