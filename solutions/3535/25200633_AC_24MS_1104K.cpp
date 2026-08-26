// POJ 3535 - A+B
// Model: gpt-5.6-terra
// Each position has an ordered digit alphabet consisting of the letters absent
// from every basis string at that position.  Lexicographic ranks are mixed-radix
// numbers, so their sum is computed by carrying from the last position left.
// The input contains one data set; n*k is at most 100000.

#include <cstdio>
#include <cstring>

int main() {
    int n, k;
    if (std::scanf("%d%d", &n, &k) != 2) return 0;

    static unsigned int forbidden[100005];
    static char s[100005];
    static char a[100005];
    static char b[100005];
    static char answer[100005];

    for (int row = 0; row < k; ++row) {
        std::scanf("%s", s);
        for (int i = 0; i < n; ++i)
            forbidden[i] |= (1u << (s[i] - 'a'));
    }
    std::scanf("%s%s", a, b);

    int carry = 0;
    for (int i = n - 1; i >= 0; --i) {
        int radix = 0;
        int da = 0;
        int db = 0;
        for (int letter = 0; letter < 26; ++letter) {
            if ((forbidden[i] & (1u << letter)) == 0) {
                if (letter < a[i] - 'a') ++da;
                if (letter < b[i] - 'a') ++db;
                ++radix;
            }
        }
        int sum = da + db + carry;
        int digit = sum % radix;
        carry = sum / radix;
        for (int letter = 0; letter < 26; ++letter) {
            if ((forbidden[i] & (1u << letter)) == 0) {
                if (digit == 0) {
                    answer[i] = static_cast<char>('a' + letter);
                    break;
                }
                --digit;
            }
        }
    }
    answer[n] = '\0';
    std::printf("%s\n", answer);
    return 0;
}
