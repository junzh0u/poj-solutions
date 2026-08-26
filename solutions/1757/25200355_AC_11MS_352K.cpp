// POJ 1757 - Binary Search
// Model: gpt-5.6-terra
// Enumerate every allowed array length.  With a strictly increasing array,
// searching for index i follows exactly the same comparisons as any
// nondecreasing array in which BinarySearch returns i, so simulating the
// given loop decides feasibility.  Consecutive feasible lengths are merged.
// The discussion board confirms the upper bound N <= 10000 is significant.

#include <iostream>

using namespace std;

int comparisonsToFind(int n, int target) {
    int p = 0;
    int q = n - 1;
    int comparisons = 0;
    while (p <= q) {
        int mid = (p + q) / 2;
        ++comparisons;
        if (mid == target) return comparisons;
        if (target < mid) q = mid - 1;
        else p = mid + 1;
    }
    return -1;
}

int main() {
    int target, wanted;
    if (!(cin >> target >> wanted)) return 0;

    int left[10000], right[10000], count = 0;
    bool inside = false;
    for (int n = 1; n <= 10000; ++n) {
        bool possible = comparisonsToFind(n, target) == wanted;
        if (possible && !inside) {
            left[count] = n;
            inside = true;
        }
        if (!possible && inside) {
            right[count] = n - 1;
            ++count;
            inside = false;
        }
    }
    if (inside) {
        right[count] = 10000;
        ++count;
    }

    cout << count << '\n';
    for (int k = 0; k < count; ++k) {
        cout << left[k] << ' ' << right[k] << '\n';
    }
    return 0;
}
