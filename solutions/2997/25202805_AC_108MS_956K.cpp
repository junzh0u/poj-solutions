// POJ 2997 - IVXLCDM
// Model: gpt-5.6-terra
// Approach: every valid numeral is M* followed by one canonical numeral from
// 0 through 999.  Taking an additional M is worth more than any possible
// suffix, so all m's are selected; test the canonical suffixes in descending
// value order as subsequences after the last selected m.
// The input is read by whole lines because spaces are part of an inscription.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static string digitRoman(int digit, char one, char five, char ten) {
    string result;
    if (digit <= 3) {
        result.assign(digit, one);
    } else if (digit == 4) {
        result += one;
        result += five;
    } else if (digit <= 8) {
        result += five;
        result.append(digit - 5, one);
    } else {
        result += one;
        result += ten;
    }
    return result;
}

static string romanBelowThousand(int value) {
    string result;
    result += digitRoman(value / 100, 'C', 'D', 'M');
    result += digitRoman((value / 10) % 10, 'X', 'L', 'C');
    result += digitRoman(value % 10, 'I', 'V', 'X');
    return result;
}

static int symbolIndex(char ch) {
    if (ch == 'I') return 0;
    if (ch == 'V') return 1;
    if (ch == 'X') return 2;
    if (ch == 'L') return 3;
    if (ch == 'C') return 4;
    if (ch == 'D') return 5;
    return 6;
}

int main() {
    string line;
    while (getline(cin, line)) {
        int lastM = -1;
        int mCount = 0;
        int i;
        for (i = 0; i < (int)line.size(); ++i) {
            if (line[i] == 'm') {
                ++mCount;
                lastM = i;
            }
        }

        const int n = (int)line.size();
        vector< vector<int> > next(n + 1, vector<int>(7, n));
        for (i = n - 1; i >= 0; --i) {
            next[i] = next[i + 1];
            char ch = line[i];
            if (ch == 'i' || ch == 'v' || ch == 'x' || ch == 'l' ||
                ch == 'c' || ch == 'd' || ch == 'm') {
                next[i][symbolIndex(ch - 'a' + 'A')] = i;
            }
        }

        int best = 0;
        for (int value = 999; value >= 0; --value) {
            string candidate = romanBelowThousand(value);
            int pos = lastM + 1;
            bool found = true;
            for (i = 0; i < (int)candidate.size(); ++i) {
                int at = next[pos][symbolIndex(candidate[i])];
                if (at == n) {
                    found = false;
                    break;
                }
                pos = at + 1;
            }
            if (found) {
                best = value;
                break;
            }
        }
        cout << mCount * 1000 + best << '\n';
    }
    return 0;
}
