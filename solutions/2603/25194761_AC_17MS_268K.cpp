// POJ 2603 - Brave balloonists
// Model: gpt-5.6-terra
// Factor each of the ten inputs.  If the product has prime exponents e_i,
// its divisor count is the product of (e_i + 1); only its last digit is needed.

#include <iostream>

int main() {
    int exponent[10001] = {0};
    int value;
    int count;
    int divisor_count = 1;

    for (int i = 0; i < 10; ++i) {
        if (!(std::cin >> value)) return 0;
        for (int factor = 2; factor * factor <= value; ++factor) {
            count = 0;
            while (value % factor == 0) {
                value /= factor;
                ++count;
            }
            exponent[factor] += count;
        }
        if (value > 1) ++exponent[value];
    }

    for (int prime = 2; prime <= 10000; ++prime) {
        if (exponent[prime] != 0) {
            divisor_count = (divisor_count * (exponent[prime] + 1)) % 10;
        }
    }
    std::cout << divisor_count << '\n';
    return 0;
}
