// POJ 3191 - The Moronic Cowmpouter
// Model: claude-sonnet-5
// Approach: Repeatedly divide n by -2 using C++ truncating division/remainder;
// whenever the remainder comes out negative, add 2 to the remainder and 1 to
// the quotient (since -2*(q+1) + (r+2) == -2*q + r), which yields the correct
// non-negative base -2 digit each step. Collect digits and print reversed;
// n == 0 is special-cased to print a single "0" as required.
// Statement note: input range is -2,000,000,000..2,000,000,000, which fits in
// a 32-bit int, but long long is used throughout to stay clear of any edge
// case around INT_MIN/INT_MAX during the division/remainder adjustment.
#include <cstdio>
#include <string>
#include <algorithm>

int main() {
    long long n;
    if (std::scanf("%lld", &n) != 1) return 0;

    if (n == 0) {
        std::printf("0\n");
        return 0;
    }

    std::string digits;
    while (n != 0) {
        long long r = n % (-2);
        n = n / (-2);
        if (r < 0) {
            r += 2;
            n += 1;
        }
        digits.push_back(static_cast<char>('0' + r));
    }
    std::reverse(digits.begin(), digits.end());
    std::printf("%s\n", digits.c_str());
    return 0;
}
