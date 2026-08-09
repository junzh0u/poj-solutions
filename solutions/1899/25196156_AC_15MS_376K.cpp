// POJ 1899 - Farmer Bill's Problem
// Model: gpt-5.6-terra
// Repeatedly merge the bounding boxes of circles whose required rectangles
// touch or overlap; every such merge is forced by the disjointness rule.
#include <iostream>
#include <vector>

using namespace std;

struct Rect {
    int left, bottom, right, top;
};

bool touches(const Rect &a, const Rect &b) {
    return a.left <= b.right && b.left <= a.right &&
           a.bottom <= b.top && b.bottom <= a.top;
}

Rect mergeRect(const Rect &a, const Rect &b) {
    Rect result;
    result.left = a.left < b.left ? a.left : b.left;
    result.bottom = a.bottom < b.bottom ? a.bottom : b.bottom;
    result.right = a.right > b.right ? a.right : b.right;
    result.top = a.top > b.top ? a.top : b.top;
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    int width, height, n;
    if (!(cin >> width >> height >> n)) return 0;
    vector<Rect> rectangles;
    for (int i = 0; i < n; ++i) {
        int x, y, radius;
        cin >> x >> y >> radius;
        Rect current;
        current.left = x - radius;
        current.right = x + radius;
        current.bottom = y - radius;
        current.top = y + radius;
        bool joined = true;
        while (joined) {
            joined = false;
            for (size_t j = 0; j < rectangles.size(); ++j) {
                if (touches(current, rectangles[j])) {
                    current = mergeRect(current, rectangles[j]);
                    rectangles.erase(rectangles.begin() + j);
                    joined = true;
                    break;
                }
            }
        }
        rectangles.push_back(current);
    }
    int protectedArea = 0;
    for (size_t i = 0; i < rectangles.size(); ++i)
        protectedArea += (rectangles[i].right - rectangles[i].left) *
                         (rectangles[i].top - rectangles[i].bottom);
    cout << width * height - protectedArea << "\n";
    return 0;
}
