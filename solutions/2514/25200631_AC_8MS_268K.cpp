// POJ 2514 - Ridiculous Addition
// Model: gpt-5.6-terra
// Find a digit in each concatenation by locating its decimal-length block.
// Addition carries from the infinite suffix, so a carry reaches position k
// exactly when the first raw digit sum to its right that is not 9 exceeds 9.

#include <iostream>

using namespace std;

#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif

i64 p10[19];

i64 firstSquareAtLeast(i64 value) {
    i64 low = 0;
    i64 high = 1000000000LL;
    while (low < high) {
        i64 mid = low + (high - low) / 2;
        if (mid * mid >= value) high = mid;
        else low = mid + 1;
    }
    return low;
}

int digitOfIntegers(i64 pos) {
    int digits;
    for (digits = 1; ; ++digits) {
        i64 first = p10[digits - 1];
        i64 count = 9 * first;
        i64 block = count * digits;
        if (pos <= block) break;
        pos -= block;
    }
    i64 number = p10[digits - 1] + (pos - 1) / digits;
    int offset = (int)((pos - 1) % digits);
    return (int)((number / p10[digits - 1 - offset]) % 10);
}

int digitOfSquares(i64 pos) {
    int digits;
    for (digits = 1; ; ++digits) {
        i64 first = firstSquareAtLeast(p10[digits - 1]);
        i64 after = firstSquareAtLeast(p10[digits]);
        i64 block = (after - first) * digits;
        if (pos <= block) {
            i64 number = first + (pos - 1) / digits;
            int offset = (int)((pos - 1) % digits);
            i64 square = number * number;
            return (int)((square / p10[digits - 1 - offset]) % 10);
        }
        pos -= block;
    }
}

int rawSum(i64 pos) {
    return digitOfIntegers(pos) + digitOfSquares(pos);
}

int main() {
    p10[0] = 1;
    int i;
    for (i = 1; i <= 18; ++i) p10[i] = p10[i - 1] * 10;

    i64 k;
    while (cin >> k && k != 0) {
        int sum = rawSum(k);
        i64 next = k + 1;
        int right;
        do {
            right = rawSum(next);
            ++next;
        } while (right == 9);
        if (right >= 10) ++sum;
        cout << (sum % 10) << '\n';
    }
    return 0;
}
