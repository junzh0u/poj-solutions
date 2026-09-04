// POJ 2290 - House Numbers
// Model: claude-sonnet-5
// For each street order, process sub-orders in the order given, using a
// tri-state array (unset / included / excluded) over house numbers 1..9999.
// A single number or a "+" range sets numbers to included unless they are
// already excluded (exclusion is sticky and cannot be undone by a later
// include, per the statement's "500 in the second sample" note). A "-"
// range always marks numbers excluded, overriding any earlier inclusion,
// and also blocks any later inclusion of the same number.
// All sub-order fields are read with formatted (whitespace-skipping) input
// rather than per physical line, because the discuss board (message 160114)
// warns that a single sub-order can be split across multiple input lines,
// and that input lines can carry extra trailing spaces. Only the street
// name needs line-based reading, since it is the one field that may itself
// contain spaces. The board (message 80961) also confirms an order can end
// up with zero addresses/digits, which uses the plural form like any count
// other than exactly 1.
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <limits>
using namespace std;

const int MAXN = 10000; // house numbers are 1..9999

int state[MAXN + 1]; // 0 = unset, 1 = included, 2 = excluded

string rstrip(const string &s) {
    size_t end = s.size();
    while (end > 0 && (s[end - 1] == '\r' || s[end - 1] == '\n' || s[end - 1] == ' ' || s[end - 1] == '\t')) {
        end--;
    }
    return s.substr(0, end);
}

int main() {
    int t;
    if (!(cin >> t)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int tc = 0; tc < t; tc++) {
        string street;
        getline(cin, street);
        street = rstrip(street);

        int n;
        cin >> n;

        memset(state, 0, sizeof(state));

        for (int i = 0; i < n; i++) {
            string tok;
            cin >> tok;
            if (tok == "+" || tok == "-") {
                long a, b, c;
                cin >> a >> b >> c;
                bool exclude = (tok == "-");
                for (long x = a; x <= b; x += c) {
                    if (exclude) {
                        state[x] = 2;
                    } else {
                        if (state[x] != 2) state[x] = 1;
                    }
                }
            } else {
                int num = atoi(tok.c_str());
                if (state[num] != 2) state[num] = 1;
            }
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        long digitCount[10];
        memset(digitCount, 0, sizeof(digitCount));
        long addresses = 0;
        for (int x = 1; x <= 9999; x++) {
            if (state[x] == 1) {
                addresses++;
                char buf[8];
                sprintf(buf, "%d", x);
                for (char *p = buf; *p; p++) {
                    digitCount[*p - '0']++;
                }
            }
        }

        long total = 0;
        for (int d = 0; d < 10; d++) total += digitCount[d];

        printf("%s\n", street.c_str());
        if (addresses == 1) {
            printf("1 address\n");
        } else {
            printf("%ld addresses\n", addresses);
        }
        for (int d = 0; d < 10; d++) {
            printf("Make %ld digit %d\n", digitCount[d], d);
        }
        if (total == 1) {
            printf("In total 1 digit\n");
        } else {
            printf("In total %ld digits\n", total);
        }
    }

    return 0;
}
