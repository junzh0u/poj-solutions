// POJ 3314 - Plaque Pack
// Model: gpt-5.6-terra
// Simulate each box as occupied unit cells.  A plaque is lowered one row at a
// time until its next move would overlap an occupied cell or the bottom.
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int top(const vector<int>& box) {
    int y;
    for (y = (int)box.size() - 1; y >= 0; --y)
        if (box[y] != 0) return y + 1;
    return 0;
}

int main() {
    int n, w, b;
    while (cin >> n >> w >> b && (n || w || b)) {
        vector<int> box(b, 0);
        vector<int> answer;
        int p;
        for (p = 0; p < n; ++p) {
            int h, r;
            cin >> h;
            vector<int> plaque(h, 0);
            for (r = 0; r < h; ++r) {
                string s;
                int c;
                cin >> s;
                for (c = 0; c < w; ++c)
                    if (s[c] == 'X') plaque[h - 1 - r] |= 1 << c;
            }

            int y = top(box);
            while (y > 0) {
                bool collide = false;
                for (r = 0; r < h; ++r)
                    if (plaque[r] && y - 1 + r < b &&
                        (box[y - 1 + r] & plaque[r])) {
                        collide = true;
                        break;
                    }
                if (collide) break;
                --y;
            }
            bool fits = true;
            for (r = 0; r < h; ++r)
                if (plaque[r] && y + r >= b) fits = false;
            if (!fits) {
                answer.push_back(top(box));
                box.assign(b, 0);
                y = 0;
            }
            for (r = 0; r < h; ++r)
                if (plaque[r]) box[y + r] |= plaque[r];
        }
        answer.push_back(top(box));
        for (p = 0; p < (int)answer.size(); ++p) {
            if (p) cout << ' ';
            cout << answer[p];
        }
        cout << '\n';
    }
    return 0;
}
