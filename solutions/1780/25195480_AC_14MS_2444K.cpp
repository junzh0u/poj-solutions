// POJ 1780 - Code
// Model: claude-sonnet-5
// Approach: the de Bruijn sequence B(10, n) is built with the classical
// recursive Lyndon-word (FKM/Duval) construction: extend a candidate
// prefix a[1..t] by repeating its aperiodic period p, and whenever a full
// length-n prefix has a period p dividing n, emit its first p characters.
// This concatenates all Lyndon words over {0..9} whose length divides n in
// lexicographic order, which is exactly the lexicographically SMALLEST
// de Bruijn sequence. The problem has no special judge and the statement
// itself does not say the answer must be lex-smallest, but the discuss
// board settles it (threads "题目哪里有说要字典序的?" / "输出字典序最小的" /
// "本题注意的几个地方": WA unless lex-smallest is printed) -- confirmed by
// reproducing the exact sample output for n=1 and n=2 with this algorithm
// before submitting. The linear answer appends the sequence's own first
// n-1 characters to its end (10^n + n - 1 total), since the cyclic
// sequence of length 10^n already contains every n-digit string once.
#include <cstdio>

static const int MAXN = 7;
static const int MAXLEN = 1000020;

int k = 10, n;
int a[MAXN + 1];
char buf[MAXLEN];
int pos;

void db(int t, int p) {
    if (t > n) {
        if (n % p == 0) {
            for (int i = 1; i <= p; ++i) {
                buf[pos++] = (char)('0' + a[i]);
            }
        }
    } else {
        a[t] = a[t - p];
        db(t + 1, p);
        for (int j = a[t - p] + 1; j < k; ++j) {
            a[t] = j;
            db(t + 1, t);
        }
    }
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        pos = 0;
        for (int i = 0; i <= n; ++i) a[i] = 0;
        db(1, 1);
        for (int i = 0; i < n - 1; ++i) {
            buf[pos] = buf[i];
            ++pos;
        }
        fwrite(buf, 1, (size_t)pos, stdout);
        putchar('\n');
    }
    return 0;
}
