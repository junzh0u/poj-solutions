// POJ 1105 - S-Trees
// Model: claude-opus-5
//
// An S-tree of depth n is a complete binary tree whose 2^n terminal nodes are
// given left-to-right as a bit string, plus a variable ordering xi1..xin saying
// which variable is tested at each depth.  Evaluating the function for a VVA is
// just walking down the tree, so the terminal reached is the one whose index in
// the leaf string has, as its bit of weight 2^(n-1-d), the VVA value of the
// variable tested at depth d.  No tree needs to be built: accumulate
//     idx = 2*idx + vva[order[d] - 1]
// over d = 0..n-1 and print leaves[idx].
//
// Statement notes: the VVA line is always indexed by x1..xn regardless of the
// variable ordering (the ordering only says at which depth each variable is
// tested).  Input ends at a test case with n = 0, which is not processed.  A
// blank line follows every test case, including the last.
//
// n <= 7, so everything fits in int; no recursion is used.

#include <cstdio>
#include <cstring>

int main() {
    int n;
    int caseNo = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        int order[8];
        for (int i = 0; i < n; ++i) {
            char var[32];
            if (scanf("%31s", var) != 1) return 0;
            /* the token looks like "x<k>"; take the digits after the 'x' */
            int k = 0;
            for (const char *p = var; *p; ++p) {
                if (*p >= '0' && *p <= '9') k = k * 10 + (*p - '0');
            }
            order[i] = k;
        }

        char leaves[200];
        if (scanf("%199s", leaves) != 1) return 0;

        int m;
        if (scanf("%d", &m) != 1) return 0;

        ++caseNo;
        printf("S-Tree #%d:\n", caseNo);
        for (int q = 0; q < m; ++q) {
            char vva[32];
            if (scanf("%31s", vva) != 1) return 0;
            int idx = 0;
            for (int d = 0; d < n; ++d) {
                int bit = (vva[order[d] - 1] == '1') ? 1 : 0;
                idx = idx * 2 + bit;
            }
            putchar(leaves[idx]);
        }
        putchar('\n');
        putchar('\n');
    }
    return 0;
}
