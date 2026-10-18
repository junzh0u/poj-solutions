// POJ 2414 - Phylogenetic Trees Inherited
// Model: gpt-6-astra
// Solve each sequence position independently with Fitch's set recurrence.
// Intersect sibling optimum sets when possible; otherwise take their union
// and pay one change. Any letter remaining at the root is optimal.
// Leaf order fixes sibling pairs; the root sequence need not be unique.
#include <cstdio>
static char leaf[1024][1001];
static unsigned int mask[2048];
int main() {
    int n, length;
    while (std::scanf("%d%d", &n, &length) == 2 && n != 0) {
        for (int i = 0; i < n; ++i) std::scanf("%1000s", leaf[i]);
        int cost = 0;
        for (int p = 0; p < length; ++p) {
            for (int i = 0; i < n; ++i)
                mask[n+i] = 1u << (leaf[i][p]-'A');
            for (int v = n-1; v >= 1; --v) {
                mask[v] = mask[2*v] & mask[2*v+1];
                if (!mask[v]) {
                    mask[v] = mask[2*v] | mask[2*v+1];
                    ++cost;
                }
            }
            int c = 25;
            while (!(mask[1] & (1u << c))) --c;
            std::putchar('A'+c);
        }
        std::printf(" %d\n", cost);
    }
    return 0;
}
