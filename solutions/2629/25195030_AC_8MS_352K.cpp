// POJ 2629 - Common permutation
// Model: claude-opus-5
//
// "a permutation of x is a subsequence of a" says nothing about order: it only
// says the multiset of letters of x fits inside the multiset of letters of a.
// So the longest x is the multiset intersection, count[c] = min(a[c], b[c]),
// and "first in alphabetical order" among its permutations is the sorted one.
//
// The trap (confirmed on the discuss board): the data contains blank lines --
// an empty string is a legal input line -- so token reads (scanf("%s") / cin>>)
// desynchronise the pairing and loop forever. Read whole lines with getline and
// count only 'a'..'z', which also drops any stray '\r' from CRLF data.

#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);

    std::string a, b;
    while (std::getline(std::cin, a)) {
        if (!std::getline(std::cin, b)) break;

        int ca[26], cb[26];
        for (int i = 0; i < 26; ++i) { ca[i] = 0; cb[i] = 0; }

        for (std::string::size_type i = 0; i < a.size(); ++i)
            if (a[i] >= 'a' && a[i] <= 'z') ++ca[a[i] - 'a'];
        for (std::string::size_type i = 0; i < b.size(); ++i)
            if (b[i] >= 'a' && b[i] <= 'z') ++cb[b[i] - 'a'];

        std::string x;
        for (int c = 0; c < 26; ++c) {
            int n = ca[c] < cb[c] ? ca[c] : cb[c];
            for (int k = 0; k < n; ++k) x += (char)('a' + c);
        }

        std::cout << x << "\n";
    }
    return 0;
}
