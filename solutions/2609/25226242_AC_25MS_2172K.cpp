// POJ 2609 - Ferry Loading
// Model: gpt-6-astra
// DP over prefix length and occupied port length. The prefix sum determines
// starboard length. Remember each last placement to reconstruct a witness.
// Only a consecutive prefix may board; neither equal counts nor equal loads
// are required. Stop at the first prefix with no feasible arrangement.
#include <cstdio>
#include <vector>
using namespace std;
int main() {
    int metres;
    while (scanf("%d", &metres) == 1) {
        int capacity = metres * 100, total = 0, car;
        vector<int> cars;
        vector< vector<unsigned char> > dp(1, vector<unsigned char>(capacity+1,0));
        dp[0][0] = 1;
        bool stopped = false;
        while (scanf("%d", &car) == 1 && car != 0) {
            if (stopped) continue;
            vector<unsigned char> next(capacity+1,0);
            bool possible = false;
            for (int p=0; p<=capacity; ++p) if (dp.back()[p]) {
                if (p+car <= capacity) { next[p+car]=1; possible=true; }
                if (total-p+car <= capacity) { next[p]=2; possible=true; }
            }
            if (!possible) { stopped=true; continue; }
            cars.push_back(car);
            total += car;
            dp.push_back(next);
        }
        int n = (int)cars.size(), p=0;
        while (!dp[n][p]) ++p;
        vector<int> side(n);
        for (int i=n; i>0; --i) {
            side[i-1] = dp[i][p];
            if (side[i-1]==1) p-=cars[i-1];
        }
        printf("%d\n", n);
        for (int i=0; i<n; ++i) puts(side[i]==1 ? "port" : "starboard");
    }
    return 0;
}
