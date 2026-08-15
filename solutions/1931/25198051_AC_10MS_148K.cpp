// POJ 1931 - Biometrics
// Model: claude-sonnet-5
//
// Two polygons with corresponding (already ordered) vertices are similar
// iff there is a single complex number z (rotation+scale, z applied by
// direct multiplication so reflections are excluded) and a translation t
// with P2[i] = z*P1[i] + t for every i.
//
// Anchor everything at vertex 0. Let k be the first index with
// P1[k] != P1[0] (a nonzero "edge" vector e1 = P1[k]-P1[0]); the matching
// vector in polygon 2 is e2 = P2[k]-P2[0], which fixes z = e2/e1
// uniquely. For every other vertex i the requirement
//   (P2[i]-P2[0]) == z * (P1[i]-P1[0])
// is checked by cross-multiplying with e1 to stay in exact integer
// complex arithmetic (coordinates are integers in [-1000,1000], so every
// intermediate product/sum fits comfortably in a 64-bit, even 32-bit,
// integer) -- no floating point, no epsilon.
//
// The statement claims "distinct vertices" but the discuss board (msgs
// 8282/8283 "注意重点/重复的点") flags that real test data contains
// duplicate points among the f features, and msg 8303/10242 raises the
// degenerate case where an entire polygon collapses to one repeated
// point. The anchor-at-first-nonzero-edge construction handles repeats
// anywhere in the vertex list correctly (if P1[i]==P1[j] the check forces
// P2[i]==P2[j]); the only case with no valid k is when every vertex of
// polygon 1 coincides with vertex 0, handled separately: a degenerate
// single-point polygon can only be similar to another single-point
// polygon (any translation, e.g. z=0 maps everything to one point; a
// true similarity with z!=0 is injective and can't produce that from
// distinct source points either way), so it is "similar" only if
// polygon 2 is likewise a single repeated point.
#include <cstdio>

static int x1[15], y1[15], x2[15], y2[15];

int main() {
    int f;
    while (scanf("%d", &f) == 1 && f != 0) {
        for (int i = 0; i < f; ++i) scanf("%d %d", &x1[i], &y1[i]);
        for (int i = 0; i < f; ++i) scanf("%d %d", &x2[i], &y2[i]);

        int k = -1;
        for (int i = 1; i < f; ++i) {
            if (x1[i] != x1[0] || y1[i] != y1[0]) { k = i; break; }
        }

        bool similar;
        if (k == -1) {
            // polygon 1 collapses to a single repeated point
            similar = true;
            for (int i = 0; i < f; ++i) {
                if (x2[i] != x2[0] || y2[i] != y2[0]) { similar = false; break; }
            }
        } else {
            int e1x = x1[k] - x1[0], e1y = y1[k] - y1[0];
            int e2x = x2[k] - x2[0], e2y = y2[k] - y2[0];
            similar = true;
            for (int i = 0; i < f; ++i) {
                int p1x = x1[i] - x1[0], p1y = y1[i] - y1[0];
                int p2x = x2[i] - x2[0], p2y = y2[i] - y2[0];
                int lhsRe = p2x * e1x - p2y * e1y;
                int lhsIm = p2x * e1y + p2y * e1x;
                int rhsRe = e2x * p1x - e2y * p1y;
                int rhsIm = e2x * p1y + e2y * p1x;
                if (lhsRe != rhsRe || lhsIm != rhsIm) { similar = false; break; }
            }
        }
        printf(similar ? "similar\n" : "dissimilar\n");
    }
    return 0;
}
