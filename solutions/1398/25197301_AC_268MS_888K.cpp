// POJ 1398 - Complete the sequence!
// Model: gpt-5.6-terra
// Approach: successive forward differences.  Values of a degree-d polynomial
// have constant d-th differences, so extending the last value of every level
// from the highest difference back to the sequence yields each next term.
// Differences can be much larger than the (bounded) requested answers, so a
// small signed integer class keeps the exact additions and subtractions safe.
#include <cstdio>

class BigInt {
    enum { BASE = 1000000000, LIMBS = 20 };
    int sign, length;
    int digit[LIMBS];

    void normalize() {
        while (length > 0 && digit[length - 1] == 0) --length;
        if (length == 0) sign = 0;
    }
    int compare_abs(const BigInt& other) const {
        int i;
        if (length != other.length) return length < other.length ? -1 : 1;
        for (i = length - 1; i >= 0; --i)
            if (digit[i] != other.digit[i]) return digit[i] < other.digit[i] ? -1 : 1;
        return 0;
    }
    void add_abs(const BigInt& other) {
        long long carry = 0;
        int i, maximum = length > other.length ? length : other.length;
        for (i = 0; i < maximum || carry; ++i) {
            long long value = carry + (i < length ? digit[i] : 0) +
                              (i < other.length ? other.digit[i] : 0);
            digit[i] = (int)(value % BASE);
            carry = value / BASE;
        }
        length = i > maximum ? i : maximum;
    }
    void subtract_abs(const BigInt& other) {
        int i, borrow = 0;
        for (i = 0; i < length; ++i) {
            int value = digit[i] - (i < other.length ? other.digit[i] : 0) - borrow;
            if (value < 0) { value += BASE; borrow = 1; }
            else borrow = 0;
            digit[i] = value;
        }
        normalize();
    }

public:
    BigInt(long long value = 0) : sign(0), length(0) {
        int i;
        for (i = 0; i < LIMBS; ++i) digit[i] = 0;
        if (value == 0) return;
        sign = value < 0 ? -1 : 1;
        if (value < 0) value = -value;
        while (value) { digit[length++] = (int)(value % BASE); value /= BASE; }
    }
    BigInt& operator+=(const BigInt& other) {
        int comparison;
        if (other.sign == 0) return *this;
        if (sign == 0) { *this = other; return *this; }
        if (sign == other.sign) { add_abs(other); return *this; }
        comparison = compare_abs(other);
        if (comparison == 0) { length = 0; sign = 0; return *this; }
        if (comparison > 0) subtract_abs(other);
        else {
            BigInt result(other);
            result.subtract_abs(*this);
            *this = result;
        }
        return *this;
    }
    BigInt& operator-=(const BigInt& other) {
        BigInt negated(other);
        negated.sign = -negated.sign;
        return *this += negated;
    }
    void print() const {
        int i;
        if (sign < 0) std::printf("-");
        if (length == 0) { std::printf("0"); return; }
        std::printf("%d", digit[length - 1]);
        for (i = length - 2; i >= 0; --i) std::printf("%09d", digit[i]);
    }
};

int main() {
    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int s, c, i, level;
        BigInt last[100], row[100];
        std::scanf("%d%d", &s, &c);
        for (i = 0; i < s; ++i) {
            long long value;
            std::scanf("%lld", &value);
            row[i] = BigInt(value);
        }
        for (level = 0; level < s; ++level) {
            last[level] = row[s - level - 1];
            for (i = 0; i + 1 < s - level; ++i) {
                BigInt next(row[i + 1]);
                next -= row[i];
                row[i] = next;
            }
        }
        for (i = 0; i < c; ++i) {
            int j;
            for (j = s - 2; j >= 0; --j) last[j] += last[j + 1];
            if (i) std::printf(" ");
            last[0].print();
        }
        std::printf("\n");
    }
    return 0;
}
