// POJ 1782 - Run Length Encoding
// Model: claude-opus-5
//
// Encode each input line independently; the newline is passed through unencoded.
// Scan the line into maximal runs of equal characters:
//   * a run of length >= 2 is emitted greedily as chunks of 9 ("9c") while more
//     than 9 remain, then the tail as "<len>c" if that tail is >= 2 long;
//   * a leftover tail of length exactly 1 (run length 10, 19, 28, ...) cannot be
//     emitted as a repeat, so it falls through into the literal buffer and joins
//     whatever non-repeating characters follow it;
//   * runs of length 1 accumulate in the literal buffer.
// A non-empty literal buffer is flushed as '1' + chars + '1', with every '1'
// inside doubled.  The buffer is flushed just before any repeat group is emitted
// and again at end of line.
//
// Ambiguities settled from the samples and the discuss board (the statement alone
// leaves both open):
//   * "1111111111" (ten '1's) encodes as "91" + "1111", i.e. the 9-chunk first and
//     the single leftover as its own literal run -- not as "5151" or similar.
//   * An empty input line encodes to an empty output line (a blank line is real
//     input, not a stray carriage return).
// Spaces are ordinary characters and may lead or trail a line, so the line has to
// be read whole (getchar loop) rather than with scanf("%s").
// A trailing '\r' is dropped defensively: the statement lists only letters,
// digits, spaces and punctuation as input characters.

#include <cstdio>
#include <string>

using namespace std;

static string pending; /* literal (non-repeating) characters awaiting a flush */

static void flushLiteral()
{
    if (pending.empty()) return;
    putchar('1');
    for (string::size_type i = 0; i < pending.size(); ++i) {
        putchar(pending[i]);
        if (pending[i] == '1') putchar('1');
    }
    putchar('1');
    pending.erase();
}

static void encodeLine(const string &s)
{
    string::size_type i = 0, n = s.size();

    pending.erase();
    while (i < n) {
        string::size_type j = i;
        while (j < n && s[j] == s[i]) ++j;
        string::size_type run = j - i;

        if (run == 1) {
            pending += s[i];
        } else {
            flushLiteral();
            while (run >= 2) {
                string::size_type k = (run < 9) ? run : 9;
                putchar((char)('0' + (int)k));
                putchar(s[i]);
                run -= k;
            }
            if (run == 1) pending += s[i];
        }
        i = j;
    }
    flushLiteral();
}

static void emitLine(string &line)
{
    if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);
    encodeLine(line);
    putchar('\n');
    line.erase();
}

int main()
{
    string line;
    int c;

    while ((c = getchar()) != EOF) {
        if (c == '\n') emitLine(line);
        else line += (char)c;
    }
    if (!line.empty()) emitLine(line);
    return 0;
}
