// POJ 2868 - Computer DJ
// Model: claude-sonnet-5
//
// The infinite sequence is: for L = 1, 2, 3, ..., concatenate (in
// lexicographic order) every length-L string over the N-letter alphabet
// {A, B, ..., Nth letter}. Query k asks for the k-th character (1-indexed)
// of that infinite concatenation, and the answer song is the one whose
// label matches that character.
//
// For a fixed L, the block of length-L words contributes L * N^L
// characters. Walk L = 1, 2, ... accumulating block sizes until the
// k-th character falls inside the current block. Within that block the
// words are ordered like base-N counting (word index 0 = "AAA...A"),
// so once we know which word (by 0-indexed rank) and which character
// position inside that word (0-indexed from the left, i.e. most
// significant digit first) holds our character, extracting that digit
// in base N gives the 0-indexed song label directly.
//
// N = 1 is special-cased: with a single letter the whole sequence is
// just that letter repeated forever, no matter how large k is, so the
// answer is always song 0. This matters because the board (message
// 49567 / 49575) reports "1 1 / a / 100000000" causing RE for programs
// that try to materialize or index by L directly for N = 1 -- the
// block-length loop for N = 1 would otherwise need L up to about
// 14142 to reach k = 1e8 (triangular growth), which is fine
// numerically but easy to mishandle with fixed-size arrays; avoiding
// any array sized by L or k sidesteps that entirely.
//
// For N >= 2 the block sizes grow exponentially (L stays under ~30 to
// reach k = 1e8), so plain 64-bit arithmetic never overflows: every
// intermediate word count N^L that we actually keep only appears in a
// block whose accumulated size stays below k <= 1e8.
//
// Ambiguity check: the sample's own second test case (N=3, titles
// Pathethique/TurkishMarch/Winter, k=16 -> Winter) was hand-verified
// against this scheme (word index 6 = "CA" in the length-2 block,
// character 0 = 'C' = Winter), confirming the concatenation is by
// character position, not by word index, and that within-word order is
// left-to-right = most-significant-digit-first.

#include <cstdio>

int main() {
    int n, q;
    bool first = true;
    while (scanf("%d %d", &n, &q) == 2 && (n || q)) {
        char titles[26][105];
        for (int i = 0; i < n; ++i) scanf("%s", titles[i]);

        if (!first) printf("\n");
        first = false;

        for (int query = 0; query < q; ++query) {
            // k fits comfortably in a 32-bit int (<= 1e8), so read it with
            // plain %d -- this sidesteps any %lld/%I64d portability trap
            // on POJ's older compilers. Only the internal block-size
            // arithmetic below needs 64-bit range.
            int k;
            scanf("%d", &k);

            if (n == 1) {
                printf("%s\n", titles[0]);
                continue;
            }

            long long total = 0; // characters accounted for before current L
            long long remain, wordIndex, charPos;
            int L;
            long long pw = 1; // N^L
            for (L = 1; ; ++L) {
                pw *= n; // N^L
                long long blockChars = (long long)L * pw;
                if (total + blockChars >= k) {
                    remain = k - total; // 1-indexed position within this block
                    wordIndex = (remain - 1) / L;
                    charPos = (remain - 1) % L; // 0-indexed from left
                    break;
                }
                total += blockChars;
            }

            // digit at position charPos (0 = most significant) of wordIndex
            // written in base n with L digits.
            long long place = 1;
            for (int i = 0; i < L - 1 - charPos; ++i) place *= n;
            long long digit = (wordIndex / place) % n;

            printf("%s\n", titles[digit]);
        }
    }
    return 0;
}
