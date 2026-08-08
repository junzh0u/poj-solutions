// POJ 2751 - Saving Endeavour
// Model: claude-opus-5
//
// Two-machine flow shop, F2||Cmax: every part is processed by S1 and then by
// S2, each workshop handles one part at a time, minimise the makespan.
// Solved by Johnson's rule:
//   set A = parts with a < b, ordered by a ascending;
//   set B = parts with a >= b, ordered by b descending;
//   process A then B, and simulate the two workshops over that sequence.
// The simulation must carry S2's own busy time: S2 starts a part at
// max(its S1 completion, S2's previous finish).  Dropping that max still
// reproduces every line of the official sample, so the sample does not
// witness the rule -- it was pinned by differential testing instead.
//
// Framing the statement leaves open, hardened rather than assumed:
//   - blocks repeat until a lone 0, and the run also stops cleanly at EOF
//     if the terminating 0 is absent;
//   - N is read into a vector, so data with N above the stated 10000 is fine;
//   - the makespan accumulates in long long, so times above the stated
//     a, b <= 100 cannot overflow.  Submitted as G++ for long long, with
//     iostream output rather than printf("%lld").
//
// Zero durations are legal (a discuss-board case turns on them) and need no
// special casing: a part with a == 0 or b == 0 simply occupies its workshop
// for no time.  Splitting the a == b parts into A instead of B is a genuine
// equivalence, not a latent bug -- either placement is a valid Johnson order.

#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>

struct Job {
    int a, b;
};

/* Johnson set A: a < b, ascending by a. */
static bool cmpA(const Job& x, const Job& y) { return x.a < y.a; }
/* Johnson set B: a >= b, descending by b. */
static bool cmpB(const Job& x, const Job& y) { return x.b > y.b; }

int main() {
    int n;
    while (std::scanf("%d", &n) == 1 && n != 0) {
        std::vector<Job> A, B;
        for (int i = 0; i < n; ++i) {
            Job j;
            if (std::scanf("%d %d", &j.a, &j.b) != 2) return 0;
            if (j.a < j.b) A.push_back(j);
            else B.push_back(j);
        }
        std::sort(A.begin(), A.end(), cmpA);
        std::sort(B.begin(), B.end(), cmpB);

        /* t1 = when S1 becomes free, t2 = when S2 becomes free. */
        long long t1 = 0, t2 = 0;
        for (size_t i = 0; i < A.size(); ++i) {
            t1 += A[i].a;
            t2 = (t1 > t2 ? t1 : t2) + A[i].b;
        }
        for (size_t i = 0; i < B.size(); ++i) {
            t1 += B[i].a;
            t2 = (t1 > t2 ? t1 : t2) + B[i].b;
        }
        std::cout << t2 << "\n";
    }
    return 0;
}
