// POJ 2005 - Blackjack
// Model: gpt-5.6-terra
// Enumerate the dealer's possible hole-card ranks after removing the three
// visible cards, weighting each outcome by its remaining multiplicity.

#include <cstdio>

int value(char c) {
    if (c == 'A') return 1;
    if (c == 'T' || c == 'J' || c == 'Q' || c == 'K') return 10;
    return c - '0';
}

int hand_value(char a, char b) {
    int sum = value(a) + value(b);
    if ((a == 'A' || b == 'A') && sum + 10 <= 21) sum += 10;
    return sum;
}

int rank_of(char c) {
    if (c == 'A') return 0;
    if (c >= '2' && c <= '9') return c - '1';
    return 9;
}

int main() {
    int n;
    while (std::scanf("%d", &n) == 1 && n) {
        char dealer, first, second;
        std::scanf(" %c %c %c", &dealer, &first, &second);
        int count[10];
        for (int i = 0; i < 9; ++i) count[i] = 4 * n;
        count[9] = 16 * n;
        --count[rank_of(dealer)];
        --count[rank_of(first)];
        --count[rank_of(second)];
        int mine = hand_value(first, second);
        int wins = 0;
        const char rank[10] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T'};
        for (int i = 0; i < 10; ++i)
            if (mine > hand_value(dealer, rank[i])) wins += count[i];
        double percentage = 100.0 * wins / (52 * n - 3);
        std::printf("%.3f%%\n\n", percentage);
    }
    return 0;
}
