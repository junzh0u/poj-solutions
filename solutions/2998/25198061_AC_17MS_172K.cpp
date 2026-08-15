// POJ 2998 - Hardware
// Model: claude-sonnet-5
// Approach: Read line by line. Each order has a road name line and a
// "<total> addresses" line (echoed verbatim). Then repeatedly read
// specification lines - either a single house number, or "+ first last
// interval" describing an arithmetic sequence of house numbers - until the
// cumulative number of addresses produced equals the stated total. Count
// digit occurrences 0..9 across all house numbers in the order and print
// the required summary, using singular "digit" for a total of exactly 1.
// Ambiguity: the input gives no explicit count of specification lines per
// order; the boundary between orders is inferred by accumulating address
// counts until the declared total is reached (confirmed by the sample:
// 13 + 1 + 9 = 23). The discuss board confirms house numbers never start
// with a leading zero digit.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

static bool readLine(string &out) {
    if (!std::getline(cin, out)) return false;
    while (!out.empty() && (out[out.size()-1] == '\r' || out[out.size()-1] == '\n'))
        out.erase(out.size()-1);
    return true;
}

int main() {
    string line;
    if (!readLine(line)) return 0;
    int n = atoi(line.c_str());
    for (int t = 0; t < n; ++t) {
        string name, totalLine;
        readLine(name);
        readLine(totalLine);
        int total = atoi(totalLine.c_str());
        int cnt[10];
        memset(cnt, 0, sizeof(cnt));
        int done = 0;
        while (done < total) {
            string spec;
            readLine(spec);
            // find first non-space character
            size_t p = 0;
            while (p < spec.size() && spec[p] == ' ') p++;
            if (p < spec.size() && spec[p] == '+') {
                int a, b, c;
                sscanf(spec.c_str() + p + 1, "%d %d %d", &a, &b, &c);
                for (int x = a; x <= b; x += c) {
                    done++;
                    int v = x;
                    if (v == 0) {
                        cnt[0]++;
                    } else {
                        while (v > 0) {
                            cnt[v % 10]++;
                            v /= 10;
                        }
                    }
                }
            } else {
                int v;
                sscanf(spec.c_str() + p, "%d", &v);
                done++;
                if (v == 0) {
                    cnt[0]++;
                } else {
                    while (v > 0) {
                        cnt[v % 10]++;
                        v /= 10;
                    }
                }
            }
        }
        int z = 0;
        for (int d = 0; d < 10; ++d) z += cnt[d];
        printf("%s\n", name.c_str());
        printf("%s\n", totalLine.c_str());
        for (int d = 0; d < 10; ++d)
            printf("Make %d digit %d\n", cnt[d], d);
        if (z == 1)
            printf("In total 1 digit\n");
        else
            printf("In total %d digits\n", z);
    }
    return 0;
}
