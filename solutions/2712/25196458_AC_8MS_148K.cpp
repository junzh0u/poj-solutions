// Model: gpt-5.6-terra
#include <cctype>
#include <iostream>
#include <string>

using namespace std;

bool isLetter(char c) {
    return isalpha(static_cast<unsigned char>(c)) != 0;
}

bool isUpper(char c) {
    return isupper(static_cast<unsigned char>(c)) != 0;
}

bool isDigit(char c) {
    return isdigit(static_cast<unsigned char>(c)) != 0;
}

bool isPunctuation(char c) {
    return c != ' ' && !isLetter(c) && !isDigit(c);
}

bool isSuspicious(const string& message) {
    for (size_t i = 0; i < message.size(); ++i) {
        const bool leftIsSpace = i == 0 || message[i - 1] == ' ';
        const bool rightIsSpace = i + 1 == message.size() || message[i + 1] == ' ';
        if (leftIsSpace && rightIsSpace && message[i] != 'a' &&
            message[i] != 'A' && message[i] != 'I') {
            return true;
        }

        if (i + 1 == message.size()) {
            continue;
        }

        const char first = message[i];
        const char second = message[i + 1];
        if (isUpper(first) && isUpper(second)) {
            return true;
        }
        if ((isDigit(first) && isLetter(second)) ||
            (isLetter(first) && isDigit(second))) {
            return true;
        }
        if (isPunctuation(first) && isPunctuation(second)) {
            const bool exactlyOneQuote = (first == '"') != (second == '"');
            if (!exactlyOneQuote) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    string message;
    while (getline(cin, message) && message != "#") {
        cout << (isSuspicious(message) ? "suspicious" : "OK") << '\n';
    }
    return 0;
}
