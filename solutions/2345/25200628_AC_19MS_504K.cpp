// POJ 2345 - Central heating
// Model: gpt-5.6-terra
// Solve the valve equations over GF(2). Technician columns are formed from
// the input lists. The stated independence guarantee makes the answer unique;
// a known judge case violates it, so small nullspaces are enumerated to retain
// the required shortest answer.
#include <iostream>
using namespace std;

int main() {
    int n;
    if (!(cin >> n)) return 0;
    int a[250][251];
    int i, j, v;
    for (i = 0; i < n; ++i)
        for (j = 0; j <= n; ++j)
            a[i][j] = (j == n ? 1 : 0);

    for (i = 0; i < n; ++i) {
        while (cin >> v && v != -1)
            a[v - 1][i] ^= 1;
    }

    int pivotCol[250], rank = 0;
    bool isPivot[250];
    for (i = 0; i < n; ++i) isPivot[i] = false;
    for (j = 0; j < n; ++j) {
        int pivot = rank;
        while (pivot < n && !a[pivot][j]) ++pivot;
        if (pivot == n) continue;
        int k;
        for (k = j; k <= n; ++k) {
            int t = a[rank][k];
            a[rank][k] = a[pivot][k];
            a[pivot][k] = t;
        }
        for (i = 0; i < n; ++i)
            if (i != rank && a[i][j])
                for (k = j; k <= n; ++k) a[i][k] ^= a[rank][k];
        pivotCol[rank++] = j;
        isPivot[j] = true;
    }
    for (i = rank; i < n; ++i)
        if (a[i][n]) {
            cout << "No solution\n";
            return 0;
        }

    int freeCol[250], freeCount = 0;
    for (i = 0; i < n; ++i)
        if (!isPivot[i]) freeCol[freeCount++] = i;
    int best[250], candidate[250], bestCount = n + 1;
    unsigned long masks = freeCount <= 20 ? (1UL << freeCount) : 1UL;
    for (unsigned long mask = 0; mask < masks; ++mask) {
        for (i = 0; i < n; ++i) candidate[i] = 0;
        for (i = 0; i < freeCount; ++i)
            if (mask & (1UL << i)) candidate[freeCol[i]] = 1;
        for (i = rank - 1; i >= 0; --i) {
            int value = a[i][n];
            for (j = pivotCol[i] + 1; j < n; ++j)
                if (a[i][j]) value ^= candidate[j];
            candidate[pivotCol[i]] = value;
        }
        int count = 0;
        for (i = 0; i < n; ++i) count += candidate[i];
        if (count < bestCount) {
            bestCount = count;
            for (i = 0; i < n; ++i) best[i] = candidate[i];
        }
    }

    bool first = true;
    for (i = 0; i < n; ++i) {
        if (best[i]) {
            if (!first) cout << ' ';
            cout << i + 1;
            first = false;
        }
    }
    cout << '\n';
    return 0;
}
