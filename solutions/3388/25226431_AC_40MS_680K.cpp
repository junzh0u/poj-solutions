// POJ 3388 - Japanese Puzzle
// Model: gpt-6-astra
// For r equal rows, picture i supplies floor(count[i]/r) columns.
// Binary-search the largest r with at least n available columns, then
// emit any n of them. Only the repeated row is printed, one entry per line.
#include <cstdio>
#include <vector>
using namespace std;
int main() {
    int n, k;
    if (scanf("%d%d", &n, &k) != 2) return 0;
    vector<int> count(k);
    for (int i = 0; i < k; ++i) scanf("%d", &count[i]);
    int lo = 1, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;
        int columns = 0;
        for (int i = 0; i < k && columns < n; ++i)
            columns += count[i] / mid;
        if (columns >= n) lo = mid;
        else hi = mid - 1;
    }
    printf("%d\n", lo);
    int remaining = n;
    for (int i = 0; i < k && remaining; ++i) {
        int take = count[i] / lo;
        if (take > remaining) take = remaining;
        for (int j = 0; j < take; ++j) printf("%d\n", i + 1);
        remaining -= take;
    }
    return 0;
}
