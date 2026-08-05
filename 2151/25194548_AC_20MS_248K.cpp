// POJ 2151 - Check the difficulty of problems
//
// M problems, T teams, and P[i][j] = probability team i solves problem j.
// Wanted: P(every team solves >= 1 problem AND some team solves >= N).
//
// The two conditions are not independent per team, so the event is assembled
// by complement over the *second* one only:
//
//   answer = P(all teams solve >= 1)
//          - P(all teams solve >= 1 and every team solves <= N-1)
//
// The subtracted term is exactly the case where the "all solve at least one"
// condition holds but the champion falls short of N. Both terms factor across
// teams, because teams are independent once the per-team condition is a plain
// range on that team's own count.
//
// Per team, a small O(M^2) DP over its own probabilities gives f[k] = P(that
// team solves exactly k problems): f is rolled forward one problem at a time
// with f[k] = f[k]*(1-p) + f[k-1]*p. Then
//   P(team solves >= 1)          = 1 - f[0]
//   P(1 <= team's count <= N-1)  = f[1] + ... + f[N-1]
// and the teams simply multiply. Total work O(T*M^2) <= 1000*900.
//
// Note N = 1 needs no special case: the second product is an empty sum, i.e.
// zero for every team, so the answer collapses to P(all solve >= 1) — correct,
// since with N = 1 the champion condition is implied by the first condition.
//
// Statement notes: the probabilities are per (team, problem), not per problem;
// the terminator is the line "0 0 0" (M = T = N = 0); output is rounded to
// three decimals. Cancellation in `all - cap` is harmless: it only bites when
// the two are nearly equal, and then the answer rounds to 0.000 anyway.

#include <cstdio>

int main() {
    int M, T, N;
    while (scanf("%d %d %d", &M, &T, &N) == 3) {
        if (M == 0 && T == 0 && N == 0) break;
        double all = 1.0;   /* every team solves >= 1 */
        double cap = 1.0;   /* every team solves >= 1 and <= N-1 */
        for (int i = 0; i < T; ++i) {
            double f[31];
            for (int k = 0; k <= M; ++k) f[k] = 0.0;
            f[0] = 1.0;
            for (int j = 0; j < M; ++j) {
                double p;
                scanf("%lf", &p);
                for (int k = j + 1; k >= 1; --k)
                    f[k] = f[k] * (1.0 - p) + f[k - 1] * p;
                f[0] *= (1.0 - p);
            }
            double atLeastOne = 1.0 - f[0];
            double between = 0.0;
            for (int k = 1; k <= N - 1; ++k) between += f[k];
            all *= atLeastOne;
            cap *= between;
        }
        printf("%.3f\n", all - cap);
    }
    return 0;
}
