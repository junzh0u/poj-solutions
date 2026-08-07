// POJ 2368 - Buttons
// Model: claude-sonnet-5
// Bash's game: heap of K buttons, players alternately take 1..L, last to take
// wins. Standard theory: the player to move loses a pile of size n iff
// n % (L+1) == 0. First player fixes K, second player picks L in [2,K) to
// guarantee a second-player win, i.e. any L with (L+1) | K, 3 <= L+1 <= K.
// Every such L is a valid answer (K itself always qualifies via L=K-1), and
// the statement asks for the least valid L, i.e. the smallest divisor d>=3
// of K, answer = d-1.
// Find d by scanning 3..floor(sqrt(K)); if none divides K, the only
// remaining divisors are K/2 (when K is even) and K itself (divisor pairing:
// if no divisor of K lies in [3, sqrt(K)], the sole small divisors are 1
// and possibly 2, so the only divisors >= sqrt(K) are K/2 and K).
#include <cstdio>

int main() {
    int k;
    if (scanf("%d", &k) != 1) return 0;
    int d = -1;
    for (int i = 3; i * i <= k; ++i) {
        if (k % i == 0) { d = i; break; }
    }
    if (d == -1) {
        if (k % 2 == 0 && k / 2 >= 3) d = k / 2;
        else d = k;
    }
    printf("%d\n", d - 1);
    return 0;
}
