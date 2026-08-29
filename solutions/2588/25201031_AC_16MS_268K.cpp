// POJ 2588 - Snakes
// Model: claude-sonnet-5
//
// Approach: Bill crosses west->east through the 1000x1000 field, avoiding the
// open disks (bite zones) around each snake. A crossing is impossible exactly
// when some connected chain of overlapping (or touching within their radii)
// snake-disks spans from the north edge (y=1000) to the south edge (y=0):
// such a chain, together with the fixed north/south edges, separates the
// field into a west part and an east part. So: seed a DFS/graph-connectivity
// walk from every disk that touches the north edge (y+r > 1000), following
// edges between disks i,j whenever dist(i,j) < r_i+r_j (their open disks
// overlap). If any visited disk also reaches the south edge (y-r < 0), no
// crossing is possible.
//
// Otherwise, every north-attached component is a "ceiling-hanging" blob that
// does not reach the floor. For such a blob, any point on the west edge that
// lies strictly between two of the blob's crossings of x=0 is a dead end: the
// blob plus the west-edge segment between those crossings encloses a pocket,
// since the blob is anchored to the north edge (there is no way "above" it).
// Hence the only genuinely reachable entry points below the blob are those
// south of ALL of the blob's crossings of x=0 - i.e. south of the minimum of
// each such disk's *lower* intersection point with x=0. Taking that minimum
// over every disk (from every north-attached component) that crosses the
// west edge gives the most-northerly valid entry y-coordinate; symmetric
// reasoning on x=1000 gives the most-northerly exit y-coordinate. When a
// component never reaches an edge at all, that edge's bound simply stays at
// its default of 1000 (the field's own north corner).
//
// This is exactly the official Waterloo local 2000.01.29 model solution's
// algorithm (recovered from icpc.student.cs.uwaterloo.ca/~acm00/000129/C.c
// and cross-checked against its C.0..C.8 test data), ported to C++98 with an
// explicit stack instead of unbounded recursion.
//
// Statement ambiguity: a snake "bites anything that passes closer than this
// distance", i.e. strictly less than r; touching the boundary (distance
// exactly r) is safe. That is why every comparison below is strict '<'.
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

static int n;
static double X[1000], Y[1000], R[1000];
static bool visited[1000];

int main() {
    while (scanf("%d", &n) == 1) {
        for (int i = 0; i < n; i++) scanf("%lf %lf %lf", &X[i], &Y[i], &R[i]);
        for (int i = 0; i < n; i++) visited[i] = false;

        bool possible = true;
        double nw = 1000.0, ne = 1000.0;

        vector<int> stack;
        for (int i = 0; i < n; i++) {
            if (visited[i]) continue;
            if (!(Y[i] + R[i] > 1000.0)) continue;
            stack.push_back(i);
            visited[i] = true;
            while (!stack.empty()) {
                int u = stack.back();
                stack.pop_back();
                for (int j = 0; j < n; j++) {
                    if (visited[j]) continue;
                    double dx = X[j] - X[u], dy = Y[j] - Y[u];
                    if (hypot(dx, dy) < R[u] + R[j]) {
                        visited[j] = true;
                        stack.push_back(j);
                    }
                }
                if (Y[u] - R[u] < 0.0) possible = false;
                if (X[u] - R[u] < 0.0) {
                    double yy = Y[u] - sqrt(R[u] * R[u] - X[u] * X[u]);
                    if (yy < nw) nw = yy;
                }
                if (X[u] + R[u] > 1000.0) {
                    double d = 1000.0 - X[u];
                    double yy = Y[u] - sqrt(R[u] * R[u] - d * d);
                    if (yy < ne) ne = yy;
                }
            }
        }

        if (possible) {
            printf("Bill enters at (0.00, %0.2f) and leaves at (1000.00, %0.2f).\n", nw, ne);
        } else {
            printf("Bill will be bitten.\n");
        }
    }
    return 0;
}
