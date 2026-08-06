// POJ 2271 - HTML
// Model: gpt-5.6-terra
// Stream whitespace-delimited words and tags, tracking the current output width.
// Tags are tokens: <br> always emits one newline; <hr> first ends a partial
// line, then emits an 80-character ruler and its newline.
#include <iostream>
#include <string>

using namespace std;

int main() {
    string token;
    int lineLength = 0;

    while (cin >> token) {
        if (token == "<br>") {
            cout << '\n';
            lineLength = 0;
        } else if (token == "<hr>") {
            if (lineLength != 0) {
                cout << '\n';
            }
            cout << string(80, '-') << '\n';
            lineLength = 0;
        } else if (lineLength == 0) {
            cout << token;
            lineLength = static_cast<int>(token.length());
        } else if (lineLength + 1 + static_cast<int>(token.length()) <= 80) {
            cout << ' ' << token;
            lineLength += 1 + static_cast<int>(token.length());
        } else {
            cout << '\n' << token;
            lineLength = static_cast<int>(token.length());
        }
    }
    if (lineLength != 0) {
        cout << '\n';
    }
    return 0;
}
