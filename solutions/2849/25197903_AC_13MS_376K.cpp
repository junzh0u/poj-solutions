// POJ 2849 - brainf*ck
// Model: claude-sonnet-5
// Read N brainfuck programs, each terminated by a line consisting solely of
// "end". Ignore any character not in <>+-.[]; a '%' starts a comment that
// discards the rest of that physical line only (the comment does not carry
// over to the next line). Execute on a 32768-byte tape with the pointer and
// byte values both wrapping (pointer 32767->0 and 0->32767; byte 255->0 and
// 0->255). Bracket matching is precomputed with a stack scan up front so
// that '[' / ']' jump directly to the paired bracket in O(1) at run time --
// the discuss board reports TLE from re-scanning for the match at every
// jump. An unmatched '[' or ']' anywhere in the program prints
// "COMPILE ERROR" in place of running it.
// Output ambiguity: the statement says to print the program's output
// "followed by a single newline character". Reproducing the sample showed
// the bundled hello-world program's own raw output has no trailing newline
// of its own ("Hello World!" with no '\n'), and the harness must append
// exactly one '\n' after it (not two) to match the sample byte-for-byte --
// confirmed by a local simulation before submitting.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

const int MAXCMD = 128005;

static char code[MAXCMD];
static int match_[MAXCMD];
static int bstack[MAXCMD];
static unsigned char mem[32768];

int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    std::cin.ignore(1000000, '\n');
    std::string line;
    for (int pnum = 1; pnum <= n; ++pnum) {
        int len = 0;
        while (std::getline(std::cin, line)) {
            while (!line.empty() &&
                   (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
                line.erase(line.size() - 1);
            if (line == "end") break;
            for (size_t i = 0; i < line.size(); ++i) {
                char c = line[i];
                if (c == '%') break;
                if (c == '<' || c == '>' || c == '+' || c == '-' ||
                    c == '.' || c == '[' || c == ']') {
                    if (len < MAXCMD) code[len++] = c;
                }
            }
        }

        int sp = 0;
        bool ok = true;
        for (int i = 0; i < len; ++i) {
            if (code[i] == '[') {
                bstack[sp++] = i;
            } else if (code[i] == ']') {
                if (sp == 0) {
                    ok = false;
                } else {
                    int j = bstack[--sp];
                    match_[i] = j;
                    match_[j] = i;
                }
            }
        }
        if (sp != 0) ok = false;

        printf("PROGRAM #%d:\n", pnum);
        if (!ok) {
            printf("COMPILE ERROR\n");
        } else {
            memset(mem, 0, sizeof(mem));
            int ptr = 0;
            int ip = 0;
            while (ip < len) {
                char c = code[ip];
                switch (c) {
                    case '>': ptr = (ptr + 1) & 32767; break;
                    case '<': ptr = (ptr + 32767) & 32767; break;
                    case '+': mem[ptr] = (unsigned char)(mem[ptr] + 1); break;
                    case '-': mem[ptr] = (unsigned char)(mem[ptr] - 1); break;
                    case '.': putchar(mem[ptr]); break;
                    case '[': if (mem[ptr] == 0) ip = match_[ip]; break;
                    case ']': if (mem[ptr] != 0) ip = match_[ip]; break;
                }
                ++ip;
            }
            putchar('\n');
        }
    }
    return 0;
}
