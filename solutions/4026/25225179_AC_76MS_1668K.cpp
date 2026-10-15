// POJ 4026 - Eve
// Model: gpt-6.1-sol
// Each birth inherits its mother's initial maternal lineage. Measurements
// label those lineages, including measurements of deceased individuals.
// Distinct known surviving labels force NO; otherwise one lineage or all
// known equal labels force YES. Any remaining unknown lineage is POSSIBLY.
// The original NEERC statement was used because POJ's statement is empty.
#include <cstdio>
#include <vector>
using namespace std;
int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        vector<int> root(n + 1), dna(n + 1, 0);
        vector<char> alive(n + 1, 1);
        char sex;
        for (int i = 1; i <= n; ++i) {
            scanf(" %c", &sex);
            root[i] = i;
        }
        int m;
        scanf("%d", &m);
        for (int i = 0; i < m; ++i) {
            int a, b;
            scanf("%d", &a);
            if (a < 0) alive[-a] = 0;
            else {
                scanf("%d %c", &b, &sex);
                root.push_back(root[b]);
                alive.push_back(1);
            }
        }
        int k;
        scanf("%d", &k);
        for (int i = 0; i < k; ++i) {
            int a, label;
            scanf("%d%d", &a, &label);
            dna[root[a]] = label;
        }
        vector<char> seen(n + 1, 0);
        int groups = 0, unknown = 0, label = 0;
        bool different = false;
        for (int i = 1; i < (int)root.size(); ++i) {
            int r = root[i];
            if (!alive[i] || seen[r]) continue;
            seen[r] = 1;
            ++groups;
            if (!dna[r]) ++unknown;
            else if (!label) label = dna[r];
            else if (label != dna[r]) different = true;
        }
        if (different) puts("NO");
        else if (groups == 1 || unknown == 0) puts("YES");
        else puts("POSSIBLY");
    }
    return 0;
}
