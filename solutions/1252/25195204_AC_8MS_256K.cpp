// POJ 1252 - Euro Efficiency
// Model: claude-opus-5
//
// Paying an amount uses coins in both directions (payment and change), so the
// cost of an amount a is the least k with a = sum of k coin values each taken
// with a + or - sign.  Every amount is a node, each coin gives unit-weight
// edges v -> v+c and v -> v-c, so a single BFS from 0 gives all costs at once;
// the answer is the sum / 100 and the max over a = 1..100.  Coin 1 is always
// present, so every amount is reachable.
//
// The only subtlety is how far the state graph must extend.  An optimal signed
// multiset can be reordered so every partial sum stays in [-98, 199]: while the
// partial sum is below the target take a positive coin (one must remain, else
// the remaining negatives could never reach the target), otherwise take a
// negative one; each coin is < 100 and the target is <= 100.  So 199 suffices;
// [-200, 1000] is used for margin.  Truncating at 100 is the classic trap here
// and the official sample does NOT expose it -- all three sample lines come out
// right with a range of [0,100], while 1 95 96 97 98 99 gives 22.19 47 instead
// of 12.29 25.
//
// Printed with integer arithmetic (%d.%02d) rather than %f: the average is
// exactly sum/100, and the discuss board's recurring "G++ WA but C++ AC" on
// this problem is a printf("%.2lf") bug, which this avoids entirely.

#include <cstdio>

const int LO = -200;
const int HI = 1000;
const int NN = HI - LO + 1;

int dis[NN];
int que[NN];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int c[6];
        int i;
        int ok = 1;
        for (i = 0; i < 6; i++)
            if (scanf("%d", &c[i]) != 1) ok = 0;
        if (!ok) break;

        for (i = 0; i < NN; i++) dis[i] = -1;
        int head = 0, tail = 0;
        dis[0 - LO] = 0;
        que[tail++] = 0;
        while (head < tail) {
            int v = que[head++];
            int d = dis[v - LO] + 1;
            for (i = 0; i < 6; i++) {
                int u = v + c[i];
                if (u <= HI && dis[u - LO] < 0) { dis[u - LO] = d; que[tail++] = u; }
                u = v - c[i];
                if (u >= LO && dis[u - LO] < 0) { dis[u - LO] = d; que[tail++] = u; }
            }
        }

        int sum = 0, mx = 0;
        for (int a = 1; a <= 100; a++) {
            int d = dis[a - LO];
            sum += d;
            if (d > mx) mx = d;
        }
        printf("%d.%02d %d\n", sum / 100, sum % 100, mx);
    }
    return 0;
}
