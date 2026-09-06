// POJ 1646 - Double Trouble
// Model: gpt-5.6-terra
// Reverse each possible ciphertext block size to recover the shifted plaintext.
// A crib occurrence has one constant alphabetic offset at every character; that
// offset is the shift key, so checking all starts finds every valid key directly.
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int main() {
    int cases;
    if (!(cin >> cases)) return 0;
    while (cases--) {
        int n;
        cin >> n;
        string cipher, part;
        while ((int)cipher.size() < n) {
            cin >> part;
            cipher += part;
        }
        string crib;
        cin >> crib;

        int bestS = 26;
        int bestM = 21;
        int m;
        for (m = 5; m <= 20; ++m) {
            string shifted = cipher;
            int begin;
            for (begin = 0; begin < n; begin += m) {
                int end = begin + m;
                if (end > n) end = n;
                reverse(shifted.begin() + begin, shifted.begin() + end);
            }
            int start;
            for (start = 0; start + (int)crib.size() <= n; ++start) {
                int s = (shifted[start] - crib[0] + 26) % 26;
                if (s == 0) continue;
                int j;
                for (j = 1; j < (int)crib.size(); ++j) {
                    if ((shifted[start + j] - crib[j] + 26) % 26 != s) break;
                }
                if (j == (int)crib.size() &&
                    (s < bestS || (s == bestS && m < bestM))) {
                    bestS = s;
                    bestM = m;
                }
            }
        }
        if (bestS == 26) cout << "Crib is not encrypted.\n";
        else cout << bestS << ' ' << bestM << '\n';
    }
    return 0;
}
