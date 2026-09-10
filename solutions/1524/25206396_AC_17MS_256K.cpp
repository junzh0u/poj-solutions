// POJ 1524 - Cowculations
// Model: claude-sonnet-5
// Approach: cow digits V U C D are base-4 digits 0 1 2 3 (confirmed on the
// discuss board and by hand-verifying both worked addition examples in the
// statement against base-4 addition-with-carry). Num1 and Num2 need not be
// tracked as fixed-width digit strings: R (shift right, drop rightmost
// symbol, insert V on the left) is exactly integer division by 4, L (shift
// left, keep everything, append V on the right) is exactly multiplication
// by 4, and A (base-4 addition with carry) is exactly ordinary integer
// addition of the base-4 values. Left-padding the final 3-operation result
// with V's to 8 symbols never changes its numeric value, so comparing
// against the target's base-4 value is equivalent to the statement's
// symbol-by-symbol comparison. Values never exceed 4^8-1 (three ops, each
// growing width by at most one digit from a 5-digit start), well within int.
#include <cstdio>

int val(char c) {
    if (c == 'V') return 0;
    if (c == 'U') return 1;
    if (c == 'C') return 2;
    return 3; // 'D'
}

long toNum(const char *s) {
    long v = 0;
    for (int i = 0; s[i]; i++) v = v * 4 + val(s[i]);
    return v;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    printf("COWCULATIONS OUTPUT\n");
    char buf[32];
    for (int t = 0; t < n; t++) {
        scanf("%s", buf); long num1 = toNum(buf);
        scanf("%s", buf); long num2 = toNum(buf);
        for (int i = 0; i < 3; i++) {
            scanf("%s", buf);
            char op = buf[0];
            if (op == 'A') num2 = num1 + num2;
            else if (op == 'R') num2 = num2 / 4;
            else if (op == 'L') num2 = num2 * 4;
            // N: no-op
        }
        scanf("%s", buf); long target = toNum(buf);
        printf(num2 == target ? "YES\n" : "NO\n");
    }
    printf("END OF OUTPUT\n");
    return 0;
}
