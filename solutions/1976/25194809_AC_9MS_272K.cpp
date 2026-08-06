// POJ 1976 - A Mini Locomotive
// Model: gpt-5.6-terra
// Dynamic programming over prefixes: for each of one, two, and three locomotives,
// either skip the current coach or end a consecutive block of at most m coaches at it.
// A monotone deque maximizes the predecessor over the last m prefix positions.
#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    if (!(cin >> tests)) return 0;
    while (tests--) {
        int n, m;
        cin >> n;
        vector<int> prefix(n + 1, 0);
        int i;
        for (i = 1; i <= n; ++i) {
            int passengers;
            cin >> passengers;
            prefix[i] = prefix[i - 1] + passengers;
        }
        cin >> m;

        vector<int> previous(n + 1, 0);
        vector<int> current(n + 1, 0);
        int locomotives;
        for (locomotives = 1; locomotives <= 3; ++locomotives) {
            fill(current.begin(), current.end(), 0);
            deque<int> candidates;
            for (i = 1; i <= n; ++i) {
                int previous_index = i - 1;
                while (!candidates.empty() &&
                       previous[candidates.back()] - prefix[candidates.back()] <=
                           previous[previous_index] - prefix[previous_index]) {
                    candidates.pop_back();
                }
                candidates.push_back(previous_index);
                while (!candidates.empty() && candidates.front() < i - m) {
                    candidates.pop_front();
                }
                int take = prefix[i] + previous[candidates.front()] - prefix[candidates.front()];
                current[i] = max(current[i - 1], take);
            }
            previous.swap(current);
        }
        cout << previous[n] << '\n';
    }
    return 0;
}
