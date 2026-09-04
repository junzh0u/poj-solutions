// POJ 3914 - DuLL
// Model: claude-sonnet-5
// Approach: simulate the sequence of program start/stop transitions. Keep a
// running instance count per program and a running "need count" per DuLL
// (how many currently-running program instances require that DuLL). Total
// memory at any moment = sum(instanceCount[q] * programSize[q]) +
// sum(dullSize[d] for d with needCount[d] > 0). A DuLL's needCount only
// matters as zero/nonzero (whether it must be resident), so incrementing it
// once per starting instance and decrementing once per exiting instance
// keeps the "resident or not" flag correct even with multiple simultaneous
// instances of the same program. Update a running max after every
// transition (S <= 32, P <= 9, N <= 20, so recomputation is trivial).
// No statement ambiguity found; verified against both sample data sets by
// hand simulation before submitting.
#include <cstdio>
#include <cstring>

int main() {
    int n, p, s;
    while (scanf("%d", &n) == 1 && n != 0) {
        scanf("%d %d", &p, &s);
        int dllSize[26];
        for (int i = 0; i < n; ++i) scanf("%d", &dllSize[i]);

        int progSize[10];
        char progDlls[10][32];
        for (int i = 1; i <= p; ++i) {
            scanf("%d %s", &progSize[i], progDlls[i]);
        }

        int instanceCount[10];
        memset(instanceCount, 0, sizeof(instanceCount));
        int needCount[26];
        memset(needCount, 0, sizeof(needCount));

        int total = 0; // sum(instanceCount[q]*progSize[q]) + sum(dllSize[d] resident)
        int maxTotal = 0;

        for (int i = 0; i < s; ++i) {
            int t;
            scanf("%d", &t);
            if (t > 0) {
                int q = t;
                instanceCount[q]++;
                total += progSize[q];
                int len = (int)strlen(progDlls[q]);
                for (int k = 0; k < len; ++k) {
                    int d = progDlls[q][k] - 'A';
                    needCount[d]++;
                    if (needCount[d] == 1) total += dllSize[d];
                }
            } else {
                int q = -t;
                instanceCount[q]--;
                total -= progSize[q];
                int len = (int)strlen(progDlls[q]);
                for (int k = 0; k < len; ++k) {
                    int d = progDlls[q][k] - 'A';
                    needCount[d]--;
                    if (needCount[d] == 0) total -= dllSize[d];
                }
            }
            if (total > maxTotal) maxTotal = total;
        }

        printf("%d\n", maxTotal);
    }
    return 0;
}
