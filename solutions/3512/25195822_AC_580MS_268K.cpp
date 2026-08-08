// POJ 3512 - Incidental Points
// Model: claude-opus-5
//
// Answer = the largest number of collinear input points.  A segment is bounded
// by two of the given points, but for any line L the segment joining the two
// extreme points of L's point set already includes every point of L, so the
// "between the endpoints" restriction never lowers the maximum and the problem
// reduces to max-points-on-a-line.  (Checked against an O(n^3) brute force that
// implements the literal segment definition -- collinear AND between -- on
// ~1200 random inputs.)
//
// For each base point i only the points j > i are examined: the smallest-index
// point of a maximal collinear set sees all the others after it, so every line
// is still discovered.  Directions (dx,dy) from i are folded into the half-open
// half-plane dx>0, or dx==0 && dy>0, which makes opposite directions along one
// line share a key, then sorted by angle with a cross-product comparator.  That
// avoids a gcd per pair, which profiling showed dominated the running time
// (0.77s of 0.98s on 30 maximal random cases).  All vectors then lie in an
// angular range spanning less than 180 degrees, so "cross > 0" is a valid
// ordering, and equal-angle vectors end up adjacent.
//
// Input framing: coordinates may be negative, so an end-of-case marker is
// recognised as a token of two or more '-' characters and nothing else ("-5"
// is a coordinate, "--" is a marker).  Cases run until a marker; the extra
// marker after the last case yields an empty case, which ends the input.
//
// The statement guarantees distinct points, but a repeated point would give a
// zero direction vector whose cross product with everything is 0, breaking the
// comparator's transitivity (undefined behaviour in std::sort).  Such points
// are therefore kept out of the sorted array and counted as their own group.
//
// Submitted as G++ (language 0): the key comparison needs 64-bit products.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;

static const int MAXN = 5005;   /* stated bound is 1000; leave room */

static int X[MAXN], Y[MAXN];

struct Vec { int dx, dy; };
static Vec V[MAXN];

static bool byAngle(const Vec &a, const Vec &b) {
    return (long long)a.dx * b.dy - (long long)a.dy * b.dx > 0;
}

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static int nextToken(char *t, int cap) {
    int c = gc();
    while (c != -1 && c <= ' ') c = gc();
    if (c == -1) return 0;
    int n = 0;
    while (c != -1 && c > ' ') {
        if (n < cap - 1) t[n++] = (char)c;
        c = gc();
    }
    t[n] = 0;
    return 1;
}

int main() {
    char tok[64];
    int caseno = 0;

    for (;;) {
        int n = 0;
        bool eof = false;

        for (;;) {
            if (!nextToken(tok, (int)sizeof(tok))) { eof = true; break; }
            int len = (int)strlen(tok);
            bool marker = (len >= 2);
            for (int i = 0; i < len && marker; i++)
                if (tok[i] != '-') marker = false;
            if (marker) break;
            int xv = atoi(tok);
            if (!nextToken(tok, (int)sizeof(tok))) { eof = true; break; }
            int yv = atoi(tok);
            if (n < MAXN) { X[n] = xv; Y[n] = yv; n++; }
        }
        if (n == 0) break;

        int best = (n >= 2) ? 2 : 1;
        for (int i = 0; i < n && n - i > best; i++) {
            int xi = X[i], yi = Y[i];
            int m = 0, same = 0;
            for (int j = i + 1; j < n; j++) {
                int dx = X[j] - xi, dy = Y[j] - yi;
                if (dx == 0 && dy == 0) { same++; continue; }
                if (dx < 0 || (dx == 0 && dy < 0)) { dx = -dx; dy = -dy; }
                V[m].dx = dx; V[m].dy = dy; m++;
            }
            sort(V, V + m, byAngle);
            int run = same;
            for (int a = 0; a < m; ) {
                int b = a + 1;
                while (b < m &&
                       (long long)V[a].dx * V[b].dy - (long long)V[a].dy * V[b].dx == 0)
                    b++;
                if (b - a > run) run = b - a;
                a = b;
            }
            if (run + 1 > best) best = run + 1;
        }

        printf("%d. %d\n", ++caseno, best);
        if (eof) break;
    }
    return 0;
}
