// POJ 2473 - Decorate the wall
// Model: gpt-5.6-terra
// Enumerate lower-left corners whose coordinates are the wall origin or the
// right/top edge of an existing painting.  Any feasible placement can slide
// down and then left until it reaches one of those boundaries.
#include <iostream>
#include <vector>
using namespace std;

struct Rect {
    int x1, y1, x2, y2;
};

int main() {
    ios::sync_with_stdio(false);
    int tests;
    if (!(cin >> tests)) return 0;
    while (tests--) {
        int n, w, h;
        cin >> n >> w >> h;
        vector<Rect> a(n);
        vector<int> xs(1, 0), ys(1, 0);
        int i, j, pw, ph;
        for (i = 0; i < n; ++i) {
            cin >> a[i].x1 >> a[i].y1 >> a[i].x2 >> a[i].y2;
            xs.push_back(a[i].x2);
            ys.push_back(a[i].y2);
        }
        cin >> pw >> ph;
        bool found = false;
        int answerX = 0, answerY = 0;
        for (i = 0; i < (int)ys.size(); ++i) {
            int y = ys[i];
            if (y + ph > h) continue;
            for (j = 0; j < (int)xs.size(); ++j) {
                int x = xs[j];
                if (x + pw > w) continue;
                bool ok = true;
                int k;
                for (k = 0; k < n; ++k) {
                    if (x < a[k].x2 && x + pw > a[k].x1 &&
                        y < a[k].y2 && y + ph > a[k].y1) {
                        ok = false;
                        break;
                    }
                }
                if (ok && (!found || y < answerY || (y == answerY && x < answerX))) {
                    found = true;
                    answerX = x;
                    answerY = y;
                }
            }
        }
        if (found) cout << answerX << ' ' << answerY << '\n';
        else cout << "Fail!\n";
    }
    return 0;
}
