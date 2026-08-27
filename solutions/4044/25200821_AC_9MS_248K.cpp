// POJ 4044 - Score Sequence
// Model: gpt-5.6-terra
// Deduplicate each class, sort both score lists descending, and use the
// longest-common-substring DP to find a consecutive common block.  Equal-size
// blocks are resolved by their first (therefore highest) descending score.
// The selected block is then ordered by units digit, breaking ties by value.

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

static vector<int> normalized(vector<int> scores) {
    sort(scores.begin(), scores.end());
    scores.erase(unique(scores.begin(), scores.end()), scores.end());
    reverse(scores.begin(), scores.end());
    return scores;
}

static bool by_units_then_value(int a, int b) {
    if (a % 10 != b % 10) return a % 10 < b % 10;
    return a < b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    cin >> tests;
    while (tests--) {
        int n1, n2;
        cin >> n1 >> n2;
        vector<int> first(n1), second(n2);
        for (int i = 0; i < n1; ++i) cin >> first[i];
        for (int i = 0; i < n2; ++i) cin >> second[i];
        first = normalized(first);
        second = normalized(second);

        vector< vector<int> > dp(first.size() + 1,
                                 vector<int>(second.size() + 1, 0));
        int best_length = 0;
        int best_end = -1;
        for (int i = 1; i <= (int)first.size(); ++i) {
            for (int j = 1; j <= (int)second.size(); ++j) {
                if (first[i - 1] == second[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    if (dp[i][j] > best_length ||
                        (dp[i][j] == best_length && best_length > 0 &&
                         first[i - dp[i][j]] > first[best_end - best_length + 1])) {
                        best_length = dp[i][j];
                        best_end = i - 1;
                    }
                }
            }
        }

        if (best_length == 0) {
            cout << "NONE\n";
            continue;
        }
        vector<int> answer(first.begin() + best_end - best_length + 1,
                           first.begin() + best_end + 1);
        for (int i = 0; i < (int)answer.size(); ++i) {
            if (i) cout << ' ';
            cout << answer[i];
        }
        cout << '\n';
        sort(answer.begin(), answer.end(), by_units_then_value);
        for (int i = 0; i < (int)answer.size(); ++i) {
            if (i) cout << ' ';
            cout << answer[i];
        }
        cout << '\n';
    }
    return 0;
}
