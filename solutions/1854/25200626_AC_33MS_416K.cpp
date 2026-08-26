// POJ 1854 - Evil Straw Warts Live
// Model: gpt-5.6-terra
// Pair each character's outermost remaining occurrences.  Moving the right
// occurrence to the current right edge costs the number of live positions
// after it; a Fenwick tree supplies that count.  A lone odd occurrence moves
// to the middle of the remaining interval.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Fenwick {
public:
    Fenwick(int n) : n_(n), bit_(n + 1, 0) {}

    void add(int index, int value) {
        for (; index <= n_; index += index & -index)
            bit_[index] += value;
    }

    int sum(int index) const {
        int result = 0;
        for (; index > 0; index -= index & -index)
            result += bit_[index];
        return result;
    }

private:
    int n_;
    vector<int> bit_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    cin >> tests;
    while (tests--) {
        string s;
        cin >> s;
        int n = (int)s.size();
        vector< vector<int> > positions(26);
        int count[26] = {0};
        int i;
        for (i = 0; i < n; ++i) {
            positions[s[i] - 'a'].push_back(i + 1);
            ++count[s[i] - 'a'];
        }

        int odd = 0;
        for (i = 0; i < 26; ++i)
            odd += count[i] & 1;
        if (odd > 1) {
            cout << "Impossible\n";
            continue;
        }

        vector<int> left(26, 0), right(26);
        for (i = 0; i < 26; ++i)
            right[i] = (int)positions[i].size() - 1;

        Fenwick live(n);
        for (i = 1; i <= n; ++i)
            live.add(i, 1);

        // At most n*(n-1)/2 = 31,996,000 swaps for n <= 8000.
        int answer = 0;
        int remaining = n;
        for (i = 1; i <= n; ++i) {
            int ch = s[i - 1] - 'a';
            if (left[ch] > right[ch] || positions[ch][left[ch]] != i)
                continue;

            int mate = positions[ch][right[ch]];
            if (mate == i) {
                answer += (remaining - 1) / 2;
                live.add(i, -1);
                ++left[ch];
                --right[ch];
                --remaining;
            } else {
                answer += remaining - live.sum(mate);
                live.add(i, -1);
                live.add(mate, -1);
                ++left[ch];
                --right[ch];
                remaining -= 2;
            }
        }
        cout << answer << '\n';
    }
    return 0;
}
