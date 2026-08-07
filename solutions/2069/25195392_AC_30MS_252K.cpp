// POJ 2069 - Super Star
// Model: claude-opus-5
//
// Smallest enclosing sphere of n <= 30 points in 3D, exactly, by enumeration
// instead of the annealing everybody on the discuss board fights with.
//
// Let c be the optimal centre and S the set of input points on the boundary.
// c lies in conv(S), so by Caratheodory in R^3 there is an affinely independent
// T subset of S, |T| <= 4, with c in conv(T).  Every point of T is at distance R
// from c, and c lies in aff(T); for an affinely independent T that circumcentre
// is unique.  So the optimal centre is the circumcentre-within-its-affine-hull
// of some 2-, 3- or 4-element subset, and enumerating all of them finds it.
// |T| >= 2 because n >= 4 and the points are distinct.
//
// The answer is min over candidate centres of (max distance to any input point).
// Written that way, *every* candidate is a valid upper bound on R, so a badly
// conditioned or outright degenerate subset can only fail to improve the answer,
// never corrupt it.  That is why there is no epsilon anywhere here: degenerate
// subsets are skipped only on an exactly zero denominator, and the finite3()
// guard exists solely to keep an infinity or a NaN out of the max-distance scan
// (where NaN would compare false and masquerade as distance zero).
//
// Output: %f rather than %lf, which is unreliable on this judge's compilers,
// and language 4 (C++) per the board's "G++ WA, C++ AC" thread.

#include <cstdio>
#include <cmath>

static int n;
static double px[40], py[40], pz[40];

/* Squared radius of the sphere centred at (cx,cy,cz) covering every point.
   Gives up as soon as it exceeds `bound`, which is only ever used to reject. */
static double evalCenter(double cx, double cy, double cz, double bound) {
    double m = 0.0;
    for (int i = 0; i < n; ++i) {
        double dx = px[i] - cx, dy = py[i] - cy, dz = pz[i] - cz;
        double d = dx * dx + dy * dy + dz * dz;
        if (d > m) { m = d; if (m >= bound) return m; }
    }
    return m;
}

/* false for NaN and for |v| >= 1e15, both of which would poison evalCenter */
static bool finite3(double a, double b, double c) {
    return a > -1e15 && a < 1e15 && b > -1e15 && b < 1e15 && c > -1e15 && c < 1e15;
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        int i, a, b, c, d;
        for (i = 0; i < n; ++i) scanf("%lf %lf %lf", &px[i], &py[i], &pz[i]);

        double best = 1e300;

        /* centroid, just to start with a finite bound */
        {
            double cx = 0.0, cy = 0.0, cz = 0.0;
            for (i = 0; i < n; ++i) { cx += px[i]; cy += py[i]; cz += pz[i]; }
            cx /= n; cy /= n; cz /= n;
            double r = evalCenter(cx, cy, cz, best);
            if (r < best) best = r;
        }

        /* |T| = 2: the centre is the midpoint */
        for (a = 0; a < n; ++a) for (b = a + 1; b < n; ++b) {
            double cx = (px[a] + px[b]) * 0.5;
            double cy = (py[a] + py[b]) * 0.5;
            double cz = (pz[a] + pz[b]) * 0.5;
            double r = evalCenter(cx, cy, cz, best);
            if (r < best) best = r;
        }

        /* |T| = 3 and 4 share the same setup: with everything measured from
           point a, the centre offset x solves 2 x.u = |u|^2 for each other
           member u of the subset, plus x in span of those u for |T| = 3. */
        for (a = 0; a < n; ++a) {
            double ex[40], ey[40], ez[40], hn[40];   /* hn = |e|^2 / 2 */
            for (i = 0; i < n; ++i) {
                ex[i] = px[i] - px[a];
                ey[i] = py[i] - py[a];
                ez[i] = pz[i] - pz[a];
                hn[i] = (ex[i] * ex[i] + ey[i] * ey[i] + ez[i] * ez[i]) * 0.5;
            }

            for (b = a + 1; b < n; ++b) {
                double ux = ex[b], uy = ey[b], uz = ez[b], hu = hn[b];
                double uu = hu + hu;

                for (c = b + 1; c < n; ++c) {
                    double vx = ex[c], vy = ey[c], vz = ez[c], hv = hn[c];
                    double vv = hv + hv;
                    double uv = ux * vx + uy * vy + uz * vz;
                    /* u x v, reused by every d below */
                    double nx = uy * vz - uz * vy;
                    double ny = uz * vx - ux * vz;
                    double nz = ux * vy - uy * vx;

                    /* |T| = 3: x = al*u + be*v from the 2x2 Gram system */
                    double D = uu * vv - uv * uv;    /* = |u x v|^2 */
                    if (D > 0.0) {
                        double al = vv * (uu - uv) / (2.0 * D);
                        double be = uu * (vv - uv) / (2.0 * D);
                        double cx = px[a] + al * ux + be * vx;
                        double cy = py[a] + al * uy + be * vy;
                        double cz = pz[a] + al * uz + be * vz;
                        if (finite3(cx, cy, cz)) {
                            double r = evalCenter(cx, cy, cz, best);
                            if (r < best) best = r;
                        }
                    }

                    /* |T| = 4: Cramer via the adjugate,
                       x = (hu*(v x w) + hv*(w x u) + hw*(u x v)) / (u.(v x w)) */
                    for (d = c + 1; d < n; ++d) {
                        double wx = ex[d], wy = ey[d], wz = ez[d], hw = hn[d];
                        double det = nx * wx + ny * wy + nz * wz;
                        if (det == 0.0) continue;    /* coplanar */
                        double vwx = vy * wz - vz * wy;
                        double vwy = vz * wx - vx * wz;
                        double vwz = vx * wy - vy * wx;
                        double wux = wy * uz - wz * uy;
                        double wuy = wz * ux - wx * uz;
                        double wuz = wx * uy - wy * ux;
                        double inv = 1.0 / det;
                        double cx = px[a] + (hu * vwx + hv * wux + hw * nx) * inv;
                        double cy = py[a] + (hu * vwy + hv * wuy + hw * ny) * inv;
                        double cz = pz[a] + (hu * vwz + hv * wuz + hw * nz) * inv;
                        if (!finite3(cx, cy, cz)) continue;
                        double r = evalCenter(cx, cy, cz, best);
                        if (r < best) best = r;
                    }
                }
            }
        }

        printf("%.5f\n", sqrt(best));
    }
    return 0;
}
