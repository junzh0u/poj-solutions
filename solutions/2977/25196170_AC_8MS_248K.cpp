// POJ 2977 - Box walking
// Model: gpt-5.6-terra
// Enumerate the four valid unfoldings for each far face; points on a face
// incident to the origin use their direct planar distance.
#include <iostream>
using namespace std;

static int sq(int x) { return x * x; }
static int mn(int a, int b) { return a < b ? a : b; }

int main() {
    int a, b, c, x, y, z;
    while (cin >> a >> b >> c >> x >> y >> z) {
        if (a == 0 && b == 0 && c == 0 && x == 0 && y == 0 && z == 0) break;
        int ans = 1000000000;
        if (x == 0 || y == 0 || z == 0) ans = sq(x) + sq(y) + sq(z);
        if (x == a) {
            ans = mn(ans, sq(x + y) + sq(z));
            ans = mn(ans, sq(x + z) + sq(y));
            ans = mn(ans, sq(c + y) + sq(a + c - z));
            ans = mn(ans, sq(b + z) + sq(a + b - y));
        }
        if (y == b) {
            ans = mn(ans, sq(y + x) + sq(z));
            ans = mn(ans, sq(y + z) + sq(x));
            ans = mn(ans, sq(a + z) + sq(b + a - x));
            ans = mn(ans, sq(c + x) + sq(b + c - z));
        }
        if (z == c) {
            ans = mn(ans, sq(z + x) + sq(y));
            ans = mn(ans, sq(z + y) + sq(x));
            ans = mn(ans, sq(a + y) + sq(c + a - x));
            ans = mn(ans, sq(b + x) + sq(c + b - y));
        }
        cout << ans << '\n';
    }
    return 0;
}
