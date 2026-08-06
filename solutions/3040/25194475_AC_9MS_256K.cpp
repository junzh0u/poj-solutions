// POJ 3040 - Allowance
//
// FJ has N (<= 20) coin denominations, each dividing the next larger one, with
// B_i coins of value V_i. Every week he must hand over at least C; maximise the
// number of weeks. Overpaying is allowed and, as the sample shows, sometimes
// necessary (the lone 10-cent coin buys one week against C = 6).
//
// The divisibility chain is what makes greedy correct: any coin still in stock
// after the descending pass is worth strictly more than the remaining need, so
// there is never a cheaper combination of smaller coins to finish a week with.
//
// One week's payment is therefore built as:
//   1. coins worth >= C are banked up front, one week each -- nothing better
//      can be done with a coin that already covers a whole week on its own;
//   2. descending by value, take min(stock, need / value) of each coin, which
//      never overshoots C;
//   3. if still short, add exactly one coin -- the cheapest one still in stock,
//      which minimises the overpayment.
// The same payment pattern is then repeated min_i(B_i / use_i) times in one
// step, so the loop runs a handful of iterations rather than once per week
// (there can be tens of millions of weeks).
//
// The answer is bounded by the total coin count (2e7) since every week spends
// at least one coin, but it is kept in long long and printed digit by digit:
// POJ's C++ and G++ disagree about %lld, and printing by hand sidesteps that.
// All input numbers fit in int, so they are read with %d.

#include <cstdio>
#include <algorithm>

struct Coin {
    long long v;
    long long b;
};

static bool byValueDesc(const Coin& a, const Coin& b) { return a.v > b.v; }

static void printLL(long long x) {
    char buf[32];
    int p = 0;
    if (x == 0) { putchar('0'); return; }
    if (x < 0) { putchar('-'); x = -x; }
    while (x > 0) { buf[p++] = (char)('0' + (int)(x % 10)); x /= 10; }
    while (p > 0) putchar(buf[--p]);
}

int main() {
    /* Every input number fits in an int (C, V <= 1e8, B <= 1e6), so read with
       %d and widen; only the answer needs 64 bits, and it is printed by hand
       because POJ's compilers disagree about %lld. */
    int n, cc;
    if (scanf("%d %d", &n, &cc) != 2) return 0;
    long long c = cc;
    Coin coin[25];
    for (int i = 0; i < n; ++i) {
        int v, b;
        if (scanf("%d %d", &v, &b) != 2) return 0;
        coin[i].v = v;
        coin[i].b = b;
    }
    std::sort(coin, coin + n, byValueDesc);

    long long ans = 0;
    /* A coin worth at least C pays a whole week by itself; nothing better can
       be done with it, so bank those weeks up front. */
    for (int i = 0; i < n; ++i) {
        if (coin[i].v >= c) {
            ans += coin[i].b;
            coin[i].b = 0;
        }
    }

    long long use[25];
    for (;;) {
        long long need = c;
        int i;
        for (i = 0; i < n; ++i) use[i] = 0;
        /* Largest first, take as many as fit without exceeding C. */
        for (i = 0; i < n; ++i) {
            if (coin[i].b > 0 && coin[i].v <= need) {
                long long take = need / coin[i].v;
                if (take > coin[i].b) take = coin[i].b;
                use[i] = take;
                need -= take * coin[i].v;
            }
        }
        /* Still short: overpay with the cheapest coin still available. Every
           coin with stock left is worth more than the remainder, so one is
           always enough. */
        if (need > 0) {
            for (i = n - 1; i >= 0; --i) {
                if (coin[i].b > use[i]) {
                    use[i]++;
                    need -= coin[i].v;
                    break;
                }
            }
        }
        if (need > 0) break; /* cannot reach C any more */

        /* Repeat this exact payment as often as the stock allows. */
        long long weeks = -1;
        for (i = 0; i < n; ++i) {
            if (use[i] > 0) {
                long long k = coin[i].b / use[i];
                if (weeks < 0 || k < weeks) weeks = k;
            }
        }
        if (weeks <= 0) break;
        ans += weeks;
        for (i = 0; i < n; ++i) coin[i].b -= use[i] * weeks;
    }

    printLL(ans);
    putchar('\n');
    return 0;
}
