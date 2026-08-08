// POJ 1569 - Myacm Triangles
// Model: claude-opus-5
//
// 4..15 monuments with integer coordinates in [0,100).  Find the largest-area
// triangle on three monuments that holds no other monument inside it OR on one
// of its edges, and print the three labels alphabetically.
//
// Brute force over all C(15,3) = 455 triples; for each, test the remaining <= 12
// monuments for containment.  All arithmetic is exact integers: work with twice
// the signed area (cross product), so no floating point is involved.  With
// coordinates < 100 a cross product is bounded by 2*99*99 = 19602, far inside
// int, and comparing doubled areas orders triangles exactly as their real areas.
//
// The rule that decides this problem is "or on an edge": containment is the
// CLOSED triangle.  Testing point P against triangle ABC via the three signed
// cross products cross(A,B,P), cross(B,C,P), cross(C,A,P), P is inside or on the
// boundary iff all three are >= 0 or all three are <= 0.  Two mistakes here are
// what the discuss board reports as the repeated cause of Wrong Answer: using
// strict signs (which lets a monument sitting exactly on an edge slip through)
// and checking only one orientation (which only handles counter-clockwise
// triples).  The statement's second sample kills both -- A(0,0) lies on edge
// B(1,0)-C(99,0), so ACD (area 4900.5) must be rejected in favour of BCD
// (area 4851) -- so both were verified against it rather than merely believed.
//
// Degenerate (collinear) triples are skipped; they can never be the answer,
// since a triangulation of any non-collinear point set always contains an empty
// triangle of positive area.  Ties in maximum area are resolved to the first
// triple in index order, i.e. the alphabetically smallest labels; the statement
// guarantees ties do not occur ("Each field contains only one such triangle").
//
// Input is read until the sentinel 0 monument count, as the statement specifies.

#include <cstdio>

static const int MAXN = 64;

static int X[MAXN], Y[MAXN];
static char L[MAXN];

/* twice the signed area of triangle (a,b,c) */
static int cross3(int a, int b, int c) {
    return (X[b] - X[a]) * (Y[c] - Y[a]) - (Y[b] - Y[a]) * (X[c] - X[a]);
}

/* does p lie inside triangle (a,b,c) or on one of its edges? (a,b,c non-degenerate) */
static int covered(int a, int b, int c, int p) {
    int d1 = cross3(a, b, p);
    int d2 = cross3(b, c, p);
    int d3 = cross3(c, a, p);
    if (d1 >= 0 && d2 >= 0 && d3 >= 0) return 1;
    if (d1 <= 0 && d2 <= 0 && d3 <= 0) return 1;
    return 0;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int i, j, k, p, best;
        char buf[64], r0, r1, r2, t;
        if (n > MAXN) n = MAXN;
        for (i = 0; i < n; i++) {
            if (scanf("%63s %d %d", buf, &X[i], &Y[i]) != 3) return 0;
            L[i] = buf[0];
        }
        best = 0;
        r0 = r1 = r2 = '?';
        for (i = 0; i < n; i++)
            for (j = i + 1; j < n; j++)
                for (k = j + 1; k < n; k++) {
                    int a2 = cross3(i, j, k);
                    int ok;
                    if (a2 < 0) a2 = -a2;
                    if (a2 <= best) continue;   /* also skips degenerate triples */
                    ok = 1;
                    for (p = 0; p < n && ok; p++) {
                        if (p == i || p == j || p == k) continue;
                        if (covered(i, j, k, p)) ok = 0;
                    }
                    if (ok) { best = a2; r0 = L[i]; r1 = L[j]; r2 = L[k]; }
                }
        /* labels in increasing alphabetical order */
        if (r0 > r1) { t = r0; r0 = r1; r1 = t; }
        if (r1 > r2) { t = r1; r1 = r2; r2 = t; }
        if (r0 > r1) { t = r0; r0 = r1; r1 = t; }
        printf("%c%c%c\n", r0, r1, r2);
    }
    return 0;
}
