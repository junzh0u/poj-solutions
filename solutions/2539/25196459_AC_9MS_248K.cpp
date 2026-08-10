// POJ 2539 - Division
// Model: gpt-5.6-terra
// For t > 1, t^b - 1 divides t^a - 1 exactly when b divides a. In that
// case build the geometric sum 1 + t^b + ... + t^(a-b), stopping once it
// reaches 100 digits. A base-10000 integer keeps the cutoff exact.

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

class BigInteger {
public:
    BigInteger() : digit(1, 1) {}

    bool multiply(int factor_value) {
        vector<int> factor;
        while (factor_value > 0) {
            factor.push_back(factor_value % BASE);
            factor_value /= BASE;
        }

        vector<int> product(digit.size() + factor.size(), 0);
        for (size_t i = 0; i < digit.size(); ++i) {
            for (size_t j = 0; j < factor.size(); ++j) {
                product[i + j] += digit[i] * factor[j];
            }
        }
        for (size_t i = 0; i + 1 < product.size(); ++i) {
            product[i + 1] += product[i] / BASE;
            product[i] %= BASE;
        }
        while (product.size() > 1 && product.back() == 0) {
            product.pop_back();
        }
        digit.swap(product);
        return has_fewer_than_100_digits();
    }

    bool add(const BigInteger &other) {
        digit.resize(max(digit.size(), other.digit.size()), 0);
        int carry = 0;
        for (size_t i = 0; i < digit.size(); ++i) {
            int value = digit[i] + carry;
            if (i < other.digit.size()) {
                value += other.digit[i];
            }
            digit[i] = value % BASE;
            carry = value / BASE;
        }
        if (carry != 0) {
            digit.push_back(carry);
        }
        return has_fewer_than_100_digits();
    }

    void print() const {
        cout << digit.back();
        cout << setfill('0');
        for (size_t i = digit.size() - 1; i > 0; --i) {
            cout << setw(4) << digit[i - 1];
        }
    }

private:
    enum { BASE = 10000 };
    vector<int> digit;

    bool has_fewer_than_100_digits() const {
        if (digit.size() < 25) {
            return true;
        }
        if (digit.size() > 25) {
            return false;
        }
        return digit.back() < 1000;
    }
};

bool evaluate(int t, int a, int b, BigInteger &answer) {
    if (t == 1 || a % b != 0) {
        return false;
    }
    if (a == b) {
        return true;
    }

    BigInteger term;
    const int last_exponent = a - b;
    for (int exponent = 1; exponent <= last_exponent; ++exponent) {
        if (!term.multiply(t)) {
            return false;
        }
        if (exponent % b == 0 && !answer.add(term)) {
            return false;
        }
    }
    return true;
}

int main() {
    int t;
    int a;
    int b;
    while (cin >> t >> a >> b) {
        cout << '(' << t << '^' << a << "-1)/(" << t << '^' << b << "-1) ";
        BigInteger answer;
        if (evaluate(t, a, b, answer)) {
            answer.print();
            cout << '\n';
        } else {
            cout << "is not an integer with less than 100 digits.\n";
        }
    }
    return 0;
}
