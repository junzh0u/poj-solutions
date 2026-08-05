// POJ 2586 - Y2K Accounting Bug
//
// Twelve months, each either a surplus of +s or a deficit of -d. Every one of
// the 8 consecutive 5-month windows is known to have posted a deficit, i.e. a
// strictly negative sum. Maximise the yearly total, or print "Deficit" if no
// positive total is possible.
//
// A window holding a surplus months sums to a*s - (5-a)*d, which is negative
// iff a*s < (5-a)*d. The left side grows with a and the right side shrinks, so
// the condition is monotone: it holds for every window iff it holds for the
// window with the most surplus months. The whole constraint therefore collapses
// to "no 5-month window contains more than m surplus months", where m is the
// largest value satisfying m*s < (5-m)*d (m <= 4, since 5 surpluses can never
// sum negative).
//
// With k surplus months the year totals k*s - (12-k)*d = k*(s+d) - 12*d, which
// is increasing in k, so the answer is driven purely by the largest k allowed
// by the window cap. bestK[m] is precomputed once by enumerating all 2^12
// patterns: bestK[0..4] = 0, 3, 6, 8, 10. Each query is then O(1)-ish.
//
// Ambiguity: "posted a deficit" is read as strictly negative (sum < 0), not
// break-even -- sample line "59 237" only works that way round (4*59 = 236 vs
// 237). Symmetrically, a best yearly total of exactly 0 is not a surplus, so it
// prints "Deficit" too; that case arises e.g. for s == d.

#include <iostream>

// bestK[m] = max number of surplus months over 12 months such that every
// consecutive 5-month window holds at most m surplus months.
static int bestK[6];

static void build() {
    for (int m = 0; m <= 5; ++m) bestK[m] = -1;
    for (int mask = 0; mask < (1 << 12); ++mask) {
        int k = 0, worst = 0;
        for (int i = 0; i < 12; ++i) if (mask >> i & 1) ++k;
        for (int st = 0; st + 5 <= 12; ++st) {
            int c = 0;
            for (int i = st; i < st + 5; ++i) if (mask >> i & 1) ++c;
            if (c > worst) worst = c;
        }
        for (int m = worst; m <= 5; ++m) if (k > bestK[m]) bestK[m] = k;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    build();
    long long s, d;
    while (std::cin >> s >> d) {
        long long best = 0;
        bool ok = false;
        for (int m = 0; m <= 4; ++m) {
            // a window with m surpluses posts a deficit iff m*s - (5-m)*d < 0
            if (m * s >= (long long)(5 - m) * d) continue;
            long long k = bestK[m];
            long long tot = k * s - (12 - k) * d;
            if (!ok || tot > best) { best = tot; ok = true; }
        }
        if (ok && best > 0) std::cout << best << "\n";
        else std::cout << "Deficit\n";
    }
    return 0;
}
