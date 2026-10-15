// POJ 1869 - Test
// Model: gpt-6.1-sol
// Compute preference reachability on the 26 activities. Mutual reachability
// partitions the appearing activities into maximal contradictory sets (SCCs).
// Scan letters in order to order both components and their members.
#include <cstdio>
#include <cstring>
int main() {
    int n;
    bool first = true;
    while (std::scanf("%d", &n) == 1 && n) {
        bool reach[26][26], used[26], printed[26];
        std::memset(reach, 0, sizeof(reach));
        std::memset(used, 0, sizeof(used));
        std::memset(printed, 0, sizeof(printed));
        for (int i = 0; i < 26; ++i) reach[i][i] = true;
        for (int q = 0; q < n; ++q) {
            char a[5], answer;
            for (int j = 0; j < 5; ++j) {
                std::scanf(" %c", &a[j]);
                used[a[j]-'A'] = true;
            }
            std::scanf(" %c", &answer);
            for (int j = 0; j < 5; ++j) reach[answer-'A'][a[j]-'A'] = true;
        }
        for (int k = 0; k < 26; ++k)
            for (int i = 0; i < 26; ++i)
                for (int j = 0; j < 26; ++j)
                    reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
        if (!first) std::putchar('\n');
        first = false;
        for (int i = 0; i < 26; ++i) if (used[i] && !printed[i]) {
            bool space = false;
            for (int j = i; j < 26; ++j) if (used[j] && reach[i][j] && reach[j][i]) {
                if (space) std::putchar(' ');
                std::putchar('A'+j);
                printed[j] = true;
                space = true;
            }
            std::putchar('\n');
        }
    }
    return 0;
}
