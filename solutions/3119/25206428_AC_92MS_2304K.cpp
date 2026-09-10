// POJ 3119 - Friends or Enemies?
// Model: claude-sonnet-5
//
// The clockwise spiral enumeration (deduced from the figure, Figure 1 in the
// statement) starts at (0,0) and then moves in direction order
// LEFT, UP, RIGHT, DOWN, LEFT, UP, ... (clockwise), with run lengths
// 1,1,2,2,3,3,4,4,... (each length used for two consecutive runs before
// increasing by one). This is the classic square spiral. Points that fall on
// the border line y = a*x + b are skipped entirely when assigning numbers
// (this also covers the (0,0)-on-border special case, since it's just the
// same skip rule applied to the very first point).
//
// For a test case we only need points up to the largest queried index, so we
// walk the canonical spiral, skip border points, and record the sequence of
// surviving (x,y) coordinates until we have enough of them. Since the border
// contributes only about one point per unit of spiral "radius" while the
// spiral covers radius^2 points, the number of raw steps needed is
// (queried index) + O(sqrt(queried index)), so this stays fast even for the
// maximum index of 65535 and up to 100 test cases.
//
// Once we have the coordinates for the two queried indices, comparing which
// side of the line y = a*x + b they fall on is just comparing the sign of
// (y - a*x - b) (never zero for a surviving point, by construction).
#include <cstdio>
#include <vector>
using namespace std;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; tc++) {
        int a, b;
        scanf("%d %d", &a, &b);
        int K;
        scanf("%d", &K);
        vector<int> ms(K), ns(K);
        int need = 0;
        for (int i = 0; i < K; i++) {
            scanf("%d %d", &ms[i], &ns[i]);
            if (ms[i] + 1 > need) need = ms[i] + 1;
            if (ns[i] + 1 > need) need = ns[i] + 1;
        }

        vector<pair<int,int> > pts;
        pts.reserve(need + 8);

        int x = 0, y = 0;
        if (y != a * x + b) pts.push_back(make_pair(x, y));

        static const int dxs[4] = {-1, 0, 1, 0};
        static const int dys[4] = {0, 1, 0, -1};
        int dirIdx = 0, len = 1, used = 0;
        bool done = (int)pts.size() >= need;
        while (!done) {
            for (int s = 0; s < len; s++) {
                x += dxs[dirIdx];
                y += dys[dirIdx];
                if (y != a * x + b) pts.push_back(make_pair(x, y));
                if ((int)pts.size() >= need) { done = true; break; }
            }
            used++;
            if (used == 2) { len++; used = 0; }
            dirIdx = (dirIdx + 1) % 4;
        }

        printf("Caso %d\n", tc);
        for (int i = 0; i < K; i++) {
            int m = ms[i], n = ns[i];
            int vm = pts[m].second - a * pts[m].first - b;
            int vn = pts[n].second - a * pts[n].first - b;
            bool sameSide = (vm > 0) == (vn > 0);
            if (sameSide) printf("Mesmo lado da fronteira\n");
            else printf("Lados opostos da fronteira\n");
        }
    }
    return 0;
}
