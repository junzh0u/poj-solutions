// POJ 2319 - COMPRESS
// Model: gpt-6.1-sol
// A k-digit suffix decodes to the first number above F with that suffix.
// It represents R exactly when R-F <= 10^k. Count digits of R-F-1,
// with at least one digit, and retain that many characters of R.
// Leading zeros in the suffix are significant; equality at 10^k is valid.
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    int first, second;
    char hyphen;
    while (cin >> first >> hyphen >> second) {
        int gap = second - first - 1;
        int length = 1;
        while (gap >= 10) {
            gap /= 10;
            ++length;
        }
        ostringstream out;
        out << second;
        string digits = out.str();
        cout << first << '-' << digits.substr(digits.size() - length) << '\n';
    }
    return 0;
}
