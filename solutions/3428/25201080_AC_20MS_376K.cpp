// POJ 3428 - Formatting function
// Model: claude-sonnet-5
//
// Approach: read the whole of stdin into one buffer, split it into lines
// (a trailing final newline does NOT create a phantom empty line, but an
// explicit blank line - two consecutive '\n' - is a genuine, valid
// argument, confirmed by the discuss board's "%d" + blank-line trick
// case). Line 0 is the format string, the rest are the arguments, in
// order, one per specifier.
//
// Parse the format string into tokens: literal runs, and %s/%d
// specifiers. Any '%' not followed by '%', 's' or 'd' (including a '%'
// as the very last character) is a formatting error. If the number of
// specifiers does not equal the number of supplied argument lines, that
// is also an error (confirmed by the board: "%s %s %s" with only two
// argument lines given is ERROR). Every %d argument must be non-empty
// and consist only of digits 0-9 -- confirmed by the board that a
// *blank* line supplied for %d is ERROR, not 0 (so "no non-digit
// character" is read together with "is a valid non-negative integer
// literal", not literally "zero-length vacuously passes"). A %s
// argument is used verbatim, spaces included (board: "can blanks appear
// in the arguments? yes"). A valid %d value has its leading zeroes
// stripped, printing "0" if the whole thing is zeroes.
//
// Validation is fully completed (format syntax, argument count, and
// every %d's digit-ness) before anything is printed, so a failure late
// in the string still produces bare "ERROR" and never partial output.
//
// Memory care: inputs can be up to ~48MB (1000 lines of 50000 chars),
// close to the 64MB limit, so lines/tokens only ever store
// pointer+length pairs into the single input buffer -- nothing is
// copied -- and the final answer is streamed straight to stdout.

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstring>

using namespace std;

static char *buf = NULL;
static size_t bufLen = 0;

struct Line {
    size_t off, len;
};

struct Token {
    int type; // 0 = literal, 1 = resolved specifier output (ptr/len set)
    const char *ptr;
    size_t len;
};

static void readAll() {
    // Stated bound: up to 1000 argument lines plus the format line, each
    // up to 50000 chars, so at most ~50.05MB of real content. Size the
    // buffer just above that directly instead of doubling from a small
    // seed -- doubling would overshoot straight past 32MB to 64MB, which
    // sits right at (or past) the 65536K memory limit. Growth below is
    // only a defensive fallback should the data violate its stated
    // bound; it grows by a modest fraction rather than doubling to keep
    // any overshoot small.
    size_t cap = 50000000 + 200000;
    buf = (char*) malloc(cap);
    size_t len = 0;
    for (;;) {
        if (len == cap) {
            size_t growth = cap / 4;
            if (growth < (1 << 20)) growth = (1 << 20);
            cap += growth;
            buf = (char*) realloc(buf, cap);
        }
        size_t n = fread(buf + len, 1, cap - len, stdin);
        if (n == 0) break;
        len += n;
    }
    bufLen = len;
}

int main() {
    readAll();

    // Split into lines; strip a trailing '\r' from each line.
    vector<Line> lines;
    size_t pos = 0;
    while (pos < bufLen) {
        size_t idx = pos;
        while (idx < bufLen && buf[idx] != '\n') idx++;
        size_t start = pos, end = idx;
        if (end > start && buf[end - 1] == '\r') end--;
        Line l; l.off = start; l.len = end - start;
        lines.push_back(l);
        pos = idx + 1;
    }

    const char *fmt; size_t flen;
    size_t argStart; // index into lines[] where arguments begin
    if (lines.empty()) {
        fmt = buf; flen = 0;
        argStart = 0;
    } else {
        fmt = buf + lines[0].off; flen = lines[0].len;
        argStart = 1;
    }
    size_t argsCount = lines.size() - argStart;

    // Parse format string into tokens.
    vector<Token> tokens;
    bool formatError = false;
    size_t specCount = 0;
    size_t i = 0;
    size_t litStart = 0;
    bool inLit = false;
    while (i < flen) {
        if (fmt[i] == '%') {
            if (inLit) {
                Token t; t.type = 0; t.ptr = fmt + litStart; t.len = i - litStart;
                tokens.push_back(t);
                inLit = false;
            }
            if (i + 1 >= flen) { formatError = true; break; }
            char c = fmt[i + 1];
            if (c == '%') {
                Token t; t.type = 0; t.ptr = fmt + i; t.len = 1; // literal '%'
                tokens.push_back(t);
                i += 2;
            } else if (c == 's' || c == 'd') {
                Token t; t.type = (c == 's') ? 2 : 3; t.ptr = NULL; t.len = 0;
                tokens.push_back(t);
                specCount++;
                i += 2;
            } else {
                formatError = true;
                break;
            }
        } else {
            if (!inLit) { inLit = true; litStart = i; }
            i++;
        }
    }
    if (!formatError && inLit) {
        Token t; t.type = 0; t.ptr = fmt + litStart; t.len = i - litStart;
        tokens.push_back(t);
    }

    bool ok = !formatError && (specCount == argsCount);
    bool argError = false;

    if (ok) {
        size_t argIdx = argStart;
        for (size_t k = 0; k < tokens.size() && !argError; k++) {
            if (tokens[k].type == 2) { // %s
                Line &l = lines[argIdx++];
                tokens[k].ptr = buf + l.off;
                tokens[k].len = l.len;
            } else if (tokens[k].type == 3) { // %d
                Line &l = lines[argIdx++];
                const char *p = buf + l.off;
                size_t n = l.len;
                if (n == 0) { argError = true; break; }
                size_t firstNonZero = n; // n means "not found" (all zero)
                bool allDigits = true;
                for (size_t j = 0; j < n; j++) {
                    if (p[j] < '0' || p[j] > '9') { allDigits = false; break; }
                    if (p[j] != '0' && firstNonZero == n) firstNonZero = j;
                }
                if (!allDigits) { argError = true; break; }
                if (firstNonZero == n) {
                    static const char zero[1] = {'0'};
                    tokens[k].ptr = zero;
                    tokens[k].len = 1;
                } else {
                    tokens[k].ptr = p + firstNonZero;
                    tokens[k].len = n - firstNonZero;
                }
            }
        }
        if (argError) ok = false;
    }

    if (!ok) {
        fputs("ERROR\n", stdout);
    } else {
        // Use a large output buffer for efficiency on big cases.
        static char outbuf[1 << 20];
        setvbuf(stdout, outbuf, _IOFBF, sizeof(outbuf));
        for (size_t k = 0; k < tokens.size(); k++) {
            if (tokens[k].len) fwrite(tokens[k].ptr, 1, tokens[k].len, stdout);
        }
        fputc('\n', stdout);
    }

    return 0;
}
