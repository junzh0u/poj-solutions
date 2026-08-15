// POJ 1791 - Paper Cutting
// Model: claude-sonnet-5
//
// The grid of a*b cards can be printed in two card orientations:
//   X = a*c, Y = b*d   (cards not rotated relative to the "a" direction)
//   X = a*d, Y = b*c   (cards rotated 90 degrees)
// and the resulting X*Y rectangle can be placed on the sheet in either of
// the two paper orientations (X,Y) <-> (E,F) or (X,Y) <-> (F,E). That
// covers all four orientations the statement's figure shows for the 1*2
// example.
//
// Splitting an m*n grid of individually separated cards by guillotine
// cuts (each cut must go straight through one whole current piece, no
// stacking) always costs exactly m*n-1 cuts: every cut turns one piece
// into two, so going from 1 piece to a*b pieces needs a*b-1 cuts no
// matter the order (confirmed on the discuss board, message 135256).
// On top of that, if the sheet is strictly larger than the placed grid
// rectangle in a given dimension, one extra trim cut removes that excess
// strip (the grid can be pushed flush against the other edge, so at most
// one trim cut per dimension is ever needed).
//
// So for a feasible placement the total cost is (a*b-1) + trimX + trimY,
// with trimX/trimY in {0,1}. Try all four (orientation, paper-orientation)
// combinations and keep the minimum feasible total; if none fit, the
// paper is too small.
//
// Verified against all four provided samples (2, 1, "too small", 10).
#include <cstdio>

// a,b <= 1000, c,d <= 1000, e,f <= 1,000,000: every product used below
// (a*b, a*c, a*d, b*c, b*d) is at most 1,000,000, comfortably inside a
// 32-bit int, so plain int/%d is enough -- no long long needed.
int main() {
    int a, b, c, d, e, f;
    while (scanf("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f) == 6) {
        if (a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0) break;

        int baseCuts = a * b - 1;
        int best = -1;

        int cfgX[2], cfgY[2];
        cfgX[0] = a * c; cfgY[0] = b * d;
        cfgX[1] = a * d; cfgY[1] = b * c;

        for (int cfg = 0; cfg < 2; ++cfg) {
            int X = cfgX[cfg], Y = cfgY[cfg];
            for (int rot = 0; rot < 2; ++rot) {
                int P = rot == 0 ? e : f;
                int Q = rot == 0 ? f : e;
                if (X <= P && Y <= Q) {
                    int cuts = baseCuts + (X < P ? 1 : 0) + (Y < Q ? 1 : 0);
                    if (best == -1 || cuts < best) best = cuts;
                }
            }
        }

        if (best == -1) {
            printf("The paper is too small.\n");
        } else {
            printf("The minimum number of cuts is %d.\n", best);
        }
    }
    return 0;
}
