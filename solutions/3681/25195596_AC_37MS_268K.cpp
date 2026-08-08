// POJ 3681 - Finding the Rectangle
// Model: claude-opus-5
//
// Smallest-area axis-parallel rectangle with integer vertices covering >= M of
// N points, where a point on a side does NOT count as covered.
//
// Because coverage is strict interior, the minimal rectangle around a chosen
// subset is [minx-1, maxx+1] x [miny-1, maxy+1], so its area is
// (maxx-minx+2) * (maxy-miny+2) with minx/maxx/miny/maxy actual point
// coordinates.  Hence it suffices to enumerate the x-range as a pair of point
// x-coordinates: sort by x, fix a left index a, extend the right index b, keep
// the y-values of the active points in a sorted array (insertion sort, one
// point per step), and take the best window of M consecutive y-values --
// min over i of ys[i+M-1]-ys[i]+2 -- as the height.  Enumerating a superset of
// (a,b) pairs is safe: a pair whose point set is a subset of the optimum's at
// the same width only ever reports a larger area, and the optimal pair is
// always among those enumerated.
//
// That is O(N^3) with a tiny constant (~N^3/3 array reads for N=200).  The
// discuss board reports O(N^3 log N) timing out here, so the y-order is
// maintained incrementally rather than re-sorted per (a,b).  Two cheap prunes:
// the b-loop breaks once width*2 >= best (width only grows and height >= 2),
// and an x-range is scored only at the last point of a run of equal x.
//
// Ambiguity: the statement does not bound the number of test cases and puts a
// blank line before each, so input is read token-wise and to EOF rather than
// trusting the leading count.  Areas are accumulated in long long in case the
// real data exceeds the stated coordinate bound of 10000.

#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

const int MAXN = 20005;

struct P { int x, y; };
static bool byx(const P& a, const P& b) { return a.x < b.x || (a.x == b.x && a.y < b.y); }

static P p[MAXN];
static int ys[MAXN];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n < 1 || n >= MAXN || m < 1 || m > n) break;
        int bad = 0;
        for (int i = 0; i < n; i++)
            if (scanf("%d %d", &p[i].x, &p[i].y) != 2) { bad = 1; break; }
        if (bad) break;

        sort(p, p + n, byx);
        long long best = -1;
        for (int a = 0; a < n; a++) {
            if (a > 0 && p[a].x == p[a - 1].x) continue;
            int cnt = 0;
            for (int b = a; b < n; b++) {
                /* insert p[b].y into the sorted prefix ys[0..cnt) */
                int v = p[b].y, k = cnt;
                while (k > 0 && ys[k - 1] > v) { ys[k] = ys[k - 1]; k--; }
                ys[k] = v;
                cnt++;
                if (b + 1 < n && p[b + 1].x == p[b].x) continue;
                int w = p[b].x - p[a].x + 2;
                if (best >= 0 && (long long)w * 2 >= best) break;
                if (cnt < m) continue;
                int bh = ys[m - 1] - ys[0];
                const int lim = cnt - m;
                for (int i = 1; i <= lim; i++) {
                    int h = ys[i + m - 1] - ys[i];
                    if (h < bh) bh = h;
                }
                long long area = (long long)w * (bh + 2);
                if (best < 0 || area < best) best = area;
            }
        }
        cout << best << "\n";
    }
    return 0;
}
