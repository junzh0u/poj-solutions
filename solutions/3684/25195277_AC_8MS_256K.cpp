// POJ 3684 - Physics Experiment
// Model: claude-opus-5
//
// Ball i (0 = bottom, radius R cm) is fastened in the tube with its lowest
// point at H + 2R*i metres and is released at second i.  Equal-mass elastic
// collisions exchange velocities, which is indistinguishable from the balls
// passing through each other, so substitute x_i = y_i - 2R*i: the ordering
// constraint y_{i+1} >= y_i + 2R becomes x_{i+1} >= x_i, the floor constraint
// y_0 >= 0 becomes x_0 >= 0, and every x_i is an independent point particle
// dropped from rest at height H at time i, bouncing elastically off the ground.
//
// So compute the N independent point trajectories at time T, sort them, and
// give the k-th smallest to the k-th ball from the bottom, offset by 2R*k.
// R is centimetres and everything else metres, hence the /100.
//
// A point dropped from H reaches the ground after t0 = sqrt(2H/g) = sqrt(H/5)
// and its motion is periodic with period 2*t0, the rising half mirroring the
// falling half; fmod keeps the phase exact even at T = 10000 with H = 1.
//
// A ball released before T is never blocked by one still in the tube: point
// positions never exceed H, so ball k stays at or below H + 2R*k, which is its
// own original slot -- at worst it grazes the ball above with zero relative
// velocity.  Balls with i > T are simply still at H + 2R*i, which the same
// formula produces because their point value stays at H (the maximum) and they
// therefore sort to the top.
//
// Ambiguity worth naming: "the height of each ball" is the height of each
// ball's lowest point listed bottom to top (the sample's "4.95 10.20" is
// ascending), not per original ball index -- the balls are identical, so the
// two readings differ only in labelling and the sorted order is what is meant.
//
// Verified against an event-driven simulation of the literal statement (real
// balls of radius R, ground bounces, pairwise velocity exchange) on 900+
// random cases and several long runs with thousands of collisions.

#include <cstdio>
#include <cmath>
#include <algorithm>

static double H;
static double t0;   /* time of a free fall from H to the ground */

/* Height of a single point particle released from rest at height H at time 0,
   bouncing elastically off the ground, evaluated at time t. */
static double pos(double t)
{
    if (t < 0.0) return H;          /* still fastened in the tube */
    double period = 2.0 * t0;
    double u = fmod(t, period);
    if (u > t0) u = period - u;     /* rising half mirrors the falling half */
    double y = H - 5.0 * u * u;
    if (y < 0.0) y = 0.0;
    return y;
}

int main()
{
    int C;
    if (scanf("%d", &C) != 1) return 0;
    while (C-- > 0) {
        int n, h, r, t;
        if (scanf("%d %d %d %d", &n, &h, &r, &t) != 4) break;
        H = h;
        t0 = sqrt(H / 5.0);
        double ball[105];
        for (int i = 0; i < n; i++) ball[i] = pos((double)t - (double)i);
        std::sort(ball, ball + n);
        double d = 2.0 * r / 100.0;   /* diameter in metres (r is centimetres) */
        for (int i = 0; i < n; i++)
            printf("%.2f%c", ball[i] + d * i, i == n - 1 ? '\n' : ' ');
    }
    return 0;
}
