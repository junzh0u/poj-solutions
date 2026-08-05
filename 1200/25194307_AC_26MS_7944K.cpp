// POJ 1200 - Crazy Search
//
// Count the distinct length-N substrings of a text drawn from an alphabet
// of NC distinct characters, given that NC^N (the number of possible
// substrings) is guaranteed not to exceed 16 million.
//
// Approach: map each of the (at most NC) distinct bytes actually seen in
// the text to a digit 0..NC-1, in order of first appearance -- any
// bijection works since only distinctness of substrings matters, not the
// digits' specific values. Every length-N window is then a base-NC
// integer in [0, NC^N), which fits in a 32-bit long, so a single flat
// "visited" byte array of size NC^N (<=16MB) can mark which windows have
// been seen. The window's value is maintained with an O(1) rolling
// update (drop the outgoing digit's contribution, shift, add the
// incoming digit) instead of recomputing from scratch, so the whole text
// is processed in a single linear pass.
//
// The text is read in one bulk fread() (rather than per-character
// getchar()) into a growable buffer and then compacted in place to strip
// whitespace, since the statement's "huge input" hint and a slow judge
// make per-byte stdio calls too costly, and the text may be wrapped
// across multiple physical lines.
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main() {
    int N, NC;
    if (scanf("%d %d", &N, &NC) != 2) return 0;

    /* Read the remaining text robustly: it may be huge and may be wrapped
       across several physical lines by the judge's input file. Bulk-read
       everything left in the stream with fread (far fewer, far cheaper
       calls than one getchar() per byte on a "huge input" text), then
       compact out whitespace in place. */
    long cap = 1 << 20; /* 1M, grows as needed */
    char *buf = (char*)malloc(cap);
    long total_read = 0;
    for (;;) {
        if (total_read == cap) {
            cap <<= 1;
            buf = (char*)realloc(buf, cap);
        }
        size_t r = fread(buf + total_read, 1, cap - total_read, stdin);
        if (r == 0) break;
        total_read += (long)r;
    }
    long len = 0;
    for (long i = 0; i < total_read; i++) {
        char ch = buf[i];
        if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') continue;
        buf[len++] = ch;
    }

    if (N <= 0 || N > len) {
        printf("0\n");
        free(buf);
        return 0;
    }

    /* Map each distinct byte to a code 0..NC-1 in order of first appearance. */
    int code[256];
    for (int i = 0; i < 256; i++) code[i] = -1;
    int nextCode = 0;
    for (long i = 0; i < len; i++) {
        unsigned char ch = (unsigned char)buf[i];
        if (code[ch] == -1) code[ch] = nextCode++;
    }

    /* total = NC^N, guaranteed by the statement not to exceed 16 million;
       comfortably fits in a 32-bit long, so no need for a C++11 long long. */
    long total = 1;
    for (int i = 0; i < N; i++) total *= NC;

    char *visited = (char*)calloc((size_t)total, 1);

    long highpow = 1; /* NC^(N-1) */
    for (int i = 0; i < N - 1; i++) highpow *= NC;

    long val = 0;
    for (int i = 0; i < N; i++) val = val * NC + code[(unsigned char)buf[i]];

    long count = 0;
    visited[val] = 1;
    count++;

    for (long i = N; i < len; i++) {
        val = (val - (long)code[(unsigned char)buf[i - N]] * highpow) * NC
              + code[(unsigned char)buf[i]];
        if (!visited[val]) {
            visited[val] = 1;
            count++;
        }
    }

    printf("%ld\n", count);

    free(buf);
    free(visited);
    return 0;
}
