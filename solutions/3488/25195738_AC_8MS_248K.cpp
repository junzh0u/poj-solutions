// POJ 3488 - Arne Saknussemm
// Model: claude-opus-5
//
// Invert the scrambling of the statement, step by step backwards.  The n
// fragments are the rows of the n x K table (K = fragment length), so reading
// the table column-wise rebuilds the string S of step 4.  Reversing S undoes
// step 3's reversal, then '_' becomes a space and '\' a line break.  Finally
// step 2's padding is removed: it consists of spaces only, and M's last
// character is guaranteed not to be a space, so stripping every trailing space
// strips exactly the padding.
//
// Note that only trailing *spaces* may be stripped -- a message is allowed to
// end with a line break, and trimming trailing whitespace in general would eat
// it.  The official sample cannot distinguish the two (neither of its messages
// ends with a line break); a round-trip test against a scrambler written from
// the statement's steps does.
//
// Datasets run to EOF and are whitespace-separated, so token reads suffice:
// fragments never contain whitespace (spaces became '_', breaks became '\').
// Each deciphered message is printed followed by an empty line.

#include <string>
#include <vector>
#include <iostream>

int main() {
    int n;
    while (std::cin >> n) {
        std::vector<std::string> rows(n);
        for (int i = 0; i < n; i++) std::cin >> rows[i];
        int K = (int)rows[0].size();
        // read the table column-wise to recover the reversed, substituted string
        std::string s;
        s.reserve((size_t)n * (size_t)K);
        for (int c = 0; c < K; c++)
            for (int r = 0; r < n; r++)
                s += rows[r][c];
        // undo the reversal, then the character substitutions
        std::string m(s.rbegin(), s.rend());
        for (size_t i = 0; i < m.size(); i++) {
            if (m[i] == '_') m[i] = ' ';
            else if (m[i] == '\\') m[i] = '\n';
        }
        // drop the padding spaces appended in step 2
        size_t e = m.size();
        while (e > 0 && m[e - 1] == ' ') e--;
        m.erase(e);
        std::cout << m << "\n\n";
    }
    return 0;
}
