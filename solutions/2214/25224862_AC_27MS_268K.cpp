// POJ 2214 - Regions
// Model: gpt-6.1-sol
// Maximize votes possible without winning a strict majority of the seats.
// Losing or tied regions can contain floor(v/2) votes. Fully fill the
// floor(M/2) regions with largest remaining capacities ceil(v/2), then add one.
// The guarantee concerns every distribution of votes, not a chosen allocation.
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;
int main() {
    int cases;
    cin >> cases;
    while (cases--) {
        int m;
        cin >> m;
        vector<int> gain(m);
        long long answer = 1;
        for (int i = 0; i < m; ++i) {
            int v;
            cin >> v;
            answer += v / 2;
            gain[i] = (v + 1) / 2;
        }
        sort(gain.begin(), gain.end(), greater<int>());
        for (int i = 0; i < m / 2; ++i) answer += gain[i];
        cout << answer << " hlasu zajisti strane vitezstvi.\n";
    }
    return 0;
}
