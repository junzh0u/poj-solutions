// POJ 2654 - Rock-Paper-Scissors Tournament
// Model: claude-sonnet-5
// Each of n players plays k games against each of the other n-1 players,
// so the number of game lines following the "n k" header is k*n*(n-1)/2,
// not k itself (the board's top thread flags this as the main trap: a
// solution that read exactly k lines desyncs and drifts into Output Limit
// Exceeded). Moves are encoded rock=0, paper=1, scissors=2; move a beats
// move b when (b+1)%3==a, which reproduces "paper beats rock, scissors
// beats paper, rock beats scissors" in one formula. Win average is w/(w+l)
// with the game itself excluding ties, so a player who only tied (or never
// played, e.g. n==1) has w+l==0 and prints "-" per the statement.
#include <cstdio>
#include <cstring>

int wins[105], losses[105];

int moveCode(const char *s) {
    if (s[0] == 'r') return 0;
    if (s[0] == 'p') return 1;
    return 2; // scissors
}

int main() {
    int n, k;
    bool first = true;
    while (scanf("%d", &n) == 1) {
        if (n == 0) break;
        scanf("%d", &k);
        // k,n <= 100 so k*n*(n-1)/2 <= 495000, well inside int range.
        int games = k * n * (n - 1) / 2;
        memset(wins, 0, sizeof(wins));
        memset(losses, 0, sizeof(losses));
        char m1[16], m2[16];
        int p1, p2;
        for (int g = 0; g < games; g++) {
            scanf("%d %s %d %s", &p1, m1, &p2, m2);
            int c1 = moveCode(m1);
            int c2 = moveCode(m2);
            if (c1 == c2) {
                // tie, no change
            } else if ((c2 + 1) % 3 == c1) {
                wins[p1]++;
                losses[p2]++;
            } else {
                wins[p2]++;
                losses[p1]++;
            }
        }
        if (!first) printf("\n");
        first = false;
        for (int i = 1; i <= n; i++) {
            int w = wins[i], l = losses[i];
            if (w + l == 0) {
                printf("-\n");
            } else {
                printf("%.3f\n", (double)w / (double)(w + l));
            }
        }
    }
    return 0;
}
