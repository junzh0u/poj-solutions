// POJ 2783 - On Storing Clothes
// Model: gpt-6.1-sol
// Simulate the circular rail, scanning empty windows from the current mark.
// Separators may be shared by neighboring batches; only clothes block a
// deposit window. On withdrawal, report newly free hooks in circular order.
#include <cstdio>
#include <cstring>
int main() {
    int N, lines;
    if (scanf("%d%d", &N, &lines) != 2) return 0;
    int state[300], length[300];
    memset(state, 0, sizeof(state));
    memset(length, 0, sizeof(length));
    int pos = 0;
    for (int op = 0; op < lines; ++op) {
        char command;
        int x;
        scanf(" %c%d", &command, &x);
        if (command == 'D') {
            int k = -1;
            if (x >= 1 && x <= N - 2) {
                int blocked = 0;
                for (int j = 0; j < x + 2; ++j)
                    blocked += state[(pos + j) % N] == 1;
                for (int d = 0; d < N; ++d) {
                    if (!blocked) { k = (pos + d) % N; break; }
                    blocked -= state[(pos + d) % N] == 1;
                    blocked += state[(pos + d + x + 2) % N] == 1;
                }
            }
            if (k < 0) puts("No space left, please come back later.");
            else {
                length[k] = x;
                state[k] = state[(k + x + 1) % N] = 2;
                for (int j = 1; j <= x; ++j) state[(k + j) % N] = 1;
                pos = (k + x + 1) % N;
                printf("The launderer gives ticket %d.\n", k);
            }
        } else {
            pos = x;
            int n = length[x];
            printf("The launderer gives back batch %d.\n", x);
            for (int j = 1; j <= n; ++j) state[(x + j) % N] = 0;
            int end = (x + n + 1) % N;
            if (state[(x + N - 1) % N] != 1) {
                state[x] = 0;
                printf("%d is freed.\n", x);
            }
            for (int j = 1; j <= n; ++j) printf("%d is freed.\n", (x + j) % N);
            if (state[(end + 1) % N] != 1) {
                state[end] = 0;
                printf("%d is freed.\n", end);
            }
            length[x] = 0;
        }
    }
    return 0;
}
