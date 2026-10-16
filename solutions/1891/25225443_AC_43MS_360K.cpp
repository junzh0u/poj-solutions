// POJ 1891 - Typesetting
// Model: gpt-6.1-sol
// Greedily fill each line, rounding each scaled character separately.
// A separating blank uses the preceding word's style, even across controls.
// Track occupancy explicitly: rounding can make a nonempty word zero-width.
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
using namespace std;
int widths[256][6];
int scaled(unsigned char c, int font, int size) {
    return (widths[c][font] * size + 5) / 10;
}
void emit(int number, const string &first, const string &last, int remaining) {
    cout << "Line " << number << ": " << first;
    if (remaining >= 0) cout << " ... " << last;
    cout << " (" << remaining << " whitespace)\n";
}
int main() {
    int n;
    if (!(cin >> n)) return 0;
    string text;
    getline(cin, text);
    for (int i = 0; i < n; ++i) {
        getline(cin, text);
        unsigned char c = text[0];
        istringstream in(text.substr(1));
        for (int f = 0; f < 6; ++f) in >> widths[c][f];
    }
    int lines, limit, paragraph = 0;
    while (cin >> lines >> limit && lines) {
        getline(cin, text);
        cout << "Paragraph " << ++paragraph << '\n';
        int font = 0, size = 10, used = 0, gap = 0, number = 0;
        bool occupied = false;
        string first, last;
        for (int i = 0; i < lines; ++i) {
            getline(cin, text);
            istringstream in(text);
            string word;
            while (in >> word) {
                if (word.size() == 3 && word[0] == '*' && word[1] == 'f'
                    && word[2] >= '1' && word[2] <= '6') {
                    font = word[2] - '1';
                    continue;
                }
                bool control = word.size() >= 3 && word.size() <= 4
                    && word[0] == '*' && word[1] == 's';
                for (unsigned j = 2; control && j < word.size(); ++j)
                    if (word[j] < '0' || word[j] > '9') control = false;
                int value = control ? atoi(word.c_str() + 2) : 0;
                if (control && value >= 1 && value <= 99) {
                    size = value;
                    continue;
                }
                int width = 0;
                for (unsigned j = 0; j < word.size(); ++j)
                    width += scaled((unsigned char)word[j], font, size);
                if (occupied && used + gap + width > limit) {
                    emit(++number, first, last, limit - used);
                    occupied = false;
                }
                if (!occupied) {
                    first = word;
                    used = width;
                    occupied = true;
                } else used += gap + width;
                last = word;
                gap = scaled(' ', font, size);
                if (used > limit) {
                    emit(++number, first, last, limit - used);
                    occupied = false;
                }
            }
        }
        if (occupied) emit(++number, first, last, limit - used);
    }
    return 0;
}
