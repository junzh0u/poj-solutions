// POJ 2305 - Basic remains
// Model: gpt-5.6-terra
// Approach: Scan p from most significant digit, maintaining its remainder modulo m.
// This avoids constructing the up-to-1000-digit input number; m fits in a signed 64-bit integer.
// The result is converted back to base b, with zero printed explicitly.
#include <iostream>
#include <string>

using namespace std;

long long parseBase(const string& text, int base) {
    long long value = 0;
    int i;
    for (i = 0; i < static_cast<int>(text.size()); ++i) {
        value = value * base + (text[i] - '0');
    }
    return value;
}

string formatBase(long long value, int base) {
    string result;
    if (value == 0) {
        return "0";
    }
    while (value > 0) {
        result = static_cast<char>('0' + value % base) + result;
        value /= base;
    }
    return result;
}

int main() {
    int base;
    string p;
    string mText;

    while (cin >> base && base != 0) {
        long long modulus;
        long long remainder = 0;
        int i;

        cin >> p >> mText;
        modulus = parseBase(mText, base);
        for (i = 0; i < static_cast<int>(p.size()); ++i) {
            remainder = (remainder * base + (p[i] - '0')) % modulus;
        }
        cout << formatBase(remainder, base) << '\n';
    }
    return 0;
}
