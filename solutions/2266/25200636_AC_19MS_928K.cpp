// POJ 2266 - Quadtree
// Model: gpt-5.6-terra
// Decode each quadtree node directly into its rectangular pixel region, then
// pack every row in XBM's least-significant-bit-first pixel convention.
// Black pixels are encoded as set bits; input order is TL, TR, BL, BR.
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

int n;
string code;
int pos;
unsigned char picture[512][512];

void decode(int top, int left, int size) {
    char c = code[pos++];
    int r, col;
    if (c == 'Q') {
        int half = size / 2;
        decode(top, left, half);
        decode(top, left + half, half);
        decode(top + half, left, half);
        decode(top + half, left + half, half);
    } else if (c == 'B') {
        for (r = top; r < top + size; ++r)
            for (col = left; col < left + size; ++col)
                picture[r][col] = 1;
    }
}

int main() {
    int r, block, bit;
    if (!(cin >> n >> code)) return 0;
    pos = 0;
    decode(0, 0, n);
    cout << "#define quadtree_width " << n << '\n';
    cout << "#define quadtree_height " << n << '\n';
    cout << "static char quadtree_bits[] = {\n";
    for (r = 0; r < n; ++r) {
        for (block = 0; block < n / 8; ++block) {
            int value = 0;
            for (bit = 0; bit < 8; ++bit)
                if (picture[r][block * 8 + bit]) value |= 1 << bit;
            printf("0x%02x,", value);
        }
        printf("\n");
    }
    cout << "};\n";
    return 0;
}
