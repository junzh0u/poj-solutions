// POJ 2698 - Servicing DVD Requests
// Model: claude-sonnet-5
// Approach: Belady's optimal offline cache replacement algorithm (MIN / OPT).
// Simulate the sequence of requests; when a requested DVD is not currently
// loaded and all k drives are full, evict the DVD among those loaded whose
// next use (strictly after the current request) is farthest in the future,
// treating a DVD with no future use as having infinite next-use distance.
// This greedy choice is a classical theorem (Belady 1966) known to minimize
// the total number of insertions for the offline paging problem, which is
// exactly this problem restated (k drives = cache of size k, "insertion" =
// cache miss/load). n <= 100 and k <= 10 keep an O(n^2) simulation trivial.
// No statement ambiguity: worked example in the text matches this algorithm
// exactly (evict DVD 2, whose next use is never, over DVD 1, used again).
#include <cstdio>

int main() {
    int m;
    if (scanf("%d", &m) != 1) return 0;
    while (m--) {
        int k, n;
        scanf("%d %d", &k, &n);
        static int req[105];
        for (int i = 0; i < n; i++) scanf("%d", &req[i]);

        static int drive[15]; // dvd id currently in each drive slot, -1 if empty
        for (int i = 0; i < 15; i++) drive[i] = -1;

        int insertions = 0;
        for (int i = 0; i < n; i++) {
            int x = req[i];
            bool present = false;
            int slot = -1;
            for (int j = 0; j < k; j++) {
                if (drive[j] == x) { present = true; break; }
                if (drive[j] == -1 && slot == -1) slot = j;
            }
            if (present) continue;

            insertions++;
            if (slot != -1) {
                drive[slot] = x;
            } else {
                // all k drives occupied; find farthest-future slot to evict
                int evict = 0;
                int farthest = -1;
                for (int j = 0; j < k; j++) {
                    int nextUse = n; // treat "never used again" as n (infinite, beyond last index)
                    for (int t = i + 1; t < n; t++) {
                        if (req[t] == drive[j]) { nextUse = t; break; }
                    }
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        evict = j;
                    }
                }
                drive[evict] = x;
            }
        }
        printf("%d\n", insertions);
    }
    return 0;
}
