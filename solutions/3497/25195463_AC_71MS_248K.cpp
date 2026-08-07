// POJ 3497 - Assemble
// Model: claude-sonnet-5
//
// Buy exactly one component per distinct type, staying within budget,
// maximizing the minimum quality among the chosen components.
//
// Sort all components across all types by quality descending. Walk the
// list; for each type track the cheapest price seen so far among
// components processed (i.e. with quality >= current threshold). The
// first prefix (in descending-quality order) at which every type has been
// seen AND the sum of per-type cheapest prices is <= budget gives the
// answer: the current item's quality. This works because as the quality
// threshold falls, each type's cheapest-so-far price is monotonically
// non-increasing (more candidates become available), so the total cost is
// monotonically non-increasing too -- feasibility, once reached, never
// becomes infeasible again. Hence the first quality value (walking from
// the top) at which the running sum fits the budget and all types are
// covered is optimal; ties in quality don't need special batching since
// every component sharing a quality value reports that same quality.
//
// The discuss board confirms two points worth pinning down locally: a
// higher-quality component is not guaranteed to be pricier than a
// lower-quality one of the same type (so only tracking the max-quality
// item per type, rather than the true running minimum price, is wrong),
// and every test case is guaranteed feasible (board: "seems like [the
// impossible case] doesn't happen" / "I think it won't happen") -- the
// -1 fallback below is defensive only.
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

struct Comp {
    int type;
    int price;
    int quality;
};

bool cmpDesc(const Comp &a, const Comp &b) {
    return a.quality > b.quality;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n;
        int b;
        scanf("%d %d", &n, &b);
        vector<Comp> comps(n);
        map<string, int> typeId;
        char typebuf[32], namebuf[32];
        for (int i = 0; i < n; i++) {
            int price, quality;
            scanf("%s %s %d %d", typebuf, namebuf, &price, &quality);
            string t(typebuf);
            map<string, int>::iterator it = typeId.find(t);
            int id;
            if (it == typeId.end()) {
                id = (int)typeId.size();
                typeId[t] = id;
            } else {
                id = it->second;
            }
            comps[i].type = id;
            comps[i].price = price;
            comps[i].quality = quality;
        }
        int numTypes = (int)typeId.size();
        sort(comps.begin(), comps.end(), cmpDesc);

        vector<int> best(numTypes, -1);
        int sum = 0;
        int covered = 0;
        int answer = -1;
        for (int i = 0; i < n; i++) {
            int ty = comps[i].type;
            int pr = comps[i].price;
            if (best[ty] == -1) {
                best[ty] = pr;
                sum += pr;
                covered++;
            } else if (pr < best[ty]) {
                sum -= (best[ty] - pr);
                best[ty] = pr;
            }
            if (covered == numTypes && sum <= b) {
                answer = comps[i].quality;
                break;
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
