// POJ 1489 - Egyptian Multiplication
// Model: claude-sonnet-5
// Simulate the Egyptian doubling multiplication algorithm: starting with
// left=1/right=a, repeatedly double both columns while doubling the left
// value would not exceed b. Since the left column values are exactly the
// powers of two 1,2,4,...,2^k with 2^k <= b < 2^(k+1), the rows summing to
// b are precisely those whose left value corresponds to a set bit of b's
// binary representation (b & left != 0); their right-column sum is a*b.
// Numbers are printed by writing, in increasing place-value order (ones,
// tens, hundreds, thousands, ten-thousands), each nonzero digit's symbol
// repeated that many times, followed by a single space (also after the
// last group, per the statement). Left-column asterisked rows get "* "
// appended after the number's own trailing space, then the line is padded
// with spaces to column 34 before the right-column number starts at
// column 35.
//
// Ambiguity / pitfalls settled via the discuss board (poj.org/bbs?problem_id=1489):
//  - Inputs a,b are <=99999, but right-column values (a times a power of
//    two) and the final product a*b can be far larger (up to ~1e10, needs
//    64-bit). The board explicitly notes this ("liangge shu jiaoda shi,
//    chengji hui chaochu int fanwei"), and a separate post gives a worked
//    example whose right-column numbers exceed 99999.
//  - There is no symbol above ten-thousands, so once a value exceeds
//    99999 the excess is simply truncated: every place (including
//    ten-thousands) is (n / place) % 10, i.e. effectively n mod 100000
//    decomposed as usual ("chengji dayu 99999 shi, gaowei bu chuli" -
//    when the product exceeds 99999 the higher digits are just dropped).
//    Verified digit-by-digit against the worked example in board message
//    340288 (a=20003, b=10002): e.g. right-column value 160024 prints as
//    "|||| nn rrrrrr" (tenthousands digit (160024/10000)%10=6, NOT the
//    unbounded quotient 16), and the final product 200070006 prints as
//    "|||||| rrrrrrr" (tenthousands digit (200070006/10000)%10=7).
//  - The official sample itself is buggy (several board posts say so,
//    e.g. message 122598): two of its five "The solution is: ..." lines
//    are missing the trailing space that every other output line has,
//    even though the statement text says every group - including the
//    last - is followed by a space. Trust the statement/board over the
//    sample's literal bytes here; this program always emits the trailing
//    space.
//  - Blank lines are not only the final input terminator: real judge
//    input can contain stray blank lines between pairs too (board thread
//    "shuru you kongxing, xuyao guolu xia" plus its reply, which posts an
//    accepted solution that skips any line parsing to a zero value while
//    hunting for the first number of a pair). This program simply skips
//    every blank line wherever it appears and stops only at true EOF,
//    which subsumes both the plain reading and the buggy-input reading.
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

string egyptian(long long n) {
    string s;
    long long vals[5] = {1, 10, 100, 1000, 10000};
    char chs[5] = {'|', 'n', '9', '8', 'r'};
    for (int i = 0; i < 5; i++) {
        int d = (int)((n / vals[i]) % 10);
        for (int j = 0; j < d; j++) s += chs[i];
        if (d > 0) s += ' ';
    }
    return s;
}

long long parseValue(const string& line) {
    long long ones = 0, tens = 0, hund = 0, thou = 0, tenthou = 0;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '|') ones++;
        else if (c == 'n') tens++;
        else if (c == '9') hund++;
        else if (c == '8') thou++;
        else if (c == 'r') tenthou++;
    }
    return ones + tens * 10 + hund * 100 + thou * 1000 + tenthou * 10000;
}

int main() {
    string line;
    vector<string> pending;
    while (true) {
        if (!std::getline(cin, line)) break; // true EOF: only real terminator
        bool blank = true;
        for (size_t i = 0; i < line.size(); i++) {
            if (!isspace((unsigned char)line[i])) { blank = false; break; }
        }
        if (blank) continue; // skip stray/terminating blank lines, keep reading

        pending.push_back(line);
        if (pending.size() == 2) {
            long long a = parseValue(pending[0]);
            long long b = parseValue(pending[1]);

            vector<pair<long long, long long> > rows;
            long long left = 1, right = a;
            while (true) {
                rows.push_back(make_pair(left, right));
                if (left * 2 > b) break;
                left *= 2;
                right *= 2;
            }

            for (size_t i = 0; i < rows.size(); i++) {
                long long l = rows[i].first, r = rows[i].second;
                bool marked = (b & l) != 0;
                string ls = egyptian(l);
                if (marked) ls += "* ";
                while (ls.size() < 34) ls += ' ';
                string rs = egyptian(r);
                printf("%s%s\n", ls.c_str(), rs.c_str());
            }
            printf("The solution is: %s\n", egyptian(a * b).c_str());
            pending.clear();
        }
    }
    return 0;
}
