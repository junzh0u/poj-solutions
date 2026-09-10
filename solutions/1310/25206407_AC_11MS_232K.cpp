// POJ 1310 - Nonstop Travel
// Model: claude-sonnet-5
//
// For each integer speed v in [30,60] mph, and each of the N (<=6) traffic
// signals at distance L_i miles with cycle G_i+Y_i+R_i seconds (green,
// yellow, red, all signals turning green at t=0), compute the arrival time
// t_i = 3600 * L_i / v seconds (multiply by 3600 before dividing by v --
// the board's "divide first then multiply" thread (message 359843) reports
// this order matters for precision) and its residue tm = t_i mod C_i.
// Passing is legal for tm in [0, G_i+Y_i] (yellow->red transition and
// red->green transition are both explicitly allowed by the statement, so
// the forbidden zone is the OPEN interval (G_i+Y_i, C_i)). A residue that
// floating point rounds to just under C_i is folded back to 0 before the
// comparison, and a small epsilon absorbs the same rounding at the G+Y
// boundary. Speed is acceptable only if every signal is legal.
#include <cstdio>
#include <cmath>
using namespace std;

int main() {
    int n;
    int caseNum = 1;
    while (scanf("%d", &n) == 1) {
        if (n == -1) break;
        double L[6], G[6], Y[6], R[6];
        for (int i = 0; i < n; i++) {
            scanf("%lf %lf %lf %lf", &L[i], &G[i], &Y[i], &R[i]);
        }
        int speeds[31], cnt = 0;
        const double eps = 1e-6;
        for (int v = 30; v <= 60; v++) {
            bool ok = true;
            for (int i = 0; i < n && ok; i++) {
                double C = G[i] + Y[i] + R[i];
                double t = 3600.0 * L[i] / v;
                double tm = fmod(t, C);
                if (tm < 0) tm += C;
                if (tm > C - eps) tm = 0.0;
                if (tm > G[i] + Y[i] + eps) ok = false;
            }
            if (ok) speeds[cnt++] = v;
        }
        printf("Case %d: ", caseNum++);
        if (cnt == 0) {
            printf("No acceptable speeds.\n");
        } else {
            bool first = true;
            int i = 0;
            while (i < cnt) {
                int j = i;
                while (j + 1 < cnt && speeds[j + 1] == speeds[j] + 1) j++;
                if (!first) printf(", ");
                first = false;
                if (i == j) printf("%d", speeds[i]);
                else printf("%d-%d", speeds[i], speeds[j]);
                i = j + 1;
            }
            printf("\n");
        }
    }
    return 0;
}
