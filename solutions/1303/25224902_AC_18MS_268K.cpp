// POJ 1303 - Byte Me!
// Model: gpt-6.1-sol
// Simulate at most four hits, taking bytes below 382 and nibbles through 500.
// An opponent's hidden byte is at most 255, so reaching max(face-up)+255
// guarantees victory, including ties. Check this before every hit.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int readBinary() {
    string s; cin >> s;
    int v = 0;
    for (size_t i = 0; i < s.size(); ++i) v = v * 2 + s[i] - '0';
    return v;
}
int main() {
    string marker;
    while (cin >> marker && marker == "START") {
        int n; cin >> n;
        int total = readBinary(); total += readBinary();
        int target = 0;
        for (int i = 0; i < n; ++i) target = max(target, readBinary() + 255);
        int bytes[4], nibbles[4];
        for (int i = 0; i < 4; ++i) bytes[i] = readBinary();
        for (int i = 0; i < 4; ++i) nibbles[i] = readBinary();
        cin >> marker;
        cout << "HAND " << n << '\n';
        int b = 0, k = 0;
        for (int hits = 0; hits < 4 && total <= 510 && total < target; ++hits) {
            if (total < 382) {
                cout << "Byte me!\n"; total += bytes[b++];
            } else if (total <= 500) {
                cout << "Nibble me!\n"; total += nibbles[k++];
            } else break;
        }
        if (total > 510) cout << "Bust!\n";
        else if (total >= target) cout << "Win!\n";
        else cout << "Lose!\n";
    }
    return 0;
}
