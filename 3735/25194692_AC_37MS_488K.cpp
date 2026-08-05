// POJ 3735 - Training little cats
// Model: gpt-5.6-terra
// Each complete move sequence is an affine transformation of the cat counts.
// Add a permanent constant coordinate, build the transformation by row updates,
// then use binary exponentiation for its m repetitions. The input permits m = 0
// (but still supplies all k commands), and swaps may name the same cat.
#include <cstdio>
#include <cstring>

typedef long long int64;

struct Matrix {
    int size;
    int64 value[101][101];

    Matrix(int dimension = 0, bool identity = false) : size(dimension) {
        std::memset(value, 0, sizeof(value));
        if (identity) {
            for (int i = 0; i < size; ++i) value[i][i] = 1;
        }
    }
};

Matrix multiply(const Matrix &left, const Matrix &right) {
    Matrix result(left.size);
    for (int i = 0; i < left.size; ++i) {
        for (int k = 0; k < left.size; ++k) {
            if (left.value[i][k] == 0) continue;
            for (int j = 0; j < left.size; ++j) {
                if (right.value[k][j] != 0) {
                    result.value[i][j] += left.value[i][k] * right.value[k][j];
                }
            }
        }
    }
    return result;
}

Matrix power(Matrix base, int64 exponent) {
    Matrix result(base.size, true);
    while (exponent > 0) {
        if (exponent & 1) result = multiply(result, base);
        base = multiply(base, base);
        exponent >>= 1;
    }
    return result;
}

int main() {
    int n, k;
    int64 m;
    while (std::scanf("%d%lld%d", &n, &m, &k) == 3 && (n || m || k)) {
        Matrix transform(n + 1, true);
        for (int move = 0; move < k; ++move) {
            char command;
            int first, second;
            std::scanf(" %c%d", &command, &first);
            --first;
            if (command == 'g') {
                for (int col = 0; col <= n; ++col) {
                    transform.value[first][col] += transform.value[n][col];
                }
            } else if (command == 'e') {
                std::memset(transform.value[first], 0, sizeof(transform.value[first]));
            } else {
                std::scanf("%d", &second);
                --second;
                for (int col = 0; col <= n; ++col) {
                    int64 temporary = transform.value[first][col];
                    transform.value[first][col] = transform.value[second][col];
                    transform.value[second][col] = temporary;
                }
            }
        }

        Matrix result = power(transform, m);
        for (int cat = 0; cat < n; ++cat) {
            if (cat) std::printf(" ");
            std::printf("%lld", result.value[cat][n]);
        }
        std::printf("\n");
    }
    return 0;
}
