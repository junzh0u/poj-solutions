// POJ 3337 - Expression Evaluator
// Model: gpt-5.6-terra
// Parse the whitespace-free expression as signed operands, applying each
// prefix change before its operand and each postfix change afterward.
#include <cctype>
#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int tests;
    if (!(cin >> tests)) return 0;
    string original;
    getline(cin, original);
    while (tests--) {
        getline(cin, original);
        string expression;
        for (size_t i = 0; i < original.size(); ++i)
            if (!isspace(static_cast<unsigned char>(original[i])))
                expression += original[i];

        int value[26];
        bool used[26];
        for (int i = 0; i < 26; ++i) {
            value[i] = i + 1;
            used[i] = false;
        }

        int result = 0;
        int sign = 1;
        size_t pos = 0;
        while (pos < expression.size()) {
            int prefix = 0;
            if (expression.compare(pos, 2, "++") == 0) {
                prefix = 1;
                pos += 2;
            } else if (expression.compare(pos, 2, "--") == 0) {
                prefix = -1;
                pos += 2;
            }
            int variable = expression[pos++] - 'a';
            used[variable] = true;
            value[variable] += prefix;
            result += sign * value[variable];

            if (pos + 1 < expression.size() &&
                expression.compare(pos, 2, "++") == 0) {
                ++value[variable];
                pos += 2;
            } else if (pos + 1 < expression.size() &&
                       expression.compare(pos, 2, "--") == 0) {
                --value[variable];
                pos += 2;
            }

            if (pos < expression.size()) {
                sign = expression[pos] == '+' ? 1 : -1;
                ++pos;
            }
        }

        cout << "Expression: " << original << "\n";
        cout << "value = " << result << "\n";
        for (int i = 0; i < 26; ++i)
            if (used[i]) cout << static_cast<char>('a' + i) << " = " << value[i] << "\n";
    }
    return 0;
}
