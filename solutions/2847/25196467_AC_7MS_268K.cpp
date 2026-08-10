// POJ 2847 - The Cubic End
// Model: gpt-5.6-terra
// Approach: Lift the cube root one decimal digit at a time from least to most
// significant.  Try each new digit and verify the needed suffix with small
// base-10 digit arrays, avoiding overflow from cubing a ten-digit number.

#include <algorithm>
#include <iostream>
#include <string>

const int MAX_DIGITS = 10;

bool suffix_matches(const int root[], const int target[], int length) {
    int square[2 * MAX_DIGITS + 1];
    int cube[3 * MAX_DIGITS + 1];
    std::fill(square, square + 2 * MAX_DIGITS + 1, 0);
    std::fill(cube, cube + 3 * MAX_DIGITS + 1, 0);

    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
            square[i + j] += root[i] * root[j];
        }
    }
    for (int i = 0; i < 2 * length; ++i) {
        square[i + 1] += square[i] / 10;
        square[i] %= 10;
    }

    for (int i = 0; i < 2 * length; ++i) {
        for (int j = 0; j < length; ++j) {
            cube[i + j] += square[i] * root[j];
        }
    }
    for (int i = 0; i < 3 * length; ++i) {
        cube[i + 1] += cube[i] / 10;
        cube[i] %= 10;
    }

    for (int i = 0; i < length; ++i) {
        if (cube[i] != target[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios::sync_with_stdio(false);

    int problem_count;
    std::cin >> problem_count;
    while (problem_count-- > 0) {
        std::string suffix;
        std::cin >> suffix;
        const int length = static_cast<int>(suffix.size());

        int target[MAX_DIGITS];
        int root[MAX_DIGITS];
        std::fill(root, root + MAX_DIGITS, 0);
        for (int i = 0; i < length; ++i) {
            target[i] = suffix[length - 1 - i] - '0';
        }

        for (int position = 0; position < length; ++position) {
            for (int digit = 0; digit <= 9; ++digit) {
                root[position] = digit;
                if (suffix_matches(root, target, position + 1)) {
                    break;
                }
            }
        }

        int highest = length - 1;
        while (highest > 0 && root[highest] == 0) {
            --highest;
        }
        for (int i = highest; i >= 0; --i) {
            std::cout << root[i];
        }
        std::cout << '\n';
    }
    return 0;
}
