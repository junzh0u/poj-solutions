// POJ 2200 - A Card Trick
// Model: gpt-5.6-terra
// Enumerate the five-card output order in deck order and retain the first
// order whose four shown cards decode to its hidden card. This directly
// implements the magician's rule and therefore also resolves the required
// lexicographic tie-break without relying on a particular assistant scheme.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Card {
    int rank;
    int suit;
    string text;
};

static bool lessCard(const Card &a, const Card &b) {
    if (a.rank != b.rank) return a.rank < b.rank;
    return a.suit < b.suit;
}

static int rankOf(const string &s) {
    if (s[0] == 'A') return 1;
    if (s[0] == 'J') return 11;
    if (s[0] == 'Q') return 12;
    if (s[0] == 'K') return 13;
    if (s[0] == '1') return 10;
    return s[0] - '0';
}

static int suitOf(char c) {
    if (c == 'C') return 0;
    if (c == 'D') return 1;
    if (c == 'H') return 2;
    return 3;
}

static bool works(const vector<Card> &p) {
    int smallest = 2;
    int i;
    for (i = 3; i < 5; ++i)
        if (lessCard(p[i], p[smallest])) smallest = i;
    int distance = smallest - 1;
    int other[2], k = 0;
    for (i = 2; i < 5; ++i)
        if (i != smallest) other[k++] = i;
    if (lessCard(p[other[1]], p[other[0]])) distance += 3;
    int decodedRank = ((p[1].rank - 1 + distance) % 13) + 1;
    return p[0].rank == decodedRank && p[0].suit == p[1].suit;
}

int main() {
    int n, tc, i;
    cin >> n;
    for (tc = 1; tc <= n; ++tc) {
        vector<Card> cards(5);
        for (i = 0; i < 5; ++i) {
            cin >> cards[i].text;
            cards[i].rank = rankOf(cards[i].text);
            cards[i].suit = suitOf(cards[i].text[cards[i].text.size() - 1]);
        }
        sort(cards.begin(), cards.end(), lessCard);
        do {
            if (works(cards)) break;
        } while (next_permutation(cards.begin(), cards.end(), lessCard));
        cout << "Problem " << tc << ":";
        for (i = 0; i < 5; ++i) cout << " " << cards[i].text;
        cout << "\n";
    }
    return 0;
}
