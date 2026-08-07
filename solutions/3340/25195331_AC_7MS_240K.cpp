// POJ 3340 - Barbara Bennett's Wild Numbers
// Model: claude-opus-5
//
// Count the length-n digit strings matching the wild number W (a '?' matches
// any digit) that are numerically greater than X, where |W| = |X| = n <= 10.
//
// Approach: single left-to-right sweep, the standard "count strictly greater"
// digit decomposition. pw[i] is the number of free completions of positions
// i..n-1 (10 per '?', 1 per fixed digit). Walking the prefix that ties X:
//   - W[i] == '?': every digit d with d > X[i] makes the whole suffix free, so
//     add ('9' - X[i]) * pw[i+1]; then keep d == X[i] and continue the tie.
//   - W[i] > X[i]: the tie is broken upward here, add pw[i+1] and stop.
//   - W[i] < X[i]: nothing can be greater along this prefix, stop.
//   - W[i] == X[i]: the tie continues.
// Falling off the end means the candidate equals X, which is not greater, so
// nothing is added there. O(n) per test case.
//
// Two things the statement leaves open, and how they are resolved:
//   * The answer does not fit in 32 bits: W = "??????????", X = "0000000000"
//     gives 9999999999. Accumulated in long long and printed with std::cout,
//     since POJ's C++ compiler predates "%lld".
//   * "n-digit numbers" could be read as forbidding a leading zero. It only
//     ever matters when X itself has a leading zero (otherwise any candidate
//     greater than X already starts with a nonzero digit), and this code, like
//     the accepted implementations on the discuss board, allows it: a match is
//     just a string of the same length agreeing with W outside the '?'s.
//
// Input is a stream of W/X pairs terminated by a line holding a single '#'.
#include <iostream>
#include <string>

int main() {
    std::string w, x;
    while (std::cin >> w) {
        if (w == "#") break;
        if (!(std::cin >> x)) break;
        int n = (int)w.size();
        long long pw[16];
        pw[n] = 1;
        for (int i = n - 1; i >= 0; --i) pw[i] = pw[i + 1] * (w[i] == '?' ? 10 : 1);
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            char xc = (i < (int)x.size()) ? x[i] : '0';
            if (w[i] == '?') {
                ans += (long long)('9' - xc) * pw[i + 1];
            } else if (w[i] > xc) {
                ans += pw[i + 1];
                break;
            } else if (w[i] < xc) {
                break;
            }
        }
        std::cout << ans << "\n";
    }
    return 0;
}
