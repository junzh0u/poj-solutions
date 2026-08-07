// POJ 2295 - A DP Problem
// Model: claude-opus-5
//
// Parse a one-variable linear equation with no parentheses and report floor(x),
// or IDENTITY / IMPOSSIBLE.
//
// Approach: single left-to-right scan collecting A (coefficient of x) and B
// (constant) of the normalized form A*x + B = 0.  A running `side` multiplier is
// +1 before '=' and -1 after, which moves the right-hand side over.  Each term
// is: a run of '+'/'-' signs (their product is the term's sign), an optional
// integer, and an optional 'x'.  A bare 'x' means 1x; "0x" contributes 0 to A
// but must still be consumed as an x-term, not as a constant.
//   A == 0 && B == 0 -> IDENTITY;  A == 0 && B != 0 -> IMPOSSIBLE.
//
// Statement ambiguity: the statement says "No unary plus or minus operators are
// allowed", but the discuss board reports WA until unary signs are handled, and
// posts concrete data such as "-1+-x--x+-10x=2", "0x=x+x+0x+1+0+1" and
// "-0x+-0=0+x" (answers -1, -1, 0).  Multiplying a run of signs is a strict
// generalization: on input that obeys the stated grammar it agrees with the
// restricted reading, so handling it costs nothing and covers the board's data.
// All three board cases are reproduced by this program.
//
// Two other traps, both from the board: the answer is the *floor*, so -3.5 must
// print -4 rather than -3 -- computed here with non-negative operands only,
// since C++98 leaves the rounding direction of negative integer division
// implementation-defined -- and equations can be longer than the buffer sizes
// people assumed (a 256-char array is reported to RE), so the line is read into
// a std::string, which cannot overflow.
//
// Bounds: coefficients are 0..1000 over a line of a few hundred characters, so
// |A| and |B| stay far inside int.

#include <stdio.h>
#include <iostream>
#include <string>

static int floorDiv(int n, int d) {
    // floor(n / d), evaluated with non-negative operands only.
    if (d < 0) { d = -d; n = -n; }
    if (n >= 0) return n / d;
    return -(((-n) + d - 1) / d);
}

int main() {
    std::ios::sync_with_stdio(false);

    int t;
    if (!(std::cin >> t)) return 0;

    std::string s;
    for (int k = 0; k < t && (std::cin >> s); ++k) {
        const int n = (int)s.size();
        int A = 0, B = 0;   // A*x + B = 0
        int side = 1;       // +1 left of '=', -1 right of it
        int i = 0;

        while (i < n) {
            if (s[i] == '=') { side = -1; ++i; continue; }

            bool consumed = false;

            int sign = 1;
            while (i < n && (s[i] == '+' || s[i] == '-')) {
                if (s[i] == '-') sign = -sign;
                ++i;
                consumed = true;
            }

            int num = 0;
            bool hasDigits = false;
            while (i < n && s[i] >= '0' && s[i] <= '9') {
                num = num * 10 + (s[i] - '0');
                ++i;
                hasDigits = true;
                consumed = true;
            }

            if (i < n && s[i] == 'x') {
                A += side * sign * (hasDigits ? num : 1);
                ++i;
                consumed = true;
            } else if (hasDigits) {
                B += side * sign * num;
            }

            if (!consumed) ++i;   // skip anything unexpected, guaranteeing progress
        }

        if (A == 0) {
            puts(B == 0 ? "IDENTITY" : "IMPOSSIBLE");
        } else {
            printf("%d\n", floorDiv(-B, A));
        }
    }
    return 0;
}
