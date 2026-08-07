// POJ 1806 - Manhattan 2025
// Model: claude-opus-5
//
// Pure simulation / direct rendering. The set of intersections reachable with u
// units of petrol is the L1 ball {(x,y,z) : |x|+|y|+|z| <= u}; the smallest
// axis-aligned cube containing it is [-u,u]^3, i.e. (2u+1) slices of
// (2u+1)x(2u+1) cells. Slices are numbered 1..2u+1 from the bottom, so slice s
// is the plane z = s-1-u. A cell prints the digit |x|+|y|+|z| when that sum is
// at most u, and '.' otherwise; each row is filled into a small buffer and
// written with one fwrite, which is what keeps a large scenario count cheap.
//
// Notes on the statement:
//  - "0 <= u <= 9" bounds the distance by 9, so a single digit always suffices,
//    and 2u+1 <= 19 bounds the row buffer. u is clamped to that stated range so
//    the fixed-size buffer is safe no matter what the input holds; on legal
//    input the clamp never fires. The discuss board has one thread worrying
//    about u > 9 ("the customer will ignore the display anyway"), and the reply
//    says ignoring that remark is fine.
//  - Output for u = 0 is the single line "0"; the shape is symmetric in every
//    axis, so row ordering inside a slice does not matter.
//  - "Print an additional blank line after each scenario" is taken literally:
//    the blank line follows every scenario, including the last.

#include <cstdio>

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    for (int i = 1; i <= t; ++i) {
        int u;
        if (scanf("%d", &u) != 1) break;
        if (u < 0) u = 0;
        if (u > 9) u = 9;
        printf("Scenario #%d:\n", i);
        char row[24];
        int w = 2 * u + 1;
        row[w] = '\n';
        for (int z = -u; z <= u; ++z) {
            printf("slice #%d:\n", z + u + 1);
            int az = z < 0 ? -z : z;
            for (int y = -u; y <= u; ++y) {
                int ay = y < 0 ? -y : y;
                for (int x = -u; x <= u; ++x) {
                    int ax = x < 0 ? -x : x;
                    int d = ax + ay + az;
                    row[x + u] = (d <= u && d <= 9) ? (char)('0' + d) : '.';
                }
                fwrite(row, 1, w + 1, stdout);
            }
        }
        putchar('\n');
    }
    return 0;
}
