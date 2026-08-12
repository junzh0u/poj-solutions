// POJ 2426 - Remainder
// Model: gpt-5.6-terra
// Breadth-first search over remainders modulo K.  A modulus operation can be
// useful in a shortest answer only as the first operation, or immediately
// after the first multiplication: before a multiplication, adding/subtracting
// M does not change x mod M; after one, x is already a multiple of M.

#include <cstdio>
#include <cstring>

const int MAXK = 1000;
const int START = 2 * MAXK;

struct Node {
    int previous;
    char operation;
};

int normalizedMod(int x, int m) {
    int r = x % m;
    if (r < 0) r += m;
    return r;
}

int main() {
    int n, k, m;
    while (std::scanf("%d%d%d", &n, &k, &m) == 3 && (n || k || m)) {
        int target = normalizedMod(n + 1, k);
        int initial = normalizedMod(n, k);
        if (initial == target) {
            std::puts("0");
            continue;
        }

        // State r is an ordinary residue.  State MAXK+r is only the state
        // reached by the first '*', where one following '%' is still allowed.
        Node from[2 * MAXK + 1];
        bool seen[2 * MAXK + 1];
        std::memset(seen, 0, sizeof(seen));
        int queue[2 * MAXK + 1];
        int head = 0, tail = 0;
        queue[tail++] = START;
        seen[START] = true;
        from[START].previous = -1;

        int answer = -1;
        while (head < tail && answer == -1) {
            int state = queue[head++];
            int r;
            if (state == START) r = initial;
            else if (state >= MAXK) r = state - MAXK;
            else r = state;

            int next[4];
            char op[4];
            int count = 0;
            if (state == START) {
                next[count] = normalizedMod(r + m, k); op[count++] = '+';
                next[count] = normalizedMod(r - m, k); op[count++] = '-';
                next[count] = MAXK + normalizedMod(r * m, k); op[count++] = '*';
                next[count] = normalizedMod(normalizedMod(n, m), k); op[count++] = '%';
            } else if (state >= MAXK) {
                next[count] = normalizedMod(r + m, k); op[count++] = '+';
                next[count] = normalizedMod(r - m, k); op[count++] = '-';
                next[count] = normalizedMod(r * m, k); op[count++] = '*';
                next[count] = 0; op[count++] = '%';
            } else {
                next[count] = normalizedMod(r + m, k); op[count++] = '+';
                next[count] = normalizedMod(r - m, k); op[count++] = '-';
                next[count] = normalizedMod(r * m, k); op[count++] = '*';
            }

            for (int i = 0; i < count; ++i) {
                int to = next[i];
                if (seen[to]) continue;
                seen[to] = true;
                from[to].previous = state;
                from[to].operation = op[i];
                queue[tail++] = to;
                if (to % MAXK == target) {
                    answer = to;
                    break;
                }
            }
        }

        if (answer == -1) {
            std::puts("0");
            continue;
        }
        char result[2 * MAXK + 1];
        int length = 0;
        for (int state = answer; state != START; state = from[state].previous)
            result[length++] = from[state].operation;
        std::printf("%d\n", length);
        while (length > 0) std::putchar(result[--length]);
        std::putchar('\n');
    }
    return 0;
}
