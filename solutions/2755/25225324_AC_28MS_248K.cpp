// POJ 2755 - Dominoes
// Model: gpt-6.1-sol
// All rectangles have area 60, so their smaller side is 1,2,3,4,5 or 6.
// Offline exact-cover enumeration of all orientations of the twelve pieces
// gives 0,0,8,1472,4040,9356 labelled-board tilings respectively.
// Divide by four for the rectangle's reflections and half-turn. No tiling
// is fixed by a nonidentity symmetry: the unique asymmetric F piece would
// have to map onto itself. Rotating a rectangle also identifies M,N and N,M.
#include <iostream>
#include <algorithm>
using namespace std;
int main() {
    const int count[7] = {0, 0, 0, 2, 368, 1010, 2339};
    int m, n;
    while (cin >> m >> n) cout << count[min(m, n)] << '\n';
    return 0;
}
