// POJ 3053 - Bowling
// Model: claude-sonnet-5
//
// For each player i we need mi (guaranteed minimum wins over any
// permutation of everybody's own scores) and Mi (maximum achievable wins
// over some permutation of everybody's own scores). A game is won by a
// player iff his score is STRICTLY greater than every other player's
// score in that game (ties never count as a win -- confirmed both by the
// statement's own wording and by the discuss board thread that reports
// people missing exactly that sentence).
//
// Key facts used (all scores lie in 0..300, so histograms suffice):
//
// Mi: choosing which of player i's own scores "try" to win a game is
// independent per opponent -- for a fixed opponent j, the maximum number
// of player i's values that can be simultaneously beaten by distinct
// values of j (i_value > j_value, one-to-one) is a classic greedy/Hall
// matching on a threshold bipartite graph, computed with an O(301) sweep
// over values. Because larger values of i only ever have MORE edges,
// this per-opponent maximum k_j is achieved by matching i's largest
// values first, and the requirement must hold for every opponent
// independently and simultaneously (opponents never compete with each
// other for the same game slot -- only for player i's slot), so
// Mi = min over opponents j of k_j.
//
// mi: player i's wins are forced down to G minus the largest possible
// number of player i's own scores that can be "blocked" by SOME other
// player's score in the same game with pool_value >= i_value (using the
// combined pool of every other player's scores, since across games any
// opponent may supply the blocker). This is again a threshold bipartite
// matching (non-strict this time), computed the same way with an
// O(301) sweep against the pooled histogram of everybody but i.
// mi = G - (max such matching).
//
// Both sweeps were validated by hand against both sample cases before
// coding (they reproduce every mi/Mi value exactly, including the
// player whose mi differs from a naive full sort intuition).
//
// Complexity: O(P^2 * 301) per test case for Mi, O(P * 301) for mi --
// independent of G beyond building the histograms, so it easily fits
// the time limit even for the largest allowed P and G, and the earlier
// TLE complaints on the discuss board (from slower approaches) do not
// apply here.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXV = 301; // scores 0..300

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int P, G;
        scanf("%d %d", &P, &G);
        static int cnt[105][MAXV];
        static int scores[105][1005];
        for (int i = 0; i < P; i++) memset(cnt[i], 0, sizeof(cnt[i]));
        int total[MAXV];
        memset(total, 0, sizeof(total));
        for (int i = 0; i < P; i++) {
            for (int g = 0; g < G; g++) {
                int s;
                scanf("%d", &s);
                scores[i][g] = s;
                cnt[i][s]++;
                total[s]++;
            }
            sort(scores[i], scores[i] + G);
        }

        for (int i = 0; i < P; i++) {
            // mi: match player i's values (ascending) against pooled others'
            // values, condition pool_value >= i_value. This needs the
            // "smallest sufficient resource" two-pointer greedy (NOT the
            // simple ascending-accumulate trick used below for Mi), because
            // here larger i-values genuinely need larger pool values held
            // in reserve, unlike the strict '>' case where any smaller
            // opponent value is an interchangeable resource.
            int pool[MAXV];
            for (int v = 0; v < MAXV; v++) pool[v] = total[v] - cnt[i][v];
            int matched = 0;
            int vcur = 0;
            for (int idx = 0; idx < G; idx++) {
                int a = scores[i][idx];
                while (vcur < MAXV && (vcur < a || pool[vcur] == 0)) vcur++;
                if (vcur >= MAXV) break;
                pool[vcur]--;
                matched++;
            }
            int mi = G - matched;

            // Mi: for each opponent, strict matching, take min
            int Mi = G; // upper bound
            for (int j = 0; j < P; j++) {
                if (j == i) continue;
                int avail2 = 0;
                int m2 = 0;
                for (int v = 0; v < MAXV; v++) {
                    int take = cnt[i][v] < avail2 ? cnt[i][v] : avail2;
                    m2 += take;
                    avail2 -= take;
                    avail2 += cnt[j][v]; // add after matching (strict >)
                }
                if (m2 < Mi) Mi = m2;
            }

            printf("%d %d\n", mi, Mi);
        }
    }
    return 0;
}
