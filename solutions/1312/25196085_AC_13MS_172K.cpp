// POJ 1312 - Numerically Speaking
// Model: claude-opus-5
//
// Words are ranked by length first, then alphabetically, starting at 1.  That
// ranking is exactly bijective base 26 with digits a=1..z=26: the count of
// words shorter than L plus the alphabetical index inside length L telescopes
// into sum(c_i * 26^(L-i)).  So word -> number is a Horner loop, and
// number -> word is repeated division by 26 where a remainder of 0 means the
// digit 'z' and a borrow of 1 from the quotient.  Dropping that borrow is the
// classic bug (it prints aaaaaaaaaaaaaaaaaaaaz for the sample's last case).
//
// 20 z's is ~2.07e28, so the value needs a bignum; it is held as decimal
// digits, which keeps everything to int arithmetic (no long long, no %lld) and
// makes the comma grouping a direct read of the digit array.
//
// Output layout: word in column 1, number in column 23, i.e. "%-22s%s".
//
// Numbers given for a number->word line are re-derived from the decoded word
// before printing, so any leading zeros in the input are normalized away.

#include <cstdio>
#include <cstring>

/* Big natural number as decimal digits, little-endian, digits[0] = units. */
static int A[600], alen;  /* working number */

static void setZero() { alen = 0; }

static void fromDecimal(const char *s) {
    int n = (int)strlen(s), i;
    alen = 0;
    for (i = n - 1; i >= 0; --i) A[alen++] = s[i] - '0';
    while (alen > 0 && A[alen - 1] == 0) --alen;
}

/* A = A * m + add   (m, add small) */
static void mulAdd(int m, int add) {
    int carry = add, i, cur;
    for (i = 0; i < alen; ++i) {
        cur = A[i] * m + carry;
        A[i] = cur % 10;
        carry = cur / 10;
    }
    while (carry > 0) {
        A[alen++] = carry % 10;
        carry /= 10;
    }
}

/* A = A / d, returns remainder */
static int divMod(int d) {
    int rem = 0, i, cur;
    for (i = alen - 1; i >= 0; --i) {
        cur = rem * 10 + A[i];
        A[i] = cur / d;
        rem = cur % d;
    }
    while (alen > 0 && A[alen - 1] == 0) --alen;
    return rem;
}

/* A = A - 1 (A > 0) */
static void decrement() {
    int i = 0;
    while (A[i] == 0) { A[i] = 9; ++i; }
    --A[i];
    while (alen > 0 && A[alen - 1] == 0) --alen;
}

static int isZero() { return alen == 0; }

/* Render A with thousands separators into out. */
static void formatNumber(char *out) {
    int p = 0, i;
    if (alen == 0) { out[p++] = '0'; out[p] = 0; return; }
    for (i = alen - 1; i >= 0; --i) {
        out[p++] = (char)('0' + A[i]);
        if (i > 0 && i % 3 == 0) out[p++] = ',';
    }
    out[p] = 0;
}

int main() {
    char line[512], word[512], num[1024];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        int n = (int)strlen(line);
        while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r' ||
                         line[n - 1] == ' ' || line[n - 1] == '\t')) line[--n] = 0;
        if (n == 0) continue;
        if (line[0] == '*') break;

        if (line[0] >= '0' && line[0] <= '9') {
            /* number -> word, bijective base 26 */
            int wlen = 0, i;
            fromDecimal(line);
            while (!isZero()) {
                int r = divMod(26);
                if (r == 0) { word[wlen++] = 'z'; decrement(); }
                else word[wlen++] = (char)('a' + r - 1);
            }
            for (i = 0; i < wlen / 2; ++i) {
                char t = word[i];
                word[i] = word[wlen - 1 - i];
                word[wlen - 1 - i] = t;
            }
            word[wlen] = 0;
            /* rebuild the number so the printed form is canonical */
            setZero();
            for (i = 0; i < wlen; ++i) mulAdd(26, word[i] - 'a' + 1);
        } else {
            int i;
            strcpy(word, line);
            setZero();
            for (i = 0; word[i]; ++i) mulAdd(26, word[i] - 'a' + 1);
        }
        formatNumber(num);
        printf("%-22s%s\n", word, num);
    }
    return 0;
}
