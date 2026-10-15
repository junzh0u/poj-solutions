// POJ 1653 - Compacting Stickers
// Model: gpt-6.1-sol
// Attach punctuation to its preceding word, then enumerate text widths.
// Greedy packing minimizes the number of lines at each width.
// The sample omits the stated line count; accept either input framing.
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;
int main() {
    string first, text, line;
    if (!getline(cin, first)) return 0;
    istringstream header(first);
    int count; string extra;
    if ((header >> count) && !(header >> extra)) {
        for (int i = 0; i < count && getline(cin, line); ++i) text += line + "\n";
    } else {
        text = first + "\n";
        while (getline(cin, line)) text += line + "\n";
    }
    vector<int> lengths;
    for (string::size_type i = 0; i < text.size();) {
        if (isalpha(static_cast<unsigned char>(text[i]))) {
            int length = 0;
            while (i < text.size() && isalpha(static_cast<unsigned char>(text[i]))) { ++length; ++i; }
            lengths.push_back(length);
        } else {
            if (text[i] == ',' || text[i] == '.' || text[i] == '!' || text[i] == '?')
                ++lengths.back();
            ++i;
        }
    }
    int total = -1, minimum = 0;
    for (int i = 0; i < (int)lengths.size(); ++i) {
        total += lengths[i] + 1;
        minimum = max(minimum, lengths[i]);
    }
    int answer = (total + 2) * 3;
    for (int width = minimum; width <= total; ++width) {
        int lines = 1, used = 0;
        for (int i = 0; i < (int)lengths.size(); ++i) {
            if (used == 0) used = lengths[i];
            else if (used + 1 + lengths[i] <= width) used += 1 + lengths[i];
            else { ++lines; used = lengths[i]; }
        }
        answer = min(answer, (width + 2) * (lines + 2));
    }
    cout << answer << '\n';
    return 0;
}
