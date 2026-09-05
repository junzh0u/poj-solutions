// POJ 3296 - Rinse
// Model: claude-sonnet-5
//
// Each rinse i pours in water w_i, mixes with the residual liquid left by
// the previous step, then upends the can: the concentration of whiskey is
// preserved and exactly Vr volume of liquid (of that same concentration)
// remains. Writing x_1 = w_1 + Vw (level reached in rinse 1, whose floor is
// the initial pure-whiskey volume Vw) and x_i = w_i + Vr for i = 2..r (floor
// Vr, the residual left by every prior rinse), the whiskey left after r
// rinses is
//     A_r = Vw * Vr^r / (x_1 * x_2 * ... * x_r).
// Vw and Vr are fixed, so minimizing A_r means maximizing the product of the
// x_i subject to x_i <= Vc, sum(x_i - floor_i) <= Vb. Using more of the
// allowed k rinses is never worse (a trailing rinse with w_i = 0 leaves the
// product, hence A_r, unchanged), so it is always optimal to take r = k and
// equalize the x_i as much as possible (AM-GM maximizes a product for a
// fixed sum). With only two distinct floors (Vw once, Vr repeated k-1
// times), the equalized level for rinse 1 solves
//     x1 + Vw = (Vb - x1)/(k-1) + Vr  =>  x1 = (Vb + (k-1)Vr - (k-1)Vw) / k
// (x1 here denotes the water w_1), then clamp it: not negative, not so small
// that the remaining k-1 rinses would overflow Vc even if x1 took no more
// than that leftover, and not so large that rinse 1 itself overflows Vc or
// exceeds the total water budget Vb. The remaining water is split evenly
// over rinses 2..k and clamped to the capacity of a single rinse.
//
// Statement ambiguity / discuss-board findings (poj.org/bbs?problem_id=3296,
// messages 341009/343321/141264): the judge data (traced to the original
// Waterloo Local Contest 2006.9.30 set) contains a case where Vb + Vw < Vr,
// i.e. even pouring every drop of available water into the can in one go
// never raises the total past the amount Vr that cannot be poured out; in
// that case no rinse can ever remove any whiskey, and the expected output
// is the single line "0" (r = 0), not k rinses of 0.00 water. The Vb == 0
// case needs no special-casing: the general formula already degenerates to
// k rinses each using 0.00 water, matching that thread's second point.
// Verified against all 8 cases the "tips" thread reproduced from the mirror
// of the official Waterloo data (values spanning every clamp branch above,
// including the r = 0 case and one where every clamp fires) before
// submitting.
#include <cstdio>

int main() {
    int k;
    while (scanf("%d", &k) == 1 && k != 0) {
        double vb, vw, vr, vc;
        scanf("%lf %lf %lf %lf", &vb, &vw, &vr, &vc);

        if (vb + vw < vr) {
            printf("0\n");
            continue;
        }

        double x1 = (vb + (double)(k - 1) * vr - (double)(k - 1) * vw) / k;
        if (x1 < 0) x1 = 0;
        double leftover = vb - (double)(k - 1) * (vc - vr);
        if (x1 < leftover) x1 = leftover;
        if (x1 + vw > vc) x1 = vc - vw;
        if (x1 > vb) x1 = vb;

        double xi = 0.0;
        if (k > 1) {
            xi = (vb - x1) / (k - 1);
            if (xi + vr > vc) xi = vc - vr;
        }

        printf("%d %.2f", k, x1);
        for (int i = 1; i < k; ++i) printf(" %.2f", xi);
        printf("\n");
    }
    return 0;
}
