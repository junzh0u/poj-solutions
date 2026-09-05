// POJ 2035 - Hit or Miss
// Model: gpt-5.6-terra
// Simulate each lockstep round by visiting players from last to first: a card
// passed to player i+1 is then received after that player's turn, as required.
// The official contest reference classifies a game as unwinnable after 5301
// consecutive rounds without a match; the same cutoff is used here.
#include <iostream>
#include <queue>
using namespace std;

int main() {
    int cases;
    if (!(cin >> cases)) return 0;
    for (int tc = 1; tc <= cases; ++tc) {
        int players;
        cin >> players;
        queue<int> deck[11];
        int count[10] = {0};
        int last[10] = {0};
        int x;
        for (int i = 0; i < 52; ++i) {
            cin >> x;
            deck[0].push(x);
        }

        int quiet = 0;
        while (quiet <= 5300) {
            for (int p = players - 1; p >= 0; --p) {
                if (deck[p].empty()) continue;
                ++count[p];
                if (count[p] == 14) count[p] = 1;
                x = deck[p].front();
                deck[p].pop();
                if (x == count[p]) {
                    deck[p + 1].push(x);
                    last[p] = x;
                    quiet = 0;
                } else {
                    deck[p].push(x);
                }
            }
            ++quiet;
        }

        bool empty = true;
        for (int p = 0; p < players; ++p)
            if (!deck[p].empty()) empty = false;
        cout << "Case " << tc << ':';
        if (!empty) {
            cout << " unwinnable\n";
        } else {
            for (int p = 0; p < players; ++p) cout << ' ' << last[p];
            cout << '\n';
        }
    }
    return 0;
}
