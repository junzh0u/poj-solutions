// POJ 3992 - Tiles of Tetris, NOT!
// Model: claude-sonnet-5
// Tiles (W x H) must all be laid in the same orientation (no rotation).
// The smallest square that can be tiled this way has side length
// L = lcm(W, H). The number of tiles needed is (L/W) * (L/H).
// Since L = W*H/gcd(W,H), this simplifies to (H/g) * (W/g) where g = gcd(W,H).
// The result can exceed 32-bit int range (up to ~10^12), so use 64-bit
// arithmetic; iostream avoids any %lld/%I64d format-specifier issue.
#include <iostream>
using namespace std;

int main() {
    long long w, h;
    while (cin >> w >> h) {
        if (w == 0 && h == 0) break;
        long long a = w, b = h;
        while (b) {
            long long t = a % b;
            a = b;
            b = t;
        }
        long long g = a;
        cout << (w / g) * (h / g) << "\n";
    }
    return 0;
}
