// POJ 3045 - Cow Acrobats
// Model: gpt-5.6-terra
// Sort cows from top to bottom by increasing W+S.  For two adjacent cows A
// above B, swapping them is never worse exactly when A.W+A.S <= B.W+B.S.
// Then scan downward: accumulated weight is the weight above the current cow.
// Risk may be negative, including when N is one.
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

struct Cow {
    int weight;
    int strength;
};

bool topFirst(const Cow &a, const Cow &b) {
    return a.weight + a.strength < b.weight + b.strength;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    if (!(cin >> n)) return 0;
    vector<Cow> cows(n);
    for (int i = 0; i < n; ++i) cin >> cows[i].weight >> cows[i].strength;
    sort(cows.begin(), cows.end(), topFirst);

    int above = 0;
    int answer = INT_MIN;
    for (int i = 0; i < n; ++i) {
        answer = max(answer, above - cows[i].strength);
        above += cows[i].weight;
    }
    cout << answer << '\n';
    return 0;
}
