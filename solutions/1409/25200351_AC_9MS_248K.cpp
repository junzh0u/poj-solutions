// POJ 1409 - 77377
// Model: gpt-5.6-terra
// Convert every dictionary word to its phone-key signature.  At each position
// in the input, try matching words in lexicographic order; depth-first search
// consequently emits complete word sequences in the required lexicographic
// order.  The promised output bound makes storing the current path sufficient.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Word {
    string text;
    string code;
};

static vector<Word> words;
static string digits;
static vector<string> chosen;

static char key(char c) {
    if (c <= 'c') return '2';
    if (c <= 'f') return '3';
    if (c <= 'i') return '4';
    if (c <= 'l') return '5';
    if (c <= 'o') return '6';
    if (c <= 's') return '7';
    if (c <= 'v') return '8';
    return '9';
}

static bool lessWord(const Word &a, const Word &b) {
    return a.text < b.text;
}

static void emit() {
    int i;
    for (i = 0; i < (int)chosen.size(); ++i) {
        if (i) cout << ' ';
        cout << chosen[i];
    }
    cout << ".\n";
}

static void search(int pos) {
    int i;
    if (pos == (int)digits.size()) {
        emit();
        return;
    }
    for (i = 0; i < (int)words.size(); ++i) {
        int len = (int)words[i].code.size();
        if (pos + len <= (int)digits.size() &&
            digits.compare(pos, len, words[i].code) == 0) {
            chosen.push_back(words[i].text);
            search(pos + len);
            chosen.pop_back();
        }
    }
}

int main() {
    int n, i, j;
    while (cin >> n && n) {
        words.clear();
        for (i = 0; i < n; ++i) {
            Word w;
            cin >> w.text;
            for (j = 0; j < (int)w.text.size(); ++j)
                w.code += key(w.text[j]);
            words.push_back(w);
        }
        cin >> digits;
        sort(words.begin(), words.end(), lessWord);
        chosen.clear();
        search(0);
        cout << "--\n";
    }
    return 0;
}
