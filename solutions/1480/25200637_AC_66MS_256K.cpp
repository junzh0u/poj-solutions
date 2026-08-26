// POJ 1480 - Optimal Programs
// Model: gpt-5.6-terra
// Iterative-deepening DFS enumerates stack programs in lexical command order.
// A candidate is evaluated simultaneously for every supplied input; invalid
// arithmetic on any one input prunes that entire prefix.

#include <cstdio>
#include <cstdlib>
#include <cstring>

const int MAX_N = 10;
const int MAX_DEPTH = 10;

int n;
int st[MAX_N][MAX_DEPTH + 1];
int target[MAX_N];
int top;
int limit;
int program[MAX_DEPTH];

const char *name[] = {"ADD", "DIV", "DUP", "MUL", "SUB"};

bool matches() {
    int i;
    if (top != 0) return false;
    for (i = 0; i < n; ++i)
        if (st[i][0] != target[i]) return false;
    return true;
}

bool dfs(int used) {
    int remaining = limit - used;
    int op, i, value[MAX_N], backup[MAX_N][MAX_DEPTH + 1], oldTop;

    // A binary operation lowers top by one and DUP raises it by one.
    if (remaining < top || ((remaining - top) & 1)) return false;
    if (used == limit) return matches();

    for (op = 0; op < 5; ++op) {
        std::memcpy(backup, st, sizeof(st));
        oldTop = top;
        if (op == 2) { // DUP
            for (i = 0; i < n; ++i) st[i][top + 1] = st[i][top];
            ++top;
            program[used] = op;
            if (dfs(used + 1)) return true;
            top = oldTop;
            std::memcpy(st, backup, sizeof(st));
            continue;
        }

        if (top == 0) continue;
        bool valid = true;
        for (i = 0; i < n; ++i) {
            int a = st[i][top - 1];
            int b = st[i][top];
            if (op == 0) value[i] = a + b;
            else if (op == 1) {
                if (b == 0) {
                    valid = false;
                    break;
                }
                value[i] = a / b;
            } else if (op == 3) value[i] = a * b;
            else value[i] = a - b;
            if (value[i] > 30000 || value[i] < -30000) {
                valid = false;
                break;
            }
        }
        if (!valid) continue;

        for (i = 0; i < n; ++i) st[i][top - 1] = value[i];
        --top;
        program[used] = op;
        if (dfs(used + 1)) return true;
        top = oldTop;
        std::memcpy(st, backup, sizeof(st));
    }
    return false;
}

int main() {
    int caseNo = 1;
    while (std::scanf("%d", &n) == 1 && n != 0) {
        int i, found = -1;
        for (i = 0; i < n; ++i) std::scanf("%d", &st[i][0]);
        for (i = 0; i < n; ++i) std::scanf("%d", &target[i]);

        for (limit = 0; limit <= MAX_DEPTH; ++limit) {
            top = 0;
            if (dfs(0)) {
                found = limit;
                break;
            }
        }

        std::printf("Program %d\n", caseNo++);
        if (found == -1) std::printf("Impossible\n\n");
        else if (found == 0) std::printf("Empty sequence\n\n");
        else {
            for (i = 0; i < found; ++i) {
                if (i) std::printf(" ");
                std::printf("%s", name[program[i]]);
            }
            std::printf("\n\n");
        }
    }
    return 0;
}
