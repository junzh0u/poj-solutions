// POJ 3923 - Ugly Windows
// Model: gpt-5.6-terra
// A visible top window has its complete frame and an otherwise empty bounding
// rectangle.  The extrema of each visible letter give that frame's rectangle.
// Checking the complete rectangle rejects both obscured frames and windows
// containing a frame of a window above them.

#include <iostream>
#include <string>
using namespace std;

int main() {
    int n, m;
    while (cin >> n >> m && (n || m)) {
        string screen[100];
        for (int i = 0; i < n; ++i) cin >> screen[i];

        int top[26], bottom[26], left[26], right[26];
        bool present[26];
        for (int c = 0; c < 26; ++c) {
            top[c] = n; bottom[c] = -1;
            left[c] = m; right[c] = -1;
            present[c] = false;
        }
        for (int r = 0; r < n; ++r)
            for (int c = 0; c < m; ++c)
                if (screen[r][c] != '.') {
                    int id = screen[r][c] - 'A';
                    present[id] = true;
                    if (r < top[id]) top[id] = r;
                    if (r > bottom[id]) bottom[id] = r;
                    if (c < left[id]) left[id] = c;
                    if (c > right[id]) right[id] = c;
                }

        for (int id = 0; id < 26; ++id)
            if (present[id] && bottom[id] - top[id] >= 2 &&
                right[id] - left[id] >= 2) {
            bool clear = true;
            for (int r = top[id]; r <= bottom[id] && clear; ++r)
                for (int c = left[id]; c <= right[id]; ++c) {
                    bool border = r == top[id] || r == bottom[id] ||
                                  c == left[id] || c == right[id];
                    if ((border && screen[r][c] != char('A' + id)) ||
                        (!border && screen[r][c] != '.')) {
                        clear = false;
                        break;
                    }
                }
            if (clear) cout << char('A' + id);
        }
        cout << '\n';
    }
    return 0;
}
