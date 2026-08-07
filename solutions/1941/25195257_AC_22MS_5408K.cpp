// POJ 1941 - The Sierpinski Fractal
// Model: claude-opus-5
//
// Geometry: a depth-n triangle is 2^n rows tall and 2^(n+1) columns wide -- "a
// side's total length of 2^n characters" is the row count.  Depth 1 is the unit
// piece " /\" / "/__\"; depth n is three depth-(n-1) triangles, apex at
// (top, left+2^(n-1)) and the two halves at (top+2^(n-1), left) and
// (top+2^(n-1), left+2^n); the missing fourth quarter is the removed middle.
// Painted on one flat char canvas, rows emitted right-trimmed ("must not
// contain any trailing blanks") plus a blank line per case (skipping it is a
// Presentation Error the discuss board is full of).
//
// n <= 10, so the canvas is at most 1024 x 2048 and every legal case's text is
// cached (~2.8 MB), making repeated cases free.  A board post claims n reaches
// 11; contested and out of spec, but larger n is still drawn -- streamed, not
// cached, so the canvas stays the only big allocation (n=11 peaks near 11 MB).
//
// Verified byte-for-byte for n=1..12 against an independent reference derived
// from the statement's words (recursively delete the middle quarter of the
// unit-triangle index grid), which reproduces the official sample, and
// cross-checked against Pascal's triangle mod 2 (cell (j,k) survives iff C(j,k)
// is odd).

#include <cstdio>
#include <string>
#include <vector>
using namespace std;

static vector<char> g;   /* flat h x W canvas, one allocation */
static int W;

/* A depth-n triangle occupies 2^n rows and 2^(n+1) columns; (top,left) is the
   top-left corner of its bounding box.  Depth 1 is the two-line unit piece.  */
static void draw(int n, int top, int left) {
    if (n <= 1) {
        char *a = &g[(size_t)top * W + left];
        char *b = a + W;
        a[1] = '/'; a[2] = '\\';
        b[0] = '/'; b[1] = '_'; b[2] = '_'; b[3] = '\\';
        return;
    }
    int half = 1 << (n - 1);                    /* rows of one sub-triangle */
    draw(n - 1, top, left + half);              /* apex        */
    draw(n - 1, top + half, left);              /* bottom left */
    draw(n - 1, top + half, left + 2 * half);   /* bottom right */
}

static void build(int n) {
    int h = 1 << n;
    W = 2 * h;
    g.assign((size_t)h * W, ' ');
    draw(n, 0, 0);
}

static int rowEnd(int i) {                      /* length after right-trimming */
    const char *row = &g[(size_t)i * W];
    int e = W;
    while (e > 0 && row[e - 1] == ' ') --e;
    return e;
}

int main() {
    /* The statement bounds n by 10, so the rendered text of every legal case
       is cached (~2.8 MB total) and repeated cases cost nothing.  A larger n
       is still drawn correctly, just streamed instead of cached, so the canvas
       stays the only big allocation.                                        */
    const int MAXCACHE = 10;
    vector<string> cache(MAXCACHE + 1);
    vector<char> done(MAXCACHE + 1, 0);

    int n;
    while (scanf("%d", &n) == 1 && n > 0) {
        int h = 1 << n;
        if (n <= MAXCACHE) {
            if (!done[n]) {
                build(n);
                string s;
                for (int i = 0; i < h; ++i) {
                    s.append(&g[(size_t)i * W], rowEnd(i));
                    s += '\n';
                }
                s += '\n';                      /* blank line after each case */
                vector<char>().swap(g);
                cache[n].swap(s);
                done[n] = 1;
            }
            fwrite(cache[n].data(), 1, cache[n].size(), stdout);
        } else {
            build(n);
            for (int i = 0; i < h; ++i) {
                fwrite(&g[(size_t)i * W], 1, rowEnd(i), stdout);
                putchar('\n');
            }
            putchar('\n');
            vector<char>().swap(g);
        }
    }
    return 0;
}
