// POJ 1588 - Reverse Roman Notation
// Model: claude-sonnet-5
// Approach: simulate a stack-based RPN calculator whose operands and
// results are printed as Roman numerals. Numbers are stored internally
// as plain integers (long long, to be safe against unbounded growth from
// chained multiplications) and only converted to/from Roman numerals at
// input/output time. '=' peeks the top of stack without popping it (the
// sample input relies on this: after printing II, the same 2 is still on
// the stack for the next multiplication). Binary ops pop two and leave
// the stack unchanged on underflow. Division by zero discards the zero
// divisor but restores the (non-zero) dividend, per the statement's
// explicit instruction; ordinary division truncates toward zero (C++
// integer division), since the statement does not specify a rounding
// rule for inexact quotients and the board has no answer either.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

long long romanToInt(const string &s) {
    long long val[128];
    memset(val, 0, sizeof(val));
    val[(int)'I'] = 1;
    val[(int)'V'] = 5;
    val[(int)'X'] = 10;
    val[(int)'L'] = 50;
    val[(int)'C'] = 100;
    val[(int)'D'] = 500;
    val[(int)'M'] = 1000;
    long long total = 0;
    int n = (int)s.size();
    for (int i = 0; i < n; ++i) {
        long long cur = val[(int)s[i]];
        if (i + 1 < n && val[(int)s[i+1]] > cur) {
            total -= cur;
        } else {
            total += cur;
        }
    }
    return total;
}

string intToRoman(long long num) {
    struct Pair { long long value; const char *sym; };
    static const Pair table[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };
    string result;
    for (int i = 0; i < 13; ++i) {
        while (num >= table[i].value) {
            num -= table[i].value;
            result += table[i].sym;
        }
    }
    return result;
}

bool isRoman(const string &s) {
    if (s.size() != 1) return true;
    char c = s[0];
    return !(c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}

int main() {
    vector<long long> stk;
    string tok;
    while (cin >> tok) {
        if (isRoman(tok)) {
            stk.push_back(romanToInt(tok));
            continue;
        }
        char op = tok[0];
        if (op == '=') {
            if (stk.empty()) {
                printf("stack underflow\n");
            } else {
                long long v = stk.back();
                if (v <= 0 || v > 4999) {
                    printf("out of range exception\n");
                } else {
                    printf("%s\n", intToRoman(v).c_str());
                }
            }
            continue;
        }
        // binary operations
        if (stk.size() < 2) {
            printf("stack underflow\n");
            continue;
        }
        long long a = stk.back(); stk.pop_back(); // top (first)
        long long b = stk.back(); stk.pop_back(); // second
        if (op == '+') {
            stk.push_back(b + a);
        } else if (op == '-') {
            stk.push_back(b - a);
        } else if (op == '*') {
            stk.push_back(b * a);
        } else if (op == '/') {
            if (a == 0) {
                printf("division by zero exception\n");
                stk.push_back(b);
            } else {
                stk.push_back(b / a);
            }
        }
    }
    return 0;
}
