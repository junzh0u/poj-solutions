// POJ 1103 - Maze
// Model: gpt-5.6-terra
// Expand every cell to a 3 by 3 raster, then flood-fill open regions.
#include <iostream>
#include <queue>
#include <string>

using namespace std;

int main() {
    int w, h, number = 0;
    while (cin >> w >> h && (w || h)) {
        string maze[75];
        int a[225][225];
        int y, x, i, count = 0, longest = 0;
        for (y = 0; y < h; ++y) cin >> maze[y];
        for (y = 0; y < 3 * h; ++y)
            for (x = 0; x < 3 * w; ++x) a[y][x] = 0;
        for (y = 0; y < h; ++y) for (x = 0; x < w; ++x) {
            int r = 3 * y, c = 3 * x;
            if (maze[y][x] == '/') a[r][c + 2] = a[r + 1][c + 1] = a[r + 2][c] = 1;
            else a[r][c] = a[r + 1][c + 1] = a[r + 2][c + 2] = 1;
        }
        for (y = 0; y < 3 * h; ++y) for (x = 0; x < 3 * w; ++x) if (!a[y][x]) {
            queue<pair<int, int> > q;
            bool border = false;
            int pixels = 0;
            a[y][x] = 1; q.push(make_pair(y, x));
            while (!q.empty()) {
                int r = q.front().first, c = q.front().second;
                static const int dr[4] = {-1, 1, 0, 0};
                static const int dc[4] = {0, 0, -1, 1};
                q.pop(); ++pixels;
                if (r == 0 || c == 0 || r == 3 * h - 1 || c == 3 * w - 1) border = true;
                for (i = 0; i < 4; ++i) {
                    int nr = r + dr[i], nc = c + dc[i];
                    if (nr >= 0 && nr < 3 * h && nc >= 0 && nc < 3 * w && !a[nr][nc]) {
                        a[nr][nc] = 1; q.push(make_pair(nr, nc));
                    }
                }
            }
            if (!border) { ++count; if (pixels / 3 > longest) longest = pixels / 3; }
        }
        cout << "Maze #" << ++number << ":\n";
        if (count) cout << count << " Cycles; the longest has length " << longest << ".\n\n";
        else cout << "There are no cycles.\n\n";
    }
    return 0;
}
