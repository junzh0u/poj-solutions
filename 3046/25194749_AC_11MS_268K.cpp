// POJ 3046 - Ant Counting
// Model: gpt-5.6-terra
// Count bounded selections from each indistinguishable ant family.  A sliding
// window computes each bounded-knapsack transition in linear time per family.
// The stated A bound is omitted, so arrays are sized from the input total.

#include <iostream>
#include <vector>

using namespace std;

const int MOD = 1000000;

int main() {
    int families, ants, low, high;
    if (!(cin >> families >> ants >> low >> high)) {
        return 0;
    }

    vector<int> count(families + 1, 0);
    int type;
    for (int i = 0; i < ants; ++i) {
        cin >> type;
        ++count[type];
    }

    vector<int> dp(ants + 1, 0);
    vector<int> next(ants + 1, 0);
    dp[0] = 1;
    int reached = 0;

    for (int family = 1; family <= families; ++family) {
        int copies = count[family];
        if (copies == 0) {
            continue;
        }

        int window = 0;
        int limit = reached + copies;
        for (int size = 0; size <= limit; ++size) {
            if (size <= reached) {
                window += dp[size];
                if (window >= MOD) {
                    window -= MOD;
                }
            }
            if (size - copies - 1 >= 0) {
                window -= dp[size - copies - 1];
                if (window < 0) {
                    window += MOD;
                }
            }
            next[size] = window;
        }
        dp.swap(next);
        reached = limit;
    }

    int answer = 0;
    for (int size = low; size <= high; ++size) {
        answer += dp[size];
        if (answer >= MOD) {
            answer -= MOD;
        }
    }
    cout << answer << '\n';
    return 0;
}
