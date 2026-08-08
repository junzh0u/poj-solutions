// POJ 1188 - Gleaming the Cubes
// Model: claude-opus-5
//
// Volume of the intersection of n axis-aligned cubes.  A cube is given as a
// corner (x,y,z) plus a positive edge length d and extends d in the POSITIVE
// x, y and z directions, i.e. it is the box [x,x+d] x [y,y+d] x [z,z+d].
//
// Ambiguity: "the positive distance which the cube extends in each of the three
// directions ... from that corner" also reads as extending d BOTH ways along
// each axis (a cube centred on the given point, edge 2d).  The sample settles
// it -- under the centred reading its first set gives 600 rather than 25.
//
// The intersection of boxes is a box: componentwise max of the low corners,
// min of the high corners, product of the three side lengths, O(n) per set.
// Each side must be clamped at 0 independently: when the cubes are disjoint
// along two axes both those sides go negative and their product is positive,
// so an unclamped program reports a nonzero volume for an empty intersection.
// The official sample never separates on this -- every set in it overlaps --
// so it is checked here against the constructed case in test_data instead.
//
// Input framing: the statement's terminating 0 is not promised to be present,
// so the read loop stops at EOF and at any non-positive count as well.
//
// Language: G++ (0), not C++ (4) -- coordinates and d are unbounded by the
// statement, so the arithmetic is long long, which POJ's C++ compiler (the one
// wanting __int64/%I64d) does not accept.  Printing goes through cout for the
// same reason: no printf conversion has to be matched to a 64-bit argument.

#include <iostream>
#include <algorithm>

int main() {
    long long n;
    while (std::cin >> n && n > 0) {
        long long lo[3], hi[3];
        bool first = true;
        for (long long i = 0; i < n; ++i) {
            long long c[3], d;
            std::cin >> c[0] >> c[1] >> c[2] >> d;
            for (int k = 0; k < 3; ++k) {
                if (first) {
                    lo[k] = c[k];
                    hi[k] = c[k] + d;
                } else {
                    lo[k] = std::max(lo[k], c[k]);
                    hi[k] = std::min(hi[k], c[k] + d);
                }
            }
            first = false;
        }
        long long vol = 1;
        for (int k = 0; k < 3; ++k) {
            long long side = hi[k] - lo[k];
            if (side < 0) side = 0;
            vol *= side;
        }
        std::cout << vol << "\n";
    }
    return 0;
}
