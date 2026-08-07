// POJ 1563 - The Snail
// Model: claude-opus-5
//
// Simulation, done in exact integer arithmetic.
//
// Day k the snail climbs max(0, U - (k-1)*U*F/100) and then always slides D.
// The fatigue loss is a CONSTANT U*F/100 per elapsed day (10% of the FIRST
// day's climb), not a compounding 10% of the previous day's climb -- the
// statement's parenthetical says so and the discuss board confirms it is the
// usual WA.
//
// Rescaling to hundredths of a foot removes floating point entirely:
//   climb_k = 100*U - (k-1)*U*F   (clamped at 0)
// is an integer, as are 100*H and 100*D. Everything fits in int
// (100*U <= 10000, heights stay <= 100*H+100*U <= 20000).
//
// Boundary semantics (both strict, per the statement and the board's
// "exactly at the top and exactly at the bottom both don't count"):
//   success iff height > 100*H after climbing;
//   failure iff height <  0     after sliding.
// So height == 100*H is not an escape and height == 0 is not a fall.
//
// Termination: F >= 1 and U >= 1, so climb_k is 0 once (k-1)*F >= 100, i.e.
// by day 101; from then on the height drops by 100*D >= 100 each night from
// at most 100*H <= 10000, so an answer always comes within ~201 days.

#include <cstdio>

int main() {
    int H, U, D, F;
    while (scanf("%d %d %d %d", &H, &U, &D, &F) == 4 && H != 0) {
        const int top = 100 * H;      /* well height, hundredths of a foot */
        const int slide = 100 * D;    /* nightly slide                     */
        int height = 0;
        for (int day = 1; ; ++day) {
            int climb = 100 * U - (day - 1) * U * F;
            if (climb < 0) climb = 0; /* never climbs a negative distance */
            height += climb;
            if (height > top) {
                printf("success on day %d\n", day);
                break;
            }
            height -= slide;          /* slides D regardless of the climb */
            if (height < 0) {
                printf("failure on day %d\n", day);
                break;
            }
        }
    }
    return 0;
}
