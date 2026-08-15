// POJ 2199 - Rate of Return
// Model: claude-sonnet-5
// Approach: binary search on the monthly interest rate i in [0,1]. For a
// candidate i, the mutual fund's equivalent value at the final reporting
// month is f(i) = sum_k amount[k] * (1+i)^(finalMonth - month[k] + 1); this
// is strictly increasing in i (all amounts positive, 1+i>0), so f(i)-value
// has a unique root in [0,1], found by ~100 bisection iterations (well past
// the precision needed for 5 fractional digits).
// No statement ambiguity beyond a resolved board dispute (msg 12484/12491)
// about whether the sample's 0.01635 or 0.01090 is correct; the statement's
// own worked formula gives 0.01635, matching the official sample output.
// Board note (msg 114682) recommends tight binary-search precision.
#include <cstdio>
#include <cmath>

int main() {
    int n;
    int caseNum = 0;
    bool firstOutput = true;
    while (scanf("%d", &n) == 1 && n != -1) {
        int month[13];
        double amount[13];
        for (int i = 0; i < n; ++i) {
            scanf("%d %lf", &month[i], &amount[i]);
        }
        int finalMonth;
        double finalValue;
        scanf("%d %lf", &finalMonth, &finalValue);

        double lo = 0.0, hi = 1.0;
        for (int iter = 0; iter < 200; ++iter) {
            double mid = (lo + hi) / 2.0;
            double total = 0.0;
            for (int i = 0; i < n; ++i) {
                int exponent = finalMonth - month[i] + 1;
                total += amount[i] * pow(1.0 + mid, exponent);
            }
            if (total < finalValue) {
                lo = mid;
            } else {
                hi = mid;
            }
        }

        ++caseNum;
        if (!firstOutput) {
            printf("\n");
        }
        firstOutput = false;
        printf("Case %d: %.5f\n", caseNum, (lo + hi) / 2.0);
    }
    return 0;
}
