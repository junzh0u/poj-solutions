// POJ 1359 - Spacecraft Malfunction
// Model: claude-sonnet-5
//
// Exactly one of the n units is faulty; every other unit is good and its
// two check reports are truthful. Each report line gives, for a unit,
// the two units it checked and, per check, whether the judge output says
// the checked unit is good (Y) or faulty (N) -- confirmed against the
// problem text: "a single character which is either Y or N, showing
// whether the result of checking is good or faulty respectively".
//
// For each candidate unit X (hypothesis: X is the faulty one, everyone
// else is good), verify every report from a unit other than X: each of
// its two claims about some target must match the target's actual status
// under the hypothesis (faulty iff target == X). Reports made by X itself
// are unconstrained since a faulty unit's statements may be anything.
// If exactly one candidate survives this check, it is the answer;
// otherwise (zero or more than one) the faulty unit cannot be identified
// and the answer is "impossible".
//
// n <= 100 and t <= 10, so brute-forcing every candidate against every
// report (O(n^2) per test case) is trivial within the time limit.
#include <cstdio>
#include <map>

using namespace std;

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int n;
        scanf("%d", &n);
        int id[105];
        int tgt[105][2];
        bool good[105][2]; // claim: true = good(Y), false = faulty(N)
        map<int, int> idx;
        for (int i = 0; i < n; i++) {
            int self;
            scanf("%d", &self);
            id[i] = self;
            for (int k = 0; k < 2; k++) {
                int tid;
                char c[4];
                scanf("%d %s", &tid, c);
                tgt[i][k] = tid;
                good[i][k] = (c[0] == 'Y');
            }
            idx[self] = i;
        }
        int consistentCount = 0;
        int answerId = -1;
        for (int cand = 0; cand < n; cand++) {
            bool valid = true;
            for (int i = 0; i < n && valid; i++) {
                if (i == cand) continue;
                for (int k = 0; k < 2; k++) {
                    int targetIdx = idx[tgt[i][k]];
                    bool expectedGood = (targetIdx != cand);
                    if (good[i][k] != expectedGood) {
                        valid = false;
                        break;
                    }
                }
            }
            if (valid) {
                consistentCount++;
                answerId = id[cand];
            }
        }
        if (consistentCount == 1) {
            printf("%d\n", answerId);
        } else {
            printf("impossible\n");
        }
    }
    return 0;
}
