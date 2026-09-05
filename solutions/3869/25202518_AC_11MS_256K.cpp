// POJ 3869 - Headshot
// Model: claude-sonnet-5
//
// The friend's landing chamber, conditioned on the observed "click", is
// uniformly distributed over the "0" positions in the (circular) string.
// If we SHOOT immediately, the cylinder advances one position from that
// (unknown, uniform-over-zeros) landing spot, so
//   P(survive | shoot) = (# zeros immediately followed by a zero) / (# zeros)
// If we ROTATE, the new position is uniform over all n chambers, so
//   P(survive | rotate) = (# zeros) / n
// Compare the two fractions via cross multiplication (all integers, n<=100,
// so plain int suffices: max product is 100*100=10000) to avoid floating
// point: c1*n vs z*z, where c1 = zeros followed by a zero, z = total zeros.
#include <cstdio>
#include <cstring>

int main() {
    char s[205];
    if (std::scanf("%s", s) != 1) return 0;
    int n = (int)std::strlen(s);
    int z = 0, c1 = 0;
    for (int i = 0; i < n; ++i) {
        if (s[i] == '0') {
            ++z;
            int j = (i + 1) % n;
            if (s[j] == '0') ++c1;
        }
    }
    int lhs = c1 * n;
    int rhs = z * z;
    if (lhs > rhs) std::printf("SHOOT\n");
    else if (lhs < rhs) std::printf("ROTATE\n");
    else std::printf("EQUAL\n");
    return 0;
}
