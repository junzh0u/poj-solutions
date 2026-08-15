// POJ 3195 - Generalized Matrioshkas
// Model: claude-sonnet-5
// Each line is one test case: a sequence of nonzero integers where -k opens
// a toy of size k and k closes the most recently opened, still-open toy,
// which must also be labeled k (a stack-matching discipline, confirmed by
// the sample: repeated labels like 2,2 nest by stack position, not by
// scanning for a matching value elsewhere). When a toy m closes, the sum of
// the sizes of the toys it directly contains (its immediate stack children)
// must be strictly less than m. The whole line is a valid "generalized
// matrioshka" iff every close obeys this and the stack is empty at line end
// (every opened toy got closed). Verified against the board (message
// 179046: "2 -2" -> Try again; message 67934: a lone "-2" -> Try again,
// i.e. an unmatched open at EOF is invalid) and against all 7 official
// samples, plus a randomized differential test against an independent
// recursive-descent parser built from the same grammar reading.
// Input has no test-case count; read to EOF, one test case per line. Blank
// lines (no tokens) carry no case and are skipped; per board message
// 179046 each real case is followed by exactly one newline with no stray
// whitespace, true for the last case too. A single line's token count can
// be very large (board messages 143509/180996 report needing large stack
// capacity), so the whole input is buffered and parsed by hand with
// dynamically-growing containers rather than any fixed-size array.

#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

int main() {
    vector<char> buf;
    buf.reserve(1 << 22);
    static char chunk[1 << 20];
    size_t r;
    while ((r = fread(chunk, 1, sizeof(chunk), stdin)) > 0) {
        buf.insert(buf.end(), chunk, chunk + r);
    }
    size_t len = buf.size(), i = 0;

    vector<int> label, sum;
    label.reserve(1 << 12);
    sum.reserve(1 << 12);

    static char outbuf[1 << 20];
    setvbuf(stdout, outbuf, _IOFBF, sizeof(outbuf));

    while (i < len) {
        label.clear();
        sum.clear();
        bool valid = true;
        bool anyToken = false;

        while (i < len && buf[i] != '\n') {
            char c = buf[i];
            if (c == ' ' || c == '\r' || c == '\t') { ++i; continue; }
            bool neg = false;
            if (buf[i] == '-') { neg = true; ++i; }
            int val = 0;
            bool got = false;
            while (i < len && buf[i] >= '0' && buf[i] <= '9') {
                val = val * 10 + (buf[i] - '0');
                ++i;
                got = true;
            }
            if (!got) { ++i; continue; }
            anyToken = true;
            int t = neg ? -val : val;
            if (valid) {
                if (t < 0) {
                    label.push_back(-t);
                    sum.push_back(0);
                } else {
                    if (label.empty() || label.back() != t) {
                        valid = false;
                    } else if (sum.back() >= t) {
                        valid = false;
                    } else {
                        label.pop_back();
                        sum.pop_back();
                        if (!label.empty()) sum.back() += t;
                    }
                }
            }
        }
        if (i < len && buf[i] == '\n') ++i;
        if (!anyToken) continue;
        if (!label.empty()) valid = false;
        fputs(valid ? ":-) Matrioshka!\n" : ":-( Try again.\n", stdout);
    }
    return 0;
}
