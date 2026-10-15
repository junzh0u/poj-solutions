// POJ 3773 - String-Matching Automata
// Model: gpt-6.1-sol
// Build the prefix function, then inherit each mismatch transition from
// the longest proper border. The accepting state also follows its border,
// so overlapping occurrences remain possible. O(26*m) time and memory.
#include <cstdio>
#include <cstring>

char pattern[10005];
int prefix[10005], go[10005][26];

int main() {
    while (std::scanf("%10004s", pattern) == 1 && std::strcmp(pattern, "0")) {
        int m = std::strlen(pattern);
        prefix[0] = 0;
        for (int i = 1; i < m; ++i) {
            int j = prefix[i - 1];
            while (j && pattern[i] != pattern[j]) j = prefix[j - 1];
            if (pattern[i] == pattern[j]) ++j;
            prefix[i] = j;
        }
        for (int i = 0; i <= m; ++i) {
            for (int c = 0; c < 26; ++c) {
                if (i < m && pattern[i] == 'a' + c) go[i][c] = i + 1;
                else go[i][c] = i ? go[prefix[i - 1]][c] : 0;
            }
            std::printf("%d", i);
            for (int c = 0; c < 26; ++c) std::printf(" %d", go[i][c]);
            std::putchar('\n');
        }
    }
    return 0;
}
