// POJ 3640 - Conformity
// Model: claude-sonnet-5
// Approach: for each frosh, sort the 5 course numbers into a canonical
// tuple so combinations that differ only in listed order compare equal.
// Sort all n tuples, sweep to find the maximum group size (popularity),
// then sum the sizes of every group that attains that maximum -- students
// in ANY tied-for-most-popular combination count, not just one group
// (sample 2 has three singleton combinations, all tied at popularity 1,
// and the expected answer is 3, the sum of all three, not 1).
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

struct Combo {
    int c[5];
    bool operator<(const Combo& o) const {
        for (int i = 0; i < 5; i++) {
            if (c[i] != o.c[i]) return c[i] < o.c[i];
        }
        return false;
    }
    bool operator==(const Combo& o) const {
        for (int i = 0; i < 5; i++) if (c[i] != o.c[i]) return false;
        return true;
    }
};

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<Combo> arr(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 5; j++) scanf("%d", &arr[i].c[j]);
            sort(arr[i].c, arr[i].c + 5);
        }
        sort(arr.begin(), arr.end());

        int maxCount = 0;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && arr[j] == arr[i]) j++;
            int cnt = j - i;
            if (cnt > maxCount) maxCount = cnt;
            i = j;
        }

        int total = 0;
        i = 0;
        while (i < n) {
            int j = i;
            while (j < n && arr[j] == arr[i]) j++;
            int cnt = j - i;
            if (cnt == maxCount) total += cnt;
            i = j;
        }

        printf("%d\n", total);
    }
    return 0;
}
