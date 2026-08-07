// POJ 3367 - Expressions
// Model: claude-opus-5
//
// Build the expression tree from the postfix string (lowercase = operand leaf,
// uppercase = operator, whose right child is the top of the stack and left child
// the one below, per the statement's `a := pop(); b := pop(); push(b O a);`),
// then emit a BFS level-order traversal (left child before right) and print it
// reversed.
//
// Why: with a queue, popping twice takes the two *oldest* pending values, so the
// queue consumes the token stream in reverse-BFS order exactly when the printed
// string is the reversed level order of the same tree.  Reversing the whole
// concatenation flips both the level order (deepest level first) and the order
// within each level (right to left), which is what the queue algorithm rebuilds.
//
// The board's only real pitfall is speed: per-node malloc/new and STL containers
// storing whole nodes have been reported TLE here.  Everything below is static
// arrays with index-based children and an explicit index queue, no allocation
// and no recursion (a degenerate chain is ~5000 deep).
#include <cstdio>
#include <cstring>

const int MAXN = 10005;

static char code_[MAXN];
static int lch[MAXN], rch[MAXN];
static int stk[MAXN];
static int que[MAXN];
static char out[MAXN];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        if (scanf("%s", code_) != 1) break;
        int n = (int)strlen(code_);
        int sp = 0;
        for (int i = 0; i < n; ++i) {
            if (code_[i] >= 'a' && code_[i] <= 'z') {
                lch[i] = rch[i] = -1;
            } else {
                int a = stk[--sp];   /* first pop  -> right operand */
                int b = stk[--sp];   /* second pop -> left operand  */
                lch[i] = b;
                rch[i] = a;
            }
            stk[sp++] = i;
        }
        int root = stk[sp - 1];
        int head = 0, tail = 0, len = 0;
        que[tail++] = root;
        while (head < tail) {
            int u = que[head++];
            out[len++] = code_[u];
            if (lch[u] != -1) que[tail++] = lch[u];
            if (rch[u] != -1) que[tail++] = rch[u];
        }
        for (int i = len - 1; i >= 0; --i) putchar(out[i]);
        putchar('\n');
    }
    return 0;
}
