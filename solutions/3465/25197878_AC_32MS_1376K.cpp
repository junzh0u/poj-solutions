// POJ 3465 - Battle
// Model: claude-sonnet-5
// Approach: Greedy simulation with a max-heap "regret" mechanism. We need
// k = ceil(H2/x) attacks to kill the dragon. Every attack round costs full
// counterattack damage A_i, except the k-th (killing) attack, which costs 0
// since the dragon dies before it can counterattack (confirmed by Sample 1,
// where round 4's A=10 would otherwise kill you). Non-attack rounds let us
// pick the better of Defend (0 change) or Heal (+ (y - A_i)), i.e. a
// guaranteed non-negative HP delta of max(0, y - A_i).
//
// We greedily attack every round until we are one attack away from k
// (count == k-1), retroactively "undoing" the highest-cost committed attack
// (via a max-heap) whenever HP would drop to <= 0; undoing round j replaces
// its -A_j delta with the non-attack delta, a net correction of
// A_j + max(0, y - A_j) = max(A_j, y), which is maximized by undoing the
// largest A_j, so a simple max-heap keyed on A_j suffices. This greedy
// prefix-maximizes the number of attacks survivable at every point (a
// standard "regret" exchange argument, confirmed against the board's own
// posted greedy+heap editorial), so the first round at which count reaches
// k-1 lets the very next round finish for free -> minimal Win round.
//
// If we exhaust all N rounds without reaching k, the answer for Lose is
// x times the maximum attack count ever validly achieved. Special case
// (also from the discuss board): the very last round (round N) never needs
// correction/undo, since failing to kill within N rounds means you lose
// regardless of your HP afterward -- so on round N we always take the
// attack unconditionally, even if it would drop HP <= 0.
//
// Statement ambiguity: whether Heal applies before or after Heilong's
// counterattack doesn't change the net per-round HP delta we use, but the
// board confirms Heal happens before the counterattack anyway.

#include <cstdio>
#include <queue>
using namespace std;

int main() {
    long long N, x, y, H1, H2;
    scanf("%lld %lld %lld %lld %lld", &N, &x, &y, &H1, &H2);
    long long k = (H2 + x - 1) / x;
    long long need = k - 1; // attacks needed before the free finishing blow

    long long HP = H1;
    long long count = 0;
    long long maxCount = 0;
    priority_queue<long long> heap;
    bool win = false;
    long long winRound = -1;

    for (long long i = 1; i <= N; ++i) {
        long long A;
        scanf("%lld", &A);
        if (count == need) {
            count++;
            win = true;
            winRound = i;
            break;
        }
        count++;
        if (i < N) {
            HP -= A;
            heap.push(A);
            while (HP <= 0) {
                long long top = heap.top();
                heap.pop();
                HP += (top > y ? top : y);
                count--;
            }
        }
        // if i == N: last round, no correction needed, attack unconditionally
        if (count > maxCount) maxCount = count;
    }

    if (win) {
        printf("Win\n%lld\n", winRound);
    } else {
        printf("Lose\n%lld\n", maxCount * x);
    }
    return 0;
}
