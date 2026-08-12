// POJ 2605 - Simple game on a grid
// Model: gpt-5.6-terra
// A full rectangle reduces by local peg-solitaire moves.  When its shorter
// side has at most two rows, it is equivalent to one line and leaves
// ceil(longer / 2) stones.  Otherwise a side divisible by three leaves two
// stones; every other rectangle can be reduced to one stone.
#include <cstdio>

int main() {
    int m, n;
    while (std::scanf("%d%d", &m, &n) == 2) {
        if (m > n) {
            int t = m;
            m = n;
            n = t;
        }
        if (m <= 2)
            std::printf("%d\n", (n + 1) / 2);
        else if (m % 3 == 0 || n % 3 == 0)
            std::printf("2\n");
        else
            std::printf("1\n");
    }
    return 0;
}
