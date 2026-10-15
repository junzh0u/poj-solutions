// POJ 3644 - Gangs
// Model: gpt-6.1-sol
// Decompose each run as E + inner + S + suffix. Later first returns
// precede earlier ones; within a block, inner rank precedes suffix rank.
// Catalan counts for 0..19 pairs fit in a signed 32-bit integer.
// N is a grid intersection, so the run contains N-1 pairs, including
// the empty run at N=1. Oversized rank strings are safely rejected.
#include <iostream>
#include <string>
#include <climits>
using namespace std;
int ways[20];
string unrank(int pairs, int rank) {
    if (pairs == 0) return "";
    for (int inside = pairs - 1; inside >= 0; --inside) {
        int tail = pairs - inside - 1;
        int block = ways[inside] * ways[tail];
        if (rank >= block) rank -= block;
        else return "E" + unrank(inside, rank / ways[tail]) + "S"
                    + unrank(tail, rank % ways[tail]);
    }
    return "";
}
int main() {
    ways[0] = 1;
    for (int n = 1; n < 20; ++n)
        for (int j = 0; j < n; ++j)
            ways[n] += ways[j] * ways[n-j-1];
    int n;
    string s;
    while (cin >> n >> s && n != 0) {
        int rank = 0;
        for (unsigned int i = 0; i < s.size(); ++i) {
            int digit = s[i] - '0';
            if (rank > (INT_MAX - digit) / 10) { rank = INT_MAX; break; }
            rank = rank * 10 + digit;
        }
        if (n < 1 || n > 20 || rank < 1 || rank > ways[n-1])
            cout << "ERROR\n";
        else cout << unrank(n-1, rank-1) << '\n';
    }
    return 0;
}
