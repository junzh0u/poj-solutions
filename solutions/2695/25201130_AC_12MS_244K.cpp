// POJ 2695 - Rick the Persistent Gnu
// Model: claude-sonnet-5
// Approach: Greedy path simulation. From the current site, consider every
// unvisited site within distance D. A candidate direction must not point
// away from FS (angle to FS's direction vector <= 90 degrees, i.e. dot
// product with F is positive) and, from the second step on, must not turn
// more than 90 degrees from the previous travel segment (dot product with
// the previous direction vector is positive). Among the surviving
// candidates, pick the one whose direction is closest to FS's direction,
// i.e. maximizes dot(dir,F)/|dir| (proportional to cos of the angle to F).
// Repeat until no candidate remains. The problem guarantees no input is
// ever close to an exact 90 degree tie (for either the FS-direction test
// or the turn test), so plain floating point comparisons (with a tiny
// epsilon) are safe and the path cannot cycle: each accepted move strictly
// increases the projection of position onto F, so no site repeats.
// Ambiguity: the statement's figure/prose leaves the "closest direction"
// tie-break rule implicit; a posted accepted solution on the discuss board
// confirmed the same rule (maximize dot(F,dir)/|dir|, first step having no
// previous-direction constraint by using a zero vector), matching this
// implementation and the worked sample (3 2 4).
#include <cstdio>
#include <cmath>
using namespace std;

int main() {
    double Fx, Fy, D;
    int N, S;
    while (scanf("%lf %lf %d %d %lf", &Fx, &Fy, &N, &S, &D) == 5) {
        if (Fx == 0 && Fy == 0 && N == 0 && S == 0 && D == 0) break;
        static double x[105], y[105];
        static bool visited[105];
        for (int i = 1; i <= N; i++) {
            scanf("%lf %lf", &x[i], &y[i]);
            visited[i] = false;
        }
        int cur = S;
        visited[cur] = true;
        bool hasPrev = false;
        double pdx = 0, pdy = 0;
        int path[205];
        int plen = 0;
        path[plen++] = cur;

        for (int step = 0; step < N; step++) {
            int best = -1;
            double bestScore = -1e300;
            for (int j = 1; j <= N; j++) {
                if (j == cur || visited[j]) continue;
                double dx = x[j] - x[cur];
                double dy = y[j] - y[cur];
                double distSq = dx * dx + dy * dy;
                double limit = D * D * (1.0 + 1e-9) + 1e-6;
                if (distSq > limit) continue;
                double dotF = dx * Fx + dy * Fy;
                if (dotF <= 1e-6) continue;
                if (hasPrev) {
                    double dotP = dx * pdx + dy * pdy;
                    if (dotP <= 1e-6) continue;
                }
                double dist = sqrt(distSq);
                double score = dotF / dist;
                if (score > bestScore) {
                    bestScore = score;
                    best = j;
                }
            }
            if (best == -1) break;
            pdx = x[best] - x[cur];
            pdy = y[best] - y[cur];
            hasPrev = true;
            cur = best;
            visited[cur] = true;
            path[plen++] = cur;
        }

        for (int i = 0; i < plen; i++) {
            if (i) printf(" ");
            printf("%d", path[i]);
        }
        printf("\n");
    }
    return 0;
}
