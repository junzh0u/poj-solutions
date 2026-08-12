// POJ 1610 - Quad Trees
// Model: gpt-5.6-terra
// Breadth-first traversal emits each quad-tree node: 1 for a split, or 0 and
// its colour for a uniform square. Bits are packed into uppercase hex digits.
// Quadrants are enqueued in row-major order (upper-left to lower-right).
#include <cstdio>

const int MAXN = 512;
const int MAXQ = 350000;
const int MAXOUT = 700000;

struct Square {
    int row, col, size;
};

char image[MAXN][MAXN];
Square queue_[MAXQ];
char bits[MAXOUT];
int bitLength;

void addBit(int bit) {
    bits[bitLength++] = (char)bit;
}

int uniform(const Square& square) {
    int value = image[square.row][square.col];
    int r, c;
    for (r = square.row; r < square.row + square.size; ++r)
        for (c = square.col; c < square.col + square.size; ++c)
            if (image[r][c] != value) return -1;
    return value;
}

int main() {
    int cases, n, tc, r, c;
    if (std::scanf("%d", &cases) != 1) return 0;
    for (tc = 0; tc < cases; ++tc) {
        std::scanf("%d", &n);
        for (r = 0; r < n; ++r)
            for (c = 0; c < n; ++c) {
                int value;
                std::scanf("%d", &value);
                image[r][c] = (char)value;
            }

        int head = 0, tail = 0;
        queue_[tail].row = 0;
        queue_[tail].col = 0;
        queue_[tail++].size = n;
        bitLength = 0;

        while (head < tail) {
            Square square = queue_[head++];
            int value = uniform(square);
            if (value != -1) {
                addBit(0);
                addBit(value);
            } else {
                int half = square.size / 2;
                addBit(1);
                queue_[tail].row = square.row;
                queue_[tail].col = square.col;
                queue_[tail++].size = half;
                queue_[tail].row = square.row;
                queue_[tail].col = square.col + half;
                queue_[tail++].size = half;
                queue_[tail].row = square.row + half;
                queue_[tail].col = square.col;
                queue_[tail++].size = half;
                queue_[tail].row = square.row + half;
                queue_[tail].col = square.col + half;
                queue_[tail++].size = half;
            }
        }
        int first = bitLength % 4;
        int position = 0;
        if (first == 0) first = 4;
        while (position < bitLength) {
            int width = position == 0 ? first : 4;
            int value = 0;
            int i;
            for (i = 0; i < width; ++i) value = value * 2 + bits[position + i];
            std::printf("%c", value < 10 ? '0' + value : 'A' + value - 10);
            position += width;
        }
        std::printf("\n");
    }
    return 0;
}
