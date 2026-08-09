// POJ 3327 - Cut the Cake
// Model: gpt-5.6-terra
// Simulate the birth-ordered pieces.  A perimeter offset identifies the side
// hit by a cut; the resulting children are appended in increasing-area order.

#include <algorithm>
#include <cstdio>
#include <vector>

struct Piece {
    int w;
    int d;
};

int main() {
    int n, w, d;
    while (std::scanf("%d%d%d", &n, &w, &d) == 3 && (n || w || d)) {
        std::vector<Piece> pieces;
        Piece first;
        first.w = w;
        first.d = d;
        pieces.push_back(first);
        for (int i = 0; i < n; ++i) {
            int p, s;
            std::scanf("%d%d", &p, &s);
            Piece old = pieces[p - 1];
            pieces.erase(pieces.begin() + p - 1);
            int perimeter = 2 * (old.w + old.d);
            int offset = s % perimeter;
            Piece a, b;
            if (offset < old.w || (offset > old.w + old.d && offset < 2 * old.w + old.d)) {
                int x = offset < old.w ? offset : 2 * old.w + old.d - offset;
                a.w = x; a.d = old.d;
                b.w = old.w - x; b.d = old.d;
            } else {
                int y = offset < old.w + old.d ? offset - old.w : perimeter - offset;
                a.w = old.w; a.d = y;
                b.w = old.w; b.d = old.d - y;
            }
            if (a.w * a.d > b.w * b.d)
                std::swap(a, b);
            pieces.push_back(a);
            pieces.push_back(b);
        }
        std::vector<int> areas;
        for (std::vector<Piece>::const_iterator it = pieces.begin(); it != pieces.end(); ++it)
            areas.push_back(it->w * it->d);
        std::sort(areas.begin(), areas.end());
        for (std::size_t i = 0; i < areas.size(); ++i)
            std::printf("%s%d", i ? " " : "", areas[i]);
        std::printf("\n");
    }
    return 0;
}
