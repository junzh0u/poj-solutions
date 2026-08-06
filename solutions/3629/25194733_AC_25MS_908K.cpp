// POJ 3629 - Card Stacking
// Model: claude-sonnet-5
// Approach: Direct simulation with a deque of card positions (1..K), one
// deque per test case (only one test case here). Bessie is player 0; the
// deal starts with the player to her right, i.e. player index (1 mod N),
// and advances cyclically through indices 1,2,...,N-1,0,1,... For each
// deal: pop the front card and give it to the current player (record it if
// the player is Bessie), then move the next P cards from front to back of
// the deque one card at a time. Ambiguity note: when fewer than P cards
// remain, the forum discuss board (message 341511, replying to 149058)
// clarifies the P-card move must still be done "one card at a time" for
// the full P steps, wrapping around the (small) remaining deque rather
// than stopping early at whatever is left. This matters: for N=5 K=10 P=4
// naive "move min(P,remaining)" gives Bessie {3,5}, but the confirmed AC
// answer is {5,9}, matching the one-card-at-a-time full-P-steps rule used
// here. Verified against both the official sample (3 9 2 -> 3 7 8) and
// that forum example (5 10 4 -> 5 9).
#include <cstdio>
#include <deque>
#include <algorithm>
using namespace std;

int main() {
    int n, k, p;
    if (scanf("%d %d %d", &n, &k, &p) != 3) return 0;

    deque<int> dq;
    for (int i = 1; i <= k; ++i) dq.push_back(i);

    deque<int> bessieCards;
    int player = 1 % n;
    for (int deal = 0; deal < k; ++deal) {
        int card = dq.front();
        dq.pop_front();
        if (player == 0) bessieCards.push_back(card);
        for (int j = 0; j < p; ++j) {
            if (dq.empty()) break;
            int c = dq.front();
            dq.pop_front();
            dq.push_back(c);
        }
        player = (player + 1) % n;
    }

    sort(bessieCards.begin(), bessieCards.end());
    for (size_t i = 0; i < bessieCards.size(); ++i) {
        printf("%d\n", bessieCards[i]);
    }
    return 0;
}
