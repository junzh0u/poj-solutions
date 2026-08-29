// POJ 2859 - A rectangle
// Model: claude-sonnet-5
// Approach: fix each given point as the bottom-left corner of the candidate
// axis-aligned A x B rectangle (A=width, B=height, both positive), and check
// whether the other three corners (x+A,y), (x,y+B), (x+A,y+B) are also among
// the given points. Since A,B>0 the bottom-left corner of any valid
// rectangle is unique, so summing over all points counts every rectangle
// exactly once. Membership is tested with binary search on the points
// sorted by (x,y) -- the discuss board (message 157133, 162700, 159911)
// documents that hash tables here are prone to WA/TLE from weak hash
// functions while plain sorted binary search passes comfortably within the
// generous 7000ms/1000ms limits. All coordinates and A,B are bounded by
// 1e9 in absolute value, so x+A and y+B stay within 2e9, comfortably inside
// 32-bit int range -- no need for long long or language-specific 64-bit
// support. Points are guaranteed distinct (confirmed on the board, message
// 49375, quoting the statement).
#include <cstdio>
#include <algorithm>
using namespace std;

struct Pt {
    int x, y;
};

bool operator<(const Pt &a, const Pt &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

Pt pts[500005];

bool exists(int n, int x, int y) {
    Pt key;
    key.x = x;
    key.y = y;
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (pts[mid].x == x && pts[mid].y == y) return true;
        if (pts[mid] < key) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

int main() {
    int n, A, B;
    if (scanf("%d", &n) != 1) return 0;
    scanf("%d %d", &A, &B);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &pts[i].x, &pts[i].y);
    }
    sort(pts, pts + n);

    int count = 0;
    for (int i = 0; i < n; i++) {
        int x = pts[i].x, y = pts[i].y;
        int xi2 = x + A, yi2 = y + B; /* both <= 2e9, fits in 32-bit int */
        if (exists(n, xi2, y) && exists(n, x, yi2) && exists(n, xi2, yi2)) {
            count++;
        }
    }
    printf("%d\n", count);
    return 0;
}
