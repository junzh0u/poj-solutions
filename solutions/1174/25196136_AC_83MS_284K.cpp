// POJ 1174 - Contact
// Model: gpt-5.6-terra
// Count every A..B-bit suffix with rolling masks, then group nonzero counts.
// Patterns are ordered by decreasing length and, within one length, reverse value.
#include <cstdio>
#include <vector>
#include <algorithm>

struct Pattern {
    int length;
    int value;
};

static bool pattern_before(const Pattern &a, const Pattern &b) {
    if (a.length != b.length) return a.length > b.length;
    return a.value > b.value;
}

int main() {
    int A, B, N;
    if (std::scanf("%d%d%d", &A, &B, &N) != 3) return 0;
    std::vector<int> count(1 << (B + 1), 0);
    int bits = 0;
    int length = 0;
    int ch;
    while ((ch = std::getchar()) != EOF && ch != '2') {
        if (ch != '0' && ch != '1') continue;
        bits = ((bits << 1) | (ch - '0')) & ((1 << B) - 1);
        ++length;
        int upper = length < B ? length : B;
        int l;
        for (l = A; l <= upper; ++l) ++count[(1 << l) | (bits & ((1 << l) - 1))];
    }

    std::vector<int> frequencies;
    int i;
    for (i = 1; i < (1 << (B + 1)); ++i)
        if (count[i] != 0) frequencies.push_back(count[i]);
    std::sort(frequencies.begin(), frequencies.end());
    frequencies.erase(std::unique(frequencies.begin(), frequencies.end()), frequencies.end());
    std::sort(frequencies.rbegin(), frequencies.rend());
    int groups = (int)frequencies.size() < N ? (int)frequencies.size() : N;
    for (i = 0; i < groups; ++i) {
        std::vector<Pattern> patterns;
        int l, v;
        for (l = A; l <= B; ++l)
            for (v = 0; v < (1 << l); ++v)
                if (count[(1 << l) | v] == frequencies[i]) {
                    Pattern p;
                    p.length = l;
                    p.value = v;
                    patterns.push_back(p);
                }
        std::sort(patterns.begin(), patterns.end(), pattern_before);
        std::printf("%d", frequencies[i]);
        int j;
        for (j = 0; j < (int)patterns.size(); ++j) {
            std::putchar(' ');
            for (l = patterns[j].length - 1; l >= 0; --l)
                std::putchar(((patterns[j].value >> l) & 1) ? '1' : '0');
        }
        std::putchar('\n');
    }
    return 0;
}
