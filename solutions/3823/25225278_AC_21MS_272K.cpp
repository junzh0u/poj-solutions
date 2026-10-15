// POJ 3823 - Decompressing in a GIF
// Model: gpt-6.1-sol
// Decode dictionary phrases with one pending entry after each phrase.
// Before reading the next code, its width includes that pending entry.
// A code naming the pending entry expands to previous + previous[0].
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int digits(size_t n) {
    int d = 1;
    while (n >= 10) { n /= 10; ++d; }
    return d;
}
int main() {
    string code;
    int tc = 0;
    while (cin >> code && code != "0") {
        int n; cin >> n;
        vector<string> dict(n);
        for (int i = 0; i < n; ++i) cin >> dict[i];
        string previous;
        size_t pos = 0;
        cout << "Case " << ++tc << ": ";
        while (pos < code.size()) {
            int width = digits(dict.size() - (previous.empty() ? 1 : 0));
            size_t value = 0;
            for (int i = 0; i < width; ++i) value = value * 10 + code[pos++] - '0';
            string current;
            if (value < dict.size()) current = dict[value];
            else current = previous + previous[0];
            if (!previous.empty()) dict.push_back(previous + current[0]);
            cout << current;
            previous = current;
        }
        cout << '\n';
    }
    return 0;
}
