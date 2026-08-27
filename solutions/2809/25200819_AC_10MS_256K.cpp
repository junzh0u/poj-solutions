// POJ 2809 - It's My Derivative
// Model: gpt-5.6-terra
// Parse every written term independently, then render its derivative, its
// substitution, and its evaluated contribution in the original term order.
// Terms are intentionally not combined: discussion-board data permits
// repeated powers and even powers written in a non-descending order.
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Term {
    long long coefficient;
    int power;
};

long long ipow(long long base, int exponent) {
    long long answer = 1;
    while (exponent-- > 0) answer *= base;
    return answer;
}

vector<Term> derivative(const string &polynomial) {
    vector<Term> answer;
    int i = 0;
    while (i < (int)polynomial.size()) {
        int sign = 1;
        if (polynomial[i] == '+' || polynomial[i] == '-') {
            if (polynomial[i] == '-') sign = -1;
            ++i;
        }
        int number = 0;
        bool hasNumber = false;
        while (i < (int)polynomial.size() && polynomial[i] >= '0' && polynomial[i] <= '9') {
            hasNumber = true;
            number = number * 10 + polynomial[i] - '0';
            ++i;
        }
        long long coefficient = sign * (hasNumber ? number : 1);
        int power = 0;
        if (i < (int)polynomial.size() && polynomial[i] == 'x') {
            power = 1;
            ++i;
            if (i < (int)polynomial.size() && polynomial[i] == '^') {
                ++i;
                power = 0;
                while (i < (int)polynomial.size() && polynomial[i] >= '0' && polynomial[i] <= '9') {
                    power = power * 10 + polynomial[i] - '0';
                    ++i;
                }
            }
        }
        if (power > 0) {
            Term term;
            term.coefficient = coefficient * power;
            term.power = power - 1;
            answer.push_back(term);
        }
    }
    return answer;
}

void printTerm(const Term &term, bool first, bool substituted, long long x) {
    if (!first && term.coefficient >= 0) cout << '+';
    if (term.coefficient < 0) cout << '-';
    long long magnitude = term.coefficient < 0 ? -term.coefficient : term.coefficient;
    if (term.power == 0) {
        cout << magnitude;
        return;
    }
    if (magnitude != 1) cout << magnitude;
    if (substituted) cout << '(' << x << ')';
    else cout << 'x';
    if (term.power > 1) cout << '^' << term.power;
}

void printPolynomial(const vector<Term> &terms, bool substituted, long long x) {
    if (terms.empty()) {
        cout << "0\n";
        return;
    }
    int i;
    for (i = 0; i < (int)terms.size(); ++i) printTerm(terms[i], i == 0, substituted, x);
    cout << '\n';
}

void printValues(const vector<Term> &terms, long long x) {
    if (terms.empty()) {
        cout << "0\n0\n";
        return;
    }
    long long total = 0;
    int i;
    for (i = 0; i < (int)terms.size(); ++i) {
        long long value = terms[i].coefficient * ipow(x, terms[i].power);
        if (i != 0 && value >= 0) cout << '+';
        cout << value;
        total += value;
    }
    cout << '\n' << total << '\n';
}

int main() {
    int n;
    if (!(cin >> n)) return 0;
    int caseNumber;
    for (caseNumber = 1; caseNumber <= n; ++caseNumber) {
        long long x;
        string polynomial;
        cin >> x >> polynomial;
        vector<Term> terms = derivative(polynomial);
        cout << "POLYNOMIAL " << caseNumber << '\n';
        cout << polynomial << '\n';
        printPolynomial(terms, false, x);
        printPolynomial(terms, true, x);
        printValues(terms, x);
    }
    return 0;
}
