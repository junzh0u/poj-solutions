// POJ 2952 - Election
// Model: gpt-5.6-terra
// Simulate each instant-runoff round.  Every group contributes all of its
// voters to its highest-ranked candidate that has not yet been eliminated.
// Among equally least popular candidates, eliminate the largest number.
#include <iostream>
using namespace std;

int main() {
    int groups, candidates;
    while (cin >> groups >> candidates && (groups || candidates)) {
        int voters[20], preference[20][5];
        for (int i = 0; i < groups; ++i) {
            cin >> voters[i];
            for (int j = 0; j < candidates; ++j) cin >> preference[i][j];
        }
        bool alive[6];
        for (int c = 1; c <= candidates; ++c) alive[c] = true;
        for (int remaining = candidates; remaining > 1; --remaining) {
            int votes[6] = {0};
            for (int i = 0; i < groups; ++i) {
                for (int j = 0; j < candidates; ++j) {
                    int c = preference[i][j];
                    if (alive[c]) {
                        votes[c] += voters[i];
                        break;
                    }
                }
            }
            int eliminated = -1, least = 1000000000;
            for (int c = 1; c <= candidates; ++c) {
                if (alive[c] && (votes[c] < least || (votes[c] == least && c > eliminated))) {
                    least = votes[c];
                    eliminated = c;
                }
            }
            alive[eliminated] = false;
        }
        for (int c = 1; c <= candidates; ++c) if (alive[c]) cout << c << '\n';
    }
    return 0;
}
