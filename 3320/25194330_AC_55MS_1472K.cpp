// POJ 3320 - Jessica's Reading Problem
#include <cstdio>
#include <algorithm>
using namespace std;

static int a[1000000];
static int srt[1000000];
static int comp[1000000];
static int cnt[1000000];

int main() {
    int p;
    if (scanf("%d", &p) != 1) return 0;
    for (int i = 0; i < p; ++i) {
        scanf("%d", &a[i]);
        srt[i] = a[i];
    }
    sort(srt, srt + p);
    int d = (int)(unique(srt, srt + p) - srt);
    for (int i = 0; i < p; ++i) {
        comp[i] = (int)(lower_bound(srt, srt + d, a[i]) - srt);
    }
    for (int i = 0; i < d; ++i) cnt[i] = 0;

    int distinctCount = 0;
    int best = p; // worst case whole book
    int left = 0;
    for (int right = 0; right < p; ++right) {
        if (cnt[comp[right]] == 0) ++distinctCount;
        ++cnt[comp[right]];
        if (distinctCount == d) {
            while (cnt[comp[left]] > 1) {
                --cnt[comp[left]];
                ++left;
            }
            int len = right - left + 1;
            if (len < best) best = len;
        }
    }
    printf("%d\n", best);
    return 0;
}
