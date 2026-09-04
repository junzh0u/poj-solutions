// POJ 3448 - Roman Expressions
// Model: claude-sonnet-5
// Approach: Ten registers hold integer values in [0,10000]. Each input line is
// either an assignment "d=expr", RESET, or QUIT. An expression is a sum/difference
// of terms separated by '+'/'-', where each term is either a single-digit register
// reference or a Roman numeral literal. Roman numerals here extend the usual
// I..MMM range: the thousands part may repeat 'M' up to ten times (to reach the
// value 10000, as the sample "1=MMMMMMMMMM" confirms), while hundreds/tens/ones
// still use the standard subtractive forms, and 'O' denotes zero on its own.
// Evaluate left to right (order doesn't matter for pure +/- sums); if any
// referenced register was never assigned (including right after RESET), or the
// final value is negative or exceeds 10000, print "Error" and leave registers
// unchanged. Ambiguity: the statement never states term order/associativity
// matters, and since only '+' and '-' appear (no multiplication/parentheses),
// summing signed terms in a single left-to-right pass is unambiguous and
// verified against the whole worked sample, including chained self-references
// after RESET and the register-not-yet-assigned Error cases.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

static int romanValue(char c) {
    switch (c) {
        case 'O': return 0;
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
    }
    return -1;
}

static int romanToInt(const string& s) {
    if (s == "O") return 0;
    int total = 0;
    int n = (int)s.size();
    for (int i = 0; i < n; ++i) {
        int v = romanValue(s[i]);
        if (i + 1 < n && v < romanValue(s[i + 1])) {
            total -= v;
        } else {
            total += v;
        }
    }
    return total;
}

static string intToRoman(int v) {
    if (v == 0) return "O";
    static const char* hund[10] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    static const char* tens[10] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    static const char* ones[10] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
    int thousands = v / 1000;
    int rem = v % 1000;
    int h = (int)(rem / 100);
    int t = (int)((rem % 100) / 10);
    int o = (int)(rem % 10);
    string result;
    for (int i = 0; i < thousands; ++i) result += 'M';
    result += hund[h];
    result += tens[t];
    result += ones[o];
    return result;
}

int main() {
    bool assigned[10];
    int value[10];
    for (int i = 0; i < 10; ++i) { assigned[i] = false; value[i] = 0; }

    string line;
    while (getline(cin, line)) {
        // strip trailing \r and whitespace
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'
               || line[line.size() - 1] == ' ' || line[line.size() - 1] == '\t')) {
            line.erase(line.size() - 1);
        }
        if (line.empty()) continue; // skip stray blank lines defensively

        if (line == "QUIT") {
            printf("Bye\n");
            break;
        }
        if (line == "RESET") {
            for (int i = 0; i < 10; ++i) assigned[i] = false;
            printf("Ready\n");
            continue;
        }

        // Assignment: line[0] is the register digit, line[1] == '='
        int reg = line[0] - '0';
        string expr = line.substr(2);

        int total = 0;
        bool error = false;
        int n = (int)expr.size();
        int pos = 0;
        int sign = 1;
        while (pos < n) {
            char c = expr[pos];
            if (c == '+') { sign = 1; ++pos; continue; }
            if (c == '-') { sign = -1; ++pos; continue; }
            int start = pos;
            while (pos < n && expr[pos] != '+' && expr[pos] != '-') ++pos;
            string term = expr.substr(start, pos - start);
            int termValue;
            if (term.size() == 1 && term[0] >= '0' && term[0] <= '9') {
                int r = term[0] - '0';
                if (!assigned[r]) {
                    error = true;
                    termValue = 0;
                } else {
                    termValue = value[r];
                }
            } else {
                termValue = romanToInt(term);
            }
            total += sign * termValue;
            sign = 1;
        }

        if (error || total < 0 || total > 10000) {
            printf("Error\n");
        } else {
            value[reg] = total;
            assigned[reg] = true;
            printf("%d=%s\n", reg, intToRoman(total).c_str());
        }
    }
    return 0;
}
