// POJ 3622 - Gourmet Grazers
// Model: claude-fable-5
// Greedy with a multiset. Each cow i needs a distinct grass with price
// C >= A_i and greenness D >= B_i; minimize the sum of chosen grass prices.
// Process cows by greenness demand B descending; insert every grass whose
// greenness D >= current B into a multiset keyed by price; assign the cow the
// cheapest grass of price >= A_i (member lower_bound). Exchange argument:
// grasses in the set satisfy the greenness demand of every remaining cow, so
// only price matters, and swapping the cheapest feasible grass into the
// current cow never hurts a later one. Sum reaches 1e5 * 1e9 = 1e14, so the
// accumulator is long long (language G++).
#include <cstdio>
#include <algorithm>
#include <set>
using namespace std;

struct P {
    int a, b; // price demand/value, greenness demand/value
};

bool byBdesc(const P& x, const P& y) { return x.b > y.b; }

P cow[100000], grass[100000];

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 0; i < n; i++) scanf("%d %d", &cow[i].a, &cow[i].b);
    for (int j = 0; j < m; j++) scanf("%d %d", &grass[j].a, &grass[j].b);
    sort(cow, cow + n, byBdesc);
    sort(grass, grass + m, byBdesc);
    multiset<int> avail;
    long long total = 0;
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j < m && grass[j].b >= cow[i].b) {
            avail.insert(grass[j].a);
            j++;
        }
        multiset<int>::iterator it = avail.lower_bound(cow[i].a);
        if (it == avail.end()) {
            printf("-1\n");
            return 0;
        }
        total += *it;
        avail.erase(it);
    }
    printf("%lld\n", total);
    return 0;
}
