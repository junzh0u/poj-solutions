// POJ 2270 - Quadtree II or: Florida Jones strikes back
// Model: claude-sonnet-5
//
// Approach: read the whole remaining input as one buffer and locate
// "quadtree_width" occurrences directly rather than trying to scanf a
// rigid line format -- the sample input's illustrative /* ... */ comments
// (explicitly noted as not being real data) can appear in real judge data
// too, so we scan for "0x" hex-byte tokens rather than relying on fixed
// token positions. This also lets us loop until EOF for however many
// images the input actually holds, since the statement never states a
// count and never says there is exactly one image.
//
// For each n x n image (n a power of two, guaranteed by the domain),
// build a 2D prefix sum of "black" pixels so any axis-aligned square's
// uniformity (all white / all black / mixed) is an O(1) check, then
// recurse: uniform square emits its color letter, otherwise emit 'Q'
// and recurse into the four quadrants in the order upper-left,
// upper-right, lower-left, lower-right. That quadrant order (rather than
// e.g. a clockwise spiral) was confirmed by hand-decoding the sample
// image and its expected output, since the statement's prose never says.
//
// Bit-to-pixel mapping (per the statement's own worked example
// WBBBBWWB -> 0x9e): pixel i (1-indexed, left to right) has weight
// 2^(i-1), and a set bit means Black.

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

static vector<vector<int> > pre;

void encode(int r0, int c0, int size, string &out) {
    int sum = pre[r0 + size][c0 + size] - pre[r0][c0 + size] - pre[r0 + size][c0] + pre[r0][c0];
    int area = size * size;
    if (sum == 0) { out.push_back('W'); return; }
    if (sum == area) { out.push_back('B'); return; }
    out.push_back('Q');
    int h = size / 2;
    encode(r0, c0, h, out);
    encode(r0, c0 + h, h, out);
    encode(r0 + h, c0, h, out);
    encode(r0 + h, c0 + h, h, out);
}

int main() {
    string all;
    {
        char buf[1 << 16];
        size_t r;
        while ((r = fread(buf, 1, sizeof(buf), stdin)) > 0) all.append(buf, r);
    }

    size_t pos = 0;
    const string key = "quadtree_width";
    string result;

    while (true) {
        size_t wpos = all.find(key, pos);
        if (wpos == string::npos) break;
        size_t p = wpos + key.size();
        while (p < all.size() && !isdigit((unsigned char)all[p])) p++;
        int n = 0;
        while (p < all.size() && isdigit((unsigned char)all[p])) { n = n * 10 + (all[p] - '0'); p++; }

        int totalBytes = n * (n / 8);
        vector<int> bytes;
        bytes.reserve(totalBytes);
        size_t q = p;
        while ((int)bytes.size() < totalBytes) {
            size_t hpos = all.find("0x", q);
            size_t d1 = hpos + 2;
            int val = 0;
            for (int k = 0; k < 2; k++) {
                char c = all[d1 + k];
                int v = 0;
                if (c >= '0' && c <= '9') v = c - '0';
                else if (c >= 'a' && c <= 'f') v = c - 'a' + 10;
                else if (c >= 'A' && c <= 'F') v = c - 'A' + 10;
                val = val * 16 + v;
            }
            bytes.push_back(val);
            q = d1 + 2;
        }
        pos = q;

        vector<string> grid(n, string(n, 'W'));
        int rowBytes = n / 8;
        for (int r = 0; r < n; r++) {
            for (int bc = 0; bc < rowBytes; bc++) {
                int b = bytes[r * rowBytes + bc];
                for (int k = 0; k < 8; k++) {
                    int bitval = (b >> k) & 1;
                    int col = bc * 8 + k;
                    grid[r][col] = bitval ? 'B' : 'W';
                }
            }
        }

        pre.assign(n + 1, vector<int>(n + 1, 0));
        for (int r = 0; r < n; r++)
            for (int c = 0; c < n; c++)
                pre[r + 1][c + 1] = pre[r][c + 1] + pre[r + 1][c] - pre[r][c] + (grid[r][c] == 'B' ? 1 : 0);

        string enc;
        encode(0, 0, n, enc);
        printf("%d\n%s\n", n, enc.c_str());
    }

    return 0;
}
