// POJ 3225 - Help with Intervals
// Model: claude-sonnet-5
// Approach: double every coordinate (point a -> 2a, "just after a" -> 2a+1) so
// that all four bracket combinations of an integer-endpoint interval become a
// closed integer range [l, r] over 0..131071. A segment tree over that range
// supports range-assign(0/1) and range-flip with lazy propagation; each command
// reduces to a combination of these two primitives (U: assign 1 inside T; D:
// assign 0 inside T; I: assign 0 outside T; C: assign 0 outside T + flip inside
// T; S: flip inside T). The final set is read off as maximal runs of 1s and
// converted back to bracket notation. No statement ambiguity found; verified by
// hand-tracing the sample to (2,3).
#include <cstdio>
#include <cstring>

const int N = 131072; // leaves cover doubled indices 0..131071
int cover[4 * N];     // 0, 1, or -1 (mixed)
int assignLazy[4 * N]; // -1 = none, else 0/1
bool flipLazy[4 * N];

void build(int node, int l, int r) {
    cover[node] = 0;
    assignLazy[node] = -1;
    flipLazy[node] = false;
    if (l == r) return;
    int mid = (l + r) / 2;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
}

// Apply a flip directly to `node` (not recursing into its children): if a
// pending assign already fully determines the subtree, compose the flip into
// that assign value; otherwise toggle the summary cover (if uniform) and mark
// flipLazy so the flip reaches the children on the next pushDown. These two
// branches are mutually exclusive by construction (assign always clears
// flipLazy), so this never double-applies a flip.
void applyFlip(int node) {
    if (assignLazy[node] != -1) {
        assignLazy[node] ^= 1;
        cover[node] = assignLazy[node];
    } else {
        if (cover[node] != -1) cover[node] ^= 1;
        flipLazy[node] = !flipLazy[node];
    }
}

void pushDown(int node) {
    int lc = node * 2, rc = node * 2 + 1;
    if (assignLazy[node] != -1) {
        int v = assignLazy[node];
        cover[lc] = v; assignLazy[lc] = v; flipLazy[lc] = false;
        cover[rc] = v; assignLazy[rc] = v; flipLazy[rc] = false;
        assignLazy[node] = -1;
    }
    if (flipLazy[node]) {
        applyFlip(lc);
        applyFlip(rc);
        flipLazy[node] = false;
    }
}

void pullUp(int node) {
    int lc = node * 2, rc = node * 2 + 1;
    cover[node] = (cover[lc] == cover[rc]) ? cover[lc] : -1;
}

void updateAssign(int node, int nl, int nr, int l, int r, int val) {
    if (r < l || r < nl || nr < l) return;
    if (l <= nl && nr <= r) {
        cover[node] = val; assignLazy[node] = val; flipLazy[node] = false;
        return;
    }
    pushDown(node);
    int mid = (nl + nr) / 2;
    if (l <= mid) updateAssign(node * 2, nl, mid, l, r, val);
    if (r > mid) updateAssign(node * 2 + 1, mid + 1, nr, l, r, val);
    pullUp(node);
}

void updateFlip(int node, int nl, int nr, int l, int r) {
    if (r < l || r < nl || nr < l) return;
    if (l <= nl && nr <= r) {
        applyFlip(node);
        return;
    }
    pushDown(node);
    int mid = (nl + nr) / 2;
    if (l <= mid) updateFlip(node * 2, nl, mid, l, r);
    if (r > mid) updateFlip(node * 2 + 1, mid + 1, nr, l, r);
    pullUp(node);
}

int leaf[N];
void fillLeaves(int node, int nl, int nr) {
    if (nl == nr) { leaf[nl] = cover[node]; return; }
    pushDown(node);
    int mid = (nl + nr) / 2;
    fillLeaves(node * 2, nl, mid);
    fillLeaves(node * 2 + 1, mid + 1, nr);
}

int main() {
    build(1, 0, N - 1);

    char cmd, lb, rb;
    int a, b;
    while (scanf(" %c %c%d,%d%c", &cmd, &lb, &a, &b, &rb) == 5) {
        int l = 2 * a + (lb == '(' ? 1 : 0);
        int r = 2 * b - (rb == ')' ? 1 : 0);
        switch (cmd) {
            case 'U':
                if (l <= r) updateAssign(1, 0, N - 1, l, r, 1);
                break;
            case 'D':
                if (l <= r) updateAssign(1, 0, N - 1, l, r, 0);
                break;
            case 'I':
                if (l <= r) {
                    if (l > 0) updateAssign(1, 0, N - 1, 0, l - 1, 0);
                    if (r < N - 1) updateAssign(1, 0, N - 1, r + 1, N - 1, 0);
                } else {
                    updateAssign(1, 0, N - 1, 0, N - 1, 0);
                }
                break;
            case 'C':
                if (l <= r) {
                    if (l > 0) updateAssign(1, 0, N - 1, 0, l - 1, 0);
                    if (r < N - 1) updateAssign(1, 0, N - 1, r + 1, N - 1, 0);
                    updateFlip(1, 0, N - 1, l, r);
                } else {
                    updateAssign(1, 0, N - 1, 0, N - 1, 0);
                }
                break;
            case 'S':
                if (l <= r) updateFlip(1, 0, N - 1, l, r);
                break;
            default:
                break;
        }
    }

    fillLeaves(1, 0, N - 1);

    bool any = false;
    char buf[64];
    int i = 0;
    bool first = true;
    while (i < N) {
        if (leaf[i]) {
            int j = i;
            while (j + 1 < N && leaf[j + 1]) j++;
            int runL = i, runR = j;
            char lbo = (runL % 2 == 0) ? '[' : '(';
            int av = (runL % 2 == 0) ? runL / 2 : (runL - 1) / 2;
            char rbo = (runR % 2 == 0) ? ']' : ')';
            int bv = (runR % 2 == 0) ? runR / 2 : (runR + 1) / 2;
            if (!first) fputc(' ', stdout);
            first = false;
            any = true;
            sprintf(buf, "%c%d,%d%c", lbo, av, bv, rbo);
            fputs(buf, stdout);
            i = j + 1;
        } else {
            i++;
        }
    }
    if (!any) fputs("empty set", stdout);
    fputc('\n', stdout);
    return 0;
}
